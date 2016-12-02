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
 * @file        Logger.cpp
 * @brief       Logger
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "Logger.h"

Logger::Logger(std::ostream * stream, int lvl)
{
    print =  stream;
    level = lvl;
}

void Logger::d(const std::string tag, const std::string message)
{
    if(level <= LEVEL_DEBUG)
    {
        // Print blue bold
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[1;34m";
        (* (print)) << "D - " << tag << " : " << message;
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[0m";
        (* (print)) << std::endl;
    }
}


void Logger::v(const std::string tag, const std::string message)
{
    if(level <= LEVEL_VERBOSE)
    {
        // (* (print)) cyan bold
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[1;36m";
        (* (print)) << "V - " << tag << " : " << message;
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[0m";
        (* (print)) << std::endl;
    }
}

void Logger::i(const std::string tag, const std::string message)
{
    if(level <= LEVEL_INFORMATIONAL)
    {
        // (* (print)) green bold
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[1;32m";
        (* (print)) << "I - " << tag << " : " << message;
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[0m";
        (* (print)) << std::endl;
    }
}

void Logger::w(const std::string tag, const std::string message)
{
    if(level <= LEVEL_WARNING)
    {
        // (* (print)) yellow bold
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[1;33m";
        (* (print)) << "W - " << tag << " : " << message;
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[0m";
        (* (print)) << std::endl;
    }
}

void Logger::e(const std::string tag, const std::string message)
{
    if(level <= LEVEL_ERROR)
    {
        // Print red bold
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[1;31m";
        (* (print)) << "E - " << tag << " : " << message;
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[0m";
        (* (print)) << std::endl;
    }
}

void Logger::c(const std::string tag, const std::string message)
{
    if(level <= LEVEL_CRITICAL)
    {
        // Print red bold
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[1;31m";
        (* (print)) << "C - " << tag << " : " << message;
        if((* (print)) == std::cout || (* (print)) == std::cerr) (* (print)) << "\033[0m";
        (* (print)) << std::endl;
    }
}
