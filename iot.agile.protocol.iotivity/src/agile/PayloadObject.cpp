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
 * Description: PayloadObject
 *              PayloadObject implemantation file. This class contains
 *              the measurment from a sensor and the metadata regarding
 *              its source
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#include "PayloadObject.h"

AGILE::PayloadObject::PayloadObject()
{
    this->deviceId = "Unknown";
    this->componentAddr.clear();
    this->payload = g_variant_new_string("Unknown");
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
