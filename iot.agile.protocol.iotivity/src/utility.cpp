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
 * @file        utility.cpp
 * @brief       Utility functions
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "utility.h"

bool is_ipv6_address(const string& str)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    struct sockaddr_in6 sa;
    return inet_pton(AF_INET6, str.c_str(), &(sa.sin6_addr))!=0;
}

bool is_ipv4_address(const string& str)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    struct sockaddr_in sa;
    return inet_pton(AF_INET, str.c_str(), &(sa.sin_addr))!=0;
}
