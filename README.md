AGILE - IoTivity Integration
======================

This repository contains instruction about the installation of [IoTivity] on a RaspberryPi and how to use it in conjunction with RIOT-OS.

#IoTivity on RaspberryPi
The installation of IoTivity on a RaspberryPi can be done by invoking
```
$ sudo -k ./install_iotivity.sh
```
It was tested on RaspberryPi 3 with Raspian Jessie version September 2016.

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

##Linux Example
The Simple-Client-Linux folder contains an IoTivity-based client written in C++ for Linux. It can be used as test client for RIOT-based scenarios.

[RIOT-OS]: https://github.com/RIOT-OS/RIOT
[IoTivity]: https://www.iotivity.org
