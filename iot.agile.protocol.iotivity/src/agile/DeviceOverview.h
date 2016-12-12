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
 * @file        DeviceOverview.h
 * @brief       DeviceOverview header file. It contains information about
 *              discovered devices
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

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

    public:
    DeviceOverview();
    DeviceOverview(string, string, string);
    DeviceOverview(string, string, string, string);
    ~DeviceOverview();

    string getName() const;
    string getProtocol() const;
    string getId() const;
    string getStatus() const;

    void setName(string);

    bool operator==(const AGILE::DeviceOverview& dev) const;
};

#endif
