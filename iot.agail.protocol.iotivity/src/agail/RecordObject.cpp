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
 * Description: RecordObject
 *              RecordObject implemantation file. This class contains
 *              the measurment from a sensor and the metadata regarding
 *              its source
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#include "RecordObject.h"

AGAIL::RecordObject::RecordObject()
{
    this->deviceId = "Unknown";
    this->componentId = "Unknown";
    this->value = "Unknown";
    this->unit = "Unknown";
    this->format = "Unknown";
    this->lastUpdate = 0;
}

AGAIL::RecordObject::RecordObject(string deviceId, string componentId, string value, string unit, string format)
{
    this->deviceId = deviceId;
    this->componentId = componentId;
    this->value = value;
    this->unit = unit;
    this->format = format;
    this->lastUpdate = std::time(0);
}

AGAIL::RecordObject::RecordObject(string deviceId, string componentId, string value, string unit, string format, double lastUpdate)
{
    this->deviceId = deviceId;
    this->componentId = componentId;
    this->value = value;
    this->unit = unit;
    this->format = format;
    this->lastUpdate = lastUpdate;
}

void AGAIL::RecordObject::updateLastUpdateToNow()
{
    this->lastUpdate = std::time(0);
}

bool AGAIL::RecordObject::operator==(const AGAIL::RecordObject & ro) const
{
    const AGAIL::RecordObject t = *this;
    return (t.deviceId == ro.deviceId && t.componentId == ro.componentId &&
            t.value == ro.value && t.unit == ro.unit &&
            t.format == ro.format && t.lastUpdate == ro.lastUpdate);
}
