/*
 * Copyright 2016 CREATE-NET
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @ingroup
 * @{
 *
 * @file        constant.h
 * @brief       Header file for AGILE constants file
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include <string>

namespace AGILE {

/* AGILE DBus Constants */
    static const std::string AGILE_NEW_DEVICE_SIGNAL_PATH = "/iot/agile/NewDevice";
    static const std::string AGILE_NEW_RECORD_SIGNAL_PATH = "/iot/agile/NewRecord";

/* AGILE Device Constants */
    static const std::string AGILE_DEVICE_CONNECTED = "CONNECTED";
    static const std::string AGILE_DEVICE_DISCONNECTED = "DISCONNECTED";
    static const std::string AGILE_DEVICE_AVAILABLE = "AVAILABLE";
    static const std::string AGILE_DEVICE_UNAVAILABLE = "UNAVAILABLE";

}
