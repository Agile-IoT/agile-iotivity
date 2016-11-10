/*
 * Copyright (C) 2016 CREATE-NET
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup
 * @{
 *
 * @file        simple_client.h
 * @brief       Header file for a Simple Client
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
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
