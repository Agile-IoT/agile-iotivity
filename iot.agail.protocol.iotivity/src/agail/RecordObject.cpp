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
