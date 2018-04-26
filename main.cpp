#include <stdlib.h>
#include <unistd.h>

#include "Bot1.h"
#include <iostream>
#include "string"
#include "boost/system/error_code.hpp"

using namespace boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

const std::string addr = "127.0.0.1";

std::string data;

io_service service;
ip::tcp::endpoint ep(ip::address::from_string(addr), 8000);// listen on 8000
ip::tcp::acceptor acc(service, ep);

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


size_t write(ip::tcp::socket& sock_, const std::string & msg) {
    try {
        return sock_.write_some(buffer(msg));
    } catch (boost::system::system_error& e){
        //e.what();
        return (size_t) -1;
    }
}

int main(int argc, char* argv[]) {
//    char *URL = argv[1];
//    int NUM = std::atoi(argv[2]);
//    //curlppGetWithResp(URL);
//    //curlppMulti(URL, NUM);
//
//    Bot bot1(URL, NUM);
//    int fails = bot1.attack();
//    std::cout << "FAILED:" << fails << std::endl;
    char buff[1024];

    io_service service;
    ip::tcp::endpoint ep(ip::address::from_string(argv[1]), 8000);// listen on 8000

    ip::tcp::socket sock(service);
    //connect to target
    sock.connect(ep);
    //write data
    for (size_t i = 0; i < atoi(argv[3]); ++i) {
        size_t res = write(sock, argv[2]);
        if (res < 0) std::cerr << "Unsuccessful write to socket" << std::endl;
    }
    size_t res1 = read(sock);
    std::cout << res1 << std::endl;

    return 0;
}