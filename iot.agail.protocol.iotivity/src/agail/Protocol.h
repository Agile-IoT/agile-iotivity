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
 * Description: Protocol
 *              Header file for AGAIL Base Protocol file
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#ifndef __AGAIL_PROTOCOL_H__
#define __AGAIL_PROTOCOL_H__

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
#include "PayloadObject.h"
#include "DBusUtility.h"

using namespace std;

namespace AGAIL {
    class Protocol;
}

class AGAIL::Protocol {

    public:
    /**
     * Properties
     */
    static const string PROPERTY_STATUS;
    static const string PROPERTY_ID;
    static const string PROPERTY_NAME;
    static const string PROPERTY_IMPLEMENTATIONID;
    static const string PROPERTY_DEVICES;
    static const string PROPERTY_DISCOVERYSTATUS;
    static const string PROPERTY_CAPABILITIES;
   
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
    static const string SIGNAL_NOTIFICATION;

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
     * Write OPTIONS
     */
    static const uint32_t PROTOCOL_WRITE_ASYNC = 0x0000;
    static const uint32_t PROTOCOL_WRITE_SYNC = 0x0001;

    static const uint32_t PROTOCOL_WRITE_NONREL = 0x0000;
    static const uint32_t PROTOCOL_WRITE_REL = 0x0002;

    static const uint32_t PROTOCOL_WRITE_NONSEC = 0x0000;
    static const uint32_t PROTOCOL_WRITE_SEC = 0x0004;

    private:
    std::vector<AGAIL::DeviceOverview> devices;
    AGAIL::RecordObject *data;
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
    static void parseComponentAddr(GVariantIter *, map<string, GVariant*> *, GVariantBuilder **);

    public:
    static Protocol *instance;
    /**
     * Bus name for AGAIL Protocol
     */
    string BUS_NAME;

    /**
     * Bus path for AGAIL Protocol
     */
    string BUS_PATH;

    /**
     * Protocol name
     */
    string PROTOCOL_NAME;

    /**
     * Protocol unique ID
     */
    string PROTOCOL_ID;

    /**
     * Protocol Implementation ID
     */
    string PROTOCOL_IMPLEMENTATIONID;


    Protocol();
    ~Protocol();
    int initBus();
    void keepAliveProtocol();
    void saveGDBusConnection(GDBusConnection *);

    //Devices list manipulation functions
    bool isNewDevice(AGAIL::DeviceOverview *);
    bool addDevice(AGAIL::DeviceOverview *);
    bool updateDevice(AGAIL::DeviceOverview *);
    bool removeDevice(AGAIL::DeviceOverview *);
    AGAIL::DeviceOverview* getDeviceFromId(string);
    AGAIL::DeviceOverview* getDeviceAt(int);
    int getDeviceListSize();
    bool emitFoundNewDeviceSignal(AGAIL::DeviceOverview *);

    bool emitNotificationSignal(AGAIL::PayloadObject *);

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
    virtual void StartDiscovery(int);
    virtual void StopDiscovery();
    virtual void Write(string, std::map<string, GVariant *>, uint32_t, GVariant *);
    virtual AGAIL::PayloadObject* Read(string, std::map<string, GVariant *>);
    virtual void Subscribe(string, std::map<string, GVariant *>);
    virtual void Unsubscribe(string, std::map<string, GVariant *>);
};

#endif
