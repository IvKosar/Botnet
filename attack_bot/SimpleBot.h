//
// Created by vosar on 4/11/18.
//

#ifndef PROJECT_BOT_H
#define PROJECT_BOT_H

#include <iostream>
#include "vector"
#include "string"
#include <algorithm>
#include <thread>

#include <boost/asio.hpp>
#include "boost/system/error_code.hpp"
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>


using namespace boost::asio;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

class SimpleBot{
public:
    const static size_t NUMBER_OF_THREADS = 4;//const_cast<unsigned int&>(boost::thread::hardware_concurrency());
    io_service services[NUMBER_OF_THREADS];
    ip::tcp::endpoint ep;
    char read_buf_[256];
    boost::asio::streambuf read_buf;

    void runner(size_t indx);

    void run();

    void connect(socket_ptr sock);

    void handle_connect(const boost::system::error_code& ec, socket_ptr sock);

    void handle_write(const boost::system::error_code& ec, size_t bytes_trans, socket_ptr sock);

    size_t read_complete(const boost::system::error_code & err, size_t bytes);

    void handle_read(const boost::system::error_code& ec, size_t bytes_tr, socket_ptr sock);

    std::string from_buf_to_string(boost::asio::streambuf& streambuf);

public:
    ip::address target;
    size_t port;
    std::string request;
    size_t socket_number;

    SimpleBot(char* target, char* port, char* request, char* socket_number);
    void attack();

};


#endif //PROJECT_BOT_H
