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
 * @ingroup
 * @{
 *
 * @file        simple_client.h
 * @brief       Header file for a Simple Client
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@fbk.eu>
 *
 * @}
 */

#ifndef PRETTY_CLIENT_H_
#define PRETTY_CLIENT_H_

#define PRINT_PRETTY_LOGS 0

#include <memory>
#include <iostream>
#include <vector>
#include <string>

#include <iotivity/resource/OCApi.h>
#include <iotivity/resource/OCPlatform.h>
#include <iotivity/resource/OCResource.h>

class SimpleClient {
    private:
        OC::PlatformConfig *platformConfig;
        OC::FindCallback resFindCallback;
        OC::GetCallback getLightCallback;
        OC::PutCallback putChangeLightCallback;

        std::vector<std::shared_ptr<OC::OCResource>> resList;

        SimpleClient();
        void initializeClient();
        void onFind(std::shared_ptr<OC::OCResource> resource);
    public:
        static SimpleClient *instance;

        void startClient(std::string);
        virtual ~SimpleClient();
        void printResourceInfo(std::shared_ptr<OC::OCResource> resource);
        int getLightInfo(std::shared_ptr<OC::OCResource>);
        void putChangeLightState(std::shared_ptr<OC::OCResource>, bool);
        void onPutChangeLightState(const OC::HeaderOptions &, const OC::OCRepresentation &, int );

        void onGetLightInfo(const OC::HeaderOptions &, const OC::OCRepresentation &, int );
        static SimpleClient *getInstance();
        std::vector<std::shared_ptr<OC::OCResource>> getResourceList();
};

#endif
