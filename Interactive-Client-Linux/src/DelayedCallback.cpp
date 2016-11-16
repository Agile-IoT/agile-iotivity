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
