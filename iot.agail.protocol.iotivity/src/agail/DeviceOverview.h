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

#ifndef __AGAIL_DEVICEOVERVIEW_H__
#define __AGAIL_DEVICEOVERVIEW_H__

#include <string>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <functional>

using namespace std;

namespace AGAIL {
    class DeviceOverview;
}

class AGAIL::DeviceOverview {
    public:
    static const string STATUS_CONNECTED;
    static const string STATUS_DISCONNECTED;
    static const string STATUS_AVAILABLE;
    static const string STATUS_UNAVAILABLE;

    static bool isValidStatus(string);

    private:
    string name;
    string protocol;
    string id;
    string status;
    double lastSeen;

    public:
    DeviceOverview();
    DeviceOverview(string, string, string);
    DeviceOverview(string, string, string, double);
    DeviceOverview(string, string, string, string);
    DeviceOverview(string, string, string, string, double);
    ~DeviceOverview();

    string getName() const;
    string getProtocol() const;
    string getId() const;
    string getStatus() const;
    double getLastSeen() const;

    void setLastSeen2Now();

    void setName(string);

    bool operator==(const AGAIL::DeviceOverview& dev) const;
};

#endif
