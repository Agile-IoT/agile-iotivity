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
# Installation Script for the building env for RIOT-OS on RPi       #
#                                                                   #
# Author: Mattia Antonini <mattia.antonini1@studenti.unipr.it>      #
#                         <m.antonini@create-net.org>               #
#                                                                   #
# This script installs the toolchain for RIOT OS                    #
#####################################################################

checksudo() {
    if [ $(id -u) -ne 0 ] ; then
        echo "Please run as root."
        echo "e.g. sudo -k "$0
        exit 1
    fi
}

welcome_message() {
    echo "This script will install the toolchain for RIOT-OS on RPi."
    echo "It will install the toolchain for ARM architecture and native."    
}

update_install_basic() {
    sudo apt-get update
    sudo apt-get -y install software-properties-common python-software-properties
    sudo apt-get -y install git pkg-config autoconf libudev-dev libusb-1.0-0-dev libtool unzip valgrind
}

install_hidapi() {
    TMP=$(mktemp)
    rm -r $TMP
    mkdir -p $TMP
    cd $TMP
    git clone http://github.com/signal11/hidapi.git
    cd hidapi
    ./bootstrap
    ./configure
    make
    sudo make install
    sudo ln -s /usr/local/lib/libhidapi-hidraw.so.0 /usr/lib/libhidapi-hidraw.so.0
}

install_openocd() {
    TMP=$(mktemp)
    rm -r $TMP
    mkdir -p $TMP
    cd $TMP
    git clone https://github.com/watr-li/OpenOCD.git openocd
    cd openocd
    ./bootstrap
    ./configure --enable-maintainer-mode \
                --enable-cmsis-dap \
                --enable-hidapi-libusb
    make
    sudo make install
}

install_toolchain_arm() {
    sudo apt-get -y install software-properties-common python-software-properties
    sudo apt-get -y remove binutils-arm-none-eabi gcc-arm-none-eabi
    sudo apt-get update
    sudo apt-get -y install gcc-arm-none-eabi
}

configure_flash() {
    sudo usermod --append --groups dialout $(whoami)

    if [ $(getent group dialout | grep $(whoami)) != "" ]; then
        echo "User $(whoami) added to group dialout! Continue..."
    else
        echo "User $(whoami) NOT added to group dialout! Exiting..."
        exit 1
    fi

    echo "KERNEL==\"hidraw*\", SUBSYSTEM==\"hidraw\", MODE=\"0664\", GROUP=\"plugdev\"" >> 99-hidraw-permissions.rules
    sudo cp ./99-hidraw-permissions.rules /etc/udev/rules.d/
    rm ./99-hidraw-permissions.rules
}

install_native() {
    sudo apt-get -y install libc6-dev  
}

finished() {
    echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
    echo "@@@                                                            @@@"
    echo "@@@   Reboot REQUIRED in order to complete the installation!   @@@"
    echo "@@@                                                            @@@"
    echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"

    read -p "Reboot NOW? Yes(Y) or No(N): " opts

    if [ "$opts" = "Y" ] || [ "$opts" = "y" ]; then
        echo "Rebooting..."
        sudo reboot
    else
        echo "Exiting..."
    fi

    exit 0
}

checksudo && welcome_message && update_install_basic && install_hidapi && install_openocd && install_toolchain_arm && configure_flash && install_native && finished
