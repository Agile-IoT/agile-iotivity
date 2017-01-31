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
 * Description: constant
 *              Header file for AGILE constants file
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#ifndef __AGILE_CONSTANTS_H__
#define __AGILE_CONSTANTS_H__

#include <string>

namespace AGILE {

/* AGILE DBus Constants */
    static const std::string AGILE_NEW_DEVICE_SIGNAL_PATH = "/iot/agile/NewDevice";
    static const std::string AGILE_NEW_RECORD_SIGNAL_PATH = "/iot/agile/NewRecord";
    static const std::string AGILE_PROTOCOL_INTERFACE = "iot.agile.Protocol";
}

#endif
