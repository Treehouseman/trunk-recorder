#include "tree.h"
#include <sys/sysinfo.h>
#include <string>
#include <ncurses.h>
#include <sstream>
#include <iostream>
#include <curses.h>
#include <string.h>
#include <stdio.h>
#include <fstream>

bool verbose = false;
bool coloren = false;
long activegroups[100];
long colorgroups[100];
int colorsystems[100];
int currcol = 0;
long csysid[10];
double load[3];
WINDOW *create_newwin(int height, int width, int starty, int startx);
WINDOW *rec_win;
WINDOW *data_win;
WINDOW *log_win;
WINDOW *hist_win;
WINDOW *cpu_win;
void destroy_win(WINDOW *local_win);
int systemnumber = 0;
int currsys;
int minmsg[10];
int maxmsg[10];
int avgmsg[10];
int syscount = 0;
int strecx = 0;
int strecy = 0;
int stmrx;
int stmry = 0;
int sthstx;
int sthsty = 1;
int stlgx;
int stlgy;
int defh = 30;
int defw = 15;
int mrendx;
int stcpux;
int stcpuy = 0;
long Speed[100];
float CPUPer[100];
long Max = 0;
long Min = 0;
int cpus = 0;
int cpuw = defw;
std::ifstream file;

void Tree::CallOver(){
	//BOOST_LOG_TRIVIAL(info) << " This call is over!!";
}
void Tree::EndCall(int msg, long tg, long elapsed){
	switch(msg){
		case 1:
		//BOOST_LOG_TRIVIAL(info) << "\tRemoving Recorded Call \tTG: " << tg << "\tElapsed: " << elapsed;
		/*else{
			std::stringstream ss;
			std::string s;
			ss << tg << " " << elapsed << "s\n";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(hist_win, c);
			wrefresh(hist_win);
		}*/
		break;
		case 2:
		//BOOST_LOG_TRIVIAL(info) << "\tFinished Removing Call \tTG: " << tg << "\tElapsed: " << elapsed << "\tCPU: " << load[0]*100/8 << "%";
		//BOOST_LOG_TRIVIAL(info) << "\tFinished Removing Call \tTG: " << tg << "\tElapsed: " << elapsed;
		/*else{
			std::stringstream ss;
			std::string s;
			ss << tg << " " << elapsed << "s\n";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(hist_win, c);
			wrefresh(hist_win);
		}*/
		break;
		case 3:
		BOOST_LOG_TRIVIAL(info) << "\tRemoving Non-Recorded Call \tTG: " << tg << "\tElapsed: " << elapsed;
		break;
		case 4:
		//BOOST_LOG_TRIVIAL(trace) << "\tRemoving TG: " << tg << "\tElapsed: " << elapsed;
		break;
	}
}
void Tree::Config(int msg, std::string data){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(info) << "System Type: " << data;
		break;
		case 2:
		BOOST_LOG_TRIVIAL(info) << "TalkgroupsFile: " << data;
		break;
		case 3:
		BOOST_LOG_TRIVIAL(info) << "Capture Directory: " << data;
		break;
		case 4:
		BOOST_LOG_TRIVIAL(info) << "Config Directory: " << data;
		break;
		case 5:
		if(data != "")
		BOOST_LOG_TRIVIAL(info) << "Upload Script: " << data;
		break;
		case 6:
		if(data != "")
		BOOST_LOG_TRIVIAL(info) << "Upload Server: " << data;
		break;
		case 7:
		BOOST_LOG_TRIVIAL(info) << "Default Mode: " << data;
		break;
		case 8:
		if(data != "")
		BOOST_LOG_TRIVIAL(info) << "Antenna: " << data;
		break;
		case 9:
		BOOST_LOG_TRIVIAL(info) << "Driver: " << data;
		break;
		case 10:
		BOOST_LOG_TRIVIAL(error) << "Failed parsing Config: " << data;
		break;
	}
}
void Tree::Config(int msg, double data){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(info) << "Control Channel: " << data;
		break;
		case 2:
		BOOST_LOG_TRIVIAL(info) << "Center: " << data;
		break;
		case 3:
		BOOST_LOG_TRIVIAL(info) << "Rate: " << data;
		break;
		case 4:
		if(data != 0)
		BOOST_LOG_TRIVIAL(info) << "Error: " << data;
		break;
		case 5:
		if(data != 0)
		BOOST_LOG_TRIVIAL(info) << "PPM Error: " << data;
		break;
		case 6:
		if(data != 0)
		BOOST_LOG_TRIVIAL(info) << "Squelch: " << data;
		break;
		case 7:
		BOOST_LOG_TRIVIAL(info) << "Max HZ: " << data;
		break;
		case 8:
		BOOST_LOG_TRIVIAL(info) << "Min HZ: " << data;
		break;
	}
}
void Tree::Config(int msg, int data){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(info) << "Gain: " << data;
		break;
		case 2:
		if(data != 0)
		BOOST_LOG_TRIVIAL(info) << "IF Gain: " << data;
		break;
		case 3:
		if(data != 0)
		BOOST_LOG_TRIVIAL(info) << "BB Gain: " << data;
		break;
		case 4:
		if(data != 0)
		BOOST_LOG_TRIVIAL(info) << "Digital Recorders: " << data;
		break;
		case 5:
		if(data != 0)
		BOOST_LOG_TRIVIAL(info) << "Debug Recorders: " << data;
		break;
		case 6:
		if(data != 0)
		BOOST_LOG_TRIVIAL(info) << "Analog Recorders: " << data;
		break;
	}
}
void Tree::StartRecorder(int msg, long tg, double freq, long tdma, bool encrypted){
	switch(msg){
		case 1:
		//BOOST_LOG_TRIVIAL(error) << "\tCall created for: " << tg << "\tFreq: " << freq << "\tCPU: " << load[0]*100/8 << "%";
		break;
	}
}
void Tree::StartRecorder(int msg, long tg, double freq){
	switch(msg){
		case 1:
		//BOOST_LOG_TRIVIAL(error) << "Got Encrypted Call: " << tg << " Freq: " << freq;
			std::stringstream ss;
			std::string s;
			ss << "Got Encrypted Call: " << tg << " Freq: " << freq << "\n";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(log_win, c);
			wrefresh(log_win);
		break;
		case 2:
		BOOST_LOG_TRIVIAL(error) << "\tTrying to record TDMA: " << freq << " For TG: " << tg;
		break;
		case 3:
		BOOST_LOG_TRIVIAL(error) << "\tTalkgroup not found: " << freq << " For TG: " << tg;
		break;
		case 4:
		BOOST_LOG_TRIVIAL(error) << "\tRecording not started because there was no source covering: " << freq << " For TG: " << tg;
		break;
	}
}
void Tree::Retune(int msg, long elapsed, double oldfreq, double newfreq, long tg, long sinceup){
	getloadavg(load, 3);
	std::stringstream ss;
	std::string s;
	long lnew = newfreq;
	long lold = oldfreq;
	switch(msg){
		case 1:
		//BOOST_LOG_TRIVIAL(info) << "\tRetune - Elapsed: " << elapsed << "s \t Old Freq: " << oldfreq << "\tFreq[ " << newfreq << "] \t talkgroup[ " << tg << " ] \tCPU: " << load[0]*100/8 << "%";
			ss << "Retune - Elapsed: " << elapsed << "s \t Old Freq: " << lold << "\tFreq[ " << lnew << "] \t talkgroup[ " << tg << " ] \tCPU: " << load[0]*100/8 << "%\n";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(log_win, c);
			wrefresh(log_win);
		break;
	}
}
void Tree::Assign(int msg, int csize, long tg, double oldfreq, double newfreq){
	switch(msg){
		case 1:
		//BOOST_LOG_TRIVIAL(info) << "\tAssign Retune - Total calls: " << csize << "\tTalkgroup: " << tg << "\tOld Freq: " << oldfreq << "\tNew Freq: " << newfreq;
		break;
	}
}
void Tree::Assign(int msg, long newtg, double freq, long newtdma, long oldtg, long oldtdma, long elapsed, long sinceup){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(info) << "\tFreq in use -  TG: " << newtg << "\tFreq: " << freq << "\tTDMA: " << newtdma << "\t Ending Existing call\tTG: " << oldtg << "\tTMDA: " << oldtdma << "\tElapsed: " << elapsed << "s \tSince update: " << sinceup;
		break;
		case 2:
		BOOST_LOG_TRIVIAL(info) << "\tFreq in use -  TG: " << newtg << "\tFreq: " << freq << "\tTDMA: " << newtdma << "\t Ending Existing call\tTG: " << oldtg << "\tTMDA: " << oldtdma << "\tElapsed: " << elapsed << "s \tSince update: " << sinceup;
		break;
	}
}
void Tree::MainMsg(int msg){
	switch(msg){
		case 1:
		//BOOST_LOG_TRIVIAL(info) << "\t\tSwitching to Different Source to Record";
		break;
		case 2:
		BOOST_LOG_TRIVIAL(error) << "\tNot recording call";
		break;
		case 3:
		//BOOST_LOG_TRIVIAL(info) << "\tNot debug recording call";
		break;
		case 4:
		BOOST_LOG_TRIVIAL(info) << "Both PPM and Error should not be set at the same time. Setting Error to 0.";
		break;
		case 5:
		BOOST_LOG_TRIVIAL(error) << "\tSystem Modulation Not Specified, assuming QPSK";
		break;
		case 6:
		BOOST_LOG_TRIVIAL(info) << "\tUnable to Retune";
		break;
		case 7:
		BOOST_LOG_TRIVIAL(info) << "\tUnable to Retune";
		break;
		case 8:
		BOOST_LOG_TRIVIAL(info) << "Loading Talkgroups...";
		break;
		case 9:
		BOOST_LOG_TRIVIAL(info) << "Stopping flow graph";
		break;
		case 10:
		BOOST_LOG_TRIVIAL(error) << "Unable to setup Control Channel Monitor";
	}
}
void Tree::CurrentSystem(int msg, int sysid){
	switch(msg){
		case 1:
		currsys = sysid;
		//BOOST_LOG_TRIVIAL(info) << "Decoding System ID " << std::hex << std::uppercase << sysid << std::nouppercase << std::dec;
		/*std::stringstream ss;
		std::string s;
		ss << "Decoding System: " << std::hex << std::uppercase << sysid << std::nouppercase << std::dec << "\n";
		s = ss.str();
		const char * c = s.c_str();
		wprintw(log_win, c);
		wrefresh(log_win);*/
		break;
	}
}
void Tree::UpdateRecorder(int msg, int csize, long tg, double oldfreq, double newfreq, long elapsed){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(trace) << "\t Update Retune - Total calls: " << csize << "\tTalkgroup: " << tg << "\tOld Freq: " << oldfreq << "\tNew Freq: " << newfreq;
	}
}
void Tree::MonMess(int msg, float mps){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(error) << "\tControl Channel Message Decode Rate: " << mps << "/sec \tCPU: 1m= " << load[0]*100/8 << "% 5m= " << load[1]*100/8 << "% 15m= " << load[2]*100/8 << "%";
	}
}
void Tree::MonSys(int msg, double control){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(info) << "Control Channel: " << control;
		break;
	}
}
void Tree::ChanId(int msg, int chan_id, long id, long channel, int csize){
	switch(msg){
		case 1:
		//BOOST_LOG_TRIVIAL(info) << "\tFind - ChanId " << chan_id << " map id " << id  << "Channel " << channel << " size " << csize << " ! Not Found !";
		break;
	}
}
void Tree::PTrunk(int msg, unsigned long ul1, unsigned long ul2, unsigned long ul3, unsigned long ul4, long l1, bool b1, std::string s1, std::string s2, double d1){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(trace) << "tsbk00\tMoto Patch Add \tsg: " << ul1 << "\tga1: " << ul2 << "\tga2: " << ul3 << "\tga3: " << ul4;
		break;
		case 2:
		BOOST_LOG_TRIVIAL(trace) << "tsbk00\tChan Grant\tChannel ID: " << std::setw(5) << ul1 << "\tFreq: "<< ul2/1000000.0 << "\tga " << std::setw(7) << ul3  << "\tTDMA " << l1 << "\tsa " << ul4 << "\tEncrypt " << b1 << "\tBandwidth: " << d1;
		break;
		case 3:
		BOOST_LOG_TRIVIAL(trace) << "tsbk00\tChan Grant\tChannel ID: " << std::setw(5) << ul1 << "\tFreq: "<< ul2/1000000.0 << "\tTDMA " << l1 << "\tsa " << ul3;
		break;
		case 4:
		BOOST_LOG_TRIVIAL(trace) << "tsbk02\tGrant Update\tChannel ID: " << std::setw(5) << ul1 << "\tFreq: " << ul2 / 1000000.0 << "\tga " << std::setw(7) << ul3 << "\tTDMA " << l1;
		break;
		case 5:
		BOOST_LOG_TRIVIAL(trace) << "MOT_GRG_CN_GRANT_UPDT(0x03): \tChannel ID: "<< std::setw(5) << ul1 << "\tFreq: " << ul2 / 1000000.0 << "\tsg " << std::setw(7) << ul3 << "\tTDMA " << l1;
		break;
		case 6:
		BOOST_LOG_TRIVIAL(trace) << "tsbk16 sndcp data ch: chan " << ul1 << " "  << ul2;
		break;
		case 7:
		BOOST_LOG_TRIVIAL(trace) << "tsbk34 iden vhf/uhf id " << std::dec << ul1 << " toff " << ul2 << " spac " << ul3 << " freq " << ul4 << " [ " << s1 << "]";
		break;
		case 8:
		BOOST_LOG_TRIVIAL(trace) << "tsbk33 iden up tdma id " << std::dec << ul1 << " f " <<  ul2 << " offset " << ul3 << " spacing " << ul4 << " slots/carrier " << l1;
		break;
		case 9:
		BOOST_LOG_TRIVIAL(trace) << "tsbk3d iden id "<< std::dec << ul1 <<" toff "<< ul2 << " spac " << ul3 << " freq " << ul4;
		break;
		case 10:
		BOOST_LOG_TRIVIAL(trace) << "tsbk3a rfss status: syid: " << ul1 << " rfid " << ul2 << " stid " << ul3 << " ch1 " << ul4 << "(" << s1 <<  ")";
		break;
		case 11:
		BOOST_LOG_TRIVIAL(trace) << "tsbk39 secondary cc: rfid " << std::dec << ul1 << " stid " << ul2 << " ch1 " << ul3 << "(" << s1 << ") ch2 " << ul4 << "(" << s2 << ") ";
		break;
		case 12:
		BOOST_LOG_TRIVIAL(trace) << "tsbk3b net stat: wacn " << std::dec << ul1 << " syid " << ul2 << " ch1 " << ul3 << "(" << s1 << ") ";
		break;
		case 13:
		BOOST_LOG_TRIVIAL(trace) << "tsbk3c\tAdjacent Status\t rfid " << std::dec << ul1 << " stid " << ul2 << " ch1 " << ul3 << "(" << s1 << ") ";
		break;
		case 14:
		BOOST_LOG_TRIVIAL(trace) << "\ttsbk3c Chan " << ul1 << "  " << ul2;
		break;
		case 15:
		BOOST_LOG_TRIVIAL(trace) << "tsbk20\tAcknowledge Response\tga " << std::setw(7) << ul1  <<"\tsa " << ul2 << "\tReserved: " << ul3;
		break;
		case 16:
		BOOST_LOG_TRIVIAL(trace) << "tsbk2c\tUnit Registration Response\tsa " << std::setw(7) << ul1 << " Source ID: " << ul2;
		break;
		case 17:
		BOOST_LOG_TRIVIAL(trace) << "tsbk2f\tUnit Deregistration ACK\tSource ID: " << std::setw(7) << ul1 <<std::endl;
		break;
		case 18:
		BOOST_LOG_TRIVIAL(trace) << "tsbk2f\tUnit Group Affiliation\tSource ID: " << std::setw(7) << ul1 << "\tGroup Address: " << ul2 << "\tAnouncement Goup: " << ul3;
		break;
	}
}
void Tree::PParse(int msg, std::string s1, long l1, long l2, long l3, long l4, long l5, long l6){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(trace) << "process_qmsg: command: " << s1;
		break;
		case 2:
		BOOST_LOG_TRIVIAL(trace) << "process_data_unit timeout";
		break;
		case 3:
		BOOST_LOG_TRIVIAL(trace) << "unknown message type " << l1;
		break;
		case 4:
		BOOST_LOG_TRIVIAL(trace) << "type " << l1 << " len " << l2 << "/"<< l3 << " opcode " << l4 << "[" << l5 << "/" << l6 << "]";
		break;
	}
}
void Tree::PActivate(int msg, int i1, long l1, long l2){
	int recorder;
	std::stringstream ss;
	std::string s;
	getloadavg(load, 3);
	switch(msg){
		case 1:
		for(int i = 0; i < 100; i++){
			//std::cout << i << " " << activegroups[i] << "\n";
			if(activegroups[i]==0){
				recorder = i;
				activegroups[i]=l2;
				colorsystems[i]=currsys;
				//std::cout << "Found Spot " << i << " open!\n";
				ScrRef();
				break;
			}
		}
		BOOST_LOG_TRIVIAL(info) << "p25_recorder.cc: Activating Logger   \t[ " << recorder << " ] - freq[ " << l1 << "] \t talkgroup[ " << l2 << " ] \tCPU: " << load[0]*100/8 << "%";
			ss  << "p25_recorder.cc: Activating Logger   \t[ " << recorder << " ] - freq[ " << l1 << "] \t talkgroup[ " << l2 << " ] \tCPU: " << load[0]*100/8 << "%\n";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(log_win, c);
			wrefresh(log_win);
		break;
		case 2:
		BOOST_LOG_TRIVIAL(info) << "p25_recorder.cc: Trying to Activate an Active Logger!!!";
		break;
		case 3:
		for(int i = 0; i < 100; i++){
			//std::cout << i << " " << activegroups[i] << "\n";
			if(activegroups[i]==l2){
				recorder = i;
				activegroups[i]=0;
				//std::cout << "Found Talkgroup in spot " << i << "!\n";
				ScrRef();
				break;
			}
		}
		BOOST_LOG_TRIVIAL(info) << "p25_recorder.cc: Deactivating Logger \t[ " << recorder << " ] - freq[ " << l1 << "] \t talkgroup[ " << l2 << " ] \tCPU: " << load[0]*100/8 << "%";
			ss << "p25_recorder.cc: Deactivating Logger \t[ " << recorder << " ] - freq[ " << l1 << "] \t talkgroup[ " << l2 << " ] \tCPU: " << load[0]*100/8 << "%\n";
			s = ss.str();
			const char * c = s.c_str();
			wprintw(log_win, c);
			wrefresh(log_win);
		break;
		case 4:
		BOOST_LOG_TRIVIAL(info) << "p25_recorder.cc: Deactivating an Inactive Logger \t[ " << i1 << " ] - freq[ " << l1 << "] \t talkgroup[ " << l2 << " ]";
		break;
		case 5:
		//BOOST_LOG_TRIVIAL(info) << "Offset set to: " << i1 << " Freq: "  << l1;
		break;
		case 6:
		BOOST_LOG_TRIVIAL(error) << "Something is probably wrong! Resampling rate too low";
		break;
		case 7:
		BOOST_LOG_TRIVIAL(error) << "Something is probably wrong! Resampling rate too low";
		break;
		case 8:
		//BOOST_LOG_TRIVIAL(info) << "Offset set to: " << i1 << " Freq: "  << l1;
		break;
	}
}
void Tree::SNCRC(int msg, int i1, uint64_t ui1){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(info) << "Bit error at bit " << i1;
		break;
		case 2:
		BOOST_LOG_TRIVIAL(info) << "I just flipped a bit!";
		break;
		case 3:
		BOOST_LOG_TRIVIAL(info) << "found a frame at " << ui1;
		break;
		case 4:
		BOOST_LOG_TRIVIAL(info) << "CRC OK";
		break;
		case 5:
		BOOST_LOG_TRIVIAL(info) << "CRC FAILED";
		break;
		case 6:
		BOOST_LOG_TRIVIAL(info) << "Deinterleave called with " << i1 << " outputs";
		break;
		case 7:
		BOOST_LOG_TRIVIAL(info) << "found a preamble at " << ui1;
		break;
		case 8:
		BOOST_LOG_TRIVIAL(info) << "consumed " << i1 << ", produced " << ui1;
		break;
	}
}
void Tree::SRC(int msg, std::string s1){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(info) << "Setting antenna to [" << s1 << "]";
		break;
		case 2:
		BOOST_LOG_TRIVIAL(info) << "[ " << s1 << " ] No Analog Recorders Available";
		break;
		case 3:
		BOOST_LOG_TRIVIAL(info) << "[ " << s1 << " ] No Debug Recorders Available";
		break;
		case 4:
		BOOST_LOG_TRIVIAL(info) << "[ " << s1 << " ] No Digital Recorders Available";
		break;
		case 5:
		BOOST_LOG_TRIVIAL(info) << "Source Device: " << s1;
		break;
	}
}
void Tree::SRC(int msg, double d1){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(info) << "Setting sample rate to: " << d1;
		break;
		case 2:
		BOOST_LOG_TRIVIAL(info) << "Actual sample rate: " << d1;
		break;
		case 3:
		BOOST_LOG_TRIVIAL(info) << "Tunning to " << d1 << "hz";
		break;
		case 4:
		BOOST_LOG_TRIVIAL(info) << "Setting sample rate to: " << d1;
		break;
		case 5:
		BOOST_LOG_TRIVIAL(info) << "Actual sample rate: " << d1;
		break;
		case 6:
		BOOST_LOG_TRIVIAL(info) << "Tunning to " << d1 << "hz";
		break;
		
	}
}
void Tree::SRCRec(int msg, int i1, int i2){
	switch(msg){
		case 1:
		//BOOST_LOG_TRIVIAL(info) << "\tTG Priority: "<< i1 << " Available Digital Recorders: " << i2;
		break;
	}
}
void Tree::SRCMsg(int msg){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(info) << "\t\tNot recording because of priority";
		break;
		case 2:
		BOOST_LOG_TRIVIAL(info) << "Not recording because of priority";
		break;
		case 3:
		BOOST_LOG_TRIVIAL(info) << "Source Device not specified";
		break;
		case 4:
		BOOST_LOG_TRIVIAL(info) << "Source device name missing, defaulting to rtl device";
		break;
		case 5:
		BOOST_LOG_TRIVIAL(info) << "SOURCE TYPE OSMOSDR (osmosdr)";
		break;
		case 6:
		BOOST_LOG_TRIVIAL(info) << "SOURCE TYPE USRP (UHD)";
		break;
		
	}
}
void Tree::Talk(int msg, std::string s1, int i1, int i2){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(error) << "Error Opening TG File: " << s1;
		break;
		case 2:
		BOOST_LOG_TRIVIAL(error) << "Warning: skipped " << i1 << " of " << i2 << " talkgroup entries! Invalid format?";
		break;
		case 3:
		BOOST_LOG_TRIVIAL(error) << "The format is very particular. See https://github.com/robotastic/trunk-recorder for example input.";
		break;
		case 4:
		BOOST_LOG_TRIVIAL(info) << "Read " << i1 << " talkgroups.";
		break;
	}
}
void Tree::Debug(int msg, int i1, long l1, long l2){
	switch(msg){
		case 1:
		BOOST_LOG_TRIVIAL(info) << "debug_recorder.cc: Deactivating Logger [ " << i1 << " ] - freq[ " << l1 << "] \t talkgroup[ " << l2 << " ]";
		break;
		case 2:
		BOOST_LOG_TRIVIAL(info) << "debug_recorder.cc: Activating Logger [ " << i1 << " ] - freq[ " << l1 << "] \t talkgroup[ " << l2 << " ]";
		break;
	}
}

bool Tree::StartCurses(){
	
	int startx, starty, width, height;
	int logw = 125;
	int logh = 20;
	int logsx = 0;
	int logsy = 35;
	int ch;
	for(int i = 0; i < 10; i++){
		csysid[i] = 0;
		maxmsg[i] = 0;
		minmsg[i] = 0;
		avgmsg[i] = 0;
	}
//newterm(NULL, stdout, stdin);
	initscr();			/* Start curses mode 		*/
	curs_set(0);
	//if (has_colors){
		coloren = true;
		start_color();
		init_pair(1, COLOR_GREEN, COLOR_BLACK);
		init_pair(2, COLOR_RED, COLOR_BLACK);
		init_pair(3, COLOR_CYAN, COLOR_BLACK);
		init_pair(4, COLOR_WHITE, COLOR_BLACK);
		init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	//}
	//cbreak();			/* Line buffering disabled, Pass on
					// * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

	height = 30;
	width = 15;
	//starty = (LINES - height) / 2;	/* Calculating for a center placement */
	//startx = (COLS - width) / 2;	/* of the window		*/
	//printw("Press F1 to exit");
	starty = 0;
	startx = 0;
	refresh();
	rec_win = create_newwin(defh, defw, strecy, strecx);
	
	log_win = create_newwin(logh-1, logw-2, logsy+1, logsx+1);
	stmrx = defw +1;
	if(syscount < 3){
	data_win = create_newwin(defh, defw, stmry, stmrx);
	mrendx = stmrx + defw;
	sthstx = mrendx + 2;
	}
	else{
	int extra = (syscount-2)*4;
	if(extra <= 0)
		extra = 0;
	mrendx = (defw*2)+1+extra;
	int mrw = defw + extra;
	sthstx = mrendx + 1;
	data_win = create_newwin(defh, mrw, stmry, stmrx);
	}
	hist_win = create_newwin(defh-2, defw-2, sthsty, sthstx);
	if(cpus == 0){
		const char* match;
		std::string temp;
		file.open("/proc/cpuinfo");
		while(std::getline(file, temp)){
			const char *temp2 = temp.c_str();
			match = strstr (temp2, "cpu MHz");
			if(match != NULL){
				cpus++;
			}
			temp = "";
		}
		file.close();
	}
	stcpux = sthstx + defw;
	if(cpus <= 5){
	cpu_win = create_newwin(defh, cpuw, stcpuy, stcpux);
	}
	else{
		cpuw += (cpus-5);
		cpu_win = create_newwin(defh, cpuw, stcpuy, stcpux);
	}
	move(logsy, logsx);
	attron(COLOR_PAIR(1));
	addch(ACS_ULCORNER);
	for(int i = 0; i < logw-2; i++){
		addch(ACS_HLINE);
	}
	addch(ACS_URCORNER);
	for(int i = 0; i < logh-1; i++){
		move(logsy+1+i, logsx);
		addch(ACS_VLINE);
		move(logsy+1+i, logsx+logw-1);
		addch(ACS_VLINE);
	}
	move(logsy+logh,logsx);
	addch(ACS_LLCORNER);
	for(int i = 0; i<logw-2;i++){
		addch(ACS_HLINE);
	}
	addch(ACS_LRCORNER);
	move(sthsty-1,sthstx-1);
	addch(ACS_ULCORNER);
	for(int i = 0; i < defw-2; i++){
		addch(ACS_HLINE);
	}
	addch(ACS_URCORNER);
	for(int i = 0; i < defh-2; i++){
		move(sthsty+i, sthstx-1);
		addch(ACS_VLINE);
		move(sthsty+i, sthstx-1+defw-1);
		addch(ACS_VLINE);
	}
	move(defh-1,sthstx-1);
	addch(ACS_LLCORNER);
	for(int i = 0; i<defw-2;i++){
		addch(ACS_HLINE);
	}
	addch(ACS_LRCORNER);
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(4));
	move(defh-1, sthstx);
	printw("Past Calls");
	move(logsy, logsx+1);
	printw("Logging");
	attroff(COLOR_PAIR(4));
	//box(screen, 20, 100);
	wborder(log_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wborder(hist_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	//wborder(hist_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	wbkgd(data_win, COLOR_PAIR(1));
	wattron(data_win, COLOR_PAIR(1));
	wborder(data_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	wattroff(data_win, COLOR_PAIR(1));
	wbkgd(rec_win, COLOR_PAIR(1));
	wattron(rec_win, COLOR_PAIR(1));
	wborder(rec_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	wattroff(rec_win, COLOR_PAIR(1));
	wattron(cpu_win, COLOR_PAIR(1));
	wborder(cpu_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	wattroff(cpu_win, COLOR_PAIR(1));
	wrefresh(rec_win);
	wrefresh(data_win);
	scrollok(log_win, true);
	scrollok(hist_win, true);
	wmove(log_win, 0, 0);
	wmove(hist_win, 0,0);
	wrefresh(log_win);
	wrefresh(hist_win);
	wrefresh(cpu_win);
	refresh();
	//wborder(hist_win, 0, 0, 0, 0, 0, 0, 0, 0);
	wrefresh(hist_win);

	
				/* End curses mode		  */
	return 1;
}
void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}
void Tree::CursesError(){
	if(verbose)
	BOOST_LOG_TRIVIAL(error) << "Ncurses Error!";
}
WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

bool Tree::SetupLog(){
	for (int i = 0; i < 100; i++){
		activegroups[i] = 0;
	}
	return true;
}

void Tree::ScrRef(){
	CPU();
	werase(rec_win);
	werase(data_win);
	werase(cpu_win);
	//werase(hist_win);
	int currsyscount = 0;
	for(int i = 0; i < 10; i++){
		if(csysid[i] !=0){
			currsyscount++;
		}
	}
	if(currsyscount != syscount){
	syscount = currsyscount;
	if(syscount > 2)
	DrawBorders();
	}

	wbkgd(rec_win, COLOR_PAIR(1));
	wbkgd(data_win, COLOR_PAIR(1));
	wbkgd(cpu_win, COLOR_PAIR(1));
	//wborder(rec_win, '.', '.', '.','.','.','.','.','.');
	//char test=\342\224\200;
	wborder(rec_win, 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(data_win, 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(cpu_win, 0, 0, 0, 0, 0, 0, 0, 0);
	//box(rec_win, 20,15);
	for (int i = 0; i < 20; i++){
	std::stringstream ss;
	std::stringstream ss2;
	std::string s2;
	std::string s;
	if(i < 9){
	wmove(rec_win, 1+i,2);
	}
	else{
	wmove(rec_win, 1+i,1);
	}
	ss2 << i+1;
	ss2 >> s2;
	const char * d = s2.c_str();
	wattron(rec_win, COLOR_PAIR(4));
	wprintw(rec_win, d);
	wattroff(rec_win, COLOR_PAIR(4));
	wmove(rec_win, 1+i,4);
	if(getcol(i)){
		wattron(rec_win, COLOR_PAIR(currcol));
		if(activegroups[i]!=0){
			ss << activegroups[i];
			ss >> s;
			const char * c = s.c_str();
			if(activegroups[i]!=0)wprintw(rec_win, c);
			wattroff(rec_win, COLOR_PAIR(currcol));
		}
		else {
			wattron(rec_win, COLOR_PAIR(4));
			ss << activegroups[i];
			ss >> s;
			const char * c = s.c_str();
			if(activegroups[i]!=0)wprintw(rec_win, c);
			wattroff(rec_win, COLOR_PAIR(4));
		}
	//		ss.str("");
		//	s = "";
		}
	}
	int datapos = 6;
	wattron(data_win, COLOR_PAIR(4));
	wattron(cpu_win, COLOR_PAIR(4));
	wmove(data_win, 1, 1);
	wmove(cpu_win, 1, 1);
	wprintw(data_win, "100");
	wprintw(cpu_win, "100");
	for(int i = 1; i < 19; i++){
		std::stringstream ss;
		std::string s;
		wmove(data_win, i+1, 2);
		wmove(cpu_win, i+1, 2);
		ss << 100-i*5;
		ss >> s;
		const char * c = s.c_str();
		wprintw(data_win, c);
		wprintw(cpu_win, c);
	}
	wmove(cpu_win, 20, 3);
	wmove(data_win, 20, 3);
	wprintw(data_win, "5");
	wprintw(cpu_win, "5");
	wmove(data_win, 21, 3);
	wmove(cpu_win, 21, 3);
	wprintw(cpu_win, "0");
	wprintw(data_win, "0");
	wmove(data_win, 22, 1);
	wmove(cpu_win, 22, 1);
	wprintw(cpu_win, "CPU");
	wprintw(data_win, "MPS");
	wmove(cpu_win, 29, 1);
	wprintw(cpu_win, "CPU");
	for(int i = 0; i < cpus; i++){
		std::stringstream ss;
		std::string s;
		wmove(cpu_win, 22, 5+i);
		ss << i;
		ss >> s;
		const char * c = s.c_str();
		wprintw(cpu_win, c);
	}
	wmove(cpu_win, 22, 5);
	wprintw(cpu_win, "Freq");
	wmove(cpu_win, 22, 6+cpus);
	wprintw(cpu_win, "151");
	wmove(cpu_win, 23, 6+cpus);
	wprintw(cpu_win, "  5");
	wmove(cpu_win, 24, 6+cpus);
	wprintw(cpu_win, "AVG");
	wattroff(cpu_win, COLOR_PAIR(4));
	wmove(data_win, 29, 1);
	wprintw(data_win, "Message Rate");
	wattroff(data_win, COLOR_PAIR(4));
	wattron(data_win, COLOR_PAIR(2));
	wmove(data_win, 24, 1);
	wprintw(data_win, "Maximum");
	wattroff(data_win, COLOR_PAIR(2));
	wattron(data_win, COLOR_PAIR(1));
	wmove(data_win, 25, 1);
	wprintw(data_win, "Average");
	wattroff(data_win, COLOR_PAIR(1));
	wattron(data_win, COLOR_PAIR(3));
	wmove(data_win, 26, 1);
	wprintw(data_win, "Minimum");
	wattroff(data_win, COLOR_PAIR(3));
	
	for(int i = 0; i < syscount; i++){
		
		std::stringstream ss;
		std::string s;
		wattron(data_win, COLOR_PAIR(4));
		wmove(data_win, 22, datapos-1);
		ss << std::hex << std::uppercase << csysid[i] << std::nouppercase << std::dec;
		ss >> s;
		const char * c = s.c_str();
		wprintw(data_win, c);
		wattroff(data_win, COLOR_PAIR(4));
		if(csysid[i] != 0){
			for(int x = 0; x < 21; x++){
				wmove(data_win, 21-x, datapos);
				if(maxmsg[i] >= (5*x)){
					wattron(data_win, COLOR_PAIR(2));
					//wprintw(data_win, "X");
					waddch(data_win, ACS_CKBOARD);
					wattroff(data_win, COLOR_PAIR(2));
				}
			}
			for(int x = 0; x < 21; x++){
				wmove(data_win, 21-x, datapos);
				if(avgmsg[i] >= (5*x)){
					wattron(data_win, COLOR_PAIR(1));
					//wprintw(data_win, "X");
					waddch(data_win, ACS_CKBOARD);
					wattroff(data_win, COLOR_PAIR(1));
				}
			}
			for(int x = 0; x < 21; x++){
				wmove(data_win, 21-x, datapos);
				if(minmsg[i] >= (5*x)){
					wattron(data_win, COLOR_PAIR(3));
					//wprintw(data_win, "X");
					waddch(data_win, ACS_CKBOARD);
					wattroff(data_win, COLOR_PAIR(3));
				}
			}
		}
		datapos = datapos+4;
	}
	wattron(cpu_win, COLOR_PAIR(3));
	for(int i = 0; i < cpus; i++){
		int cpupos = 5;
		if(Speed[i] != 0){
			for(int x = 0; x < 21; x++){
				wmove(cpu_win, 21-x, cpupos+i);
				if(CPUPer[i] >= (5*x)){
					if(i%2==0)
						waddch(cpu_win, ACS_CKBOARD);
					else
						waddch(cpu_win, ' '|A_REVERSE);
				}
			}
		}
	}
	for(int i = 0; i < 3; i++){
		getloadavg(load, 3);
		for(int x = 0; x < 21; x++){
			wmove(cpu_win, 21-x, 6+cpus+i);
			if(((load[i]/8)*100) >= (5*x)){
				if(i%2!=0)
					waddch(cpu_win, ACS_CKBOARD);
				else
					waddch(cpu_win, ' '|A_REVERSE);
			}
		}
	}
	wattroff(cpu_win, COLOR_PAIR(3));
	wattron(rec_win, COLOR_PAIR(4));
	wmove(rec_win, 29, 1);
	wprintw(rec_win, "Recorders");
	wattroff(rec_win, COLOR_PAIR(4));
		wrefresh(rec_win);
		wrefresh(data_win);
		wrefresh(cpu_win);
}
void Tree::EndWin(){
	endwin();
}
void Tree::setColor(long currid, long tg){
				//for(int i = 0; i < 100; i ++){
				//if(activegroups[i]==tg){	
				//colorsystems[i]=currid;
				//}
				//}
}
void Tree::getSys(long inarr[10]){
	for(int i = 0; i < 10; i++){
	//csysid[i]=inarr[i];
	}
}

bool Tree::getcol(int loc){
	/*for(int i = 0; i < 10; i++){
		if(csysid[i] == colorsystems[loc]){
			currcol = i;
			return true;
		}
	}*/
	if(activegroups[loc]<10000){
		currcol=2;
		return true;
	}
	else if(activegroups[loc] > 50000 && activegroups[loc] < 60000){
		currcol=3;
		return true;
	}
	else if(activegroups[loc] > 60000) {
		currcol=5;
		return true;
	}
	return false;
}
void Tree::msgdata(long sysids[10], int syshistory[10][60], int pos, bool looped){
	if(looped){
	for(int i = 0; i < 10; i++){
		csysid[i] = sysids[i];
	}
	for(int i = 0; i < 10; i++){
		int minm = 100;
		for(int x = 0; x < 60; x++){
			if(syshistory[i][x] < minm){
				minm = syshistory[i][x];
			}
		}
		minmsg[i]=minm;
	}
	for(int i = 0; i < 10; i++){
		int maxm = 0;
		for(int x = 0; x < 60; x++){
			if(syshistory[i][x] > maxm){
				maxm = syshistory[i][x];
			}
		}
		maxmsg[i]=maxm;
	}
	for(int i = 0; i < 10; i++){
		int avgm = 0;
		for(int x = 0; x < 60; x++){
			avgm = avgm + syshistory[i][x];
		}
		avgmsg[i]=avgm/60;
	}
	}
	else{
	for(int i = 0; i < 10; i++){
		csysid[i] = sysids[i];
	}
	for(int i = 0; i < 10; i++){
		int minm = 100;
		for(int x = 0; x < pos; x++){
			if(syshistory[i][x] < minm){
				minm = syshistory[i][x];
			}
		}
		minmsg[i]=minm;
	}
	for(int i = 0; i < 10; i++){
		int maxm = 0;
		for(int x = 0; x < pos; x++){
			if(syshistory[i][x] > maxm){
				maxm = syshistory[i][x];
			}
		}
		maxmsg[i]=maxm;
	}
	for(int i = 0; i < 10; i++){
		int avgm = 0;
		for(int x = 0; x < pos; x++){
			avgm = avgm + syshistory[i][x];
		}
		if(pos != 0)
		avgmsg[i]=avgm/pos;
	}
	}
}
void Tree::CallHist(long tg, long elapsed){
	std::stringstream ss;
	std::string s;
	ss << tg << " " << elapsed << "s\n";
	s = ss.str();
	const char * c = s.c_str();
	wprintw(hist_win, c);
	wrefresh(hist_win);
}

void Tree::DrawBorders(){
	werase(cpu_win);
	werase(hist_win);
	werase(data_win);
	move(sthsty-1,sthstx-1);
	printw(" ");
	for(int i = 0; i < defw-2; i++){
		printw(" ");
	}
	printw(" ");
	for(int i = 0; i < defh-2; i++){
		move(sthsty+i, sthstx-1);
		printw(" ");
		move(sthsty+i, sthstx-1+defw-1);
		printw(" ");
	}
	move(defh-1,sthstx-1);
	printw(" ");
	for(int i = 0; i<defw-2;i++){
		printw(" ");
	}
	printw(" ");
	attroff(COLOR_PAIR(1));
	int extra = (syscount-2)*4;
	mrendx = (defw*2)+1+extra;
	int mrw = defw + extra;
	sthstx = mrendx + 2;
	wresize(data_win, defh, mrw);
	wattron(data_win, COLOR_PAIR(1));
	wborder(data_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	wattroff(data_win, COLOR_PAIR(1));
	
	
	mvwin(hist_win, sthsty, sthstx);
	wborder(hist_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	stcpux = sthstx + defw;
	mvwin(cpu_win, stcpuy, stcpux);
	wattron(cpu_win, COLOR_PAIR(1));
	wborder(cpu_win, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	wattroff(cpu_win, COLOR_PAIR(1));
	wattron(cpu_win, COLOR_PAIR(4));
	wmove(cpu_win, defh-1, 1);
	wprintw(cpu_win, "CPU");
	wattroff(cpu_win, COLOR_PAIR(4));
	move(sthsty-1,sthstx-1);
	attron(COLOR_PAIR(1));
	addch(ACS_ULCORNER);
	for(int i = 0; i < defw-2; i++){
		addch(ACS_HLINE);
	}
	addch(ACS_URCORNER);
	for(int i = 0; i < defh-2; i++){
		move(sthsty+i, sthstx-1);
		addch(ACS_VLINE);
		move(sthsty+i, sthstx-1+defw-1);
		addch(ACS_VLINE);
	}
	move(defh-1,sthstx-1);
	addch(ACS_LLCORNER);
	for(int i = 0; i<defw-2;i++){
		addch(ACS_HLINE);
	}
	addch(ACS_LRCORNER);
	attroff(COLOR_PAIR(1));
	attron(COLOR_PAIR(4));
	move(defh-1, sthstx);
	printw("Past Calls");
	wrefresh(hist_win);
	wrefresh(data_win);
	wrefresh(cpu_win);
	refresh();
	scrollok(hist_win, true);
	wmove(hist_win, 0,0);
}
void Tree::CPU(){
	for(int i = 0; i < 100; i++){
		Speed[i] = 0;
		CPUPer[i]=0;
	}
	int cpu = 0;
	const char* match;
	std::string temp;
	std::ifstream file;
	file.open("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
	//printf("Getting Max\n");
	while(file.good()){
		file >> Max;
		//printf("Max is %ld\n",Max);
		if(Max != 0){
			Max = Max/1000;
			break;
		}
	}
	file.close();
	file.open("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq");
	//printf("Getting Min\n");
	while(file.good()){
		file >> Min;
		//printf("Min is %ld\n",Min);
		if(Min != 0){
			Min = Min/1000;
			break;
		}
	}
	file.close();
	file.open("/proc/cpuinfo");
	while(std::getline(file, temp)){
		const char *temp2 = temp.c_str();
		match = strstr (temp2, "cpu MHz");
		if(match != NULL){
			sscanf (match, "cpu MHz : %ld", &Speed[cpu]);
			cpu++;
		}
		temp = "";
	}
	file.close();
	for(int i = 0; i < 100; i++){
		if(Speed[i]!=0){
			float sp = Speed[i]-Min;
			float mx = Max-Min;
			float per = ((sp/mx)*100);
			CPUPer[i]=per;
		}
	}
}