/*
 * Copyright (C) 2016 CREATE-NET
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup
 * @{
 *
 * @file        IoTivityProtocolImp.h
 * @brief       Header file for IoTivity protocol implementation for AGILE
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#ifndef IOTIVITY_PROTOCOL_H_
#define IOTIVITY_PROTOCOL_H_

#define PRINT_PRETTY_LOGS 0

#include "utility.h"

#include <unistd.h>
#include <ctime>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <iostream>
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iotivity/resource/OCApi.h>
#include <iotivity/resource/OCPlatform.h>
#include <iotivity/resource/OCResource.h>

#include "boost/any.hpp"
using namespace std;
using namespace OC;


#endif
