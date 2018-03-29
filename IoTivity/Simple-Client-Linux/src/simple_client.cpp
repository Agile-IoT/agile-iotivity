/*******************************************************************************
 * Copyright (C) 2017 Create-Net / FBK.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-2.0/
 * 
 * Contributors:
 *     Create-Net / FBK - initial API and implementation
 ******************************************************************************/
 

/**
 * @ingroup
 * @{
 *
 * @file        simple_client.cpp
 * @brief       This is a simple Linux Client
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@fbk.eu>
 *
 * @}
 */

#include "simple_client.h"
#include <unistd.h>
#include <ctime>
#include <string>
#include <algorithm>
#include <typeinfo>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
using namespace OC;

bool light_state = false;

//Configuration
string addr = "ff03::158";
string res_discovery_uri = string(OC_RSRVD_WELL_KNOWN_URI);
OCConnectivityType connectivityType(CT_ADAPTER_IP);
//End Configuration

//Attributes
SimpleClient *SimpleClient::instance=nullptr;
//End Attributes

//Mutexes
mutex onFindMutex;

SimpleClient::~SimpleClient()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " <<__PRETTY_FUNCTION__ << std::endl;
#endif
}

SimpleClient::SimpleClient()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif

    initializeClient();
}

void SimpleClient::initializeClient()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif

    platformConfig = new PlatformConfig(ServiceType::InProc, //
                        ModeType::Client, //
                        "0.0.0.0", //
                        0, //
                        OC::QualityOfService::HighQos);

    OCPlatform::Configure(*platformConfig);
    resFindCallback = bind(&SimpleClient::onFind, this, placeholders::_1);
}


SimpleClient* SimpleClient::getInstance()
{
#if PRINT_PRETTY_LOGS
       #include <netinet/in.h>
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if(instance==nullptr)
    {
        instance = new SimpleClient();
    }
    return instance;
}

void SimpleClient::startClient(string host)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    OCPlatform::findResource(host.c_str(), //
                             res_discovery_uri.c_str(),
                             connectivityType,
                             resFindCallback,
                             OC::QualityOfService::HighQos);  

    sleep(10); //performing discovery  
}

void SimpleClient::onFind(shared_ptr<OCResource> resource)
{
    onFindMutex.lock();
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    cout << "\nResource Found" << endl;

    if(std::find(resList.begin(), resList.end(), resource) == resList.end())
    {
        resList.push_back(resource);
    } 
    
    onFindMutex.unlock();
}

void SimpleClient::printResourceInfo(shared_ptr<OC::OCResource> resource)
{
    cout << "Host: " << resource->host() << endl;
    cout << "URI: " << resource->uri() << endl;

    cout << "Resource Types:" << endl;
    for (auto & resourceTypes : resource->getResourceTypes())
    {
        cerr << "\t" << resourceTypes << endl;
    }

    cerr << "Resource Interfaces: " << endl;
    for (auto & resourceInterfaces : resource->getResourceInterfaces())
    {
        cerr << "\t" << resourceInterfaces << endl;
    }
}

vector<std::shared_ptr<OC::OCResource>> SimpleClient::getResourceList()
{
    return resList;
}


int SimpleClient::getLightInfo(std::shared_ptr<OC::OCResource> lightRes)
{
    if(lightRes)
    {
         QueryParamsMap test;
         getLightCallback = bind(&SimpleClient::onGetLightInfo, this,placeholders::_1, placeholders::_2, placeholders::_3);
         lightRes->get(test, getLightCallback);
         return 0;
    }
    return -1;
}

void SimpleClient::onGetLightInfo(const HeaderOptions &hOps, const OCRepresentation &rep, int errCode)
{
    if(errCode == OC_STACK_OK)
    {
        bool state=false;
        cout << "GET Successful!" << endl;
        rep.getValue("state", state);
        cout << "State: " << (state?"ON":"OFF") << endl;
        light_state = state;
    }
    else
    {
        cout << "---> Error Code: " << errCode << " <---" <<endl;
    }
}

void SimpleClient::putChangeLightState(std::shared_ptr<OC::OCResource> lightRes, bool state)
{
    if(lightRes)
    {
         OCRepresentation rep;

         cout << "PUT on Light Resource!" << endl;

         QueryParamsMap queryParamsMap;
         rep.setValue("state", state);
         putChangeLightCallback = bind(&SimpleClient::onPutChangeLightState, this,placeholders::_1, placeholders::_2, placeholders::_3);
         lightRes->put(rep, queryParamsMap, putChangeLightCallback);
    }
}

void SimpleClient::onPutChangeLightState(const HeaderOptions &hOps, const OCRepresentation &rep, int errCode)
{
    if(errCode == 4)
    {
        bool state;
        cout << "PUT Successful!" << endl;
    }
    else
    {
        cout << "---> Error Code: " << errCode << " <---" <<endl;
    }
}

bool is_ipv6_address(const string& str)
{
    struct sockaddr_in6 sa;
    return inet_pton(AF_INET6, str.c_str(), &(sa.sin6_addr))!=0;
}

bool is_ipv4_address(const string& str)
{
    struct sockaddr_in sa;
    return inet_pton(AF_INET, str.c_str(), &(sa.sin_addr))!=0;
}

/*****************************************************************************************************/
void print_usage(char* program_name)
{
    cout << "This simple client tries to discover OIC resources. If it discovers an oic.r.light it tries to change its state" << endl;
    cout << endl; 
    cout << "Usage: " << program_name << " <destination> [command]" << endl;
    cout << "\t<destination>: It is the destination address of discovery. It can be a multicast or unicast address" << endl;
    cout << "\tcommand:" << endl;
    cout << "\t\toff: switch off the light (Default)" << endl;
    cout << "\t\ton: switch on the light" << endl;
    cout << "\t\ttoggle: toggle the light" << endl;
}

int main(int argc, char *argv[])
{
    bool state = false;
    bool toggle = false;
    vector<std::shared_ptr<OC::OCResource>> list;
    std::shared_ptr<OC::OCResource> lightRes = nullptr;

    string command;

    if(argc>1)
    {
        addr = std::string(argv[1]);
        if(!is_ipv6_address(addr)&&!is_ipv4_address(addr))
        {
             cerr << addr << " is not a valid IPv4/IPv6 address" <<endl;
             return -1;
        }
    }
    else
    {
        print_usage(argv[0]);
        return -1;
    }

    if(argc>2)
    {
        command = argv[2];
        if(command.compare("on")==0) 
        {
            state=true;
            toggle = false;
        }
        else if(command.compare("toggle")==0) 
        {
            state=false;
            toggle = true;
        }
        else if(command.compare("off")==0) 
        {
            state=false;
            toggle = false;
        }
        else
        {
            cerr << "Input command \"" << command << "\" not recognized!" << endl << endl;
            print_usage(argv[0]);
            return -1;
        }
    }

    SimpleClient *client = SimpleClient::getInstance();

    cout << "Client is Running!" << endl;

    client->startClient(addr);
    list = client->getResourceList();

    cout << "Found " << list.size() << " Resource(s)" <<endl;
   
    if(list.size()==0) return 0; /* Exit if there aren't resources */

    for(auto & resource : list)
    {
        cout << endl;
        client->printResourceInfo(resource);
        for (auto & resourceType : resource->getResourceTypes())
        {
           if(resourceType.compare("oic.r.light")==0)
           {
               cout << "Light resource found!" << endl;
               lightRes = resource;
               break;
           }
        }
    }

    if(lightRes == nullptr)
    {
         cout << "Light resource NOT found" << endl;
         return 0;
    }

    if(toggle)
    {
        client->getLightInfo(lightRes);
        sleep(3);
        client->putChangeLightState(lightRes, !light_state);
        sleep(3);
        client->getLightInfo(lightRes);
        sleep(3);
    }
    else
    {
        client->getLightInfo(lightRes);
        sleep(3);
        client->putChangeLightState(lightRes, state);
        sleep(3);
        client->getLightInfo(lightRes);
        sleep(3);
    }
    return 0;
}
