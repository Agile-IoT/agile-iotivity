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

AGAIL - IoTivity Integration (AGAIL APIv2)
======================

This repository contains the alpha version of the AGAIL-IoTivity protocol.
It contains also some IoTivity-based applications for Linux and RIOT-OS.

Exposes the [iot.agail.Protocol V2](https://github.com/Agile-IoT/agile-api-spec/issues/15) interface

- DBus interface name **iot.agail.protocol.iotivity**
- DBus interface path **/iot/agail/protocol/iotivity**

#IoTivity on RaspberryPi
The installation of IoTivity on a RaspberryPi can be done by invoking
```
$ sudo -k ./install_iotivity.sh
```
It was tested on RaspberryPi 3 with Raspian Jessie version September 2016.

#Run the AGAIL-IoTivity Protocol

Under the `scripts` directory you can find different scripts to setup and start / stop the modules.

*Note* IoTivity must be already installed in the system.

- `./scripts/start.sh iotivity` will compile and start the module.

- `./scripts/stop.sh` will stop/kill the modules.

Under the `protocol_test_scripts` directory you can find different test script for the protocol.

#Applications
##RIOT-OS
The RIOT folder contains the IoTivity pkg for [RIOT-OS]. It allows to create embedded devices on which IoTivity runs. At the moment, the **Security** module is **disabled**! 

###Install RIOT-OS Building Environment on RaspberryPi
RIOT-OS requires a well-configured building environment. It can be installed by invoking
```
$ sudo -k ./install_build_env_RIOT.sh
```
It was tested on RaspberryPi 3 with Raspian Jessie version September 2016.

###Integration in RIOT-OS
In order to use IoTivity over RIOT-OS you have to copy the folder pkg/iotivity in the pkg folder inside of your RIOT root. Then you have to add the following line to the application's makefile
```
USEPKG += iotivity
```
The compilation system will do the rest.
###Examples
Inside the folder examples/iotivity-examples there are some examples to test the pkg.

##Linux Examples
###Simple Client
The [IoTivity/Simple-Client-Linux](IoTivity/Simple-Client-Linux) folder contains an IoTivity-based client written in C++ for Linux. It can be used as test client for RIOT-based scenarios.

###Interactive Client
The [IoTivity/Interactive-Client-Linux](IoTivity/Interactive-Client-Linux) folder contains an interactive IoTivity-based client. It can perform resource discoveries and custom CRUDN requests with a simple terminal-based interface.

[RIOT-OS]: https://github.com/RIOT-OS/RIOT
[IoTivity]: https://www.iotivity.org
