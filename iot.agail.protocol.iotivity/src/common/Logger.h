/*******************************************************************************
 * Copyright (c) 2016, 2017 FBK CREATE-NET
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    https://www.eclipse.org/legal/epl-2.0/
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Description: Logger
 *              Logger. Header file
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#ifndef __COMMON_LOGGER_H__
#define __COMMON_LOGGER_H__

#include <string>
#include <iostream>
#include <assert.h>

class Logger {

    public:
    static const int LEVEL_DEBUG = 0;
    static const int LEVEL_VERBOSE = 1;
    static const int LEVEL_INFORMATIONAL = 2;
    static const int LEVEL_WARNING = 3;
    static const int LEVEL_ERROR = 4;
    static const int LEVEL_CRITICAL = 5;

    private:
    std::ostream * print = nullptr;
    int level;

    public:
    Logger(std::ostream *, int);
    void d(const std::string, const std::string);
    void v(const std::string, const std::string);
    void i(const std::string, const std::string);
    void w(const std::string, const std::string);
    void e(const std::string, const std::string);
    void c(const std::string, const std::string);
};

#endif
