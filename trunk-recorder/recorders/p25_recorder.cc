
#include "p25_recorder.h"
#include <boost/log/trivial.hpp>


p25_recorder_sptr make_p25_recorder(Source *src)
{
  return gnuradio::get_initial_sptr(new p25_recorder(src));
}

p25_recorder::p25_recorder(Source *src)
  : gr::hier_block2("p25_recorder",
                    gr::io_signature::make(1, 1, sizeof(gr_complex)),
                    gr::io_signature::make(0, 0, sizeof(float)))
{
  source = src;
  freq   = source->get_center();
  center = source->get_center();
  config = source->get_config();
  long samp_rate = source->get_rate();
  qpsk_mod       = source->get_qpsk_mod();
  silence_frames = source->get_silence_frames();
  talkgroup      = 0;
  long capture_rate = samp_rate;

  num = 0;

  state = inactive;

  double offset = freq - center;


  double symbol_rate         = 4800;
  double samples_per_symbol  = 10;    // was 10
  double system_channel_rate = symbol_rate * samples_per_symbol;
  double symbol_deviation    = 600.0; // was 600.0


  std::vector<float> sym_taps;
  const double pi = M_PI; // boost::math::constants::pi<double>();

  timestamp = time(NULL);
  starttime = time(NULL);


  double gain_mu      = 0.025;               // 0.025
  double costas_alpha = 0.04;
  double bb_gain      = src->get_fsk_gain(); // was 1.0

  baseband_amp = gr::blocks::multiply_const_ff::make(bb_gain);


  double xlate_bandwidth = 5000; // 24260.0


  valve = gr::blocks::copy::make(sizeof(gr_complex));
  valve->set_enabled(false);

  lpf_coeffs = gr::filter::firdes::low_pass_2(1.0, capture_rate, 6250, 1500, 100,gr::filter::firdes::WIN_HANN);

  //lpf_coeffs = gr::filter::firdes::low_pass(1.0, capture_rate, xlate_bandwidth, 1000, gr::filter::firdes::WIN_HANN);

   //int decimation = floor(capture_rate / system_channel_rate);
  int decimation = int(capture_rate / 96000);

  std::vector<gr_complex> dest(lpf_coeffs.begin(), lpf_coeffs.end());
BOOST_LOG_TRIVIAL(error) << "Size of LPF: " << dest.size();
/*
     prefilter = gr::filter::freq_xlating_fir_filter_ccf::make(decimation,
                lpf_coeffs,
                offset,
                samp_rate);*/

  prefilter = make_freq_xlating_fft_filter(decimation,
                                           dest,
                                           offset,
                                           samp_rate);

  double resampled_rate = double(capture_rate) / double(decimation); // rate at
                                                                   // output of
                                                                   // self.lpf
  double arb_rate  = (double(system_channel_rate) / resampled_rate);
  double arb_size  = 32;
  double arb_atten = 100;


  // Create a filter that covers the full bandwidth of the output signal

  // If rate >= 1, we need to prevent images in the output,
  // so we have to filter it to less than half the channel
  // width of 0.5.  If rate < 1, we need to filter to less
  // than half the output signal's bw to avoid aliasing, so
  // the half-band here is 0.5*rate.
  double percent = 0.80;

  if (arb_rate <= 1) {
    double halfband = 0.5 * arb_rate;
    double bw       = percent * halfband;
    double tb       = (percent / 2.0) * halfband;

    BOOST_LOG_TRIVIAL(info) << "Arb Rate: " << arb_rate << " Half band: " << halfband << " bw: " << bw << " tb: " << tb;

    // As we drop the bw factor, the optfir filter has a harder time converging;
    // using the firdes method here for better results.
    arb_taps = gr::filter::firdes::low_pass_2(arb_size, arb_size, bw, tb, arb_atten,
                                              gr::filter::firdes::WIN_BLACKMAN_HARRIS);
  } else {
    BOOST_LOG_TRIVIAL(error) << "Something is probably wrong! Resampling rate too low";
    exit(0);

    /*
       double halfband = 0.5;
       double bw = percent*halfband;
       double tb = (percent/2.0)*halfband;
       double ripple = 0.1;

       bool made = False;
       while not made:
        try:
            self._taps = optfir.low_pass(self._size, self._size, bw, bw+tb,
               ripple, atten)
            made = True
        except RuntimeError:
            ripple += 0.01
            made = False
            print("Warning: set ripple to %.4f dB. If this is a problem, adjust
               the attenuation or create your own filter taps." % (ripple))

     # Build in an exit strategy; if we've come this far, it ain't working.
            if(ripple >= 1.0):
                raise RuntimeError("optfir could not generate an appropriate
     #filter.")*/
  }


  arb_resampler = gr::filter::pfb_arb_resampler_ccf::make(arb_rate, arb_taps);

  // on a trunked network where you know you will have good signal, a carrier
  // power squelch works well. real FM receviers use a noise squelch, where
  // the received audio is high-passed above the cutoff and then fed to a
  // reverse squelch. If the power is then BELOW a threshold, open the squelch.

  squelch_db = source->get_squelch_db();

  if (squelch_db != 0) {


    // Non-blocking as we are using squelch_two as a gate.
    squelch = gr::analog::pwr_squelch_cc::make(squelch_db, 0.01, 10, false);

    //  based on squelch code form ham2mon
    // set low -200 since its after demod and its just gate for previous squelch so that the audio
    // recording doesn't contain blank spaces between transmissions
    squelch_two = gr::analog::pwr_squelch_ff::make(-200, 0.01, 0, true);
  }


  agc = gr::analog::feedforward_agc_cc::make(16, 1.0);

  double omega      = double(system_channel_rate) / double(symbol_rate);
  double gain_omega = 0.1  * gain_mu * gain_mu;
  double alpha      = costas_alpha;
  double beta       = 0.125 * alpha * alpha;
  double fmax       = 2400; // Hz
  fmax = 2 * pi * fmax / double(system_channel_rate);

  costas_clock = gr::op25_repeater::gardner_costas_cc::make(omega, gain_mu, gain_omega, alpha,  beta, fmax, -fmax);

  // Perform Differential decoding on the constellation
  diffdec = gr::digital::diff_phasor_cc::make();

  // take angle of the difference (in radians)
  to_float = gr::blocks::complex_to_arg::make();

  // convert from radians such that signal is in -3/-1/+1/+3
  rescale = gr::blocks::multiply_const_ff::make((1 / (pi / 4)));

  double freq_to_norm_radians = pi/(system_channel_rate/2.0);
  double fc = 0.0;
  double fd = 600.0;
  double pll_demod_gain = 1.0/(fd*freq_to_norm_radians);
  pll_freq_lock = gr::analog::pll_freqdet_cf::make	(	(symbol_rate/2.0*1.2)*freq_to_norm_radians, (fc+(3*fd*1.9))*freq_to_norm_radians, (fc+(-3*fd*1.9))*freq_to_norm_radians);
  pll_amp = gr::blocks::multiply_const_ff::make(pll_demod_gain * bb_gain);

  baseband_noise_filter_taps = gr::filter::firdes::low_pass_2(1.0, system_channel_rate, symbol_rate/2.0*1.175, symbol_rate/2.0*0.125, 20.0, gr::filter::firdes::WIN_KAISER, 6.76);
  noise_filter = gr::filter::fft_filter_fff::make(1.0, baseband_noise_filter_taps);
  double symbol_decim = 1;

  valve = gr::blocks::copy::make(sizeof(gr_complex));
  valve->set_enabled(false);

  for (int i = 0; i < samples_per_symbol; i++) {
    sym_taps.push_back(1.0 / samples_per_symbol);
  }

  sym_filter    =  gr::filter::fir_filter_fff::make(symbol_decim, sym_taps);
  tune_queue    = gr::msg_queue::make(20);
  traffic_queue = gr::msg_queue::make(2);
  rx_queue      = gr::msg_queue::make(100);
  const float l[] = { -2.0, 0.0, 2.0, 4.0 };

  //  const float l[] = { -3.0, -1.0, 1.0, 3.0 };
  std::vector<float> slices(l, l + sizeof(l) / sizeof(l[0]));
  fsk4_demod = gr::op25_repeater::fsk4_demod_ff::make(tune_queue, system_channel_rate, symbol_rate);
  slicer     = gr::op25_repeater::fsk4_slicer_fb::make(slices);

  int udp_port               = 0;
  int verbosity              = 1; // 10 = lots of debug messages
  const char *wireshark_host = "127.0.0.1";
  bool do_imbe               = 1;
  bool do_output             = 1;
  bool do_msgq               = 0;
  bool do_audio_output       = 1;
  bool do_tdma               = 0;
  op25_frame_assembler = gr::op25_repeater::p25_frame_assembler::make(0, wireshark_host, udp_port, verbosity, do_imbe, do_output, silence_frames, do_msgq, rx_queue, do_audio_output, do_tdma);


  converter = gr::blocks::short_to_float::make(1, 32768.0); //8192.0); // 8192.0);
                                                            // //2048.0 //1 /
                                                            // 32768.0
  levels = gr::blocks::multiply_const_ff::make(source->get_digital_levels());
  tm *ltm = localtime(&starttime);

  std::stringstream path_stream;
  //path_stream << boost::filesystem::current_path().string() <<  "/" << 1900 + ltm->tm_year << "/" << 1 + ltm->tm_mon << "/" << ltm->tm_mday;
  path_stream << this->config->capture_dir << "/junk";

  boost::filesystem::create_directories(path_stream.str());
  int nchars = snprintf(filename, 160, "%s/%ld-%ld_%g.wav", path_stream.str().c_str(), talkgroup, timestamp, freq);
  if (nchars >= 160) {
    BOOST_LOG_TRIVIAL(error) << "Analog Recorder: Path longer than 160 charecters";
  }
  wav_sink = gr::blocks::nonstop_wavfile_sink::make(filename, 1, 8000, 16);


  if (!qpsk_mod) {
    connect(self(),        0, valve,         0);
    connect(valve,         0, prefilter,     0);
    connect(prefilter,     0, arb_resampler, 0);
    if (squelch_db != 0) {
      connect(arb_resampler, 0,  squelch,        0);
      connect(squelch,        0, pll_freq_lock,      0);
    } else {
      connect(arb_resampler, 0, pll_freq_lock,      0);
    }
    connect(pll_freq_lock,             0, pll_amp,         0);
    connect(pll_amp,         0, noise_filter,         0);
    connect(noise_filter,         0, sym_filter,           0);
    connect(sym_filter,           0, fsk4_demod,           0);
    connect(fsk4_demod,           0, slicer,               0);
    connect(slicer,               0, op25_frame_assembler, 0);
    connect(op25_frame_assembler, 0, converter,            0);
    connect(converter,            0, levels,           0);
    if (squelch_db != 0) {
      connect(levels,           0, squelch_two,    0);
      connect(squelch_two,    0, wav_sink,             0);
    } else {
    connect(levels,           0, wav_sink,             0);
    }
  } else {
    connect(self(),    0, valve,         0);
    connect(valve,     0, prefilter,     0);
    connect(prefilter, 0, arb_resampler, 0);
    if (squelch_db != 0) {
      connect(arb_resampler, 0,  squelch,        0);
      connect(squelch,        0, agc,      0);
    } else {
      connect(arb_resampler, 0, agc,      0);
    }
    connect(agc,           0, costas_clock,         0);
    connect(costas_clock,  0, diffdec,              0);
    connect(diffdec,       0, to_float,             0);
    connect(to_float,      0, rescale,              0);
    connect(rescale,       0, slicer,               0);
    connect(slicer,        0, op25_frame_assembler, 0);
    connect(op25_frame_assembler, 0, converter,  0);
    connect(converter,            0, levels, 0);
    if (squelch_db != 0) {
      connect(levels,           0, squelch_two,    0);
      connect(squelch_two,    0, wav_sink,             0);
    } else {
    connect(levels,           0, wav_sink,             0);
    }
  }
}

void p25_recorder::clear() {
  op25_frame_assembler->clear();
}

void p25_recorder::autotune() {
  if (!qpsk_mod) {
    gr::message::sptr msg;
    msg = tune_queue->delete_head_nowait();

    if (msg != 0) {
      BOOST_LOG_TRIVIAL(info) << "p25_recorder.cc: Freq:\t" << freq << "\t Tune: " << msg->arg1();
      //tune_offset(freq + (msg->arg1()*100));
      tune_queue->flush();
    }
  }
}

p25_recorder::~p25_recorder() {}



Source * p25_recorder::get_source() {
  return source;
}

int p25_recorder::get_num() {
  return num;
}

bool p25_recorder::is_active() {
  if (state == active) {
    return true;
  } else {
    return false;
  }
}

bool p25_recorder::is_idle() {
  if (state == active) {
    return !squelch->unmuted();
  }
  return true;
}
double p25_recorder::get_freq() {
  return freq;
}

double p25_recorder::get_current_length() {
  return wav_sink->length_in_seconds();
}

int p25_recorder::lastupdate() {
  return time(NULL) - timestamp;
}

long p25_recorder::elapsed() {
  return time(NULL) - starttime;
}

void p25_recorder::tune_offset(double f) {
  freq = f;
  int offset_amount = (f - center);
  prefilter->set_center_freq(offset_amount); // have to flip this for 3.7
  // BOOST_LOG_TRIVIAL(info) << "Offset set to: " << offset_amount << " Freq: "
  //  << freq;
}

State p25_recorder::get_state() {
  return state;
}

void p25_recorder::stop() {
  if (state == active) {
    //op25_frame_assembler->clear();
    BOOST_LOG_TRIVIAL(error) << "p25_recorder.cc: Stopping Logger \t[ " << num << " ] - freq[ " << freq << "] \t talkgroup[ " << talkgroup << " ]";
    state = inactive;
    valve->set_enabled(false);
    wav_sink->close();
  } else {
    BOOST_LOG_TRIVIAL(error) << "p25_recorder.cc: Trying to Stop an Inactive Logger!!!";
  }
}

void p25_recorder::start(Call *call, int n) {
  if (state == inactive) {
    timestamp = time(NULL);
    starttime = time(NULL);

    talkgroup = call->get_talkgroup();
    freq      = call->get_freq();

    // num = n;
      if (!qpsk_mod) {
        fsk4_demod->reset();
      }
    BOOST_LOG_TRIVIAL(info) << "p25_recorder.cc: Starting Logger   \t[ " << num << " ] - freq[ " << freq << "] \t talkgroup[ " << talkgroup << " ]";

    int offset_amount = (freq - center);
    prefilter->set_center_freq(offset_amount);

    wav_sink->open(call->get_filename());
    state = active;
    valve->set_enabled(true);
  } else {
    BOOST_LOG_TRIVIAL(error) << "p25_recorder.cc: Trying to Start an already Active Logger!!!";
  }
}
