#USAGE: python stopdiscovery.py

import sys
import dbus

BUS_NAME = "iot.agile.protocol.iotivity"
OBJECT_PATH = "/iot/agile/protocol/iotivity"
BUS_IF = "iot.agile.Protocol"

bus = dbus.SessionBus()

proxy = bus.get_object(BUS_NAME, OBJECT_PATH)
proxy_if = dbus.Interface(proxy, dbus_interface=BUS_IF)


print "\nPARAM: \n"
print "None"

ret = proxy_if.StopDiscovery()

print "\nRET: \n"
print ret
print "\n"
