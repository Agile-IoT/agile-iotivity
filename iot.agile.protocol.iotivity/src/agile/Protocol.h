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
 * @file        Protocol.h
 * @brief       Header file for AGILE Base Protocol file
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#ifndef __AGILE_PROTOCOL_H__
#define __AGILE_PROTOCOL_H__

#include <string>
#include <unistd.h>
#include <ctime>
#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <map>

#include <gio/gunixfdlist.h>
#include <gio/gio.h>

#include "DeviceOverview.h"
#include "constants.h"
#include "RecordObject.h"

using namespace std;

namespace AGILE {
    class Protocol;
}

class AGILE::Protocol {

    public:
    /**
     * Properties
     */
    static const string PROPERTY_STATUS;
    static const string PROPERTY_DRIVER;
    static const string PROPERTY_NAME;
    static const string PROPERTY_DATA;
    static const string PROPERTY_DEVICES;
    static const string PROPERTY_DISCOVERYSTATUS;
   
    /**
     * Methods
     */
    static const string METHOD_CONNECT;
    static const string METHOD_DISCONNECT;
    static const string METHOD_STARTDISCOVERY;
    static const string METHOD_STOPDISCOVERY;
    static const string METHOD_WRITE;
    static const string METHOD_READ;
    static const string METHOD_SUBSCRIBE;
    static const string METHOD_UNSUBSCRIBE;

    /**
     * Signals
     */
    static const string SIGNAL_FOUNDNEWDEVICE;
    static const string SIGNAL_NEWRECORD;

    /**
     * Protocol status
     */
    static const string PROTOCOL_STATUS_AVAILABLE;
    static const string PROTOCOL_STATUS_UNAVAILABLE;
    static const string PROTOCOL_STATUS_NOT_CONFIGURED;
    static const string PROTOCOL_STATUS_DISABLED;
    static const string PROTOCOL_STATUS_ENABLED;
    static const string PROTOCOL_STATUS_FAILURE;

    /**
     * Protocol discovery status
     */
    static const string PROTOCOL_DISCOVERY_STATUS_RUNNING;
    static const string PROTOCOL_DISCOVERY_STATUS_NONE;
    static const string PROTOCOL_DISCOVERY_STATUS_FAILURE;

    /**
     * Errors
     */
    static const int PROTOCOL_DBUS_INIT_NOERR = 0;
    static const int PROTOCOL_DBUS_INIT_ERROR = 1;

    /**
     * Write Errors
     */
    static const string PROTOCOL_WRITE_STATUS_DONE;
    static const string PROTOCOL_WRITE_STATUS_NOTIMPLEMENTED;
    static const string PROTOCOL_WRITE_STATUS_ARGSNOTVALID;
    static const string PROTOCOL_WRITE_STATUS_SIGARGSNOTVALID;
    static const string PROTOCOL_WRITE_STATUS_GENERICERROR;
    static const string PROTOCOL_WRITE_STATUS_TIMEOUT;
    static const string PROTOCOL_WRITE_STATUS_FAILED;

    private:
    std::vector<AGILE::DeviceOverview> devices;
    AGILE::RecordObject *data;
    static const gchar PROTOCOL_INTROSPECTION[];
    guint owner_id;
    GMainLoop *mainloop;
    GDBusNodeInfo *introspection_data = NULL;
    GDBusConnection *connection = NULL;
    string discoveryStatus = PROTOCOL_DISCOVERY_STATUS_NONE;

    static void onBusAcquired(GDBusConnection *, const gchar *, gpointer);
    static void onNameAcquired(GDBusConnection *, const gchar *, gpointer);
    static void onNameLost(GDBusConnection *, const gchar *, gpointer);

    static void handleMethodCall(GDBusConnection *, const gchar *, const gchar *, const gchar *, const gchar *, GVariant *, GDBusMethodInvocation *, gpointer);
    static GVariant* handleGetProperty(GDBusConnection *, const gchar *, const gchar *, const gchar *, const gchar *, GError **, gpointer);
    static gboolean handleSetProperty(GDBusConnection *, const gchar *, const gchar *, const gchar *, const gchar *, GVariant *, GError **, gpointer);

    static const GDBusInterfaceVTable interface_vtable;

    public:
    static Protocol *instance;
    /**
     * Bus name for AGILE Protocol
     */
    string BUS_NAME;

    /**
     * Bus path for AGILE Protocol
     */
    string BUS_PATH;

    /**
     * Protocol name
     */
    string PROTOCOL_NAME;

    /**
     * Protocol driver name
     */
    string DRIVER_NAME;

    Protocol();
    ~Protocol();
    int initBus();
    void keepAliveProtocol();
    void saveGDBusConnection(GDBusConnection *);

    //Devices list manipulation functions
    bool isNewDevice(AGILE::DeviceOverview *);
    bool addDevice(AGILE::DeviceOverview *);
    bool updateDevice(AGILE::DeviceOverview *);
    bool removeDevice(AGILE::DeviceOverview *);
    AGILE::DeviceOverview* getDeviceFromId(string);
    AGILE::DeviceOverview* getDeviceAt(int);
    int getDeviceListSize();
    bool emitFoundNewDeviceSignal(AGILE::DeviceOverview *);

    //RecordObjectManipulation
    AGILE::RecordObject* getLastRecordObject();
    void storeRecordObject(AGILE::RecordObject *);
    bool emitNewRecordSignal(AGILE::RecordObject *);

    //DiscoveryStatus Manipulation
    void setDiscoveryStatus(string);
    string getDiscoveryStatus();

    /**
     * Methods should be implemented in child class
     */

    virtual void onBusAcquiredCb(GDBusConnection *, const gchar *, gpointer);
    virtual void onNameAcquiredCb(GDBusConnection *, const gchar *, gpointer);
    virtual void onNameLostCb(GDBusConnection *, const gchar *, gpointer);

    virtual void onUnknownMethod(string);
    virtual void onUnknownProperty(string);

    //Methods
    virtual void Connect(string);
    virtual void Disconnect(string);
    virtual void StartDiscovery();
    virtual void StopDiscovery();
    virtual string Write(string, GVariant*);
    virtual AGILE::RecordObject* Read(string, GVariant*);
    virtual void Subscribe(string, GVariant*);
    virtual void Unsubscribe(string, GVariant*);
};

#endif
