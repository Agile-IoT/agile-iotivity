Interactive Client Linux
====================
This is an interactive IoTivity-based client written for Linux. It is able to discover every resource by doing a query to a specific IP address (unicast or multicast). This client was tested with the [IoTivity example] for RIOT-OS.

#Features
At the moment, this client is not complete. It supports:
 - Resource Discovery (finds all resources)
 - Retrieving device Information and parsing
 - Interaction with discovered resources
	 - GET
	 - PUT
   
Next versions will support also:
 - Selective Resource Discovery
 - Interaction with discovered resources
	 - POST
	 - DELETE
	 - OBSERVE

#Build
Go to the /Interactive-Client-Linux and type
```
 $ make client
```
NOTE: IoTivity was installed using the script install_iotivity.sh available on this repo.

#Usage
The command syntax is
```
 $ bin/client <destination>
```
where < destination > is the destination address of discovery. It can be a multicast or unicast IPv4/IPv6 address.

An example is:
```
 $ bin/client ff03::158
```

[IoTivity example]: https://github.com/Agile-IoT/agile-iotivity/tree/master/RIOT/examples/iotivity_examples#l2n_comm
