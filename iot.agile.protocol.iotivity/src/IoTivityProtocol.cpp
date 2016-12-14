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
 * @file        IoTivityProtocol.cpp
 * @brief       IoTivity protocol implementation for AGILE
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "IoTivityProtocol.h"

using namespace std;
using namespace OC;

//Attributes
const string IoTivityProtocol::TAG = "IoTivityProtocol";
const string IoTivityProtocol::KEY_URI = "URI";
OCConnectivityType connectivityType(CT_ADAPTER_IP);
//End Attributes

IoTivityProtocol::IoTivityProtocol() : AGILE::Protocol()
{
    BUS_NAME = "iot.agile.protocol.iotivity";
    BUS_PATH = "/iot/agile/protocol/iotivity";
    PROTOCOL_NAME = "IoTivity";
    DRIVER_NAME = "iotivity";

    log = new Logger(&std::cout, Logger::LEVEL_DEBUG);

}

IoTivityProtocol* IoTivityProtocol::getInstance()
{
    if(!instance)
    {
        instance = new IoTivityProtocol();
    }

    return (IoTivityProtocol*) instance;
}

void IoTivityProtocol::startProtocol(string da)
{
    int ret = 0;

    log->i(TAG, "Protocol starting...");
    log->d(TAG, "Initializing DBus");
   
    ret = initBus();
    if(ret == PROTOCOL_DBUS_INIT_ERROR)
    {
        log->c(TAG, "DBus Initialization failed!");
        return;
    }

    log->d(TAG, "Initializing IoTivity");
    /* Configure Platform */
    platformConfig = new PlatformConfig(ServiceType::InProc, //
                        ModeType::Client, //
                        "0.0.0.0", //
                        0, //
                        OC::QualityOfService::HighQos);
    OCPlatform::Configure(*platformConfig);
    log->d(TAG, "Platform configured");
    log->v(TAG, "Multicast Destination address is: " + da);

    destinationAddress = da;

    log->i(TAG, "IoTivity initialized!");
    log->i(TAG, "Press CTRL+C to exit...");
    keepAliveProtocol();
}

void IoTivityProtocol::onBusAcquiredCb(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    log->d(TAG, "Bus Acquired!");
}

void IoTivityProtocol::onNameAcquiredCb(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    log->d(TAG, "Name " + string(name) + " acquired!");
    log->i(TAG, "DBus initialized!");
}

void IoTivityProtocol::onNameLostCb(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    log->c(TAG, "Name " + string(name) + " lost!");
    exit(1);
}

void IoTivityProtocol::onUnknownMethod(string method)
{
    log->e(TAG, "Method " + method + " is Unknown!");
}

void IoTivityProtocol::onUnknownProperty(string property)
{
    log->e(TAG, "Property " + property + " is Unknown!");
}

void IoTivityProtocol::Connect(string deviceId)
{
    log->e(TAG, "Connect not implemented yet! DevID: " + deviceId);
}

void IoTivityProtocol::Disconnect(string deviceId)
{
    log->e(TAG, "Disconnect not implemented yet DevID: " + deviceId);
}

void IoTivityProtocol::StartDiscovery()
{
    log->v(TAG, "StartDiscovery invoked");
    if(discoveryPeriodicCallback)
    {
        log->w(TAG, "Discovery is already started");
        return;
    }
    discoveryPeriodicCallback = new PeriodicCallback(DISCOVERY_DELAY*1000, false, bind(&IoTivityProtocol::doDiscovery, this));
    //TODO: discovery should be done every 15-60 secs
}

void IoTivityProtocol::StopDiscovery()
{
    onDiscoveryMutex.unlock();

    log->v(TAG, "StopDiscovery invoked");
    if(!discoveryPeriodicCallback)
    {
        log->w(TAG, "StopDiscovery invoked when discovery is not running");
        return;
    }
    discoveryPeriodicCallback->stopThread();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    delete discoveryPeriodicCallback;
    discoveryPeriodicCallback = nullptr;
    log->d(TAG, "DiscoveryPeriodicCallback removed");
}

AGILE::RecordObject* IoTivityProtocol::Read(string deviceId, GVariant* arguments)
{
    gchar *value_uri;
    Resource *r = NULL;

    log->v(TAG, "Read invoked");
    log->d(TAG, "DeviceID: " + deviceId);

    if(g_variant_type_equal(g_variant_get_type(arguments), "a{ss}"))
    {
        if(g_variant_lookup(arguments, KEY_URI.c_str(), "&s", &value_uri))
        {
            log->d(TAG, "Key URI is present");
            log->d(TAG, "URI: " + string(value_uri));
            onReadMutex.lock();
            log->d(TAG, "onReadMutex LOCKED");

            for(auto w : resources)
            {
                if(w.resource->host() == deviceId && w.resource->uri() == value_uri)
                {
                    log->d(TAG, "Resource found in cache");
                    r = &w;
                    break;
                }
            }

            if(r != NULL)
            {
                QueryParamsMap qpm;
                string output = "";
                r->resource->get(qpm, bind(&IoTivityProtocol::onReadCallback, this, placeholders::_1, placeholders::_2, placeholders::_3, &output));

                readDelayedCallback = new DelayedCallback(READ_TIMEOUT*1000, true, bind(&IoTivityProtocol::onReadTimeout, this, deviceId, string(value_uri)));

                onReadMutex.lock();
                log->v(TAG, "Read Payload: " + output);
                onReadMutex.unlock();
                log->d(TAG, "onReadMutex UNLOCKED");
                AGILE::RecordObject * ro = new AGILE::RecordObject(deviceId, string(value_uri), output, "json", "json");
                storeRecordObject(ro);
                return ro;
            }
            onReadMutex.unlock();
            log->d(TAG, "onReadMutex UNLOCKED");
            log->w(TAG, "Resource NOT found, Read ignored...");
        }
        else
        {
            log->e(TAG, "Key URI is absent, Read ignored...");
        }
    }
    else
    {
        log->e(TAG, "Arguments: " + string(g_variant_print(arguments, TRUE)));
        log->e(TAG, "Arguments Variant has an UNEXPECTED signature: " + string(g_variant_get_type_string(arguments)));
    }

    return new AGILE::RecordObject();
}

void IoTivityProtocol::onReadCallback(const HeaderOptions &hOps, const OCRepresentation &rep, int errCode, string *output)
{
    string json = "{";

    log->v(TAG, "onReadCallback invoked");
    readDelayedCallback->stopThread();

    if(errCode == OC_STACK_OK)
    {
        int i = 0;
        for (auto & elem : rep.getValues())
        {
            json += "\"" + elem.first + "\": \"" + rep.getValueToString(elem.first) + "\"";
            i++;
            if(i < rep.getValues().size())
            {
                json += ", ";
            }
        }
    }
    else
    {
        json += "\"error\": " + std::to_string(errCode);
    }
    json += "}";
    *output = json;
    onReadMutex.unlock();
}

void IoTivityProtocol::onReadTimeout(string deviceId, string URI)
{
    log->w(TAG, "Read Timeout");
    log->w(TAG, "deviceId: " + deviceId);
    log->w(TAG, "URI: " + URI);
    onReadMutex.unlock();
}

string IoTivityProtocol::DiscoveryStatus()
{
    if(discoveryPeriodicCallback) return PROTOCOL_DISCOVERY_STATUS_RUNNING;
    else return PROTOCOL_DISCOVERY_STATUS_NONE;
}

void IoTivityProtocol::doDiscovery()
{
    log->v(TAG, "Performing discovery...");
    OCPlatform::findResource(destinationAddress.c_str(),
                             string(OC_RSRVD_WELL_KNOWN_URI).c_str(),
                             connectivityType,
                             bind(&IoTivityProtocol::onDiscovery, this, placeholders::_1),
                             OC::QualityOfService::HighQos);
}

void IoTivityProtocol::onDiscovery(std::shared_ptr<OC::OCResource> resource)
{
    onDiscoveryMutex.lock();

    log->v(TAG, "Resource Discovered");

    string name = "IoTivity Node";
    string id = resource->host();
    AGILE::DeviceOverview *dev = new AGILE::DeviceOverview(name, PROTOCOL_NAME, id);

    if(addDevice(dev)){ //If the device is new, it is added in the device list
        log->i(TAG, "New device discovered! ID: " + dev->getId());
        bool res = emitFoundNewDeviceSignal(dev);
        if(res)
        {
            log->d(TAG, "NewDeviceFound signal emitted!");
        }
        else
        {
            log->e(TAG, "Error during NewDeviceFound signal emission");
        }
    }
    else {
        log->v(TAG, "Device with ID " + dev->getId() + " is already known");
    }

    Resource *res = new Resource(resource);

    if(std::find(resources.begin(), resources.end(), *res) != resources.end())
    {
        log->d(TAG, "Resource already discovered");
    }
    else
    {
        log->v(TAG, "New resource discovered " + resource->host() + resource->uri());
        resources.push_back(*res);
    }


    onDiscoveryMutex.unlock();
}
/************************************************************************************************/

void print_usage(char* program_name)
{
    cout << "TODO: usage" << endl;
}


int main(int argc, char** argv)
{
    //At the moment it supports only IPv6-based discoveries
    IoTivityProtocol::getInstance()->startProtocol("ff03::158");
    return 0;
}
