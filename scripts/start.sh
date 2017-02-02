#!/bin/sh

MODULE=${1:-all}

TOEXPORT=""

BASEDIR=$(dirname $0)

if [ ! -z "$DISPLAY" ]; then
  echo ">> DISPLAY available, reusing current display"
else
  export DISPLAY=:0
  TOEXPORT="\n$TOEXPORT\nexport DISPLAY=$DISPLAY"
fi

ME=`whoami`

if [ ! -z "$DBUS_SESSION_BUS_ADDRESS" ]; then
  echo ">> DBUS_SESSION_BUS_ADDRESS available, reusing current instance"
else

  if [ `pgrep -U $ME dbus-daemon -c` -gt 0 ]; then

    echo ">> DBus session available"

    MID=`sed "s/\n//" /var/lib/dbus/machine-id`
    DISPLAYID=`echo $DISPLAY | sed "s/://"`
    SESSFILEPATH="/home/$ME/.dbus/session-bus/$MID-$DISPLAYID"

    if [ -e $SESSFILEPATH ]; then
      echo ">> Loading DBus session instance address from local file"
      echo ">> Source: $SESSFILEPATH"
      . "$SESSFILEPATH"
    else
      echo "Cannot get Dbus session address. Panic!"
    fi

  else
    export `dbus-launch`
    sleep 2
    echo "++ Started a new DBus session instance"
  fi

fi

TOEXPORT="\n$TOEXPORT\nexport DBUS_SESSION_BUS_ADDRESS=$DBUS_SESSION_BUS_ADDRESS"

if [ -z "$DBUS_SESSION_BUS_ADDRESS" ]; then
  echo "!! Cannot export DBUS_SESSION_BUS_ADDRESS. Exit"
  exit 1
fi
export DBUS_SESSION_BUS_ADDRESS

if [ $MODULE = 'all' ] || [ $MODULE = 'iotivity' ]; then
  ./scripts/stop.sh "protocol.iotivity"

   if [ ! -e "$BASEDIR/../iot.agail.protocol.iotivity/bin" ]; then
     echo "Compiling..."
     make -C $BASEDIR/../iot.agail.protocol.iotivity/ agail_iotivity
   else
     make -C $BASEDIR/../iot.agail.protocol.iotivity/ agail_iotivity_tmp
   fi

  echo "Launching... AGAIL IoTivity protocol"
  $BASEDIR/../iot.agail.protocol.iotivity/bin/agail_iotivity realm
fi


echo "Module launched use this variables in the shell:"
echo $TOEXPORT
echo ""

wait
