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
 * @file        client.cpp
 * @brief       This is a generic IoTivity Linux Client
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@fbk.eu>
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

#include "boost/any.hpp"

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

void Client::printError(string mex)
{
    cout << "\033[1;31m";
    cout << "Error: " << mex;
    cout << "\033[0m\n";
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
            printError("Option not valid!");
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
                    printError("Error during discovery...");
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
                printError("Unknown option");
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
    char ch;
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
        cout << "then press ENTER." << endl;
        cout << "Choice: ";
        cin >> c;
        cin.clear();
        while ((ch = getchar()) != '\n' && ch != EOF);
        //c = int(readCharFromTTY())-48;

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
                printError("Unknown choice");
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
                    printError("POST Not Implemented yet!");
                    printMessage("Press any key to return to continue...");
                    readCharFromTTY(false);
                    break;
                case OP_PUT:
                    onPutMutex.lock();
                    if(res.isInterfacePresent(Resource::INTERFACE_R))
                    {
                        printError("PUT cannot be done on a READABLE resource!");
                    }
                    else
                    {
                        res.registerPUTCallbacks(bind(&Client::onPutGeneric, this,placeholders::_1, placeholders::_2, placeholders::_3), bind(&Client::onPutTimeout, this), 5000);
                        res.execPUT(loadAttributeMap());
                        onPutMutex.lock();
                    }
                    printMessage("Press any key to return to continue...");
                    readCharFromTTY(false);
                    onPutMutex.unlock();
                    break;
                case OP_DELETE:
                    printError("DELETE Not Implemented yet!");
                    printMessage("Press any key to return to continue...");
                    readCharFromTTY(false);
                    break;
                case OP_ANOTHER_RES:
                    c++;
                    break;
                default:
                   printError("Unknown choice!");
            }
        } 
    }
    while(c != 0);
    return;
}

std::map<string, boost::any> Client::loadAttributeMap()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    std::map<string, boost::any> attributeMap;
    string line;
    printMessage("Now, you can define attributes for the request.");
    printMessage("Datatype supported:");
    printMessage("\t\tbool");
    printMessage("\t\tstring");
    printMessage("\t\tuint8, int8, uint16, int16, int32");
    printMessage("syntax: <type> <name> <value>");
    printMessage("Type ENTER to continue");
    printMessage("e.g. bool state false");
    printMessage("e.g. uint8 power 4");
    printMessage("e.g. string something HelloWorld");
    
    while(1)
    {
        cout << "Attribute: ";
        getline(cin, line);
        if(line.size()==0) break;
        string type = line.substr(0, line.find_first_of(" "));
        line = line.substr(line.find_first_of(" ")+1);
        string name = line.substr(0, line.find_first_of(" "));
        line = line.substr(line.find_first_of(" ")+1);
        string value = line.substr(0, line.find_first_of(" "));

        if(type.size()==0 || name.size() == 0 || value.size() == 0)
        {
            printError("At least one parameter is empty");
        }

        if(type=="bool")
        {
            if(value == "true")
            {
               attributeMap[name] = true;
            }
            else if(value == "false")
            {
               attributeMap[name] = false;
            }
            else
            {
                printError("Value unknown. It can be true or false");
            }
        }
        else if(type=="string")
        {
            attributeMap[name] = value;
        }
        else if(type=="uint8")
        {
            long int tmp = stol(value);
            if(tmp < 0 || tmp >255)
            {
                printError(to_string(tmp) + " is a value out of range.");
            }
            else
            {
                uint8_t v = (uint8_t) tmp; 
                attributeMap[name] = v;
            }
        }
        else if(type=="int8")
        {
            long int tmp = stol(value);
            if(tmp < -128 || tmp > 127)
            {
                printError(to_string(tmp) + " is a value out of range.");
            }
            else
            {
                int8_t v = (int8_t) tmp; 
                attributeMap[name] = v;
            }
        }
        else if(type=="uint16")
        {
            long int tmp = stol(value);
            if(tmp < 0 || tmp > 65535)
            {
                printError(to_string(tmp) + " is a value out of range.");
            }
            else
            {
                int16_t v = (int16_t) tmp; 
                attributeMap[name] = v;
            }
        }
        else if(type=="int16")
        {
            long int tmp = stol(value);
            if(tmp < -32768 || tmp > 32767)
            {
                printError(to_string(tmp) + " is a value out of range.");
            }
            else
            {
                int16_t v = (int16_t) tmp; 
                attributeMap[name] = v;
            }
        }
        else if(type=="int32")
        {
            long int tmp = stoll(value);
            if(tmp < -2147483648 || tmp > 2147483647)
            {
                printError(to_string(tmp) + " is a value out of range.");
            }
            else
            {
                int32_t v = (int32_t) tmp; 
                attributeMap[name] = v;
            }
        }
        else
        {
            printError("Type Unknown");
        }
    }
    
    printMessage(to_string(attributeMap.size()) + " attribute(s) set");

    return attributeMap;
}

char Client::readCharFromTTY(bool showMessage)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
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
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
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
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
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
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
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
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    onGetMutex.unlock();
    printMessage("GET Timeout");
}

void Client::onPutGeneric(const HeaderOptions &hOps, const OCRepresentation &rep, int errCode)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if(errCode == 4)
    {
        printMessage("PUT Successful");        
    }
    else
    {
        printMessage(string("ErrorCode ")+to_string(errCode));
    }
    onPutMutex.unlock();
}

void Client::onPutTimeout()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    onPutMutex.unlock();
    printMessage("PUT Timeout");
}
/************************************************************************************************/

bool is_ipv6_address(const string& str)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    struct sockaddr_in6 sa;
    return inet_pton(AF_INET6, str.c_str(), &(sa.sin6_addr))!=0;
}

bool is_ipv4_address(const string& str)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    struct sockaddr_in sa;
    return inet_pton(AF_INET, str.c_str(), &(sa.sin_addr))!=0;
}

void print_usage(char* program_name)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    cout << "This is an Interactive IoTivity Client" << endl;
    cout << endl; 
    cout << "Usage: " << program_name << " <destination>" << endl;
    cout << "\t<destination>: It is the destination address of discovery. It can be a multicast or unicast address" << endl;
}


int main(int argc, char** argv)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
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
