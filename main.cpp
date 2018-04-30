#include <stdlib.h>
#include <unistd.h>

#include "Bot1.h"
#include <iostream>
#include <thread>
#include "vector"
#include "string"

#include "boost/system/error_code.hpp"
#include <boost/thread/thread.hpp>
#include <boost/functional.hpp>

const int DELAY = 5;

//using namespace boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

boost::mutex data_mut;
boost::mutex io_mutex;
boost::condition_variable cond_var;
bool ready_socket = false;

//
//const std::string addr = "127.0.0.1";
//
//std::string data;
//
//io_service service;
//ip::tcp::endpoint ep(ip::address::from_string(addr), 8000);// listen on 8000
//ip::tcp::acceptor acc(service, ep);

/*struct talk_to_svr
{
    talk_to_svr(const std::string & username): sock_(service), started_(true), username_(username) {}
    void connect(ip::tcp::endpoint ep)
    {
        sock_.connect(ep);
    }
    void loop()
    {
        write("login " + username_ + "\n");
        read_answer();
//        while ( started_)
//        {
//            write_request();
//            read_answer();
//            boost::this_thread::sleep(5);
//        }
    }

    void write(const std::string & msg) { sock_.write_some(buffer(msg)); }
    size_t read_complete(char * buf, const boost::system::error_code & err, size_t bytes)
    {
        if ( err) return 0;
        bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }

    void write_request()
    {
        write("ping\n");
    }
    void read_answer()
    {
        already_read_ = 0;
        read(sock_, buffer(buff_),
             boost::bind(&talk_to_svr::read_complete, this, _1, _2));
        process_msg();
    }
    void process_msg()
    {
        std::string msg(buff_, already_read_);
//        if ( msg.find("login ") == 0) on_login();
//        else if ( msg.find("ping") == 0) on_ping(msg);
//        else if ( msg.find("clients ") == 0) on_clients(msg);
//        else std::cerr << "invalid msg " << msg << std::endl;
    }
    std::string username() const { return username_; }
private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    int already_read_;
    char buff_[max_msg];
    bool started_;
    std::string username_;
};*/

//size_t read_complete(char * buf, const boost::system::error_code & err, size_t bytes)
//{
//    if ( err) return 0;
//    bool found = std::find(buf, buf + bytes, '\n') < buf + bytes;
//    // we read one-by-one until we get to enter, no buffering
//    return found ? 0 : 1;
//}
//
//std::string process_msg(std::string& msg) {
//    return msg;
//}
//
//size_t read_answer(ip::tcp::socket& sock_)
//{
//    char buff_[1024];
//    size_t already_read_ = 0;
//    try {
//        read(sock_, buffer(buff_),
//             boost::bind(read_complete, _1, _2));
//    }catch(boost::system::system_error& e){
//        return 1;
//    }
//    process_msg(std::ref(std::string(buff_)));
//    return 0;
//}
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
//    char *URL = argv[1];
//    int NUM = std::atoi(argv[2]);
//    //curlppGetWithResp(URL);
//    //curlppMulti(URL, NUM);
//
//    Bot bot1(URL, NUM);
//    int fails = bot1.attack();
//    std::cout << "FAILED:" << fails << std::endl;
    io_service service;
    ip::address target = ip::address::from_string(argv[1]);
    ip::tcp::endpoint ep(target, 2021);
    ip::tcp::acceptor acc(service, ep);
    const int socket_number = atoi(argv[3]);

    //std::vector<boost::thread> connection_threads;

    // writing threads
    std::string msg = std::string(argv[2]);
    std::vector<boost::thread> write_threads;

    // connection
    socket_ptr sockets[socket_number];
    for (size_t i = 0; i < socket_number; ++i) {
        socket_ptr sock(new ip::tcp::socket(service));
        sockets[i] = sock;
        //connection_threads.emplace_back(boost::thread(sock_connect, sock, boost::ref(ep) ));
        sock_connect(sock, boost::ref(ep));
        boost::this_thread::sleep_for(boost::chrono::milliseconds(DELAY));
    }

    //write data
    size_t sock_indx;
    for (size_t i = 0; i < socket_number; ++i) {
        sock_indx = i % socket_number;
        //msg.append("A");
        //write_threads.emplace_back(boost::thread(my_write, sockets[sock_indx], std::string(msg), boost::ref(target) ));
        my_write(sockets[sock_indx], std::string(msg), boost::ref(target));
    }

//    for (auto &thr : connection_threads) {
//        thr.join();
//    }
//    for (auto &thr : write_threads) {
//        thr.join();
//    }


    return 0;
}
