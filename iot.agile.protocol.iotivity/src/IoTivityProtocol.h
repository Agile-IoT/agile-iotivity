/*
 * Copyright 2016 CREATE-NET
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @ingroup
 * @{
 *
 * @file        IoTivityProtocol.h
 * @brief       Header file for IoTivity protocol implementation for AGILE
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#ifndef __IOTIVITY_PROTOCOL_H__
#define __IOTIVITY_PROTOCOL_H__

#include "common/utility.h"
#include "common/Logger.h"
#include "common/PeriodicCallback.h"

#include "agile/constants.h"
#include "agile/Protocol.h"
#include "agile/DeviceOverview.h"

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

#include <iotivity/resource/OCApi.h>
#include <iotivity/resource/OCPlatform.h>
#include <iotivity/resource/OCResource.h>

#include "boost/any.hpp"

using namespace std;
using namespace OC;

class IoTivityProtocol : public AGILE::Protocol {
    private:
    string destinationAddress = "127.0.0.1";
    static const string TAG;
    static const int DISCOVERY_DELAY = 2;
    Logger* log;

    PeriodicCallback *discoveryPeriodicCallback = nullptr;

    OC::PlatformConfig *platformConfig;
    IoTivityProtocol();

    mutex onDiscoveryMutex;

    public:
    
    static IoTivityProtocol* getInstance();
    void startProtocol(string);

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
    void StartDiscovery();
    void StopDiscovery();
  
    string DiscoveryStatus();

    //IoTivity callbacks
    void doDiscovery();
    void onDiscovery(std::shared_ptr<OC::OCResource>);
};

#endif
