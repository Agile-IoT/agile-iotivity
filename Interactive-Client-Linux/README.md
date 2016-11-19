Interactive Client Linux
====================
This is an IoTivity-based client written for Linux. It is able to discover every resource by doing a query to a specific IP address (unicast or multicast). This client was tested with the [IoTivity example] for RIOT-OS.

#Features
At the moment this client is not complete. It supports:
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
NOTE: IoTivity was installed using the script install_iotivity.sh available on this re

#Usage
The command syntax is
```
 $ bin/client <destination>
```
where < destination > is the destination address of discovery. It can be a multicast or unicast IPv4/IPv6 address.
An example is (with the IPv6 RIOT-based example):
```
 $ bin/client ff03::158
```
or
```
 $ bin/client 224.0.1.187
```

#Test with the RIOT-based Application
This is a general IoTivity client but can be used to test the RIOT-based applications.
First of all, run the client:
```
 $ bin/client ff03::158
```
It starts the discovery phase:
```
Client: destination address is ff03::158
Client: Starting...
Client: NEW Resource Found: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/oic/p
Client: NEW Resource Found: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/oic/d
Client: NEW Resource Found: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/light/1
Client: There are 3 resource(s) in cache
```

Once it is completed, and it has found at least one resource, it enters in the Main Menu:
```
++================================================++
||                                                ||
||                 IoTivity CLIENT                ||
||                                                ||
++================================================++

Destination Address: ff03::158
Resource(s) discovered: 3

What do you want to do?
1 - Discovery ALL
2 - Print Resources information (From Cache)
3 - Show all Devices (From Nodes)
4 - Interact with Resources

0 - Exit
Choice:
```
Type one option and it enters in the chosen option (without typing ENTER).
##Discovery ALL
This feature allows to discover new resources that are not found during the previous discoveries. The output is:
```
Client: Discovery...
0: listenCallback(): failed to create resource. clientResponse: 255
Client: Resource Found: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/oic/p
Client: Resource Found: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/oic/d
Client: Resource Found: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/light/1
Client: NEW Resource Found: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/fan/1
Client: There are 4 resource(s) in cache

Client: Press any key to return to main menu...
```
If a resource is just discovered it is marked with the NEW label.

##Print Resources information (From Cache)
This functionality prints the information on cached (discovered) resources. It returns Host, URI, Types and Interfaces:
```
Client: Resource(s) discovered:

Host: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789
URI: /oic/p
Resource Types:
	oic.wk.p
Resource Interfaces: 
	oic.if.r
	oic.if.baseline

Host: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789
URI: /oic/d
Resource Types:
	oic.d.light
Resource Interfaces: 
	oic.if.r
	oic.if.baseline

Host: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789
URI: /light/1
Resource Types:
	oic.r.light
Resource Interfaces: 
	oic.if.rw
	oic.if.baseline

Client: Press any key to return to main menu...
```

##Show all Devices (From Nodes)
This feature sends a GET query to every resource with URI `/oic/d`. It parses responses and returns information:
```
Client: Host: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789
Client: Name: RIOT's light
Client: UUID: 0d9f5186-7a60-42df-6e6c-02890136b058
Client: Resource Type: oic.d.light
```

##Interact with Resources
This functionality allows to create custom requests to discovered resources. First of all, choose a resource:
```
++================================================++
||                                                ||
||             Interact with Resources            ||
||                                                ||
++================================================++

Choose a resource:
1 - coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/oic/p
2 - coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/oic/d
3 - coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/light/1

0 - Return to Main Menu
Choice: 
```
For example 3:
```
You have selected: coap://[2001:db8::5859:1c2a:64c7:c48a]:56789/light/1
Choose an operation:
1 -  Retrieve (GET)
2 -  Create (POST)
3 -  Update (PUT)
4 -  Delete (DELETE)

0 - Choose another resource
Choice: 
```
Now, you can select the desired option.
### Retrieve
It performs a GET request to the selected resource:
```
Performing GET...
state: false
Client: Press any key to return to continue...
```

###Create
It performs a POST request to the selected resource. NOTE: This functionality is not implemented yet.

###Update
It performs a PUT request to the selected resource. It is possible to set which attributes you want.
```
Client: Now, you can define attributes for the request.
Client: Datatype supported:
Client: 		bool
Client: 		string
Client: 		uint8, int8, uint16, int16, int32
Client: syntax: <type> <name> <value>
Client: Type ENTER to continue
Client: e.g. bool state false
Client: e.g. uint8 power 4
Client: e.g. string something HelloWorld
Attribute:
```
Now, enter your attributes, e.g. `bool state true` and type ENTER. If you want, you can enter a new attributes. Leave the attribute empty to send the request.
```
Attribute: bool state true
Attribute: 
Client: 1 attribute(s) set
Performing PUT...
Client: PUT Successful
Client: Press any key to return to continue...
```
You can verify if the resource is changed by performing a GET on this resource.
Note: PUT requests can be performed to resource without the `oic.if.r` (READABLE) interface.

###Delete
It performs a DELETE request to the selected resource. NOTE: This functionality is not implemented yet.


[IoTivity example]: https://github.com/Agile-IoT/agile-iotivity/tree/master/RIOT/examples/iotivity_examples#l2n_comm

