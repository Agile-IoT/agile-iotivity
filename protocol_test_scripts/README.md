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
Command:
```
 $ ./test_method.sh "read.py coap://[aaaa::5859:1c2a:64c7:c48a]:56789"
```

Response:
```
>> DBus session available
>> Loading DBus session instance address from local file
>> Source: /path/to/.dbus/session-bus/0123456789abcdeffedcba9876543210-0

PARAM:

dbus.Array([dbus.Struct(('URI', '/light/1'), signature=dbus.Signature('(sv)'))], signature=dbus.Signature('a(sv)'), variant_level=1)

RET:

dbus.Struct((dbus.String(u'coap://[aaaa::5859:1c2a:64c7:c48a]:56789'), dbus.Array([dbus.Struct((dbus.String(u'URI'), dbus.String(u'/light/1', variant_level=1)), signature=None)], signature=dbus.Signature('(sv)')), dbus.String(u'{"state": true}', variant_level=1), dbus.Double(1485854105.0)), signature=None)
```

Response on timeout:
```
>> DBus session available
>> Loading DBus session instance address from local file
>> Source: /path/to/.dbus/session-bus/0123456789abcdeffedcba9876543210-0

PARAM:

dbus.Array([dbus.Struct(('URI', '/light/1'), signature=dbus.Signature('(sv)'))], signature=dbus.Signature('a(sv)'), variant_level=1)

RET:

dbus.Struct((dbus.String(u'coap://[aaaa::5859:1c2a:64c7:c48a]:56789'), dbus.Array([dbus.Struct((dbus.String(u'URI'), dbus.String(u'/light/1', variant_level=1)), signature=None)], signature=dbus.Signature('(sv)')), dbus.String(u'timeout', variant_level=1), dbus.Double(1485853839.0)), signature=None)
```

###Write
Command:
```
 $ ./test_method.sh "write.py coap://[aaaa::5859:1c2a:64c7:c48a]:56789 true"
```

Response:
```
>> DBus session available
>> Loading DBus session instance address from local file
>> Source: /path/to/.dbus/session-bus/0123456789abcdeffedcba9876543210-0

PARAM:

dbus.Array([dbus.Struct(('URI', '/light/1'), signature=dbus.Signature('(sv)'))], signature=dbus.Signature('a(sv)'), variant_level=1)
{"state":true}

RET:

None

###Subscribe
Command:
```
 $ ./test_method.sh "subscribe.py coap://[aaaa::5859:1c2a:64c7:c48a]:56789"
```

Response:
```
>> DBus session available
>> Loading DBus session instance address from local file
>> Source: /path/to/.dbus/session-bus/0123456789abcdeffedcba9876543210-0

PARAM:

dbus.Array([dbus.Struct(('URI', '/light/1'), signature=dbus.Signature('(sv)'))], signature=dbus.Signature('a(sv)'), variant_level=1)

RET:

None

```

###Unsubscribe
TODO
