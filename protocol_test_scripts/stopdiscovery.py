#USAGE: python stopdiscovery.py

import sys
import dbus

BUS_NAME = "iot.agail.protocol.iotivity"
OBJECT_PATH = "/iot/agail/protocol/iotivity"
BUS_IF = "iot.agail.Protocol"

bus = dbus.SessionBus()

proxy = bus.get_object(BUS_NAME, OBJECT_PATH)
proxy_if = dbus.Interface(proxy, dbus_interface=BUS_IF)


print "\nPARAM: \n"
print "None"

ret = proxy_if.StopDiscovery()

print "\nRET: \n"
print ret
print "\n"
