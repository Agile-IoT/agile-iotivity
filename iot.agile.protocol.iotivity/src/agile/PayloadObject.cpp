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
 * @file        PayloadObject.cpp
 * @brief       PayloadObject implemantation file. This class contains 
 *              the measurment from a sensor and the metadata regarding
 *              its source 
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "PayloadObject.h"

AGILE::PayloadObject::PayloadObject()
{
    this->deviceId = "Unknown";
    this->componentAddr.clear();
    this->payload = NULL;
    this->lastUpdate = 0;
}

AGILE::PayloadObject::PayloadObject(string deviceId, map<string, GVariant *> componentAddr, GVariant *payload)
{
    this->deviceId = deviceId;
    this->componentAddr = componentAddr;
    this->payload = payload;
    this->lastUpdate = std::time(0);
}

AGILE::PayloadObject::PayloadObject(string deviceId, map<string, GVariant *> componentAddr, GVariant *payload, double lastUpdate)
{
    this->deviceId = deviceId;
    this->componentAddr = componentAddr;
    this->payload = payload;
    this->lastUpdate = lastUpdate;
}

void AGILE::PayloadObject::updateLastUpdateToNow()
{
    this->lastUpdate = std::time(0);
}

bool AGILE::PayloadObject::operator==(const AGILE::PayloadObject & ro) const
{
    const AGILE::PayloadObject t = *this;
    return (t.deviceId == ro.deviceId && t.componentAddr == ro.componentAddr &&
            t.payload == ro.payload && t.lastUpdate == ro.lastUpdate);
}
