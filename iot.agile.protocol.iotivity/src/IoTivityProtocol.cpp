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

string addr = "ff03::158"; //TODO: it should be read as argument

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
