#USAGE: python subscribe.py <devId>

import sys
import dbus

BUS_NAME = "iot.agile.protocol.iotivity"
OBJECT_PATH = "/iot/agile/protocol/iotivity"
BUS_IF = "iot.agile.Protocol"

bus = dbus.SessionBus()

proxy = bus.get_object(BUS_NAME, OBJECT_PATH)
proxy_if = dbus.Interface(proxy, dbus_interface=BUS_IF)

uri_struct = dbus.Struct(["URI", "/light/1"], signature='(sv)')

param_array = dbus.Array([uri_struct], signature='a(sv)', variant_level=1)

devId = sys.argv[1]

print "\nPARAM: \n"
print param_array

ret = proxy_if.Subscribe(devId, param_array)

print "\nRET: \n"
print ret
print "\n"
