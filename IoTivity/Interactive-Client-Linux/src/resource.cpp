/*******************************************************************************
 * Copyright (C) 2017 Create-Net / FBK.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 * 
 * Contributors:
 *     Create-Net / FBK - initial API and implementation
 ******************************************************************************/

/**
 * @ingroup
 * @{
 *
 * @file        resource.cpp
 * @brief       Resource class
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@fbk.eu>
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

#include "boost/any.hpp"

using namespace std;
using namespace OC;

const string Resource::INTERFACE_RW = "oic.if.rw";
const string Resource::INTERFACE_R = "oic.if.r";

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

bool Resource::execPUT(std::map<std::string, boost::any> args)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    cout << "Performing PUT..." << endl;
    if(resource)
    {
        if(putDelay < 0)
        {
             cout << "Delay is lower than 0. Request suppressed." << endl;
             return false;
        }
        
        if(isInterfacePresent(INTERFACE_R))
        {
             cout << "PUT cannot be done on a READABLE resource. Request suppressed." << endl;
             return false;
        }

        QueryParamsMap queryParamsMap;
        OCRepresentation rep = generateOCRepresentation(args);

        putDelayedCallback = new DelayedCallback(putDelay, true, bind(&Resource::putExpirationInternalCallback, this));
        resource->put(rep, queryParamsMap, bind(&Resource::onPutCallback, this, placeholders::_1, placeholders::_2, placeholders::_3));
        return true;
    }
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

void Resource::registerPUTCallbacks(OC::GetCallback cb, std::function<void()> expCb, int delay)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    putCallback = cb;
    putExpirationExternalCallback = expCb;
    putDelay = delay;
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

void Resource::onPutCallback(const OC::HeaderOptions &hOps, const OC::OCRepresentation &rep, int errorCode)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    putDelayedCallback->stopThread();
    putCallback(hOps, rep, errorCode);
}

OCRepresentation Resource::generateOCRepresentation(std::map<std::string, boost::any> args)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    OCRepresentation rep;

    for(auto &param : args)
    {
        try /* booleans */
        {
            bool value = boost::any_cast<bool>(param.second);
            rep.setValue(param.first, value);
#if PRINT_PRETTY_LOGS
            cout << "type: bool name: " << param.first << " value: " << value << endl;
#endif
            continue;
        }
        catch(boost::bad_any_cast &e)
        {
        }

        try /* strings */
        {
            string value = boost::any_cast<string>(param.second);
            rep.setValue(param.first, value);
#if PRINT_PRETTY_LOGS
            cout << "type: string name: " << param.first << " value: " << value << endl;
#endif
            continue;
        }
        catch(boost::bad_any_cast &e)
        {
        }
    
        try /* uint8_t */
        {
            uint8_t value = boost::any_cast<uint8_t>(param.second);
            rep.setValue(param.first, value);
#if PRINT_PRETTY_LOGS
            cout << "type: uint8_t name: " << param.first << " value: " << to_string(value) << endl;
#endif
            continue;
        }
        catch(boost::bad_any_cast &e)
        {
        }

        try /* int8_t */
        {
            int8_t value = boost::any_cast<int8_t>(param.second);
            rep.setValue(param.first, value);
#if PRINT_PRETTY_LOGS
            cout << "type: int8_t name: " << param.first << " value: " << value << endl;
#endif
            continue;
        }
        catch(boost::bad_any_cast &e)
        {
        }

        try /* uint16_t */
        {
            uint16_t value = boost::any_cast<uint16_t>(param.second);
            rep.setValue(param.first, value);
#if PRINT_PRETTY_LOGS
            cout << "type: uint16_t name: " << param.first << " value: " << value << endl;
#endif
            continue;
        }
        catch(boost::bad_any_cast &e)
        {
        }

        try /* int16_t */
        {
            int16_t value = boost::any_cast<int16_t>(param.second);
            rep.setValue(param.first, value);
#if PRINT_PRETTY_LOGS
            cout << "type: int16_t name: " << param.first << " value: " << value << endl;
#endif
            continue;
        }
        catch(boost::bad_any_cast &e)
        {
        }

        try /* int32_t */
        {
            int32_t value = boost::any_cast<int32_t>(param.second);
            rep.setValue(param.first, value);
            cout << "type: int32_t name: " << param.first << " value: " << value << endl;
            continue;
        }
        catch(boost::bad_any_cast &e)
        {
        }
#if PRINT_PRETTY_LOGS
        cout << "Cast failed! Key: " << param.first << endl;
#endif
    }

    return rep;
}

bool Resource::isInterfacePresent(string interface)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    for (auto & resourceInterface : resource->getResourceInterfaces())
    {
        if(interface == resourceInterface)
        {
            return true;
        }
    }
    return false;
}

bool Resource::operator==(const Resource& res) const
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    const Resource t = *this;
    return (res.getHost() == t.getHost() && t.getURI() == res.getURI());
}
