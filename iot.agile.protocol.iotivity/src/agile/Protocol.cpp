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
const string AGILE::Protocol::METHOD_CONNECT = "Connect";
const string AGILE::Protocol::METHOD_DISCONNECT = "Disconnect";
const string AGILE::Protocol::METHOD_STARTDISCOVERY = "StartDiscovery";
const string AGILE::Protocol::METHOD_STOPDISCOVERY = "StopDiscovery";
const string AGILE::Protocol::METHOD_WRITE = "Write";
const string AGILE::Protocol::METHOD_READ = "Read";
const string AGILE::Protocol::METHOD_SUBSCRIBE = "Subscribe";
const string AGILE::Protocol::METHOD_UNSUBSCRIBE = "Unsubscribe";
const string AGILE::Protocol::PROTOCOL_STATUS_AVAILABLE = "AVAILABLE";
const string AGILE::Protocol::PROTOCOL_STATUS_UNAVAILABLE = "UNAVAILABLE";
const string AGILE::Protocol::PROTOCOL_STATUS_NOT_CONFIGURED = "NOT_CONFIGURED";
const string AGILE::Protocol::PROTOCOL_STATUS_DISABLED = "DISABLED";
const string AGILE::Protocol::PROTOCOL_STATUS_ENABLED = "ENABLED";
const string AGILE::Protocol::PROTOCOL_STATUS_FAILURE = "FAILURE";
const string AGILE::Protocol::PROTOCOL_DISCOVERY_STATUS_RUNNING = "RUNNING";
const string AGILE::Protocol::PROTOCOL_DISCOVERY_STATUS_NONE = "NONE";
const string AGILE::Protocol::PROTOCOL_DISCOVERY_STATUS_FAILURE = "FAILURE";
const gchar AGILE::Protocol::PROTOCOL_INTROSPECTION[] =
    "<node name='/iot/agile/Protocol'>"
    "  <interface name='iot.agile.Protocol'>"
    "  <method name='Connect'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "  </method>"
    "  <method name='Disconnect'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "  </method>"
    "  <method name='StartDiscovery'>"
    "  </method>"
    "  <method name='StopDiscovery'>"
    "  </method>"
/*    "  <method name='Write'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='arguments' type='v' direction='in'/>"
    "    <arg name='return' type='{ss}' direction='out'/>"
    "  </method>"
    "  <method name='Read'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='arguments' type='v' direction='in'/>"
    "    <arg name='return' type='{sssssd}' direction='out'/>"
    "  </method>"
    "  <method name='Subscribe'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='arguments' type='v' direction='in'/>"
    "  </method>"
    "  <method name='Unsubscribe'>"
    "    <arg name='deviceId' type='s' direction='in'/>"
    "    <arg name='arguments' type='v' direction='in'/>"
    "    <arg name='return' type='{sssssd}' direction='out'/>"
    "  </method>"
*/    "  <property name='Status' type='s' access='read' />"
    "  <property name='Driver' type='s' access='read' />"
    "  <property name='Name' type='s' access='read' />"
    "  <property name='Data' type='{sssssd}' access='read' />"
/*    "<signal name='DataChanged'>"
    "  <arg name='Data' type='{sssssd}' />"
    "</signal>"
    "  <property name='Devices' type='{sss(i)}' access='read' />"
    "<signal name='DevicesChanged'>"
    "  <arg name='Devices' type='{sss(i)}' />"
    "</signal>"
*/    "  </interface>"
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

    return 0;
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

void AGILE::Protocol::handleMethodCall(GDBusConnection *connection, const gchar *sender, const gchar *object_path, const gchar *interface_name, const gchar *method_name, GVariant *parameters, GDBusMethodInvocation *invocation, gpointer user_data)
{
    //METHOD CONNECT
    if(g_strcmp0(method_name, METHOD_CONNECT.c_str()) == 0)
    {
        const gchar *deviceId;
        g_variant_get (parameters, "(&s)", &deviceId);
        instance->Connect(string(deviceId));
    }
    //METHOD DISCONNECT
    if(g_strcmp0(method_name, METHOD_DISCONNECT.c_str()) == 0)
    {
        const gchar *deviceId;
        g_variant_get (parameters, "(&s)", &deviceId);
        instance->Disconnect(string(deviceId));
    }
    //METHOD START DISCOVERY
    if(g_strcmp0(method_name, METHOD_STARTDISCOVERY.c_str()) == 0)
    {
        instance->StartDiscovery();
    }
    //METHOD STOP DISCOVERY
    else if(g_strcmp0(method_name, METHOD_STOPDISCOVERY.c_str()) == 0)
    {
        instance->StopDiscovery();
    }
}

GVariant* AGILE::Protocol::handleGetProperty(GDBusConnection *connection, const gchar *sender, const gchar *obj_path, const gchar *interface_name, const gchar *property_name, GError **error, gpointer user_data)
{
    GVariant * ret;
    cout << property_name << endl;
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

string AGILE::Protocol::DiscoveryStatus()
{
    return PROTOCOL_DISCOVERY_STATUS_NONE;
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
