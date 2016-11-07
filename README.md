AGILE - IoTivity Integration
======================

This repository contains some IoTivity-based applications for different OSes.

#RIOT-OS
The RIOT folder contains the IoTivity pkg for [RIOT-OS]. It allows to create embedded devices on which IoTivity runs. At the moment, the **Security** module is **disabled**! 
##Integration in RIOT-OS
In order to use IoTivity over RIOT-OS you have to copy the folder pkg/iotivity in the pkg folder inside of your RIOT root. Then you have to add the following line to the application's makefile
```
USEPKG += iotivity
```
The compilation system will do the rest.
##Examples
Inside the folder examples/iotivity-examples there are some examples to test the pkg.

#Linux Example
The Simple-Client-Linux folder contains an IoTivity-based client written in C++ for Linux. It can be used as test client for RIOT-based scenarios.

[RIOT-OS]: https://github.com/RIOT-OS/RIOT
