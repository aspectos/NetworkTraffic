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