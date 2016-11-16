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

using namespace std;
using namespace OC;

class Resource {
    private:
        std::shared_ptr<OC::OCResource> resource;
        OC::GetCallback getCallback;
        OC::PostCallback postCallback;
        OC::PutCallback putCallback;
        OC::DeleteCallback deleteCallback;
        
    public:
        Resource();
        Resource(std::shared_ptr<OC::OCResource>);

        bool execGET();
        bool execPOST();
        bool execPUT();
        bool execDELETE();

        void registerGETCallback(OC::GetCallback);
        void registerPOSTCallback(OC::PostCallback);
        void registerPUTCallback(OC::PutCallback);
        void registerDELETECallback(OC::DeleteCallback);

        std::shared_ptr<OC::OCResource> getOCResourceObj();
        void setOCResourceObj(std::shared_ptr<OC::OCResource>);

        string getURI() const;
        string getHost() const;

        bool operator==(const Resource& res) const;
};
#endif
