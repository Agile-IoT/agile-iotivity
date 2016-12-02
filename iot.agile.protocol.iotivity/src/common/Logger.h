/*
 * Copyright 2016 CREATE-NET
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
