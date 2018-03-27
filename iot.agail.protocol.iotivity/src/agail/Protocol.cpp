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
 * Description: Protocol
 *              AGAIL Base Protocol file
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#include "Protocol.h"
#include <iostream>


AGAIL::Protocol *AGAIL::Protocol::instance = nullptr;
const string AGAIL::Protocol::PROPERTY_STATUS = "Status";
const string AGAIL::Protocol::PROPERTY_ID = "Id";
const string AGAIL::Protocol::PROPERTY_NAME = "Name";
const string AGAIL::Protocol::PROPERTY_IMPLEMENTATIONID = "ImplementationId";
const string AGAIL::Protocol::PROPERTY_DEVICES = "Devices";
const string AGAIL::Protocol::PROPERTY_DISCOVERYSTATUS = "DiscoveryStatus";
const string AGAIL::Protocol::PROPERTY_CAPABILITIES = "Capabilities";
const string AGAIL::Protocol::METHOD_CONNECT = "Connect";
const string AGAIL::Protocol::METHOD_DISCONNECT = "Disconnect";
const string AGAIL::Protocol::METHOD_STARTDISCOVERY = "StartDiscovery";
const string AGAIL::Protocol::METHOD_STOPDISCOVERY = "StopDiscovery";
const string AGAIL::Protocol::METHOD_WRITE = "Write";
const string AGAIL::Protocol::METHOD_READ = "Read";
const string AGAIL::Protocol::METHOD_SUBSCRIBE = "Subscribe";
const string AGAIL::Protocol::METHOD_UNSUBSCRIBE = "Unsubscribe";
const string AGAIL::Protocol::SIGNAL_FOUNDNEWDEVICE = "FoundNewDeviceSignal";
const string AGAIL::Protocol::SIGNAL_NOTIFICATION = "NotificationSignal";
const string AGAIL::Protocol::PROTOCOL_STATUS_AVAILABLE = "AVAILABLE";
const string AGAIL::Protocol::PROTOCOL_STATUS_UNAVAILABLE = "UNAVAILABLE";
const string AGAIL::Protocol::PROTOCOL_STATUS_NOT_CONFIGURED = "NOT_CONFIGURED";
const string AGAIL::Protocol::PROTOCOL_STATUS_DISABLED = "DISABLED";
const string AGAIL::Protocol::PROTOCOL_STATUS_ENABLED = "ENABLED";
const string AGAIL::Protocol::PROTOCOL_STATUS_FAILURE = "FAILURE";
const string AGAIL::Protocol::PROTOCOL_DISCOVERY_STATUS_RUNNING = "RUNNING";
const string AGAIL::Protocol::PROTOCOL_DISCOVERY_STATUS_NONE = "NONE";
const string AGAIL::Protocol::PROTOCOL_DISCOVERY_STATUS_FAILURE = "FAILURE";
const gchar AGAIL::Protocol::PROTOCOL_INTROSPECTION[] =
    "<node name='/iot/agail/Protocol'>"
    "  <interface name='iot.agail.Protocol'>"
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
    "    <arg name='duration' type='i' direction='in'/>"
    "  </method>"
    "  <method name='StopDiscovery'>"
    "    <annotation name='org.freedesktop.DBus.Description' value='Stop device discovery' />"
    "  </method>"
    "  <method name='Write'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='componentAddr' type='a(sv)' direction='in'/>"
    "    <arg name='flags' type='u' direction='in'/>"
    "    <arg name='payload' type='v' direction='in'/>"
    "  </method>"
    "  <method name='Read'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='componentAddr' type='a(sv)' direction='in'/>"
    "    <arg name='return' type='(sa(sv)vd)' direction='out'/>"
    "  </method>"
    "  <method name='Subscribe'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='componentAddr' type='a(sv)' direction='in'/>"
    "  </method>"
    "  <method name='Unsubscribe'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='componentAddr' type='a(sv)' direction='in'/>"
    "  </method>"
    "  <property name='Status' type='s' access='read' />"
    "  <property name='ImplementationId' type='s' access='read' />"
    "  <property name='Name' type='s' access='read' />"
    "  <property name='Id' type='s' access='read' />"
    "  <property name='Devices' type='a(ssssd)' access='read' />"
    "  <property name='DiscoveryStatus' type='s' access='read' />"
    "  <property name='Capabilities' type='a(sv)' access='read' />"
    "<signal name='NotificationSignal'>"
    "  <arg name='Data' type='(sa(sv)vd)' />"
    "</signal>"
    "<signal name='FoundNewDeviceSignal'>"
    "  <arg name='Device' type='(ssssd)' />"
    "</signal>"
    "  </interface>"
    "</node>";

const GDBusInterfaceVTable AGAIL::Protocol::interface_vtable =
{
  AGAIL::Protocol::handleMethodCall,
  AGAIL::Protocol::handleGetProperty,
  AGAIL::Protocol::handleSetProperty
};

AGAIL::Protocol::Protocol()
{
    BUS_NAME = "Unknown";
    BUS_PATH = "Unknown";
    PROTOCOL_NAME = "Unknown";
    PROTOCOL_ID = "Unknown";
    PROTOCOL_IMPLEMENTATIONID = "Unknown";

    data = new AGAIL::RecordObject();
}

// TODO: implement it
AGAIL::Protocol::~Protocol()
{
}

int AGAIL::Protocol::initBus()
{
    introspection_data = g_dbus_node_info_new_for_xml (PROTOCOL_INTROSPECTION, NULL);
    if(introspection_data == NULL)
    {
        return PROTOCOL_DBUS_INIT_ERROR;
    }

    owner_id = g_bus_own_name(G_BUS_TYPE_SESSION,
                             BUS_NAME.c_str(),
                             G_BUS_NAME_OWNER_FLAGS_NONE,
                             &AGAIL::Protocol::onBusAcquired,
                             &AGAIL::Protocol::onNameAcquired,
                             &AGAIL::Protocol::onNameLost,
                             NULL,
                             NULL);

    mainloop = g_main_loop_new (NULL, FALSE);

    return PROTOCOL_DBUS_INIT_NOERR;
}

void AGAIL::Protocol::saveGDBusConnection(GDBusConnection *conn)
{
    connection = conn;
}

void AGAIL::Protocol::onBusAcquired(GDBusConnection *conn, const gchar *name, gpointer user_data)
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

void AGAIL::Protocol::onNameAcquired(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    instance->onNameAcquiredCb(conn, name, user_data);
}

void AGAIL::Protocol::onNameLost(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    instance->onNameLostCb(conn, name, user_data);
}

void AGAIL::Protocol::onUnknownMethod(string method)
{
    std::cout << "onUnknownMethod not implemented! Arg: " << method << std::endl;
}

void AGAIL::Protocol::onUnknownProperty(string property)
{
    std::cout << "onUnknownProperty not implemented! Arg: " << property << std::endl;
}

void AGAIL::Protocol::handleMethodCall(GDBusConnection *connection, const gchar *sender, const gchar *object_path, const gchar *interface_name, const gchar *method_name, GVariant *parameters, GDBusMethodInvocation *invocation, gpointer user_data)
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
        int duration;
        g_variant_get (parameters, "(i)", &duration);
        instance->StartDiscovery(duration);
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
        GVariantIter *iterator;
        GVariantBuilder *compAddrBuilder;

        std::map<string, GVariant *> componentAddr;
        componentAddr.clear();

        g_variant_get (parameters, "(&sa(sv))", &deviceId, &iterator);
        parseComponentAddr(iterator, &componentAddr, &compAddrBuilder);

        PayloadObject *data = instance->Read(string(deviceId), componentAddr);
        out = g_variant_new("((sa(sv)vd))", data->deviceId.c_str(), compAddrBuilder, g_variant_deep_copy(data->payload), data->lastUpdate);

        delete data;
        g_variant_builder_unref(compAddrBuilder);
    }
    //METHOD WRITE
    else if(g_strcmp0(method_name, METHOD_WRITE.c_str()) == 0)
    {
        const gchar *deviceId;
        uint32_t flags;
        GVariant *payload;
        GVariantIter *iterator;
        GVariantBuilder *compAddrBuilder;

        std::map<string, GVariant *> componentAddr;
        componentAddr.clear();

        g_variant_get (parameters, "(&sa(sv)uv)", &deviceId, &iterator, &flags, &payload);
        parseComponentAddr(iterator, &componentAddr, &compAddrBuilder);

        instance->Write(string(deviceId), componentAddr, flags, payload);
        g_variant_builder_unref(compAddrBuilder);
    }
    //METHOD SUBSCRIBE
    else if(g_strcmp0(method_name, METHOD_SUBSCRIBE.c_str()) == 0)
    {
        const gchar *deviceId;
        GVariantIter *iterator;
        GVariantBuilder *compAddrBuilder;

        std::map<string, GVariant *> componentAddr;
        componentAddr.clear();

        g_variant_get (parameters, "(&sa(sv))", &deviceId, &iterator);
        parseComponentAddr(iterator, &componentAddr, &compAddrBuilder);
        instance->Subscribe(string(deviceId), componentAddr);
        g_variant_builder_unref(compAddrBuilder);
    }
    //METHOD UNSUBSCRIBE
    else if(g_strcmp0(method_name, METHOD_UNSUBSCRIBE.c_str()) == 0)
    {
        const gchar *deviceId;
        GVariantIter *iterator;
        GVariantBuilder *compAddrBuilder;

        std::map<string, GVariant *> componentAddr;
        componentAddr.clear();

        g_variant_get (parameters, "(&sa(sv))", &deviceId, &iterator);
        parseComponentAddr(iterator, &componentAddr, &compAddrBuilder);
        instance->Unsubscribe(string(deviceId), componentAddr);
        g_variant_builder_unref(compAddrBuilder);
    }
    else
    {
        instance->onUnknownMethod(method_name);
    }

    g_dbus_method_invocation_return_value(invocation, out);
}

GVariant* AGAIL::Protocol::handleGetProperty(GDBusConnection *connection, const gchar *sender, const gchar *obj_path, const gchar *interface_name, const gchar *property_name, GError **error, gpointer user_data)
{
    GVariant * ret;
    ret = NULL;

    //PROPERTY PROTOCOL NAME
    if(g_strcmp0(property_name, PROPERTY_NAME.c_str()) == 0)
    {
        ret = g_variant_new_string(instance->PROTOCOL_NAME.c_str());
    }
    //PROPERTY UNIQUE ID
    else if(g_strcmp0(property_name, PROPERTY_ID.c_str()) == 0)
    {
        ret = g_variant_new_string(instance->PROTOCOL_ID.c_str());
    }
    //PROPERTY STATUS
    else if(g_strcmp0(property_name, PROPERTY_STATUS.c_str()) == 0)
    {
        ret = g_variant_new_string(PROTOCOL_STATUS_AVAILABLE.c_str());
    }
    //PROPERTY DEVICES
    else if(g_strcmp0(property_name, PROPERTY_DEVICES.c_str()) == 0)
    {
        GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("a(ssssd)"));
        for(int i=0; i<instance->getDeviceListSize(); i++) {
           AGAIL::DeviceOverview *d = instance->getDeviceAt(i);
           g_variant_builder_add(builder, "(ssssd)", d->getName().c_str(),
               d->getProtocol().c_str(), d->getId().c_str(), d->getStatus().c_str(), d->getLastSeen());
        }
        ret = g_variant_new("a(ssssd)", builder);
        g_variant_builder_unref(builder);        
    }
    //PROPERTY IMPLEMENTATION ID
    else if(g_strcmp0(property_name, PROPERTY_IMPLEMENTATIONID.c_str()) == 0)
    {
        ret = g_variant_new_string(instance->PROTOCOL_IMPLEMENTATIONID.c_str());
    }
    //PROPERTY DISCOVERY STATUS
    else if(g_strcmp0(property_name, PROPERTY_DISCOVERYSTATUS.c_str()) == 0)
    {
        ret = g_variant_new_string(instance->getDiscoveryStatus().c_str());
    }
    //PROPERTY CAPABILITIES
    else if(g_strcmp0(property_name, PROPERTY_CAPABILITIES.c_str()) == 0)
    {
        //TODO: RESERVED FOR FUTURE USE (Now it returns an empty array)
        GVariantBuilder *builder = g_variant_builder_new(G_VARIANT_TYPE("a(sv)"));
        ret = g_variant_new("a(sv)", builder);
        g_variant_builder_unref(builder);
    }
    //PROPERTY UNKNOWN
    else
    {
        instance->onUnknownProperty(property_name);
    }
    return ret;
}

gboolean AGAIL::Protocol::handleSetProperty(GDBusConnection *connection, const gchar *sender, const gchar *obj_path, const gchar *interface_name, const gchar *property_name, GVariant *value, GError **error, gpointer user_data)
{
    cout << "handleSetProperty" << endl;
}

void AGAIL::Protocol::keepAliveProtocol()
{
    g_main_loop_run(mainloop);
    g_bus_unown_name(owner_id);
    g_dbus_node_info_unref(introspection_data);

    return;
}

bool AGAIL::Protocol::emitFoundNewDeviceSignal(AGAIL::DeviceOverview *dev)
{
    GError *local_error;
    GVariant * device_variant;

    local_error = NULL;

    device_variant = g_variant_new("(ssssd)", dev->getName().c_str(), dev->getProtocol().c_str(),
        dev->getId().c_str(), dev->getStatus().c_str(), dev->getLastSeen());

    g_dbus_connection_emit_signal(connection,
                                  NULL,
                                  BUS_PATH.c_str(),
                                  AGAIL::AGAIL_PROTOCOL_INTERFACE.c_str(), 
                                  SIGNAL_FOUNDNEWDEVICE.c_str(),
                                  device_variant,
                                  &local_error);

    if(local_error == NULL)
    {
       return true;
    }

    return false;
}

bool AGAIL::Protocol::isNewDevice(AGAIL::DeviceOverview *dev)
{
    return (std::find(devices.begin(), devices.end(), *dev) == devices.end());
}

bool AGAIL::Protocol::addDevice(AGAIL::DeviceOverview *dev)
{
    if(isNewDevice(dev)) {
        devices.push_back(*dev);
        return true;
    }
    return false;
}

bool AGAIL::Protocol::updateDevice(AGAIL::DeviceOverview *dev)
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

bool AGAIL::Protocol::removeDevice(AGAIL::DeviceOverview *dev)
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

AGAIL::DeviceOverview* AGAIL::Protocol::getDeviceFromId(string id)
{
    AGAIL::DeviceOverview* t = new AGAIL::DeviceOverview("", PROTOCOL_NAME, id);
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

AGAIL::DeviceOverview* AGAIL::Protocol::getDeviceAt(int pos)
{
    if(pos>=0 || pos<devices.size())
    {
        return &devices.at(pos);
    }
    return NULL;
}

int AGAIL::Protocol::getDeviceListSize()
{
    return devices.size();
}

void AGAIL::Protocol::parseComponentAddr(GVariantIter *iter, map<string, GVariant*> *caMap, GVariantBuilder **caBuilder)
{
    GVariant *value;
    const gchar *key;

    *caBuilder = g_variant_builder_new(G_VARIANT_TYPE("a(sv)"));

    //Extraction component address from dbus to map
    while(g_variant_iter_loop(iter, "(&sv)", &key, &value))
    {
        caMap->insert({string(key), g_variant_deep_copy(value)});
        g_variant_builder_add(*caBuilder, "(sv)", string(key).c_str(), g_variant_deep_copy(value));
    }
    g_variant_iter_free(iter);
}

bool AGAIL::Protocol::emitNotificationSignal(AGAIL::PayloadObject *po)
{
    GError *local_error;
    GVariant * notification_variant;
    GVariantBuilder *compAddrBuilder;

    local_error = NULL;

    compAddrBuilder = g_variant_builder_new(G_VARIANT_TYPE("a(sv)"));

    for(auto const& elem : po->componentAddr)
    {
        g_variant_builder_add(compAddrBuilder, "(sv)", string(elem.first).c_str(), g_variant_deep_copy(elem.second));
    }

    notification_variant = g_variant_new("((sa(sv)vd))", po->deviceId.c_str(), compAddrBuilder, g_variant_deep_copy(po->payload), po->lastUpdate);

    g_dbus_connection_emit_signal(connection,
                                  NULL,
                                  BUS_PATH.c_str(),
                                  AGAIL::AGAIL_PROTOCOL_INTERFACE.c_str(), 
                                  SIGNAL_NOTIFICATION.c_str(),
                                  notification_variant,
                                  &local_error);

    g_variant_builder_unref(compAddrBuilder);

    if(local_error == NULL)
    {
       return true;
    }

    return false;
}

void AGAIL::Protocol::setDiscoveryStatus(string status)
{
    if(status == PROTOCOL_DISCOVERY_STATUS_RUNNING ||
       status == PROTOCOL_DISCOVERY_STATUS_NONE ||
       status == PROTOCOL_DISCOVERY_STATUS_FAILURE)
    {
        discoveryStatus = status;
    }
}

string AGAIL::Protocol::getDiscoveryStatus()
{
    return discoveryStatus;
}

void AGAIL::Protocol::Connect(string deviceId)
{
    std::cout << "Connect not implemented! Arg: " << deviceId << std::endl;
}

void AGAIL::Protocol::Disconnect(string deviceId)
{
    std::cout << "Disconnect not implemented! Arg: " << deviceId << std::endl;
}

void AGAIL::Protocol::StartDiscovery(int duration)
{
    std::cout << "StartDiscovery not implemented! Duration: " << duration <<std::endl;
}

void AGAIL::Protocol::StopDiscovery()
{
    std::cout << "StopDiscovery not implemented!" << std::endl;
}

AGAIL::PayloadObject* AGAIL::Protocol::Read(string deviceId, std::map<string, GVariant *> componentAddr)
{
    std::cout << "Read not Implemented." << std::endl;
    return new PayloadObject();
}

void AGAIL::Protocol::Write(string deviceId, std::map<string, GVariant *> componentAddr, uint32_t flags, GVariant *payload)
{
    std::cout << "Write not Implemented." << std::endl;
}

void AGAIL::Protocol::Subscribe(string deviceId, std::map<string, GVariant *> componentAddr)
{
    std::cout << "Subscribe not Implemented." << std::endl;
}

void AGAIL::Protocol::Unsubscribe(string deviceId, std::map<string, GVariant *> componentAddr)
{
    std::cout << "Unsubscribe not Implemented." << std::endl;
}

void AGAIL::Protocol::onBusAcquiredCb(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    std::cout << "onBusAcquiredCb not implemented!" << std::endl;
}

void AGAIL::Protocol::onNameAcquiredCb(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    std::cout << "onNameAcquiredCb not implemented!" << std::endl;
}

void AGAIL::Protocol::onNameLostCb(GDBusConnection *conn, const gchar *name, gpointer user_data)
{
    std::cout << "onNameLostCb not implemented!" << std::endl;
}
