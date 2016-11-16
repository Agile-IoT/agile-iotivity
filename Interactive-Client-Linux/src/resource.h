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
 * @file        resource.h
 * @brief       Header file for Resource Class
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

#define PRINT_PRETTY_LOGS 0

#include <memory>
#include <iostream>
#include <vector>
#include <string>

#include <iotivity/resource/OCApi.h>
#include <iotivity/resource/OCPlatform.h>
#include <iotivity/resource/OCResource.h>

#include "DelayedCallback.h"

using namespace std;
using namespace OC;

class Resource {
    private:
        std::shared_ptr<OC::OCResource> resource;
        OC::GetCallback getCallback;
        OC::PostCallback postCallback;
        OC::PutCallback putCallback;
        OC::DeleteCallback deleteCallback;
        std::function<void(void)> getExpirationExternalCallback;
        std::function<void(void)> postExpirationExternalCallback;
        std::function<void(void)> putExpirationExternalCallback;
        std::function<void(void)> deleteExpirationExternalCallback;
        int getDelay=0;
        int postDelay=0;
        int putDelay=0;
        int deleteDelay=0;
        DelayedCallback *getDelayedCallback;
        DelayedCallback *posttDelayedCallback;
        DelayedCallback *puttDelayedCallback;
        DelayedCallback *deleteDelayedCallback;
        void getExpirationInternalCallback();
        void postExpirationInternalCallback();
        void putExpirationInternalCallback();
        void deleteExpirationInternalCallback();

        void onGetCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int);
        void onPostCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int);
        void onPutCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int);
        void onDeleteCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int);

    public:
        Resource();
        Resource(std::shared_ptr<OC::OCResource>);

        bool execGET();
        bool execPOST();
        bool execPUT();
        bool execDELETE();

        void registerGETCallbacks(OC::GetCallback, std::function<void(void)>, int);
        void registerPOSTCallbacks(OC::PostCallback, std::function<void(void)>, int);
        void registerPUTCallbacks(OC::PutCallback, std::function<void(void)>, int);
        void registerDELETECallbacks(OC::DeleteCallback, std::function<void(void)>, int);

        std::shared_ptr<OC::OCResource> getOCResourceObj();
        void setOCResourceObj(std::shared_ptr<OC::OCResource>);

        string getURI() const;
        string getHost() const;

        bool operator==(const Resource& res) const;
};
#endif
