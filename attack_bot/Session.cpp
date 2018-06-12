//
// Created by vosar on 6/12/18.
//

#include "Session.h"

void Session::connect() {
    this->sock.async_connect(this->ep_,
                              [this](const boost::system::error_code &ec) {
                                  handle_connect(ec);
                              });
}

void Session::handle_connect(const boost::system::error_code &ec) {
    if (!ec) {
        size_t bytes_tr;
        this->sock.async_write_some(buffer(this->request),
                                     [this, sock](const boost::system::error_code &ec, size_t bytes_trans) {
                                         handle_write(ec, bytes_trans);
                                     });
        //write(*sock, buffer(this->request));
    } else {
        std::cerr << ec.message() << std::endl;
    }

}

void Session::handle_write(const boost::system::error_code &ec, size_t bytes_trans) {
    if (!ec) {
//        std::thread reader(show_read, sock, this->mut_);
//        reader.detach();
        //boost::asio::streambuf read_buf;
        async_read_until(*(this->sock), this->read_buf, "/",

                         [this](const boost::system::error_code &ec, size_t bytes_tr) {
                             handle_read(ec, bytes_tr);
                         });
    } else {
        std::cerr << ec.message() << std::endl;
    }
}

//[this](const boost::system::error_code & ec, size_t bytes){
//read_complete(ec, bytes);
//},

void Session::handle_read(const boost::system::error_code &ec, size_t bytes_tr) {
    if (!ec) {
        std::cout << "Received bytes: " << bytes_tr << std::endl;
        std::string res = from_buf_to_string(this->read_buf);
        std::cout << "Received message " << std::string(res.begin(), res.begin()+bytes_tr) << std::endl;
        this->sock.close();
    } else {
        std::cerr << ec.message() << std::endl;
    }
}

size_t Session::read_complete(const boost::system::error_code & err, size_t bytes)
{
    if ( err) return 0;
    std::string buf = from_buf_to_string(this->read_buf);
    ssize_t found = buf.find("/");
    return (found < buf.length()) ? 0 : 1;
}

std::string Session::from_buf_to_string(boost::asio::streambuf& streambuf){
    return {boost::asio::buffers_begin(streambuf.data()),
            boost::asio::buffers_end(streambuf.data())};
}