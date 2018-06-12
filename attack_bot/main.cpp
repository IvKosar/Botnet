#include <stdlib.h>
#include <unistd.h>

#include "SimpleBot.h"
#include <boost/timer/timer.hpp>

const int DELAY = 5;

int main(int argc, char *argv[]) {
    /*
     * Args: IP, Port, Request_Message, # of requests
     */
    SimpleBot bot(argv[1], argv[2], argv[3], argv[4]);
    boost::timer::auto_cpu_timer timer;
    bot.attack();

    return 0;
}
