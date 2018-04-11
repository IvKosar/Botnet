#include "curlGames.h"
#include <stdlib.h>
#include "Bot.h"


/**
 * This example is made to show you how you can use the Options.
 */
int main(int argc, char* argv[])
{
    char *URL = argv[1];
    int NUM = std::atoi(argv[2]);
    //curlppGetWithResp(URL);
    //curlppMulti(URL, NUM);

    Bot bot1(URL, NUM);
    int fails = bot1.attack();
    std::cout << "FAILED:" << fails << std::endl;

    return 0;
}