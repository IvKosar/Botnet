//
// Created by vosar on 4/22/18.
//

#ifndef BOTNET_BOT1_H
#define BOTNET_BOT1_H

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using namespace boost::asio;

class Bot1{
protected:
    //Attrs
    char* target;
    int number_of_requests;

public:
    Bot1(char* target, int num_of_req){
        this->target = target;
        this->number_of_requests = num_of_req;
    };

    void do_request() {
    }

};


#endif //BOTNET_BOT1_H
