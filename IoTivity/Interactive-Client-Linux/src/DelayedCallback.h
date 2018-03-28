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
 * @file        DelayedCallback.h
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

class DelayedCallback
{
private:
    volatile bool continueThread = true;
public:
    DelayedCallback(int after, bool async, std::function<void(void)> task);
    void stopThread();
};
