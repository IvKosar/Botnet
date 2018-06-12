//
// Created by vosar on 6/12/18.
//

#ifndef BOTNET_SESSION_H
#define BOTNET_SESSION_H

#include <boost/asio.hpp>
#include "boost/system/error_code.hpp"

#include <iostream>
#include "vector"
#include "string"
#include <memory>

using namespace boost::asio;

class Session:public std::enable_shared_from_this<Session> {
public:
    io_service service_;
    ip::tcp::endpoint ep_;
    ip::tcp::socket sock;
    std::string request;
    streambuf read_buf;

    explicit Session(io_service serv, ip::tcp::endpoint ep, std::string request):
    {
            this->service_ = serv;
            this->ep_ = ep;
            this->sock(service_)
        this->request = request;
    }

    void connect();

    void handle_connect(const boost::system::error_code& ec);

    void handle_write(const boost::system::error_code& ec, size_t bytes_trans);

    size_t read_complete(const boost::system::error_code & err, size_t bytes);

    void handle_read(const boost::system::error_code& ec, size_t bytes_tr);

    std::string from_buf_to_string(streambuf& streambuf);

};


#endif //BOTNET_SESSION_H
