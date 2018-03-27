/*******************************************************************************
 * Copyright (C) 2017 Create-Net / FBK.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-2.0/
 * 
 * Contributors:
 *     Create-Net / FBK - initial API and implementation
 ******************************************************************************/

/**
 * @ingroup     Examples
 * @{
 *
 * @file        server.c
 * @brief       This is an IoTivity Server.
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "oc_api.h"
#include "xtimer.h"
#include "pthread_cond.h"
#include "thread.h"

extern int _netif_config(int argc, char **argv);

static int quit;
static mutex_t mutex;
static pthread_cond_t cv;
static struct timespec ts;
static char _oc_main_stack[THREAD_STACKSIZE_MAIN];

#define OC_QUEUE_SIZE     (8)
static msg_t _oc_msg_queue[OC_QUEUE_SIZE];

//Resources registration functions
extern void res_light_register(void);

static void
app_init(void)
{
    oc_init_platform("RIOT-OS", NULL, NULL);

    oc_add_device("/oic/d", "oic.d.light", "RIOT's light", "1.0", "1.0", NULL,
                  NULL);
}


static void
register_resources_func(void)
{
    res_light_register();
}

static void
signal_event_loop_func(void)
{
    mutex_lock(&mutex);
    pthread_cond_signal(&cv);
    mutex_unlock(&mutex);
}

void *
oc_main_thread(void *arg)
{
    (void)arg;

    pthread_cond_init(&cv, NULL);

    static const oc_handler_t handler = { .init = app_init,
                                          .signal_event_loop = signal_event_loop_func,
                                          .register_resources =
                                              register_resources_func };

    msg_init_queue(_oc_msg_queue, OC_QUEUE_SIZE);

    if (oc_main_init(&handler) < 0) {
        PRINT("server_oic: failed to initialize stack\n");
        return NULL;
    }

    /* print network addresses */
    puts("server_oic: Configured network interfaces:");
    _netif_config(0, NULL);



    oc_clock_time_t next_event;
    while (!quit) {
        next_event = oc_main_poll();
        mutex_lock(&mutex);
        if (next_event == 0) {
            pthread_cond_wait(&cv, &mutex);
        }
        else {
            ts.tv_sec = (next_event / OC_CLOCK_SECOND);
            ts.tv_nsec = (next_event % OC_CLOCK_SECOND) * 1.e09 / OC_CLOCK_SECOND;
            pthread_cond_timedwait(&cv, &mutex, &ts);
        }
        mutex_unlock(&mutex);
    }

    oc_main_shutdown();

    return NULL;
}

int
main(void)
{
    PRINT("server_oic: Waiting for address autoconfiguration...");
    xtimer_sleep(10);

    thread_create(_oc_main_stack, sizeof(_oc_main_stack), 6, 0, oc_main_thread,
                  NULL, "OCF event thread");

    fgetc(stdin);

    quit = 1;
    signal_event_loop_func();

    return 0;
}
