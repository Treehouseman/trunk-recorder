#ifndef CALL_UPLOADER_H
#define CALL_UPLOADER_H

#include "uploader.h"

class Call;


class System;

struct call_data_t {
	std::string upload_server2;
	std::string scheme2;
	std::string buffpath;
	std::string path2;
	std::string hostname2;
	std::string port2;
	long talkgroup2;
	bool second_server;
	int nac;
	bool isnormal;
        long thread_id;
        long talkgroup;
        double freq;
        long start_time;

        long stop_time;
        bool encrypted;
        bool emergency;
        bool audio_archive;
        char filename[160];
        char converted[160];
        std::string upload_server;
        std::string server;
        std::string scheme;
        std::string hostname;
        std::string port;
        std::string path;
        std::string api_key;
        std::string short_name;
        int tdma_slot;
        bool phase2_tdma;
        long source_count;
        Call_Source source_list[50];
        long freq_count;
        Call_Freq freq_list[50];
        long error_list_count;
        Call_Error error_list[50];
};

void send_call(Call *call, System *sys, Config config);
void send_bcall(Call *call, System *sys, Config config);
void send_ccall(Call *call, System *sys, Config config);

#endif
