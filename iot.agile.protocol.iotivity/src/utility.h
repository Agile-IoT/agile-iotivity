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
 * @file        utility.h
 * @brief       Header file for Utility functions
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

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
