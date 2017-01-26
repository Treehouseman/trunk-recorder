#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <string>

struct Config {
        std::string upload_script;
        std::string upload_server;
        std::string capture_dir;
		std::string upload_server2;
		std::string buffpath;
        int call_timeout;
};

#endif
