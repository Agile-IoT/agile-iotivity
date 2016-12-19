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

#include "DelayedCallback.h"

struct wrap {
    DelayedCallback *instance;
    std::function<void(void)> task;
    int delay;

    wrap(DelayedCallback *dCb, std::function<void(void)> func, int d) :
        instance(dCb), task(func), delay(d) {}
};

extern "C" void* delayed_thread_main_func(void *f)
{
    std::auto_ptr< wrap > w( static_cast< wrap* >( f ) );
    w->instance->threadFunction(w->delay, w->task);
}

DelayedCallback::DelayedCallback(int after, bool async, std::function<void(void)> task)
{
    if (async)
    {
       wrap *w = new wrap(this, task, after);
       threadId = pthread_create(&thread, NULL, delayed_thread_main_func, w);
    }
    else
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(after));
        task();
    }
}

void *DelayedCallback::threadFunction(int after, std::function<void(void)> task)
{
    int i = 0;
    while(i<after) {
        usleep(1000);
        i++;
    }

    fired = true;
    task();
    pthread_exit(0);
}

bool DelayedCallback::stopThread()
{
    int *ret;
    pthread_cancel(thread);
    pthread_join(thread, (void **) &ret);
    if(ret == PTHREAD_CANCELED) return true;
    return false;
}

bool DelayedCallback::isFired()
{
    return fired;
}
