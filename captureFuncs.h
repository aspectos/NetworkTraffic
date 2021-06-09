// Copyright 2021, Pejman Taslimi
//  You may distribute under the terms of either the GNU General Public
//  License or the Artistic License, as specified in the MYLICENSE file.
/*
 * FILENAME     :	
 * DESCRIPTION  :	
 * 
 * Author       : Mani Tasl
 * Date         : Jun 2021.
 *
 */

#ifndef CAPTURE_FUNCS
#define CAPTURE_FUNCS
#include<iostream>
#include<pcap.h>
#include<chrono>
#include<vector>
#include<functional>    //For std::function

using StatusUpdateHandler = std::function<void(int iPacketCount)>;
class PcapLib{
    pcap_if_t* mIfDevs;
    char* mcpDev;
    std::string mwFilename;
    pcap_t* mDescr;
    pcap_dumper_t* mDumper;
    time_t mtEndTime;
public:
    PcapLib();
    ~PcapLib();
    int miDurationSec;
    std::vector<std::string> mvwIfNames;
    std::vector<std::string>& getIfNames(void);
    int captureInit(const std::string wDevName, const std::string wFilename);
    int captureFor(int iDurationSec, StatusUpdateHandler);
};


#endif // CAPTURE_FUNCS