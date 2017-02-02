/*******************************************************************************
 * Copyright (c) 2016, 2017 FBK CREATE-NET
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Description: DelayedCallback
 *              DelayedCallback. Implementation file
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

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
