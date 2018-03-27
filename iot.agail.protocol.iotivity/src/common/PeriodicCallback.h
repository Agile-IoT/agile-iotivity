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
 * Description: PeriodicCallback
 *              PeriodicCallback. Header file
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

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
