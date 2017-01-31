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
 * Description: RecordObject
 *              RecordObject header file. This class contains the
 *              measurment from a sensor and the metadata regarding
 *              its source
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#ifndef __AGILE_RECORDOBJECT_H__
#define __AGILE_RECORDOBJECT_H__

#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>

using namespace std;

namespace AGILE {
    class RecordObject;
}

class AGILE::RecordObject {
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

    bool operator==(const AGILE::RecordObject&) const;
};

#endif
