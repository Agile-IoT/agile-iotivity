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
 *              PeriodicCallback. Implementation file
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

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
