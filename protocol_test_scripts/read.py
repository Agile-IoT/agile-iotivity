#-------------------------------------------------------------------------------
# Copyright (C) 2017 Create-Net / FBK.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0
# which accompanies this distribution, and is available at
# https://www.eclipse.org/legal/epl-2.0/
# 
# Contributors:
#     Create-Net / FBK - initial API and implementation
#-------------------------------------------------------------------------------

#USAGE: python read.py <<devId>>

import sys
import dbus

BUS_NAME = "iot.agail.protocol.iotivity"
OBJECT_PATH = "/iot/agail/protocol/iotivity"
BUS_IF = "iot.agail.Protocol"

bus = dbus.SessionBus()

proxy = bus.get_object(BUS_NAME, OBJECT_PATH)
proxy_if = dbus.Interface(proxy, dbus_interface=BUS_IF)

uri_struct = dbus.Struct(["URI", "/light/1"], signature='(sv)')

param_array = dbus.Array([uri_struct], signature='a(sv)', variant_level=1)

devId = sys.argv[1]

print "\nPARAM: \n"
print param_array

ret = proxy_if.Read(devId, param_array)

print "\nRET: \n"
print ret
print "\n"
