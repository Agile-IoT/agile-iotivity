/*******************************************************************************
 * Copyright (c) 2016, 2017 FBK CREATE-NET
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    https://www.eclipse.org/legal/epl-2.0/
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Description: constant
 *              Header file for AGAIL constants file
 *
 * Contributors:
 *    Mattia Antonini
 *******************************************************************************/

#ifndef __AGAIL_CONSTANTS_H__
#define __AGAIL_CONSTANTS_H__

#include <string>

namespace AGAIL {

/* AGILE DBus Constants */
    static const std::string AGAIL_NEW_DEVICE_SIGNAL_PATH = "/iot/agail/NewDevice";
    static const std::string AGAIL_NEW_RECORD_SIGNAL_PATH = "/iot/agail/NewRecord";
    static const std::string AGAIL_PROTOCOL_INTERFACE = "iot.agail.Protocol";
}

#endif
