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
 * @file        DelayedCallback.cpp
 * @brief       
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include <functional>
#include <chrono>
#include <future>
#include <cstdio>
#include "DelayedCallback.h"

DelayedCallback::DelayedCallback(int after, bool async, std::function<void(void)> task)
{

    if (async)
    {
        continueThread = true;
        std::thread([this, after, task]() {
            int i = 0;
            while(continueThread && i<after) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                i++;
            }
            if(continueThread) task();
        }).detach();
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(after));
        task();
    }
}

void DelayedCallback::stopThread()
{
    continueThread = false;
}
