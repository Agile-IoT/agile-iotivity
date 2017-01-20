#USAGE: python write.py <state>

import sys
import dbus

BUS_NAME = "iot.agile.protocol.iotivity"
OBJECT_PATH = "/iot/agile/protocol/iotivity"
BUS_IF = "iot.agile.Protocol"

bus = dbus.SessionBus()

proxy = bus.get_object(BUS_NAME, OBJECT_PATH)
proxy_if = dbus.Interface(proxy, dbus_interface=BUS_IF)

state = sys.argv[1]

uri_struct = dbus.Struct(["URI", "/light/1"], signature='(sv)')

param_compAddr = dbus.Array([uri_struct], signature='a(sv)', variant_level=1)

param_payload = dbus.String("{\"state\":" + state + "}");

ret = proxy_if.Write("coap://[aaaa::5859:1c2a:64c7:c48a]:56789", param_compAddr, 0, param_payload)

print "\nPARAM: \n"
print param_compAddr
print param_payload
print "\nRET: \n"
print ret
print "\n"
