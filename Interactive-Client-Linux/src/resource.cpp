/*
 * Copyright (C) 2016 CREATE-NET
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup
 * @{
 *
 * @file        resource.cpp
 * @brief       Resource class
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "resource.h"

#include <unistd.h>
#include <ctime>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
using namespace OC;

Resource::Resource()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    resource = nullptr;
}

Resource::Resource(std::shared_ptr<OC::OCResource> res)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    resource = res;
}

bool Resource::execGET()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    cout << "Performing GET..." << endl;
    if(resource)
    {
        if(getDelay < 0)
        {
             cout << "Delay is lower than 0. Request suppressed." << endl;
             return false;
        }
        QueryParamsMap qpm;
        resource->get(qpm, bind(&Resource::onGetCallback, this, placeholders::_1, placeholders::_2, placeholders::_3));
        getDelayedCallback = new DelayedCallback(getDelay, true, bind(&Resource::getExpirationInternalCallback, this));
        return true;
    }
    return false;
}

bool Resource::execPOST()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    cout << "Performing POST..." << endl;
}

bool Resource::execPUT()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    cout << "Performing PUT..." << endl;
}

bool Resource::execDELETE()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    cout << "Performing DELETE..." << endl;
}

std::shared_ptr<OC::OCResource> Resource::getOCResourceObj()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    return this->resource;
}

void Resource::setOCResourceObj(std::shared_ptr<OC::OCResource> res)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    this->resource = res;
}

string Resource::getURI() const
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    return resource->uri();
}

string Resource::getHost() const
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    return resource->host();
}

void Resource::registerGETCallbacks(OC::GetCallback cb, std::function<void()> expCb, int delay)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    getCallback = cb;
    getExpirationExternalCallback = expCb;
    getDelay = delay;
}

void Resource::getExpirationInternalCallback()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    getExpirationExternalCallback();
}

void Resource::postExpirationInternalCallback()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    postExpirationExternalCallback();
}

void Resource::putExpirationInternalCallback()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    putExpirationExternalCallback();
}

void Resource::deleteExpirationInternalCallback()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    deleteExpirationExternalCallback();
}

void Resource::onGetCallback(const OC::HeaderOptions &hOps, const OC::OCRepresentation &rep, int errorCode)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    getDelayedCallback->stopThread();
    getCallback(hOps, rep, errorCode);
}

bool Resource::operator==(const Resource& res) const
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    const Resource t = *this;
    return (res.getHost() == t.getHost() && t.getURI() == res.getURI());
}
