//
// Created by vosar on 4/11/18.
//

#ifndef PROJECT_BOT_H
#define PROJECT_BOT_H

#include "curlGames.h"

class Bot {
protected:
    //Attrs
    char* target;
    int number_of_requests;
public:
    //Constructor
    Bot(char* target, int num_of_req){
        this->target = target;
        this->number_of_requests = num_of_req;
    };

    // getters/setters
    char* get_target(){
        return this->target;
    }

    void set_target(char* new_target){
        this->target = new_target;
    }

    int get_number_of_requests(){
        return this->number_of_requests;
    }

    void set_number_of_requests(int num_of_req){
        this->number_of_requests = num_of_req;
    }

    // Methods
    int attack(){
        // Return number of failed requests

        try {
            curlpp::Cleanup cleaner;

            // create request
            curlpp::Easy *request;
            int nbLeft;
            // create array of requests
            curlpp::Multi requests;
            for (size_t i=0; i<number_of_requests; ++i){
                request = new curlpp::Easy;
                (*request).setOpt(new curlpp::options::Url(target));
                (*request).setOpt(curlpp::options::Verbose(true));
                requests.add(request);

            }

            /* we start some action by calling perform right away */
            while(!requests.perform(&nbLeft)) {};

            while(nbLeft) {
                struct timeval timeout;
                int rc; /* select() return code */

                fd_set fdread;
                fd_set fdwrite;
                fd_set fdexcep;
                int maxfd;

                FD_ZERO(&fdread);
                FD_ZERO(&fdwrite);
                FD_ZERO(&fdexcep);

                /* set a suitable timeout to play around with */
                timeout.tv_sec = 1;
                timeout.tv_usec = 0;

                /* get file descriptors from the transfers */
                requests.fdset(&fdread, &fdwrite, &fdexcep, &maxfd);

                rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);

                switch(rc) {
                    case -1:
                        /* select error */
                        nbLeft = 0;
                        printf("select() returns error, this is badness\n");
                        break;
                    case 0:
                    default:
                        /* timeout or readable/writable sockets */
                        while(!requests.perform(&nbLeft)) {};
                        break;
                }
            }

            std::cout << "NB lefts: " << nbLeft << std::endl;

            /* See how the transfers went */
            /*
               Multi::info returns a list of:
               std::pair< curlpp::Easy, curlpp::Multi::Info >
            */
            int number_of_fails = 0;
            curlpp::Multi::Msgs msgs = requests.info();
            for(curlpp::Multi::Msgs::iterator pos = msgs.begin();
                pos != msgs.end();
                pos++) {
                if(pos->second.msg == CURLMSG_DONE) {
                    //requests.
                    printf("Some request done with code %ld\n", curlpp::infos::ResponseCode::get(*pos->first));
                    if (curlpp::infos::ResponseCode::get(*pos->first) == 0) ++number_of_fails;
                }
            }
            return number_of_fails;
        }
        catch ( curlpp::LogicError & e ) {
            std::cout << e.what() << std::endl;
        }
        catch ( curlpp::RuntimeError & e ) {
            std::cout << e.what() << std::endl;
        }
        return -1;
    }
};


#endif //PROJECT_BOT_H
