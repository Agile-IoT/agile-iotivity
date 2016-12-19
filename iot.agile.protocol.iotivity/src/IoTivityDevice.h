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
 * @file        IoTivityDevice.h
 * @brief       Header file for DeviceOverview for IoTivity Devices.
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#ifndef __IOTIVITY_DEVICE_H__
#define __IOTIVITY_DEVICE_H__

#include "agile/DeviceOverview.h"

class IoTivityDevice : public AGILE::DeviceOverview {
    private:
    int timeoutCounter = 0;
    
    public:
    IoTivityDevice();
    IoTivityDevice(string, string, string);
    IoTivityDevice(string, string, string, string);
    ~IoTivityDevice();
    
    void increaseTimeoutCounter();
    int getTimeoutCounter();
    void resetTimeoutCounter();
};

#endif
