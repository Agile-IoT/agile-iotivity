#!/bin/sh
#-------------------------------------------------------------------------------
# Copyright (C) 2017 Create-Net / FBK.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License v1.0
# which accompanies this distribution, and is available at
# http://www.eclipse.org/legal/epl-v10.html
# 
# Contributors:
#     Create-Net / FBK - initial API and implementation
#-------------------------------------------------------------------------------
#####################################################################
#                                                                   #
# Installation Script for IoTivity                                  #
#                                                                   #
# Author: Mattia Antonini <mattia.antonini1@studenti.unipr.it>      #
#                         <m.antonini@create-net.org>               #
#                                                                   #
# This script downloads, unzips, compiles IoTivity and then it      #
# copies binaries in /usr. Tested with IoTivity 1.2.0.              #
#####################################################################

# Setup Params
IOTIVITY_OS=linux          #linux/android/arduino/yocto/tizen
IOTIVITY_ARCH=arm      #x86/x86_64/arm/arm-v7a/armeabi-v7a/arm64
IOTIVITY_VERS=1.2.0
IOTIVITY_TRANSPORT=ALL      #ALL/BT/BLE/IP/NFC/none
IOTIVITY_SECURED=0         #0/1
IOTIVITY_RELEASE=yes       #yes/no
IOTIVITY_ROUTING=EP        #EP/GW
IOTIVITY_BUILD_SAMPLE=OFF  #ON/OFF

#Environment Vars
IOTIVITY_WORKING_DIR=$(dirname $(readlink -f $0))"/iotivity_wdir"
IOTIVITY_COMPILED_BINARIES_DIR=$(dirname $(readlink -f $0))"/iotivity_wdir/iotivity-"${IOTIVITY_VERS}"/out/"${IOTIVITY_OS}"/"${IOTIVITY_ARCH}"/release/"

#Functions
usage() {
    echo "Usage: $0 <Transport>" 
}

checksudo() {
    if [ $(id -u) -ne 0 ] ; then
        echo "Please run as root."
        echo "e.g. sudo -k "$0
        exit 1
    fi
}

review_params() {
    echo "IoTivity will be installed with the following settings:"
    echo ""
    echo "TARGET_OS: "${IOTIVITY_OS}
    echo "TARGET_ARCH: "${IOTIVITY_ARCH}
    echo "TARGET_TRANSPORT: "${IOTIVITY_TRANSPORT}
    echo "SECURED: "${IOTIVITY_SECURED}
    echo "RELEASE: "${IOTIVITY_RELEASE}
    echo "ROUTING: "${IOTIVITY_ROUTING}
    echo "BUILD_SAMPLE":${IOTIVITY_BUILD_SAMPLE}
    echo "IoTivity Version: "${IOTIVITY_VERS}
    echo "IoTivity working directory: "${IOTIVITY_WORKING_DIR}
    echo ""
    read -p "If you agree type Y otherwise N: " opts

    if [ "$opts" = "Y" ] || [ "$opts" = "y" ]; then
        echo "Setup accepted..."
    elif [ "$opts" = "N" ] || [ "$opts" = "n" ]; then
        echo "Exiting..."
        exit 1
    else
        echo "Choice unknown! Exiting..."
        exit 1
    fi
}

setup_dep() {
    sudo apt-get update
    sudo apt-get -y install scons build-essential libboost-dev libexpat1-dev libboost-thread-dev libboost-program-options-dev uuid-dev libssl-dev libglib2.0-dev
}

download_unzip() {
    if [ ! -d "${IOTIVITY_WORKING_DIR}" ] ; then
       mkdir ${IOTIVITY_WORKING_DIR}
    fi

    cd ${IOTIVITY_WORKING_DIR}

    if [ ! -d "iotivity-${IOTIVITY_VERS}" ] ; then
           wget -c http://mirrors.kernel.org/iotivity/${IOTIVITY_VERS}/iotivity-${IOTIVITY_VERS}.zip
           unzip iotivity-${IOTIVITY_VERS}.zip
    fi

    cd iotivity-${IOTIVITY_VERS}
    if [ ! -d "extlibs/tinycbor/tinycbor" ] ; then
        git clone https://github.com/01org/tinycbor.git extlibs/tinycbor/tinycbor -b v0.3.2
    fi
}

compile() {
    scons TARGET_ARCH=${IOTIVITY_ARCH} TARGET_OS=${IOTIVITY_OS} TARGET_TRANSPORT=${IOTIVITY_TRANSPORT} RELEASE=${IOTIVITY_RELEASE} SECURED=${IOTIVITY_SECURED} ROUTING=${IOTIVITY_ROUTING} BUILD_SAMPLE=${IOTIVITY_BUILD_SAMPLE}
}

copy_binaries(){
    sudo cp ${IOTIVITY_COMPILED_BINARIES_DIR}/*.a /usr/lib
    sudo cp ${IOTIVITY_COMPILED_BINARIES_DIR}/*.so /usr/lib

    if [ ! -d "/usr/include/iotivity" ] ; then
       sudo mkdir /usr/include/iotivity
    fi
    
    sudo cp -r ${IOTIVITY_COMPILED_BINARIES_DIR}/include/resource /usr/include/iotivity/
    sudo cp -r ${IOTIVITY_COMPILED_BINARIES_DIR}/include/service /usr/include/iotivity/
    sudo cp -r ${IOTIVITY_COMPILED_BINARIES_DIR}/include/c_common /usr/include/iotivity/resource/
}

finished() {
    echo "Installation Completed!"
    echo "You can find an example here: https://github.com/Agile-IoT/agile-iotivity"
    exit 0
}

checksudo && review_params && setup_dep && download_unzip && compile && copy_binaries && finished
