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
const string IoTivityProtocol::KEY_PAYLOAD = "payload";
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
                    r = new Resource(w.resource);
                    break;
                }
            }

            if(r != NULL)
            {
                QueryParamsMap qpm;
                AGILE::RecordObject *ro;
                string output = "";
                readDelayedCallback = new DelayedCallback(READ_TIMEOUT*1000, true, bind(&IoTivityProtocol::onReadTimeout, this, deviceId, string(value_uri)));
                r->resource->get(qpm, bind(&IoTivityProtocol::onReadCallback, this, placeholders::_1, placeholders::_2, placeholders::_3, &output, readDelayedCallback));

                onReadMutex.lock();
                if(readDelayedCallback->isFired())
                {
                    ro = new AGILE::RecordObject(deviceId, string(value_uri), "timeout", "", "");
                }
                else
                {
                    log->v(TAG, "Read Payload: " + output);
                    ro = new AGILE::RecordObject(deviceId, string(value_uri), output, "json", "json");
                    storeRecordObject(ro);
                    delete r;
                }
                onReadMutex.unlock();
                log->d(TAG, "onReadMutex UNLOCKED");
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

void IoTivityProtocol::onReadCallback(const HeaderOptions &hOps, const OCRepresentation &rep, int errCode, string *output, DelayedCallback* timeoutDelayedCallback)
{
    int k =0;
    string json = "{";

    if(!timeoutDelayedCallback->isFired())
    {
        timeoutDelayedCallback->stopThread();
    }
    else
    {
        return;
    }

    log->v(TAG, "onReadCallback invoked");

    if(errCode == OC_STACK_OK)
    {
        int i = 0;
        string tmp;

        for(auto& val : rep)
        {
            tmp = "";
            switch(val.type())
            {
                case AttributeType::Integer:
                    tmp = to_string(static_cast<int>(val));
                    json += "\"" + val.attrname() + "\": " + tmp;
                    break;
                case AttributeType::Double:
                    tmp = boost::lexical_cast<std::string>(val.getValue<double>());
                    json += "\"" + val.attrname() + "\": " + tmp;
                    break;
                case AttributeType::Boolean:
                    tmp = (val.getValue<bool>() ? "true" : "false");
                    json += "\"" + val.attrname() + "\": " + tmp;
                    break;
                case AttributeType::String:
                    json += "\"" + val.attrname() + "\": \"" + val.getValue<string>() + "\"";
                    break;
                case AttributeType::Null:
                    log->w(TAG, "Null. Type NOT implemented yet");
                    break;
                case AttributeType::OCRepresentation:
                    log->w(TAG, "OCRepresentation. Type NOT implemented yet");
                    break;
                case AttributeType::Vector:
                    log->w(TAG, "Vector. Type NOT implemented yet");
                    break;
                case AttributeType::Binary:
                    log->w(TAG, "Binary. Type NOT implemented yet");
                    break;
                default:
                    log->w(TAG, "Type Unknown... I skip it");
                    break;
            }

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

string IoTivityProtocol::Write(string deviceId, GVariant* arguments)
{
    gchar *value_uri;
    gchar *value_payload;
    Resource *r = nullptr;

    log->v(TAG, "Write invoked");
    log->d(TAG, "DeviceID: " + deviceId);

    if(g_variant_type_equal(g_variant_get_type(arguments), "a{ss}"))
    {
        if(g_variant_lookup(arguments, KEY_URI.c_str(), "&s", &value_uri) &&
           g_variant_lookup(arguments, KEY_PAYLOAD.c_str(), "&s", &value_payload))
        {
            log->d(TAG, "Keys URI and payload are present");
            log->d(TAG, "URI: " + string(value_uri));
            log->d(TAG, "Payload: " + string(value_payload));
            onWriteMutex.lock();

            for(Resource w : resources)
            {
                if(w.resource->host() == deviceId && w.resource->uri() == value_uri)
                {
                    log->d(TAG, "Resource found in cache");
                    r = new Resource(w.resource);
                    break;
                }
            }

            if(r != nullptr)
            {
                OC::OCRepresentation rep = generateRepresentationFromJSON(string(value_payload));
                QueryParamsMap queryParamsMap;

                writeDelayedCallback = new DelayedCallback(WRITE_TIMEOUT*1000, true, bind(&IoTivityProtocol::onWriteTimeout, this, deviceId, string(value_uri), string(value_payload)));

                r->resource->put(rep, queryParamsMap, bind(&IoTivityProtocol::onWriteCallback, this, placeholders::_1, placeholders::_2, placeholders::_3, writeDelayedCallback));

                log->d(TAG, "onWriteMutex LOCKED");
                onWriteMutex.lock();
                onWriteMutex.unlock();

                delete r;

                if(writeDelayedCallback->isFired())
                {
                    return PROTOCOL_WRITE_STATUS_TIMEOUT;
                }

                log->d(TAG, "onWriteMutex UNLOCKED");
                return PROTOCOL_WRITE_STATUS_DONE;
            }
            onWriteMutex.unlock();
            log->d(TAG, "onWriteMutex UNLOCKED");
            log->w(TAG, "Resource NOT found, Write ignored...");
            return PROTOCOL_WRITE_STATUS_GENERICERROR;
        }
        else
        {
            log->e(TAG, "Key URI and/or payload is/are absent, Read ignored...");
        }
        return PROTOCOL_WRITE_STATUS_ARGSNOTVALID;
    }
    else
    {
        log->e(TAG, "Arguments: " + string(g_variant_print(arguments, TRUE)));
        log->e(TAG, "Arguments Variant has an UNEXPECTED signature: " + string(g_variant_get_type_string(arguments)));
        return PROTOCOL_WRITE_STATUS_SIGARGSNOTVALID;
    }

    return PROTOCOL_WRITE_STATUS_GENERICERROR;
}

OC::OCRepresentation IoTivityProtocol::generateRepresentationFromJSON(string json)
{
    OC::OCRepresentation *rep = new OC::OCRepresentation();
    std::istringstream str(json);
    ptree root;
    read_json(str, root);

    for (ptree::const_iterator it = root.begin(); it != root.end(); ++it) {
        try /* booleans */
        {
            rep->setValue(it->first, it->second.get_value<bool>());
            log->d(TAG, "(Boolean) " + it->first + ": " + it->second.get_value<std::string>());
            continue;
        }
        catch(boost::property_tree::ptree_bad_data &e)
        {
        }

        try /* integers */
        {
            rep->setValue(it->first, it->second.get_value<int>());
            log->d(TAG, "(Integer) " + it->first + ": " + it->second.get_value<std::string>());
            continue;
        }
        catch(boost::property_tree::ptree_bad_data &e)
        {
        }

        try /* doubles */
        {
            rep->setValue(it->first, it->second.get_value<double>());
            log->d(TAG, "(Double) " + it->first + ": " + it->second.get_value<std::string>());
            continue;
        }
        catch(boost::property_tree::ptree_bad_data &e)
        {
        }

        try /* strings */
        {
            rep->setValue(it->first, it->second.get_value<std::string>());
            log->d(TAG, "(String) " + it->first + ": " + it->second.get_value<std::string>());
            continue;
        }
        catch(boost::property_tree::ptree_bad_data &e)
        {
        }

        log->w(TAG, "Key " + it->first + " has an UNKNOWN type");
    }

    return *rep;
}

void IoTivityProtocol::onWriteCallback(const HeaderOptions &hOps, const OCRepresentation &rep, int errCode, DelayedCallback* timeoutDelayedCallback)
{
    if(!timeoutDelayedCallback->isFired())
    {
        timeoutDelayedCallback->stopThread();
    }
    else
    {
        return;
    }

    log->v(TAG, "onWriteCallback invoked");

    if(errCode == OC_STACK_OK)
    {
        
    }
    onWriteMutex.unlock();
}

void IoTivityProtocol::onWriteTimeout(string deviceId, string URI, string payload)
{
    log->w(TAG, "Write Timeout");
    log->w(TAG, "deviceId: " + deviceId);
    log->w(TAG, "URI: " + URI);
    log->w(TAG, "Payload: " + payload);
    onWriteMutex.unlock();
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
