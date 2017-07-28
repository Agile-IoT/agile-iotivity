#-------------------------------------------------------------------------------
# Copyright (C) 2017 Create-Net / FBK.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
# 
# Contributors:
#     Create-Net / FBK - initial API and implementation
#-------------------------------------------------------------------------------

#USAGE: python write.py <devId> <state>

import sys
import dbus

BUS_NAME = "iot.agail.protocol.iotivity"
OBJECT_PATH = "/iot/agail/protocol/iotivity"
BUS_IF = "iot.agail.Protocol"

bus = dbus.SessionBus()

proxy = bus.get_object(BUS_NAME, OBJECT_PATH)
proxy_if = dbus.Interface(proxy, dbus_interface=BUS_IF)

devId = sys.argv[1]
state = sys.argv[2]

uri_struct = dbus.Struct(["URI", "/light/1"], signature='(sv)')

param_compAddr = dbus.Array([uri_struct], signature='a(sv)', variant_level=1)

param_payload = dbus.String("{\"state\":" + state + "}");

ret = proxy_if.Write(devId, param_compAddr, 0, param_payload)

print "\nPARAM: \n"
print param_compAddr
print param_payload
print "\nRET: \n"
print ret
print "\n"
