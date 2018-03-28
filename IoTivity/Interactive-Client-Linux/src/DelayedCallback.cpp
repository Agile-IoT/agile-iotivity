/*******************************************************************************
 * Copyright (C) 2017 Create-Net / FBK.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 * 
 * Contributors:
 *     Create-Net / FBK - initial API and implementation
 ******************************************************************************/

/**
 * @ingroup
 * @{
 *
 * @file        DelayedCallback.cpp
 * @brief       
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@fbk.eu>
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
