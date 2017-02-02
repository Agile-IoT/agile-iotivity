#USAGE: python startdiscovery.py <N secs>

import sys
import dbus

BUS_NAME = "iot.agail.protocol.iotivity"
OBJECT_PATH = "/iot/agail/protocol/iotivity"
BUS_IF = "iot.agail.Protocol"

bus = dbus.SessionBus()

proxy = bus.get_object(BUS_NAME, OBJECT_PATH)
proxy_if = dbus.Interface(proxy, dbus_interface=BUS_IF)

delay_discovery = dbus.Int32(sys.argv[1])


print "\nPARAM: \n"
print delay_discovery

ret = proxy_if.StartDiscovery(delay_discovery)

print "\nRET: \n"
print ret
print "\n"
