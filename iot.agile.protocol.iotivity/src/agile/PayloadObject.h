/*
 * Copyright 2017 CREATE-NET
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
 * @file        PayloadObject.h
 * @brief       PayloadObject header file. This class contains the
 *              measurment from a sensor and the metadata regarding
 *              its source 
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#ifndef __AGILE_PAYLOADOBJECT_H__
#define __AGILE_PAYLOADOBJECT_H__

#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <map>

#include <gio/gio.h>


using namespace std;

namespace AGILE {
    class PayloadObject;
}

class AGILE::PayloadObject {
    public:
    string deviceId;
    map<string, GVariant *> componentAddr;
    GVariant* payload;
    double lastUpdate;

    PayloadObject();
    PayloadObject(string, map<string, GVariant *>, GVariant *);
    PayloadObject(string, map<string, GVariant *>, GVariant *, double);

    void updateLastUpdateToNow();

    bool operator==(const AGILE::PayloadObject&) const;
};

#endif
