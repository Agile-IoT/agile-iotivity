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
 * @file        PeriodicCallback.h
 * @brief       
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#ifndef __COMMON_PERIODICCALLBACK_H__
#define __COMMON_PERIODICCALLBACK_H__

#include <functional>
#include <chrono>
#include <future>
#include <cstdio>
#include <pthread.h>
#include <iostream>
#include <unistd.h>

class PeriodicCallback
{
private:
    pthread_t thread;
    int threadId;
    bool fired = false;
public:
    PeriodicCallback(int after, bool pre, std::function<void(void)> task);
    ~PeriodicCallback();
    void *threadFunction(int after, bool pre, std::function<void(void)> task);
    bool stopThread();
};

#endif