#include <stdlib.h>
#include <unistd.h>

#include "SimpleBot.h"

const int DELAY = 5;

int main(int argc, char *argv[]) {
    /*
     * Args: IP, Port, Request_Message, # of requests
     */
    SimpleBot bot(argv[1], argv[2], argv[3], argv[4]);
    ptime start = boost::posix_time::microsec_clock::universal_time();
    bot.attack();
    ptime finish = boost::posix_time::microsec_clock::universal_time();
    if(bot.serv_failed){
        std::cout << "SERVER KILLED" << std::endl;
    }
    boost::posix_time::time_duration td = finish - start;
    std::cout << "Time spent: " << td.total_milliseconds() << " millisec" << std::endl;
    return 0;
}
