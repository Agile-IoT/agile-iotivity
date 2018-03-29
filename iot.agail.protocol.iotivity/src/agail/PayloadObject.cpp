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

#include "PayloadObject.h"

AGAIL::PayloadObject::PayloadObject()
{
    this->deviceId = "Unknown";
    this->componentAddr.clear();
    this->payload = g_variant_new_string("Unknown");
    this->lastUpdate = 0;
}

AGAIL::PayloadObject::PayloadObject(string deviceId, map<string, GVariant *> componentAddr, GVariant *payload)
{
    this->deviceId = deviceId;
    this->componentAddr = componentAddr;
    this->payload = payload;
    this->lastUpdate = std::time(0);
}

AGAIL::PayloadObject::PayloadObject(string deviceId, map<string, GVariant *> componentAddr, GVariant *payload, double lastUpdate)
{
    this->deviceId = deviceId;
    this->componentAddr = componentAddr;
    this->payload = payload;
    this->lastUpdate = lastUpdate;
}

void AGAIL::PayloadObject::updateLastUpdateToNow()
{
    this->lastUpdate = std::time(0);
}

bool AGAIL::PayloadObject::operator==(const AGAIL::PayloadObject & ro) const
{
    const AGAIL::PayloadObject t = *this;
    return (t.deviceId == ro.deviceId && t.componentAddr == ro.componentAddr &&
            t.payload == ro.payload && t.lastUpdate == ro.lastUpdate);
}
