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
