//
// Created by vosar on 4/22/18.
//

#ifndef BOTNET_BOT1_H
#define BOTNET_BOT1_H

#include <boost/asio.hpp>
//#include <boost/thread/thread.hpp>

using namespace boost::asio;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

class Bot1{
private:

protected:
    //Attrs
    io_service service;
    ip::tcp::endpoint ep;
    int PORT;

    char* target;
    size_t number_of_connections;
    size_t number_of_requests;
    std::string request;

    //fucns
    void sock_connect(socket_ptr sock, ip::tcp::endpoint &ep) {
        sock->connect(ep);
        //boost::lock_guard<boost::mutex> lg(io_mutex);
        //ready_socket = true;
        //cond_var.notify_one();
        //boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
    }

    void my_write(socket_ptr sock_, std::string msg, ip::address &addr) {
        try {
//        boost::unique_lock<boost::mutex> lck(data_mut);
//        cond_var.wait(lck, [] { return ready_socket; });
//
//        boost::lock_guard<boost::mutex> lg(io_mutex);
//        ready_socket = false;
            //    boost::lock_guard<boost::mutex> lg(data_mut);
            size_t res = sock_->write_some(buffer(msg));
        } catch (boost::system::system_error &e) {
            e.what();
            //return (size_t) -1;
        }
    }

public:
    Bot1(char* target, size_t num_of_conn, size_t num_of_req){
        this->target = target;
        this ->number_of_connections = num_of_conn;
        this->number_of_requests = num_of_req;
        //this->ep(ip::address::from_string(std::string(target)), PORT);
        ip::tcp::acceptor acc(service, ep);
    };

    void do_request() {
    }

};


#endif //BOTNET_BOT1_H
