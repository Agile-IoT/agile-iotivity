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
 * @file        resource.h
 * @brief       Header file for Resource Class
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@fbk.eu>
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
#include <map>

#include <iotivity/resource/OCApi.h>
#include <iotivity/resource/OCPlatform.h>
#include <iotivity/resource/OCResource.h>

#include "DelayedCallback.h"

#include "boost/any.hpp"

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
        DelayedCallback *postDelayedCallback;
        DelayedCallback *putDelayedCallback;
        DelayedCallback *deleteDelayedCallback;
        void getExpirationInternalCallback();
        void postExpirationInternalCallback();
        void putExpirationInternalCallback();
        void deleteExpirationInternalCallback();

        void onGetCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int);
        void onPostCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int);
        void onPutCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int);
        void onDeleteCallback(const OC::HeaderOptions &, const OC::OCRepresentation &, int);

        OCRepresentation generateOCRepresentation(std::map<std::string, boost::any>);

    public:
        static const string INTERFACE_RW;
        static const string INTERFACE_R;

        Resource();
        Resource(std::shared_ptr<OC::OCResource>);

        bool execGET();
        bool execPOST(); /* Not Implemented */
        bool execPUT(std::map<std::string, boost::any>);
        bool execDELETE(); /* Not Implemented */

        void registerGETCallbacks(OC::GetCallback, std::function<void(void)>, int);
        void registerPOSTCallbacks(OC::PostCallback, std::function<void(void)>, int); /* Not Implemented */
        void registerPUTCallbacks(OC::PutCallback, std::function<void(void)>, int);
        void registerDELETECallbacks(OC::DeleteCallback, std::function<void(void)>, int); /* Not Implemented */

        bool isInterfacePresent(string);

        std::shared_ptr<OC::OCResource> getOCResourceObj();
        void setOCResourceObj(std::shared_ptr<OC::OCResource>);

        string getURI() const;
        string getHost() const;

        bool operator==(const Resource& res) const;
};
#endif
