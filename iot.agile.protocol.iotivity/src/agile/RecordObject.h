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
 * @file        RecordObject.h
 * @brief       RecordObject header file. This class contains the
 *              measurment from a sensor and the metadata regarding
 *              its source 
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#ifndef __AGILE_RECORDOBJECT_H__
#define __AGILE_RECORDOBJECT_H__

#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;

namespace AGILE {
    class RecordObject;
}

class AGILE::RecordObject {
    public:
    string deviceId;
    string componentId;
    string value;
    string unit;
    string format;
    double lastUpdate;

    RecordObject();
    RecordObject(string, string, string, string, string);
    RecordObject(string, string, string, string, string, double);

    void updateLastUpdateToNow();

    bool operator==(const AGILE::RecordObject&) const;
};

#endif
