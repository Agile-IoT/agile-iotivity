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
 * @file        Protocol.cpp
 * @brief       AGILE Base Protocol file
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "Protocol.h"
#include <iostream>


AGILE::Protocol *AGILE::Protocol::instance = nullptr;
const string AGILE::Protocol::PROPERTY_STATUS = "Status";
const string AGILE::Protocol::PROPERTY_DRIVER = "Driver";
const string AGILE::Protocol::PROPERTY_NAME = "Name";
const string AGILE::Protocol::PROPERTY_DATA = "Data";
const string AGILE::Protocol::PROPERTY_DEVICES = "Devices";
const string AGILE::Protocol::PROPERTY_DISCOVERYSTATUS = "DiscoveryStatus";
const string AGILE::Protocol::METHOD_CONNECT = "Connect";
const string AGILE::Protocol::METHOD_DISCONNECT = "Disconnect";
const string AGILE::Protocol::METHOD_STARTDISCOVERY = "StartDiscovery";
const string AGILE::Protocol::METHOD_STOPDISCOVERY = "StopDiscovery";
const string AGILE::Protocol::METHOD_WRITE = "Write";
const string AGILE::Protocol::METHOD_READ = "Read";
const string AGILE::Protocol::METHOD_SUBSCRIBE = "Subscribe";
const string AGILE::Protocol::METHOD_UNSUBSCRIBE = "Unsubscribe";
const string AGILE::Protocol::SIGNAL_FOUNDNEWDEVICE = "FoundNewDeviceSignal";
const string AGILE::Protocol::SIGNAL_NEWRECORD = "NewRecordSignal";
const string AGILE::Protocol::PROTOCOL_STATUS_AVAILABLE = "AVAILABLE";
const string AGILE::Protocol::PROTOCOL_STATUS_UNAVAILABLE = "UNAVAILABLE";
const string AGILE::Protocol::PROTOCOL_STATUS_NOT_CONFIGURED = "NOT_CONFIGURED";
const string AGILE::Protocol::PROTOCOL_STATUS_DISABLED = "DISABLED";
const string AGILE::Protocol::PROTOCOL_STATUS_ENABLED = "ENABLED";
const string AGILE::Protocol::PROTOCOL_STATUS_FAILURE = "FAILURE";
const string AGILE::Protocol::PROTOCOL_DISCOVERY_STATUS_RUNNING = "RUNNING";
const string AGILE::Protocol::PROTOCOL_DISCOVERY_STATUS_NONE = "NONE";
const string AGILE::Protocol::PROTOCOL_DISCOVERY_STATUS_FAILURE = "FAILURE";
const string AGILE::Protocol::PROTOCOL_WRITE_STATUS_DONE = "DONE";
const string AGILE::Protocol::PROTOCOL_WRITE_STATUS_NOTIMPLEMENTED = "NOTIMPLEMENTED";
const string AGILE::Protocol::PROTOCOL_WRITE_STATUS_ARGSNOTVALID = "ARGSNOTVALID";
const string AGILE::Protocol::PROTOCOL_WRITE_STATUS_SIGARGSNOTVALID = "SIGARGSNOTVALID";
const string AGILE::Protocol::PROTOCOL_WRITE_STATUS_GENERICERROR = "ERROR";
const string AGILE::Protocol::PROTOCOL_WRITE_STATUS_TIMEOUT = "TIMEOUT";
const string AGILE::Protocol::PROTOCOL_WRITE_STATUS_FAILED = "FAILED";
const gchar AGILE::Protocol::PROTOCOL_INTROSPECTION[] =
    "<node name='/iot/agile/Protocol'>"
    "  <interface name='iot.agile.Protocol'>"
    "  <method name='Connect'>"
    "  <annotation name='org.freedesktop.DBus.Description' value='Setup connection and initialize protocol connection for the given device' />"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "  </method>"
    "  <method name='Disconnect'>"
    "    <annotation name='org.freedesktop.DBus.Description' value='Safely disconnect the device from the protocol adapter' />"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "  </method>"
    "  <method name='StartDiscovery'>"
    "    <annotation name='org.freedesktop.DBus.Description' value='Start device discovery' />"
    "  </method>"
    "  <method name='StopDiscovery'>"
    "    <annotation name='org.freedesktop.DBus.Description' value='Stop device discovery' />"
    "  </method>"
    "  <method name='Write'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='arguments' type='v' direction='in'/>"
    "    <arg name='return' type='s' direction='out'/>"
    "  </method>"
    "  <method name='Read'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='arguments' type='v' direction='in'/>"
    "    <arg name='return' type='(sssssd)' direction='out'/>"
    "  </method>"
    "  <method name='Subscribe'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='arguments' type='v' direction='in'/>"
    "  </method>"
    "  <method name='Unsubscribe'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='arguments' type='v' direction='in'/>"
    "  </method>"
    "  <property name='Status' type='s' access='read' />"
    "  <property name='Driver' type='s' access='read' />"
    "  <property name='Name' type='s' access='read' />"
    "  <property name='Devices' type='a(ssss)' access='read' />"
    "  <property name='Data' type='(sssssd)' access='read' />"
    "  <property name='DiscoveryStatus' type='s' access='read' />"
    "<signal name='NewRecordSignal'>"
    "  <arg name='Data' type='(sssssd)' />"
    "</signal>"
    "<signal name='FoundNewDeviceSignal'>"
    "  <arg name='Device' type='(ssss)' />"
    "</signal>"
    "  </interface>"
    "</node>";

const GDBusInterfaceVTable AGILE::Protocol::interface_vtable =
{
  AGILE::Protocol::handleMethodCall,
  AGILE::Protocol::handleGetProperty,
  AGILE::Protocol::handleSetProperty
};

AGILE::Protocol::Protocol()
{
    BUS_NAME = "Unknown";
    BUS_PATH = "Unknown";
    PROTOCOL_NAME = "Unknown";
    DRIVER_NAME = "Unknown";

    data = new AGILE::RecordObject();
}

// TODO: implement it
AGILE::Protocol::~Protocol()
{
}

int AGILE::Protocol::initBus()
{
    introspection_data = g_dbus_node_info_new_for_xml (PROTOCOL_INTROSPECTION, NULL);
    if(introspection_data == NULL)
    {
        return PROTOCOL_DBUS_INIT_ERROR;
    }

    owner_id = g_bus_own_name(G_BUS_TYPE_SESSION,
                             BUS_NAME.c_str(),
                             G_BUS_NAME_OWNER_FLAGS_NONE,
                             &AGILE::Protocol::onBusAcquired,
                             &AGILE::Protocol::onNameAcquired,
                             &AGILE::Protocol::onNameLost,
                             NULL,
                             NULL);

    mainloop = g_main_loop_new (NULL, FALSE);

    return PROTOCOL_DBUS_INIT_NOERR;
}

void AGILE::Protocol::saveGDBusConnection(GDBusConnection *conn)
{
    connection = conn;
}

void AGILE::Protocol::onBusAcquired(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    guint registration_id;

    registration_id = g_dbus_connection_register_object(conn,
                                                       instance->BUS_PATH.c_str(),
                                                       instance->introspection_data->interfaces[0],
                                                       &interface_vtable,
                                                       NULL,  /* user_data */
                                                       NULL,  /* user_data_free_func */
                                                       NULL); /* GError** */

    instance->saveGDBusConnection(conn);
    instance->onBusAcquiredCb(conn, name, user_data);
}

void AGILE::Protocol::onNameAcquired(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    instance->onNameAcquiredCb(conn, name, user_data);
}

void AGILE::Protocol::onNameLost(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    instance->onNameLostCb(conn, name, user_data);
}

void AGILE::Protocol::onUnknownMethod(string method)
{
    std::cout << "onUnknownMethod not implemented! Arg: " << method << std::endl;
}

void AGILE::Protocol::onUnknownProperty(string property)
{
    std::cout << "onUnknownProperty not implemented! Arg: " << property << std::endl;
}

void AGILE::Protocol::handleMethodCall(GDBusConnection *connection, const gchar *sender, const gchar *object_path, const gchar *interface_name, const gchar *method_name, GVariant *parameters, GDBusMethodInvocation *invocation, gpointer user_data)
{
    GVariant * out;
    out = NULL;
    //METHOD CONNECT
    if(g_strcmp0(method_name, METHOD_CONNECT.c_str()) == 0)
    {
        const gchar *deviceId;
        g_variant_get (parameters, "(&s)", &deviceId);
        instance->Connect(string(deviceId));
    }
    //METHOD DISCONNECT
    else if(g_strcmp0(method_name, METHOD_DISCONNECT.c_str()) == 0)
    {
        const gchar *deviceId;
        g_variant_get (parameters, "(&s)", &deviceId);
        instance->Disconnect(string(deviceId));
    }
    //METHOD START DISCOVERY
    else if(g_strcmp0(method_name, METHOD_STARTDISCOVERY.c_str()) == 0)
    {
        instance->StartDiscovery();
        instance->setDiscoveryStatus(PROTOCOL_DISCOVERY_STATUS_RUNNING);
    }
    //METHOD STOP DISCOVERY
    else if(g_strcmp0(method_name, METHOD_STOPDISCOVERY.c_str()) == 0)
    {
        instance->StopDiscovery();
        instance->setDiscoveryStatus(PROTOCOL_DISCOVERY_STATUS_NONE);
    }
    //METHOD READ
    else if(g_strcmp0(method_name, METHOD_READ.c_str()) == 0)
    {
        const gchar *deviceId;
        GVariant *arguments;
        g_variant_get (parameters, "(&sv)", &deviceId, &arguments);
        RecordObject *data = instance->Read(string(deviceId), arguments);
        out = g_variant_new("((sssssd))", data->deviceId.c_str(), data->componentId.c_str(), data->value.c_str(), data->unit.c_str(), data->format.c_str(), data->lastUpdate);
        delete data;
    }
    //METHOD WRITE
    else if(g_strcmp0(method_name, METHOD_WRITE.c_str()) == 0)
    {
        const gchar *deviceId;
        GVariant *arguments;
        g_variant_get (parameters, "(&sv)", &deviceId, &arguments);
        string ret = instance->Write(string(deviceId), arguments);
        out = g_variant_new("(s)", ret.c_str());
    }
    //METHOD SUBSCRIBE
    else if(g_strcmp0(method_name, METHOD_SUBSCRIBE.c_str()) == 0)
    {
        const gchar *deviceId;
        GVariant *arguments;
        g_variant_get (parameters, "(&sv)", &deviceId, &arguments);
        instance->Subscribe(string(deviceId), arguments);
    }
    //METHOD UNSUBSCRIBE
    else if(g_strcmp0(method_name, METHOD_UNSUBSCRIBE.c_str()) == 0)
    {
        const gchar *deviceId;
        GVariant *arguments;
        g_variant_get (parameters, "(&sv)", &deviceId, &arguments);
        instance->Unsubscribe(string(deviceId), arguments);
    }
    else
    {
        instance->onUnknownMethod(method_name);
    }

    g_dbus_method_invocation_return_value(invocation, out);
}

GVariant* AGILE::Protocol::handleGetProperty(GDBusConnection *connection, const gchar *sender, const gchar *obj_path, const gchar *interface_name, const gchar *property_name, GError **error, gpointer user_data)
{
    GVariant * ret;
    ret = NULL;

    //PROPERTY PROTOCOL NAME
    if(g_strcmp0(property_name, PROPERTY_NAME.c_str()) == 0)
    {
        ret = g_variant_new_string(instance->PROTOCOL_NAME.c_str());
    }
    //PROPERTY DRIVER NAME
    else if(g_strcmp0(property_name, PROPERTY_DRIVER.c_str()) == 0)
    {
        ret = g_variant_new_string(instance->DRIVER_NAME.c_str());
    }
    //PROPERTY DRIVER NAME
    else if(g_strcmp0(property_name, PROPERTY_STATUS.c_str()) == 0)
    {
        ret = g_variant_new_string(PROTOCOL_STATUS_AVAILABLE.c_str());
    }
    //PROPERTY DEVICES
    else if(g_strcmp0(property_name, PROPERTY_DEVICES.c_str()) == 0)
    {
        GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("a(ssss)"));
        for(int i=0; i<instance->getDeviceListSize(); i++) {
           AGILE::DeviceOverview *d = instance->getDeviceAt(i);
           g_variant_builder_add(builder, "(ssss)", d->getName().c_str(),
               d->getProtocol().c_str(), d->getId().c_str(), d->getStatus().c_str());
        }
        ret = g_variant_new("a(ssss)", builder);
        g_variant_builder_unref(builder);        
    }
    //PROPERTY DATA
    else if(g_strcmp0(property_name, PROPERTY_DATA.c_str()) == 0)
    {
        AGILE::RecordObject* data = instance->getLastRecordObject();
        ret = g_variant_new("(sssssd)", data->deviceId.c_str(), data->componentId.c_str(), data->value.c_str(), data->unit.c_str(), data->format.c_str(), data->lastUpdate);     
    }
    //PROPERTY DISCOVERY STATUS
    else if(g_strcmp0(property_name, PROPERTY_DISCOVERYSTATUS.c_str()) == 0)
    {
        ret = g_variant_new_string(instance->getDiscoveryStatus().c_str());
    }
    //PROPERTY UNKNOWN
    else
    {
        instance->onUnknownProperty(property_name);
    }
    return ret;
}

gboolean AGILE::Protocol::handleSetProperty(GDBusConnection *connection, const gchar *sender, const gchar *obj_path, const gchar *interface_name, const gchar *property_name, GVariant *value, GError **error, gpointer user_data)
{
    cout << "handleSetProperty" << endl;
}

void AGILE::Protocol::keepAliveProtocol()
{
    g_main_loop_run(mainloop);
    g_bus_unown_name(owner_id);
    g_dbus_node_info_unref(introspection_data);

    return;
}

bool AGILE::Protocol::emitFoundNewDeviceSignal(AGILE::DeviceOverview *dev)
{
    GError *local_error;
    GVariant * device_variant;

    local_error = NULL;

    device_variant = g_variant_new("(ssss)", dev->getName().c_str(), dev->getProtocol().c_str(),
        dev->getId().c_str(), dev->getStatus().c_str());

    g_dbus_connection_emit_signal(connection,
                                  NULL,
                                  BUS_PATH.c_str(),
                                  AGILE::AGILE_PROTOCOL_INTERFACE.c_str(), 
                                  SIGNAL_FOUNDNEWDEVICE.c_str(),
                                  device_variant,
                                  &local_error);

    if(local_error == NULL)
    {
       return true;
    }

    return false;
}

bool AGILE::Protocol::isNewDevice(AGILE::DeviceOverview *dev)
{
    return (std::find(devices.begin(), devices.end(), *dev) == devices.end());
}

bool AGILE::Protocol::addDevice(AGILE::DeviceOverview *dev)
{
    if(isNewDevice(dev)) {
        devices.push_back(*dev);
        return true;
    }
    return false;
}

bool AGILE::Protocol::updateDevice(AGILE::DeviceOverview *dev)
{
    if(!isNewDevice(dev)) {
        for(int i=0; i<devices.size(); i++) {
            if(devices.at(i) == *dev)
            {
                devices.at(i) = *dev;
                return true;
            }
        }
    }
    return false;
}

bool AGILE::Protocol::removeDevice(AGILE::DeviceOverview *dev)
{
    if(!isNewDevice(dev)) {
        for(int i=0; i<devices.size(); i++) {
            if(devices.at(i) == *dev)
            {
                devices.erase(devices.begin()+i);
                return true;
            }
        }
    }
    return false;
}

AGILE::DeviceOverview* AGILE::Protocol::getDeviceFromId(string id)
{
    AGILE::DeviceOverview* t = new AGILE::DeviceOverview("", PROTOCOL_NAME, id);
    if(!isNewDevice(t)) {
        for(int i=0; i<devices.size(); i++) {
            if(devices.at(i) == *t)
            {
                return &devices.at(i);
            }
        }
    }
    return NULL;
}

AGILE::DeviceOverview* AGILE::Protocol::getDeviceAt(int pos)
{
    if(pos>=0 || pos<devices.size())
    {
        return &devices.at(pos);
    }
    return NULL;
}

int AGILE::Protocol::getDeviceListSize()
{
    return devices.size();
}

AGILE::RecordObject* AGILE::Protocol::getLastRecordObject()
{
    return data;
}

void AGILE::Protocol::storeRecordObject(AGILE::RecordObject* ro)
{
    *data = *ro;
}

bool AGILE::Protocol::emitNewRecordSignal(AGILE::RecordObject *ro)
{
    GError *local_error;
    GVariant * record_variant;

    local_error = NULL;

    record_variant = g_variant_new("((sssssd))", ro->deviceId.c_str(), ro->componentId.c_str(), ro->value.c_str(), ro->unit.c_str(), ro->format.c_str(), ro->lastUpdate);

    g_dbus_connection_emit_signal(connection,
                                  NULL,
                                  BUS_PATH.c_str(),
                                  AGILE::AGILE_PROTOCOL_INTERFACE.c_str(), 
                                  SIGNAL_NEWRECORD.c_str(),
                                  record_variant,
                                  &local_error);

    storeRecordObject(ro);

    if(local_error == NULL)
    {
       return true;
    }

    return false;
}

void AGILE::Protocol::setDiscoveryStatus(string status)
{
    if(status == PROTOCOL_DISCOVERY_STATUS_RUNNING ||
       status == PROTOCOL_DISCOVERY_STATUS_NONE ||
       status == PROTOCOL_DISCOVERY_STATUS_FAILURE)
    {
        discoveryStatus = status;
    }
}

string AGILE::Protocol::getDiscoveryStatus()
{
    return discoveryStatus;
}

void AGILE::Protocol::Connect(string deviceId)
{
    std::cout << "Connect not implemented! Arg: " << deviceId << std::endl;
}

void AGILE::Protocol::Disconnect(string deviceId)
{
    std::cout << "Disconnect not implemented! Arg: " << deviceId << std::endl;
}

void AGILE::Protocol::StartDiscovery()
{
    std::cout << "StartDiscovery not implemented!" << std::endl;
}

void AGILE::Protocol::StopDiscovery()
{
    std::cout << "StopDiscovery not implemented!" << std::endl;
}

AGILE::RecordObject* AGILE::Protocol::Read(string deviceId, GVariant* arguments)
{
    std::cout << "Read not Implemented." << std::endl;
    return new RecordObject();
}

string AGILE::Protocol::Write(string deviceId, GVariant* arguments)
{
    std::cout << "Write not Implemented." << std::endl;
    return PROTOCOL_WRITE_STATUS_NOTIMPLEMENTED;
}

void AGILE::Protocol::Subscribe(string deviceId, GVariant* arguments)
{
    std::cout << "Subscribe not Implemented." << std::endl;
}

void AGILE::Protocol::Unsubscribe(string deviceId, GVariant* arguments)
{
    std::cout << "Unsubscribe not Implemented." << std::endl;
}

void AGILE::Protocol::onBusAcquiredCb(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    std::cout << "onBusAcquiredCb not implemented!" << std::endl;
}

void AGILE::Protocol::onNameAcquiredCb(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    std::cout << "onNameAcquiredCb not implemented!" << std::endl;
}

void AGILE::Protocol::onNameLostCb(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    std::cout << "onNameLostCb not implemented!" << std::endl;
}
