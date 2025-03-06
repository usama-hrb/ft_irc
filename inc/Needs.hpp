#pragma once

#include <iostream>
#include <vector> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#include <poll.h> 
#include <csignal> 
#include <map>
#include <cstring>
#include <sstream>
#include "ReplyCodes.hpp"


#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"
#define END "\e[0;0m"
#define BLU "\e[1;34m"    // Bold Blue
#define MAG "\e[1;35m"    // Bold Magenta
#define CYA "\e[1;36m"    // Bold Cyan
#define BLA "\e[0;30m"    // Black (non-bold)
#define GRA "\e[1;90m"    // Bold Gray
#define PUR "\e[0;95m"    // Purple (non-bold)
#define ORA "\e[1;91m"    // Bold Orange (often a bright red shade)

extern volatile sig_atomic_t g_keepRunning;