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
 * @file        IoTivityProtocol.cpp
 * @brief       IoTivity protocol implementation for AGILE
 *
 * @author      Mattia Antonini <mattia.antonini1@studenti.unipr.it>
 *                              <m.antonini@create-net.org>
 *
 * @}
 */

#include "IoTivityProtocol.h"

using namespace std;
using namespace OC;
using namespace AGILE;

string addr = "ff03::158"; //TODO: it should be read as argument

//Attributes
IoTivityProtocol *IoTivityProtocol::instance = nullptr;

OCConnectivityType connectivityType(CT_ADAPTER_IP);
const string IoTivityProtocol::BUS_NAME = "iot.agile.protocol.iotivity";
const string IoTivityProtocol::BUS_PATH = "/iot/agile/protocol/iotivity";
const string IoTivityProtocol::PROTOCOL_NAME = "IoTivity";
const string IoTivityProtocol::DRIVER_NAME = "iotivity";
//End Attributes

IoTivityProtocol* IoTivityProtocol::getInstance()
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    if(!instance)
    {
        instance = new IoTivityProtocol();
    }

    return instance;
}


/************************************************************************************************/

void print_usage(char* program_name)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    cout << "TODO: usage" << endl;
}


int main(int argc, char** argv)
{
#if PRINT_PRETTY_LOGS
    cerr << "Function: " << __PRETTY_FUNCTION__ << std::endl;
#endif
    

    cout << "HI!" << endl;
    
    return 0;
}
