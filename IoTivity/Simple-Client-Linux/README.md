<!--
# Copyright (C) 2017 Create-Net / FBK.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
# 
# Contributors:
#     Create-Net / FBK - initial API and implementation
-->

Simple Client Linux Example
========================

This is a simple IoTivity client written for Linux. It is able to discover every resource by doing a query to a specific IP address (unicast or multicast). If some resources have as ResourceType *oic.r.light*, it tries to interact with them. This client was tested with the [IoTivity example][1] for RIOT-OS. 

#Build
Go to the /Simple-Client-Linux and type
```
 $ make
```
NOTE: IoTivity was installed using the script `install_iotivity.sh` available on this repo.

#Usage
The command syntax is
```
 $ bin/simple_client <destination> [command]
```
where 
 - destination: It is the destination address of discovery. It can be a multicast or unicast IPv4/IPv6 address.
 - command:
	 - off: switch off the light (Default)
	 - on: switch on the light
	 - toggle: toggle the light

An example is:
```
 $ bin/simple_client ff03::158 toggle
```

#Test the RIOT-based application
As mentioned previously, this client was developed as test client for this [IoTivity example][1]. In order to run the test, type
```
 $ bin/simple_client ff03::158 toggle
```
The output should be similar to this:
```
Client is Running!
0: listenCallback(): failed to create resource. clientResponse: 255

Resource Found

Resource Found

Resource Found
Found 3 Resource(s)

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
Light resource found!
GET Successful!
State: OFF
PUT on Light Resource!
PUT Successful!
GET Successful!
State: ON

```

*Note*: Delays for Discoveries and Requests are 10 and 3 seconds, respectively. They are hard-coded in the client code.

 [1]: https://github.com/Agile-IoT/agile-iotivity/tree/master/RIOT/examples/iotivity_examples#l2n_comm
