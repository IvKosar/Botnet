#include "curlGames.h"

int curlGet(void) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
        /* always cleanup */
        curl_easy_cleanup(curl);
        int resCode = res;
        return resCode;
    }
    return -1;
}

int curlppGetWithResp(char* url){
    const long MyPort = 8000;
    try
    {
        curlpp::Cleanup myCleanup;

        // First easy example.
        {
            // The first easiest example is to retreive the content of
            // a web page and put it in a stream.
            //std::cout << URL <<std::endl;
            std::cout << "RESPONSE " << curlpp::options::Url(url) << " END"<< std::endl;

            // You don't need to use just the standard outputs. You
            // can use any stream:
            std::ostringstream os;
            os << curlpp::options::Url(url);
        }

        // More elaborate example.
        {
            // What the previous example done there was simply
            // to create a curlpp::Easy class, which is the basic
            // object in cURLpp, and then set the Url option.
            // curlpp::options classes are the primitives that allow to specify
            // values to the requests.
            curlpp::options::Url myUrl(url);
            curlpp::Easy myRequest;
            myRequest.setOpt(myUrl);

            // Now that all the options we wanted to set are there, we need to
            // actually do the request. the "perform" method does actually that.
            // With that call, the request will be done and the content of that URL
            // will be printed in std::cout (which is the default).
            myRequest.perform();
            std::cout << "Response code 0: "
                      << curlpp::infos::ResponseCode::get(myRequest)
                      << std::endl;

            // If we wanted to put the content of the URL within a string stream
            // (or any type of std::ostream, for that matter), like the first example,
            // we would use the WriteStrem option like this:
            std::ostringstream os;
            curlpp::options::WriteStream ws(&os);
            myRequest.setOpt(ws);
            myRequest.perform();

            // There is some shorcut within curlpp that allow you to write shorter code
            // like this:
            os << myRequest;

            // That would do exactly what the previous code was doing.
        }

        // Creation of the URL option.
        curlpp::options::Url myUrl(url);

        // Copy construct from the other URL.
        curlpp::options::Url myUrl2(myUrl);

        // Creation of the port option.
        curlpp::options::Port myPort(MyPort);

        // Creation of the request.
        curlpp::Easy myRequest;

        // Creation of an option that contain a copy of the URL option.
        curlpp::OptionBase *mytest = myUrl.clone();
        myRequest.setOpt(*mytest);

        // You can reuse the base option for other type of option
        // and set the option to the request. but first, don't forget
        // to delete the previous memory. You can delete it since the
        // option is internally duplicated for the request.
        delete mytest;
        mytest = myPort.clone();
        myRequest.setOpt(*mytest);
        delete mytest;

        // You can clone an option directly to the same type of
        // option.
        curlpp::options::Url *myUrl3 = myUrl.clone();
        myRequest.setOpt(myUrl3);
        // Now myUrl3 is owned by the request we will NOT use
        // it anymore.

        //Efficient
        myRequest.setOpt(new curlpp::options::Url("example.com"));
        // Note that with this the request will use directly this
        // instance we just created. Be aware that if you pass an
        // Option pointer to the setOpt function, it will consider
        // the instance has its own instance. The Option instance
        // will be deleted when the request will be deleted, so
        // don't use the instance further in your code.


        // Doing the previous line is efficient as this:
        myRequest.setOpt(myUrl.clone());


        // You can retreive the value of a specific option.
        std::cout << "URL " << myUrl2.getValue() << " END"<< std::endl;

        // Perform the transaction with the options set.
        myRequest.perform();
        std::cout << "Response code 1: "
                  << curlpp::infos::ResponseCode::get(myRequest)
                  << std::endl;

    }

    catch( curlpp::RuntimeError &e )
    {
        std::cout << e.what() << std::endl;
    }

    catch( curlpp::LogicError &e )
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

int curlppMulti(char* url, int number_of_requests){
    try {
        curlpp::Cleanup cleaner;

        /*curlpp::Easy request1;
        curlpp::Easy request2;

        request1.setOpt(new curlpp::options::Url(url1));
        request1.setOpt(new curlpp::options::Verbose(true));

        request2.setOpt(new curlpp::options::Url(url2));
        request2.setOpt(new curlpp::options::Verbose(true));*/

        curlpp::Easy *request;
        int nbLeft;
        curlpp::Multi requests;
        for (size_t i=0; i<number_of_requests; ++i){
            request = new curlpp::Easy;
            (*request).setOpt(new curlpp::options::Url(url));
            (*request).setOpt(curlpp::options::Verbose(true));
            requests.add(request);

        }
        /*requests.add(&request1);
        requests.add(&request2);*/

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
        curlpp::Multi::Msgs msgs = requests.info();
        for(curlpp::Multi::Msgs::iterator pos = msgs.begin();
            pos != msgs.end();
            pos++) {
            if(pos->second.msg == CURLMSG_DONE) {
                //requests.
                printf("Some request done with code %ld\n", curlpp::infos::ResponseCode::get(*pos->first));
            }
        }
}
    catch ( curlpp::LogicError & e ) {
        std::cout << e.what() << std::endl;
    }
    catch ( curlpp::RuntimeError & e ) {
        std::cout << e.what() << std::endl;
    }


    return 0;
}