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
 * @file        oic_client.cpp
 * @brief       This is a generic IoTivity Linux Client
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "resource.h"
#include "client.h"

#include <unistd.h>
#include <ctime>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <iostream>
#include <vector>
#include <termios.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
using namespace OC;

//Attributes
Client *Client::instance=nullptr;
OCConnectivityType connectivityType(CT_ADAPTER_IP);
const string Client::URI_DEVICE = "/oic/d";
//End Attributes

Client::Client()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
}

void Client::printMessage(string mex)
{
    cout << "Client: " << mex << endl;
}

Client* Client::getInstance()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if(!instance)
    {
        instance = new Client();
    }

    return instance;
}

void Client::startClient(string da)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif

    bool discovery_done = false;

    if(started)
    {
        cerr << "Client already started...Exiting..." << endl;
        return;
    }
    started = true;
    dest_addr = da;
    printMessage("destination address is " + dest_addr);
    printMessage("Starting...");

    /* Configure Platform */
    platformConfig = new PlatformConfig(ServiceType::InProc, //
                        ModeType::Client, //
                        "0.0.0.0", //
                        0, //
                        OC::QualityOfService::HighQos);
    OCPlatform::Configure(*platformConfig);
    resDiscoveryCallback = bind(&Client::onDiscovery, this, placeholders::_1);

    discovery_done = doDiscovery(4);

    if(discovery_done && list.size()>0)
    {
        printMessage(string("There are ")+to_string(list.size())+string(" resource(s) in cache"));
        main();
    }
    else
    {
        printMessage("0 resources found! Exiting...");
        return;
    }
}

bool Client::doDiscovery(int delay)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif   

    if(delay<=0) return false;

    OCPlatform::findResource(dest_addr.c_str(), //
                             res_discovery_uri.c_str(),
                             connectivityType,
                             resDiscoveryCallback,
                             OC::QualityOfService::HighQos);  

    sleep(delay); //performing discovery 
    return true; 
}

void Client::onDiscovery(std::shared_ptr<OC::OCResource> resource)
{
    onDiscoveryMutex.lock();
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    Resource *tmp = new Resource(resource);
    if(std::find(list.begin(), list.end(), *tmp) != list.end())
    {
        printMessage("Resource Found: " + tmp->getHost() + tmp->getURI());
    } 
    else
    {
        //Resource not present
        list.push_back(*tmp);
        printMessage("NEW Resource Found: " + tmp->getHost() + tmp->getURI());
    }
    onDiscoveryMutex.unlock();
}

void Client::main()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    int c;
    bool discovery_done;

    do
    {
        system("clear");
        cout << "++================================================++" << endl;
        cout << "||                                                ||" << endl;
        cout << "||                 IoTivity CLIENT                ||" << endl;
        cout << "||                                                ||" << endl;
        cout << "++================================================++" << endl;
        cout << endl;
        cout << "Destination Address: " << dest_addr << endl;
        cout << "Resource(s) discovered: " << list.size() << endl;
        cout << endl;
        cout << "What do you want to do?" << endl;
        cout << OPT_DISCOVERY_ALL << " - Discovery ALL" << endl;
        cout << OPT_SHOW_RES_CACHE << " - Print Resources information (From Cache)" << endl;
        cout << OPT_SHOW_ALL_DEVICES << " - Show all Devices (From Nodes)" << endl; 
        cout << OPT_INTERACT_RES << " - Interact with Resources" << endl;
        cout << endl;
        cout << OPT_EXIT << " - Exit" << endl;
        c = int(readCharFromTTY())-48;

        while(c<0 || c>4)
        {
            printMessage("Option not valid!");
            c = int(readCharFromTTY()) - 48;
        }

        switch(c)
        {
            case OPT_DISCOVERY_ALL:
                cout << endl;
                printMessage("Discovery...");
                discovery_done = doDiscovery(5);
                if(discovery_done)
                {
                    printMessage(string("There are ")+to_string(list.size())+string(" resource(s) in cache"));
                }
                else
                {
                    printMessage("Error during discovery...");
                }
                cout << endl;
                printMessage("Press any key to return to main menu...");
                readCharFromTTY(false);
                break;

            case OPT_SHOW_RES_CACHE:
                cout << endl;
                printMessage("Resource(s) discovered:");
                for (auto & resource : list)
                {
                    cout << endl;
                    printResourceInfo(resource.getOCResourceObj());
                }
                cout << endl;
                printMessage("Press any key to return to main menu...");
                readCharFromTTY(false);
                break;

            case OPT_SHOW_ALL_DEVICES:
                cout << endl;
                printMessage("Device(s) discovered:");
                
                for (auto & resource : list)
                {
                    if(resource.getURI() == URI_DEVICE)
                    {
                        onGetMutex.lock();
                        resource.registerGETCallbacks(bind(&Client::onGetDevice, this,placeholders::_1, placeholders::_2, placeholders::_3), bind(&Client::onGetTimeout, this), 5000);
                        resource.execGET();
                        onGetMutex.lock();
                        onGetMutex.unlock();
                    }
                }
                cout << endl;
                printMessage("Press any key to return to main menu...");
                readCharFromTTY(false);
                break;

            case OPT_INTERACT_RES:
                cout << endl;
                interactResources();
                break;

            case OPT_EXIT:
                break;

            default:
                printMessage("Unknown option");
                printMessage("Press any key to return to main menu...");
                readCharFromTTY(false);
        }
    }
    while(c != 0);
    printMessage("Exiting...");
    return;
}

void Client::interactResources()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif

    int res_id;
    int op;
    int c;
    int i;
    Resource res;

    do
    {
        i = 1;
        system("clear");
        cout << "++================================================++" << endl;
        cout << "||                                                ||" << endl;
        cout << "||             Interact with Resources            ||" << endl;
        cout << "||                                                ||" << endl;
        cout << "++================================================++" << endl;
        cout << endl;
        cout << "Choose a resource:" << endl;

         
        for (auto & resource : list)
        {
            cout << i << " - " << resource.getHost() << resource.getURI() << endl;
            i++; 
        }

        cout << endl;
        cout << OPT_EXIT << " - Return to Main Menu" << endl;
        c = int(readCharFromTTY())-48;

        if(c>0 && c < i)
        {
            res_id = c-1;
            res = list.at(res_id);
            cout << "You have selected: " <<res.getHost() << res.getURI() << endl;
            cout << "Choose an operation:" << endl;
            cout << OP_GET << " -  Retrieve (GET)" << endl;
            cout << OP_POST << " -  Create (POST)" << endl;
            cout << OP_PUT << " -  Update (PUT)" << endl;
            cout << OP_DELETE << " -  Delete (DELETE)" << endl;
            cout << endl;
            cout << OP_ANOTHER_RES << " - Choose another resource" << endl;
            c = int(readCharFromTTY())-48; 

            while(c<0 || c>4)
            {
                cout << "Unknown choice!" << endl;
                c = int(readCharFromTTY())-48; 
            }

            switch(c)
            {
                case OP_GET:
                    onGetMutex.lock();
                    res.registerGETCallbacks(bind(&Client::onGetGeneric, this,placeholders::_1, placeholders::_2, placeholders::_3), bind(&Client::onGetTimeout, this), 5000);
                    res.execGET();
                    onGetMutex.lock();
                    printMessage("Press any key to return to continue...");
                    readCharFromTTY(false);
                    onGetMutex.unlock();
                    break;
                case OP_POST:
                    printMessage("POST Not Implemented yet!");
                    printMessage("Press any key to return to continue...");
                    readCharFromTTY(false);
                    break;
                case OP_PUT:
                    printMessage("PUT Not Implemented yet!");
                    printMessage("Press any key to return to continue...");
                    readCharFromTTY(false);
                    break;
                case OP_DELETE:
                    printMessage("DELETE Not Implemented yet!");
                    printMessage("Press any key to return to continue...");
                    readCharFromTTY(false);
                    break;
                case OP_ANOTHER_RES:
                    c++;
                    break;
                default:
                    cout << "Unknown choice!" << endl;
            }
        } 
    }
    while(c != 0);
    return;
}

char Client::readCharFromTTY(bool showMessage)
{
    struct termios old_tio, new_tio;
    unsigned char c;

    /* get the terminal settings for stdin */
    tcgetattr(STDIN_FILENO,&old_tio);

    /* we want to keep the old setting to restore them a the end */
    new_tio=old_tio;

    /* disable canonical mode (buffered i/o) */
    new_tio.c_lflag &=(~ICANON);

    /* set the new settings immediately */
    tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);

    if(showMessage) cout << "Choice: ";
    c = getchar();
    cout << endl;

    /* restore the former settings */
    tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);

    return c;
}

void Client::printResourceInfo(shared_ptr<OC::OCResource> resource)
{
    cout << "Host: " << resource->host() << endl;
    cout << "URI: " << resource->uri() << endl;

    cout << "Resource Types:" << endl;
    for (auto & resourceTypes : resource->getResourceTypes())
    {
        cout << "\t" << resourceTypes << endl;
    }

    cout << "Resource Interfaces: " << endl;
    for (auto & resourceInterfaces : resource->getResourceInterfaces())
    {
        cout << "\t" << resourceInterfaces << endl;
    }
}

void Client::onGetDevice(const HeaderOptions &hOps, const OCRepresentation &rep, int errCode)
{
    if(errCode == OC_STACK_OK)
    {
        string name;
        string uuid;
        rep.getValue("n", name);
        rep.getValue("di", uuid);
        printMessage("Host: "+rep.getHost());
        printMessage("Name: "+name);
        printMessage("UUID: "+uuid);
        for (auto & resourceTypes : rep.getResourceTypes())
        {
            printMessage("Resource Type: " + resourceTypes);
        }

        cout << endl;
    }
    else
    {
        printMessage(string("ErrorCode ")+to_string(errCode));
    }
    onGetMutex.unlock();
}

void Client::onGetGeneric(const HeaderOptions &hOps, const OCRepresentation &rep, int errCode)
{
    if(errCode == OC_STACK_OK)
    {
        if(rep.getUri() == URI_DEVICE)
        {
            onGetDevice(hOps, rep, errCode);
        }
        else
        {
            for (auto & element : rep.getValues())
            {
                cout << element.first << ": " << rep.getValueToString(element.first) << endl; 
            }
        }
        
    }
    else
    {
        printMessage(string("ErrorCode ")+to_string(errCode));
    }
    onGetMutex.unlock();
}

void Client::onGetTimeout()
{
    onGetMutex.unlock();
    printMessage("GET Timeout");
}
/************************************************************************************************/

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

void print_usage(char* program_name)
{
    cout << "TODO Usage" << endl;
}

int main(int argc, char** argv)
{

    string addr;

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

    Client::getInstance()->startClient(addr);
    return 0;
}
