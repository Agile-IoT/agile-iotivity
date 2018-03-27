/*******************************************************************************
 * Copyright (c) 2016, 2017 FBK CREATE-NET
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    https://www.eclipse.org/legal/epl-2.0/
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Description: IoTivityDevice
 *              Implementation file for DeviceOverview for IoTivity Devices.
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/
 
#include "IoTivityDevice.h"

IoTivityDevice::IoTivityDevice()
    : DeviceOverview()
{
}

IoTivityDevice::IoTivityDevice(string name, string protocol, string id)
    : DeviceOverview(name, protocol, id)
{
}

IoTivityDevice::IoTivityDevice(string name, string protocol, string id, string status)
    : DeviceOverview(name, protocol, id, status)
{
}

IoTivityDevice::~IoTivityDevice()
{
}

void IoTivityDevice::increaseTimeoutCounter()
{
    timeoutCounter++;
}

int IoTivityDevice::getTimeoutCounter()
{
    return timeoutCounter;
}

void IoTivityDevice::resetTimeoutCounter()
{
    timeoutCounter=0;
}
