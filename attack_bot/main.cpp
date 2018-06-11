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


//    char *URL = argv[1];
//    int NUM = std::atoi(argv[2]);
//    //curlppGetWithResp(URL);
//    //curlppMulti(URL, NUM);
//
//    Bot bot1(URL, NUM);
//    int fails = bot1.attack();
//    std::cout << "FAILED:" << fails << std::endl;
//    io_service service;
//    ip::address target = ip::address::from_string(argv[1]);
//    ip::tcp::endpoint ep(target, 2021);
//    ip::tcp::acceptor acc(service, ep);
//    const int socket_number = atoi(argv[3]);
//
//    //std::vector<boost::thread> connection_threads;
//
//    // writing threads
//    std::string msg = std::string(argv[2]);
//    std::vector<boost::thread> write_threads;
//
//    // connection
//    socket_ptr sockets[socket_number];
//    for (size_t i = 0; i < socket_number; ++i) {
//        socket_ptr sock(new ip::tcp::socket(service));
//        sockets[i] = sock;
//        //connection_threads.emplace_back(boost::thread(sock_connect, sock, boost::ref(ep) ));
//        sock_connect(sock, boost::ref(ep));
//        boost::this_thread::sleep_for(boost::chrono::milliseconds(DELAY));
//    }
//
//    //write data
//    size_t sock_indx;
//    for (size_t i = 0; i < socket_number; ++i) {
//        sock_indx = i % socket_number;
//        //msg.append("A");
//        //write_threads.emplace_back(boost::thread(my_write, sockets[sock_indx], std::string(msg), boost::ref(target) ));
//        my_write(sockets[sock_indx], std::string(msg), boost::ref(target));
//    }

//    for (auto &thr : connection_threads) {
//        thr.join();
//    }
//    for (auto &thr : write_threads) {
//        thr.join();
//    }


    return 0;
}
