//
// Created by vosar on 4/11/18.
//

#include "SimpleBot.h"

SimpleBot::SimpleBot(char *target, char *port, char *request, char *socket_number) {
    this->target = ip::address::from_string(target);;
    this->port = (size_t) atoi(port);
    this->ep = ip::tcp::endpoint(this->target, this->port);
    this->request = std::string(request);
    this->socket_number = (size_t) atoi(socket_number);
    this->delim = std::string("/");
}

void SimpleBot::attack() {
    run();
    std::vector<std::thread> threads;
    for (size_t i = 0; i < this->NUMBER_OF_THREADS; ++i) {
        threads.push_back(std::thread(
                [i, this]() {
                    runner(i);
                }
        ));
    }
    boost::this_thread::sleep(boost::posix_time::millisec(500));

    for (auto &thr : threads) {
        thr.join();
    }
}

void SimpleBot::runner(size_t indx) {
    this->services[indx].run();
}

void SimpleBot::run() {
    size_t portion = std::max<size_t>(this->socket_number / NUMBER_OF_THREADS, 1), conns_left = this->socket_number, indx = 0;
    do {
        if (portion > conns_left) portion = conns_left;
        for (size_t i = 0; i < portion; ++i) {
            socket_ptr sock(new ip::tcp::socket(this->services[indx]));

//            deadline_ptr deadline_(new deadline_timer(sock.get()->get_io_context()));
//            deadline_.get().e
//            check_deadline(deadline_, sock);

            connect(sock);
        }
        conns_left -= portion;
        indx = ++indx % NUMBER_OF_THREADS;
    }while(conns_left > 0);
}

void SimpleBot::connect(socket_ptr sock) {
    sock.get()->async_connect(this->ep,
                              [this, sock](const boost::system::error_code &ec) {
                                  handle_connect(ec, sock);
                              });
}

void SimpleBot::handle_connect(const boost::system::error_code &ec, socket_ptr sock) {
    if (!ec) {
        size_t bytes_tr;
        sock.get()->async_write_some(buffer(this->request),
                                     [this, sock](const boost::system::error_code &ec, size_t bytes_trans) {
                                         handle_write(ec, bytes_trans, sock);
                                     });
        //write(*sock, buffer(this->request));
    }else if (ec == error::timed_out || ec == error::connection_refused){
        std::cout << "REFUSED" << std::endl;
        on_down(sock);
    }
    else {
        std::cerr << ec.message() << std::endl;
    }

}

void SimpleBot::handle_write(const boost::system::error_code &ec, size_t bytes_trans, socket_ptr sock) {
    if (!ec) {
        deadline_ptr deadline(new deadline_timer(sock.get()->get_io_context(),
                                                 microsec_clock::universal_time() + this->timeout));
        deadline.get()->async_wait(
                    [this, sock, deadline](const boost::system::error_code& e) {
                        on_timeout(e, sock, deadline);
                    });

        async_read_until(*(sock.get()), this->read_buf, this->delim,
                         [this, sock, deadline](const boost::system::error_code &ec, size_t bytes_tr) {
                             handle_read(ec, bytes_tr, sock, deadline);
                         });

//        async_read_until(*(sock.get()), this->read_buf, this->delim,
//                         [this, sock](const boost::system::error_code &ec, size_t bytes_tr) {
//                             handle_read(ec, bytes_tr, sock);
//                         });
    }else if (ec == error::connection_reset){
        std::cout << "RESET" << std::endl;
        on_down(sock);
    }

    else {
        std::cerr << "Mess " << ec.message() << std::endl;
    }
}

void SimpleBot::on_timeout(const boost::system::error_code& e, socket_ptr sock, deadline_ptr deadline){
    if (e != boost::asio::error::operation_aborted)
    {
        std::cout << "TIMEOUT" << std::endl;
        on_down(sock);
    }
}

//[this](const boost::system::error_code & ec, size_t bytes){
//read_complete(ec, bytes);
//},

void SimpleBot::on_down(socket_ptr sock){
    sock.get()->close();
    for (auto& srv : this->services){
        srv.stop();
    }
    this->serv_failed = true;
}

void SimpleBot::handle_read(const boost::system::error_code &ec, size_t bytes_tr, socket_ptr sock, deadline_ptr deadline) {
    if (!ec) {
        deadline.get()->cancel_one();
        std::cout << "Received bytes: " << bytes_tr << std::endl;
        std::string res = from_buf_to_string(this->read_buf);
        std::cout << "Received message " << std::string(res.begin(), res.begin()+bytes_tr) << std::endl;
        sock.get()->close();
    } else if (ec == error::operation_aborted || ec == error::eof) {
        std::cout << "EOF" << std::endl;
       on_down(sock);
    }
    else{
        std::cerr <<"Mess "<< ec.message() << std::endl;
    }
}


std::string SimpleBot::from_buf_to_string(boost::asio::streambuf& streambuf){
    return {boost::asio::buffers_begin(streambuf.data()),
            boost::asio::buffers_end(streambuf.data())};
}