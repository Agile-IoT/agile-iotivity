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
