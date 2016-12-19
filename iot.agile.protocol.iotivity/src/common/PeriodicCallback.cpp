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
 * @file        PeriodicCallback.cpp
 * @brief       
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "PeriodicCallback.h"

struct wrap {
    PeriodicCallback *instance;
    std::function<void(void)> task;
    int delay;
    bool pre;

    wrap(PeriodicCallback *dCb, std::function<void(void)> func, int d, bool p) :
        instance(dCb), task(func), delay(d), pre(p) {}
};

extern "C" void* periodic_thread_main_func(void *f)
{
    std::auto_ptr< wrap > w( static_cast< wrap* >( f ) );
    w->instance->threadFunction(w->delay, w->pre, w->task);
}

PeriodicCallback::PeriodicCallback(int after, bool pre, std::function<void(void)> task)
{
    wrap *w = new wrap(this, task, after, pre);
    threadId = pthread_create(&thread, NULL, periodic_thread_main_func, w);
}

PeriodicCallback::~PeriodicCallback()
{
}

void *PeriodicCallback::threadFunction(int after, bool pre, std::function<void(void)> task)
{
    int i;
    while(1) {
        i = 0;

        if(!pre) task();

        while(i<after) {
            usleep(1000);
            i++;
        }
        if(pre) task();
    }
    pthread_exit(0);
}

bool PeriodicCallback::stopThread()
{
    int *ret;
    pthread_cancel(thread);
    pthread_join(thread, (void **) &ret);
    if(ret == PTHREAD_CANCELED) return true;
    return false;
}
