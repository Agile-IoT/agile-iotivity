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
 * @file        RecordObject.cpp
 * @brief       RecordObject implemantation file. This class contains 
 *              the measurment from a sensor and the metadata regarding
 *              its source 
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "RecordObject.h"

AGILE::RecordObject::RecordObject()
{
    this->deviceId = "Unknown";
    this->componentId = "Unknown";
    this->value = "Unknown";
    this->unit = "Unknown";
    this->format = "Unknown";
    this->lastUpdate = 0;
}

AGILE::RecordObject::RecordObject(string deviceId, string componentId, string value, string unit, string format)
{
    this->deviceId = deviceId;
    this->componentId = componentId;
    this->value = value;
    this->unit = unit;
    this->format = format;
    this->lastUpdate = std::time(0);
}

AGILE::RecordObject::RecordObject(string deviceId, string componentId, string value, string unit, string format, long int lastUpdate)
{
    this->deviceId = deviceId;
    this->componentId = componentId;
    this->value = value;
    this->unit = unit;
    this->format = format;
    this->lastUpdate = lastUpdate;
}

void AGILE::RecordObject::updateLastUpdateToNow()
{
    this->lastUpdate = std::time(0);
}
