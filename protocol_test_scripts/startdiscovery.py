#USAGE: python startdiscovery.py <N secs>

import sys
import dbus

BUS_NAME = "iot.agile.protocol.iotivity"
OBJECT_PATH = "/iot/agile/protocol/iotivity"
BUS_IF = "iot.agile.Protocol"

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
