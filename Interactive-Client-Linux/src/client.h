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
 * @file        client.h
 * @brief       Header file of a generic IoTivity Linux Client
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#define PRINT_PRETTY_LOGS 0

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

class Client {
    private:
        static const int OPT_DISCOVERY_ALL = 1;
        static const int OPT_SHOW_RES_CACHE = 2;
        static const int OPT_SHOW_ALL_DEVICES = 3;
        static const int OPT_INTERACT_RES = 4;
        static const int OPT_EXIT = 0;
        static const int OP_GET = 1;
        static const int OP_POST = 2;
        static const int OP_PUT = 3;
        static const int OP_DELETE = 4;
        static const int OP_ANOTHER_RES = 0;

        static const string URI_DEVICE;

        vector<Resource> list;
        bool started = false;
        string dest_addr = "127.0.0.1";
        string res_discovery_uri = string(OC_RSRVD_WELL_KNOWN_URI);

        //Mutexes
        mutex onDiscoveryMutex;
        mutex onGetMutex;
        mutex onPutMutex;

        OC::PlatformConfig *platformConfig;
        OC::FindCallback resDiscoveryCallback;

        Client();
        void onDiscovery(std::shared_ptr<OC::OCResource> resource);
        void main();
        char readCharFromTTY(bool showMessage=true);
        std::map<string, boost::any> loadAttributeMap();
        void printResourceInfo(shared_ptr<OC::OCResource> resource);
        void onGetDevice(const OC::HeaderOptions &, const OC::OCRepresentation &, int );
        void onGetGeneric(const OC::HeaderOptions &, const OC::OCRepresentation &, int);
        void onPutGeneric(const OC::HeaderOptions &, const OC::OCRepresentation &, int);
        void interactResources();
        void onGetTimeout(void);
        void onPutTimeout(void);

    public:
        static Client *instance;

        static void printMessage(string);
        static void printError(string);
        static Client* getInstance();
        void startClient(string);
        bool doDiscovery(int);
};
#endif
