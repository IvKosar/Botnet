//
// Created by vosar on 4/11/18.
//

#ifndef PROJECT_CURLGAMES_H
#define PROJECT_CURLGAMES_H
#include <iostream>
#include <string>
#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Multi.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>
#include <Info.hpp>

#include <curl/curl.h>

int curlGet(void);
int curlppGetWithResp(char* url);
int curlppMulti(char* url, int number_of_requests);
#endif //PROJECT_CURLGAMES_H
