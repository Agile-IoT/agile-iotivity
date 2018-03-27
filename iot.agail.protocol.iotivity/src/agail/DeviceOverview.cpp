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
 * Description: DeviceOverview
 *              DeviceOverview implementation. It contains information about
 *              discovered devices
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#include "DeviceOverview.h"

const string AGAIL::DeviceOverview::STATUS_CONNECTED = "CONNECTED";
const string AGAIL::DeviceOverview::STATUS_DISCONNECTED = "DISCONNECTED";
const string AGAIL::DeviceOverview::STATUS_AVAILABLE = "AVAILABLE";
const string AGAIL::DeviceOverview::STATUS_UNAVAILABLE = "UNAVAILABLE";

AGAIL::DeviceOverview::DeviceOverview()
{
    this->name = "Unknown";
    this->protocol = "Unknown";
    this->id = "Unknown";
    this->status = STATUS_UNAVAILABLE;
    this->lastSeen = 0;
}

AGAIL::DeviceOverview::DeviceOverview(string name, string protocol, string id)
{
    this->name = name;
    this->protocol = protocol;
    this->id = id;
    this->status = STATUS_AVAILABLE;
    setLastSeen2Now();
}

AGAIL::DeviceOverview::DeviceOverview(string name, string protocol, string id, string status)
{
    this->name = name;
    this->protocol = protocol;
    this->id = id;
    if(isValidStatus(status))
    { 
        this->status = status;
    }
    else
    {
        this->status = STATUS_UNAVAILABLE;
    }
    setLastSeen2Now();
}

AGAIL::DeviceOverview::DeviceOverview(string name, string protocol, string id, double lastSeen)
{
    this->name = name;
    this->protocol = protocol;
    this->id = id;
    this->status = STATUS_AVAILABLE;
    this->lastSeen = lastSeen;
}

AGAIL::DeviceOverview::DeviceOverview(string name, string protocol, string id, string status, double lastSeen)
{
    this->name = name;
    this->protocol = protocol;
    this->id = id;
    if(isValidStatus(status))
    {
        this->status = status;
    }
    else
    {
        this->status = STATUS_UNAVAILABLE;
    }
    this->lastSeen = lastSeen;
}


AGAIL::DeviceOverview::~DeviceOverview()
{
}

bool AGAIL::DeviceOverview::isValidStatus(string status)
{
    return (status == STATUS_CONNECTED || 
            status == STATUS_DISCONNECTED ||
            status == STATUS_AVAILABLE ||
            status == STATUS_UNAVAILABLE );
}

string AGAIL::DeviceOverview::getName() const
{
    return name;
}

string AGAIL::DeviceOverview::getProtocol() const
{
    return protocol;
}

string AGAIL::DeviceOverview::getId() const
{
    return id;
}

string AGAIL::DeviceOverview::getStatus() const
{
    return status;
}

double AGAIL::DeviceOverview::getLastSeen() const
{
    return lastSeen;
}

void AGAIL::DeviceOverview::setName(string name)
{
    this->name = name;
}

void AGAIL::DeviceOverview::setLastSeen2Now()
{
    this->lastSeen = std::time(0);
}

bool AGAIL::DeviceOverview::operator==(const AGAIL::DeviceOverview & dev) const
{
    const AGAIL::DeviceOverview t = *this;
    return (dev.getProtocol() == t.getProtocol() && t.getId() == dev.getId());
}
