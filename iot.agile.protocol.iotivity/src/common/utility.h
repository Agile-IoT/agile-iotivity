/*******************************************************************************
 * Copyright (c) 2016, 2017 FBK CREATE-NET
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Description: Utility
 *              Utility Functions
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#ifndef __COMMON_UTILITY_H__
#define __COMMON_UTILITY_H__

#include <unistd.h>
#include <ctime>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <iostream>
#include <vector>
#include <termios.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

bool is_ipv6_address(const string&);
bool is_ipv4_address(const string& str);

#endif
