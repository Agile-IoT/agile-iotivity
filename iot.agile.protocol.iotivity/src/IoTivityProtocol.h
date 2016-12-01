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

#ifndef IOTIVITY_PROTOCOL_H_
#define IOTIVITY_PROTOCOL_H_

#define PRINT_PRETTY_LOGS 0

#include "common/utility.h"

#include "agile/constants.h"

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

#include <dbus-cxx.h>

using namespace std;
using namespace OC;

class IoTivityProtocol {
    private:

    /**
     * Bus name for AGILE IoTivity Protocol
     */
    static const string BUS_NAME;

    /**
     * Bus path for AGILE IoTivity Protocol
     */
    static const string BUS_PATH;

    /**
     * Protocol name
     */
    static const string PROTOCOL_NAME;

    /**
     * Protocol driver name
     */
    static const string DRIVER_NAME;

    /**
     * IoTivityProtocol must be a singleton
     */
    static IoTivityProtocol *instance;

    public:
    
    IoTivityProtocol* getInstance();

};

#endif
