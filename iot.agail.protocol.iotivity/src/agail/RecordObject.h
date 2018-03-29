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

#ifndef __AGAIL_RECORDOBJECT_H__
#define __AGAIL_RECORDOBJECT_H__

#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;

namespace AGAIL {
    class RecordObject;
}

class AGAIL::RecordObject {
    public:
    string deviceId;
    string componentId;
    string value;
    string unit;
    string format;
    double lastUpdate;

    RecordObject();
    RecordObject(string, string, string, string, string);
    RecordObject(string, string, string, string, string, double);

    void updateLastUpdateToNow();

    bool operator==(const AGAIL::RecordObject&) const;
};

#endif
