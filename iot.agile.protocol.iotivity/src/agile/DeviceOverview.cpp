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
 * Description: DeviceOverview
 *              DeviceOverview implementation. It contains information about
 *              discovered devices
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#include "DeviceOverview.h"

const string AGILE::DeviceOverview::STATUS_CONNECTED = "CONNECTED";
const string AGILE::DeviceOverview::STATUS_DISCONNECTED = "DISCONNECTED";
const string AGILE::DeviceOverview::STATUS_AVAILABLE = "AVAILABLE";
const string AGILE::DeviceOverview::STATUS_UNAVAILABLE = "UNAVAILABLE";

AGILE::DeviceOverview::DeviceOverview()
{
    this->name = "Unknown";
    this->protocol = "Unknown";
    this->id = "Unknown";
    this->status = STATUS_UNAVAILABLE;
    this->lastSeen = 0;
}

AGILE::DeviceOverview::DeviceOverview(string name, string protocol, string id)
{
    this->name = name;
    this->protocol = protocol;
    this->id = id;
    this->status = STATUS_AVAILABLE;
    setLastSeen2Now();
}

AGILE::DeviceOverview::DeviceOverview(string name, string protocol, string id, string status)
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

AGILE::DeviceOverview::DeviceOverview(string name, string protocol, string id, double lastSeen)
{
    this->name = name;
    this->protocol = protocol;
    this->id = id;
    this->status = STATUS_AVAILABLE;
    this->lastSeen = lastSeen;
}

AGILE::DeviceOverview::DeviceOverview(string name, string protocol, string id, string status, double lastSeen)
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


AGILE::DeviceOverview::~DeviceOverview()
{
}

bool AGILE::DeviceOverview::isValidStatus(string status)
{
    return (status == STATUS_CONNECTED || 
            status == STATUS_DISCONNECTED ||
            status == STATUS_AVAILABLE ||
            status == STATUS_UNAVAILABLE );
}

string AGILE::DeviceOverview::getName() const
{
    return name;
}

string AGILE::DeviceOverview::getProtocol() const
{
    return protocol;
}

string AGILE::DeviceOverview::getId() const
{
    return id;
}

string AGILE::DeviceOverview::getStatus() const
{
    return status;
}

double AGILE::DeviceOverview::getLastSeen() const
{
    return lastSeen;
}

void AGILE::DeviceOverview::setName(string name)
{
    this->name = name;
}

void AGILE::DeviceOverview::setLastSeen2Now()
{
    this->lastSeen = std::time(0);
}

bool AGILE::DeviceOverview::operator==(const AGILE::DeviceOverview & dev) const
{
    const AGILE::DeviceOverview t = *this;
    return (dev.getProtocol() == t.getProtocol() && t.getId() == dev.getId());
}
