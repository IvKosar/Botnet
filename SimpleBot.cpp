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
    } else {
        std::cerr << ec.message() << std::endl;
    }

}

void SimpleBot::handle_write(const boost::system::error_code &ec, size_t bytes_trans, socket_ptr sock) {
    if (!ec) {
        std::string response;
        async_read(*(sock.get()), buffer(response), transfer_exactly(1),
                                    [this, sock, response](const boost::system::error_code &ec, size_t bytes_tr) {
                                        handle_read(ec, bytes_tr, sock, std::ref(response));
                                    });
    } else {
        std::cerr << ec.message() << std::endl;
    }
}

void SimpleBot::handle_read(const boost::system::error_code &ec, size_t bytes_tr, socket_ptr sock,
                            const std::string &response) {
    if (!ec) {
        std::cout << "Sent and Received messages are equal: " << response << std::endl;
        sock.get()->close();
    } else {
        std::cerr << ec.message() << std::endl;
    }
}

