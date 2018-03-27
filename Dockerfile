#-------------------------------------------------------------------------------
# Copyright (C) 2017 Create-Net / FBK.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the Eclipse Public License 2.0
# which accompanies this distribution, and is available at
# https://www.eclipse.org/legal/epl-2.0/
# 
# Contributors:
#     Create-Net / FBK - initial API and implementation
#-------------------------------------------------------------------------------
FROM resin/raspberrypi2-debian:jessie-20161010

# Install wget and curl
RUN apt-get clean && apt-get update && apt-get install -y \
  wget \
  curl \
  && apt-get clean && rm -rf /var/lib/apt/lists/*

# Add packages
RUN apt-get update && apt-get install --no-install-recommends -y \
    build-essential \
    git\
    ca-certificates \
    apt \
    software-properties-common \
    unzip \
    cpp \
    binutils \
    gettext \
    libc6-dev \
    make \
    cmake \
    cmake-data \
    pkg-config \
    clang \
    gcc-4.9 \
    g++-4.9 \
    libglib2.0-0 \
    libglib2.0-dev \
    qdbus \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/app

#RUN apt-get update \
#  && apt-get upgrade -y \
#  && apt-get clean  && rm -rf /var/lib/apt/lists/*

# Install building dependencies
RUN apt-get update && apt-get -y install \
  scons \
  libboost-dev \
  libboost-thread-dev \
  libexpat1-dev \
  libboost-program-options-dev \
  python-dev \
  autotools-dev \
  libicu-dev \
  libbz2-dev \
  uuid-dev \
  libssl-dev \
  libglib2.0-dev \
  dbus \
  libdbus-glib-1-dev \
  && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN wget -c http://mirrors.kernel.org/iotivity/1.2.0/iotivity-1.2.0.zip \
  && unzip iotivity-1.2.0.zip \
  && rm iotivity-1.2.0.zip \
  && cd iotivity-1.2.0 \
  && git clone https://github.com/01org/tinycbor.git extlibs/tinycbor/tinycbor -b v0.3.2 \
  && cd /usr/src/app/iotivity-1.2.0 \
  && scons TARGET_ARCH=arm-v7a \
  TARGET_OS=linux \
  TARGET_TRANSPORT=IP \
  RELEASE=yes \
  SECURED=0 \
  ROUTING=EP \
  BUILD_SAMPLE=OFF \
  && cd .. \
  && cd ./iotivity-1.2.0/out/linux/arm-v7a/release \
  && cp *.a /usr/lib \
  && cp *.so /usr/lib \
  && mkdir /usr/include/iotivity \
  && cp -r ./include/resource /usr/include/iotivity/ \
  && cp -r ./include/service /usr/include/iotivity/ \
  && cp -r ./include/c_common /usr/include/iotivity/resource/ \
  && cd ../../../../.. \
  && rm -rf iotivity-1.2.0 \
  && ls -l

RUN git clone https://github.com/agile-iot/agile-iotivity \
  && cd agile-iotivity \
  && ls \
  && cd iot.agail.protocol.iotivity \
  && make agail_iotivity \
  && cd /usr/src/app \
  && mkdir .agile_bus

CMD [ "/bin/bash", "/usr/src/app/agile-iotivity/scripts/start.sh" ]
