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
