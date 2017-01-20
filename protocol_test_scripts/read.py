#USAGE: python read.py

import sys
import dbus

BUS_NAME = "iot.agile.protocol.iotivity"
OBJECT_PATH = "/iot/agile/protocol/iotivity"
BUS_IF = "iot.agile.Protocol"

bus = dbus.SessionBus()

proxy = bus.get_object(BUS_NAME, OBJECT_PATH)
proxy_if = dbus.Interface(proxy, dbus_interface=BUS_IF)
{"URI":"/light/1", "TEST": "chudf"}

uri_struct = dbus.Struct(["URI", "/light/1"], signature='(sv)')

param_array = dbus.Array([uri_struct], signature='a(sv)', variant_level=1)

print "\nPARAM: \n"
print param_array

ret = proxy_if.Read("coap://[aaaa::5859:1c2a:64c7:c48a]:56789", param_array)

print "\nRET: \n"
print ret
print "\n"
