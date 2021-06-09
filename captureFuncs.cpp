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

#include<iostream>
#include<pcap.h>
#include<chrono>
#include<vector>
#include<functional>    //For std::function

#include"captureFuncs.h"

PcapLib::PcapLib(){
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&mIfDevs, errbuf) == -1) {
        std::cout << "pcap_lookupdev() failed: " << errbuf << std::endl;
        exit(1);
    }
}

PcapLib::~PcapLib(){
    pcap_freealldevs(mIfDevs);
}

std::vector<std::string>& PcapLib::getIfNames(void){
    mvwIfNames.clear();
    mvwIfNames.push_back(std::string(mIfDevs->name));
    for(pcap_if_t* d = mIfDevs->next; d != NULL; d = d->next)
        mvwIfNames.push_back(std::string(d->name));
    return mvwIfNames;
}

int PcapLib::captureInit(const std::string wDevName, const std::string wFilename){
    mwFilename = wFilename;
    mcpDev = const_cast<char *>(wDevName.c_str());
    char errbuf[PCAP_ERRBUF_SIZE];

    mDescr = pcap_open_live(mcpDev, BUFSIZ, 0, -1, errbuf);
    if (mDescr == NULL) {
        std::cout << "pcap_open_live() failed: " << errbuf << std::endl;
        return 1;
    }

    mDumper = pcap_dump_open(mDescr,mwFilename.c_str());
    if (mDumper == NULL) {
        std::cout << "pcap_dump_open() failed: " << pcap_geterr(mDescr)
            << "\n filename = " << mwFilename << std::endl;
        return 1;
    }

    return 0;
}

int PcapLib::captureFor(int iDurationSec, StatusUpdateHandler handler){
    miDurationSec = iDurationSec;
    time_t timEnd = std::time(nullptr) + miDurationSec;

    int iCount;
    while (timEnd > std::time(nullptr)) {
        // if(pcap_dispatch(descr, -1, packetHandler, (u_char*) &iBatch) == PCAP_ERROR){
        iCount = pcap_dispatch(mDescr, -1, &pcap_dump, (u_char*)mDumper);
        if( iCount == PCAP_ERROR){
            std::cout <<pcap_geterr(mDescr);
            return 1;
        }
        // if(iCount)
        //     std::cout <<iCount <<" packets captured. (in class method)\n";
        handler(iCount);
    }

    pcap_dump_close(mDumper);
    pcap_close(mDescr);
    return 0;
}
