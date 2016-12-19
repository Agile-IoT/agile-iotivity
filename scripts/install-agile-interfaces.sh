#!/bin/sh

CURRDIR=`pwd`
DEPS=${1:-$CURRDIR/iot.agile.protocol.BLE/deps}
BUILD=$DEPS/build

#clean up first
if [ -e "$DEPS/agile-interfaces" ] ; then
  rm -r $DEPS/agile-interfaces
  rm $DEPS/agile-interfaces*
  # drop from local repo eventually
  if [ -e ~/.m2/repository/iot/agile/agile-interfaces ] ; then
    rm -r  ~/.m2/repository/iot/agile/agile-interfaces
  fi
fi

if [ ! -e "$BUILD" ] ; then
  mkdir -p $BUILD
fi

if [ ! -e "$BUILD/agile-interfaces" ] ; then
  cd $BUILD
  git clone https://github.com/Agile-IoT/agile-api-spec.git
   cd agile-api-spec/agile-dbus-java-interface
  chmod 755  ./scripts/install-dependencies.sh
  ./scripts/install-dependencies.sh
   mvn package
   cp target/agile-interfaces-1.0.jar $DEPS
  cd ..
fi

cd $DEPS

mvn install:install-file -Dfile=$DEPS/agile-interfaces-1.0.jar \
                         -DgroupId=iot.agile \
                         -DartifactId=agile-interfaces \
                         -Dversion=1.0 \
                         -Dpackaging=jar \
                         -DgeneratePom=true \
                         -DlocalRepositoryPath=$DEPS
