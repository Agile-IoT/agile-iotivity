/*******************************************************************************
 * Copyright (c) 2016, 2017 FBK CREATE-NET
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Description: IoTivityDevice
 *              Header file for DeviceOverview for IoTivity Devices.
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#ifndef __IOTIVITY_DEVICE_H__
#define __IOTIVITY_DEVICE_H__

#include "agail/DeviceOverview.h"

class IoTivityDevice : public AGAIL::DeviceOverview {
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
