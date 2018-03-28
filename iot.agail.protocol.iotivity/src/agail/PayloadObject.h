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

#ifndef __AGAIL_PAYLOADOBJECT_H__
#define __AGAIL_PAYLOADOBJECT_H__

#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <map>

#include <gio/gio.h>


using namespace std;

namespace AGAIL {
    class PayloadObject;
}

class AGAIL::PayloadObject {
    public:
    string deviceId;
    map<string, GVariant *> componentAddr;
    GVariant* payload;
    double lastUpdate;

    PayloadObject();
    PayloadObject(string, map<string, GVariant *>, GVariant *);
    PayloadObject(string, map<string, GVariant *>, GVariant *, double);

    void updateLastUpdateToNow();

    bool operator==(const AGAIL::PayloadObject&) const;
};

#endif
