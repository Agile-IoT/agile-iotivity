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
 * Description: DeviceOverview
 *              DeviceOverview header file. It contains information about
 *              discovered devices
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#ifndef __AGILE_DEVICEOVERVIEW_H__
#define __AGILE_DEVICEOVERVIEW_H__

#include <string>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <functional>

using namespace std;

namespace AGILE {
    class DeviceOverview;
}

class AGILE::DeviceOverview {
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

    bool operator==(const AGILE::DeviceOverview& dev) const;
};

#endif
