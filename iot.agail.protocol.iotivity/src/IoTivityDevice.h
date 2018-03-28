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
