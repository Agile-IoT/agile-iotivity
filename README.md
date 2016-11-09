AGILE - IoTivity Integration
======================

This repository contains instruction about the installation of [IoTivity] on a RaspberryPi and how to use it in conjunction with RIOT-OS.

#IoTivity on RaspberryPi
The installation of IoTivity on a RaspberryPi can be done by invoking
```
$ sudo -k ./install_iotivity.sh {Transport}
```
where Transport can be ALL/BT/BLE/IP/NFC/none. If you are undecided, choose ALL.
It was tested on RaspberryPi 3 with Raspian Jessie version September 2016.

#Applications
##RIOT-OS
The RIOT folder contains the IoTivity pkg for [RIOT-OS]. It allows to create embedded devices on which IoTivity runs. At the moment, the **Security** module is **disabled**! 
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
