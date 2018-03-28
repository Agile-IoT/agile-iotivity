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
