#Test the AGILE-IoTivity Protocol
This folder contains some test scripts for the AGILE-IoTivity Protocol.

##Preliminary Step
 - Run the AGILE-IoTivity protocol
 - Deploy the RIOT based scenario by following this [Tutorial](https://github.com/Agile-IoT/agile-iotivity/wiki/%5BTutorial%5D-Configure-IoTivity-on-a-RaspberryPi-and-test-it-with-RIOT-based-nodes#run_scenario) (Steps 4, 5.1 and 5.2)

##Usage
The sintax is very simple
```
 $ ./test_method.sh "<file_name>.py <args>"
```

##Examples
###Start Discovery
Command:
```
 $ ./test_method.sh "startdiscovery.py 5"
```

Response:
```
>> DBus session available
>> Loading DBus session instance address from local file
>> Source: /path/to/.dbus/session-bus/0123456789abcdeffedcba9876543210-0

PARAM: 

5

RET: 

None
```

###Stop Discovery
Command:
```
 $ ./test_method.sh "stopdiscovery.py"
```

Response:
```
>> DBus session available
>> Loading DBus session instance address from local file
>> Source: /path/to/.dbus/session-bus/0123456789abcdeffedcba9876543210-0

PARAM: 

None

RET: 

None
```

###Read
TODO

###Write
TODO

###Subscribe
TODO

###Unsubscribe
TODO
