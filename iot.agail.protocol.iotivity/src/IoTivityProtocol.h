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
#ifndef __IOTIVITY_PROTOCOL_H__
#define __IOTIVITY_PROTOCOL_H__

#include "common/utility.h"
#include "common/Logger.h"
#include "common/PeriodicCallback.h"
#include "common/DelayedCallback.h"

#include "agail/constants.h"
#include "agail/Protocol.h"
#include "agail/DeviceOverview.h"
#include "agail/PayloadObject.h"

#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iotivity/resource/OCApi.h>
#include <iotivity/resource/OCPlatform.h>
#include <iotivity/resource/OCResource.h>

#include "boost/any.hpp"
#include "boost/lexical_cast.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "Resource.h"
#include "IoTivityDevice.h"

using namespace std;
using namespace OC;
using namespace boost::property_tree;

class IoTivityProtocol : public AGAIL::Protocol {
    private:
    string destinationAddress = "127.0.0.1";
    static const string TAG;
    static const string KEY_URI;
    static const string KEY_PAYLOAD;
    static const int DISCOVERY_DELAY = 2;
    static const int READ_TIMEOUT = 5;
    static const int WRITE_TIMEOUT = 5;
    static const int TIMEOUT_THRESHOLD = 3;
    Logger* log;

    std::vector<Resource> resources;

    PeriodicCallback *discoveryPeriodicCallback = nullptr;
    DelayedCallback *stopDiscoveryDelayedCallback = nullptr;

    OC::PlatformConfig *platformConfig;
    IoTivityProtocol();

    void manageTimeout(Resource *);

    mutex onDiscoveryMutex;
    mutex onReadMutex;
    mutex onWriteMutex;
    mutex onSubscribeMutex;
    mutex onUnsubscribeMutex;

    public:
    
    static IoTivityProtocol* getInstance();
    void startProtocol(string);
    OC::OCRepresentation generateRepresentationFromJSON(string);

    //DBus init callbacks
    void onBusAcquiredCb(GDBusConnection *, const gchar *, gpointer);
    void onNameAcquiredCb(GDBusConnection *, const gchar *, gpointer);
    void onNameLostCb(GDBusConnection *, const gchar *, gpointer);

    //error callbacks
    void onUnknownMethod(string);
    void onUnknownProperty(string);

    //APIs
    void Connect(string);
    void Disconnect(string);
    void StartDiscovery(int);
    void StopDiscovery();
    AGAIL::PayloadObject* Read(string, std::map<string, GVariant *>);
    void Write(string, std::map<string, GVariant *>, uint32_t, GVariant *);
    void Subscribe(string, std::map<string, GVariant *>);
    void Unsubscribe(string, std::map<string, GVariant *>);

    //IoTivity callbacks
    void doDiscovery();
    void onDiscovery(std::shared_ptr<OC::OCResource>);

    void onReadCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int, string *, DelayedCallback *);
    void onReadTimeout(Resource *);

    void onWriteCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int, DelayedCallback *, Resource *, string, bool*);
    void onWriteTimeout(Resource *, string);

    void onObserveCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int, const int&, Resource *);
    void onObserveTimeout(Resource *);
};

#endif
