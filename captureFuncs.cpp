#include<iostream>
#include<pcap.h>
#include<chrono>
#include<vector>

// using namespace std;

class PcapLib{
    pcap_if_t* mIfDevs;
    char* mDev;
    pcap_t* mDescr;
    pcap_dumper_t* mDumper;
    time_t mtEndTime;
public:
    PcapLib();
    int miPacketCount,miDurationSec;
    std::string mwFilename;
    std::vector<std::string> mvwIfNames;
    std::vector<std::string>& getIfNames(void);
};

PcapLib::PcapLib(){
    char errbuf[PCAP_ERRBUF_SIZE];
    if (pcap_findalldevs(&mIfDevs, errbuf) == -1) {
        std::cout << "pcap_lookupdev() failed: " << errbuf << std::endl;
        exit(1);
    }
}

std::vector<std::string>& PcapLib::getIfNames(void){
    
    mvwIfNames.clear();
    mvwIfNames.push_back(std::string(mIfDevs->name));
    for(pcap_if_t* d = mIfDevs->next; d != NULL; d = d->next)
        mvwIfNames.push_back(std::string(d->name));

    return mvwIfNames;
}


/*static int packetCount = 0;
void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
    std::cout << ++packetCount << " packet(s) captured\tlen: " << pkthdr->len <<" \ttime: " <<pkthdr->ts.tv_usec 
        <<" \tbatch: " <<*((int*)userData) <<std::endl;
}*/

int main(int argc, char *argv[]) {
    if(argc < 3){
        std::cout <<"Usage: "<<argv[0] <<" {filename} {seconds}\n\n";
        return 1;
    }
    std::string strFilename = argv[1], strTime = argv[2];
    char *dev;
    pcap_if_t *alldevs;
    pcap_t *descr;
    pcap_dumper_t* dumper;
    char errbuf[PCAP_ERRBUF_SIZE];

    // dev = pcap_lookupdev(errbuf);
    // if (dev == NULL) {
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cout << "pcap_lookupdev() failed: " << errbuf << std::endl;
        return 1;
    }

    std::vector<std::string> vwIfNames;
    vwIfNames.push_back(std::string(alldevs->name));
    // std::cout <<alldevs->name <<std::endl;
    for(pcap_if_t* d = alldevs->next; d != NULL; d = d->next)
        vwIfNames.push_back(std::string(d->name));
        // std::cout <<d->name  <<std::endl;
    
    int i;
    for(auto& wIfName: vwIfNames)
        std::cout <<++i <<". " <<wIfName <<std::endl;
    // pcap_freealldevs(alldevs);
    



    dev = alldevs->name;
    descr = pcap_open_live(dev, BUFSIZ, 0, -1, errbuf);
    if (descr == NULL) {
        std::cout << "pcap_open_live() failed: " << errbuf << std::endl;
        return 1;
    }

    dumper = pcap_dump_open(descr,strFilename.c_str());
    if (dumper == NULL) {
        std::cout << "pcap_dump_open() failed: " << pcap_geterr(descr)
            << "\n filename = " << strFilename << std::endl;
        return 1;
    }

    time_t timEnd = std::time(nullptr) + std::stoi(strTime);
    int iBatch, iCount;
    while (timEnd > std::time(nullptr)) {
        iBatch++; // std::cout <<"Batch: " << iBatch++ <<std::endl;
        // if(pcap_dispatch(descr, -1, packetHandler, (u_char*) &iBatch) == PCAP_ERROR){
        iCount = pcap_dispatch(descr, -1, &pcap_dump, (u_char*)dumper);
        if( iCount == PCAP_ERROR){
            std::cout <<pcap_geterr(descr);
            return 1;
        }
        if(iCount)
            std::cout <<iCount <<" packets captured.\n";

    }

    pcap_dump_close(dumper);
    pcap_close(descr);


//   if (pcap_loop(descr, 10, packetHandler, NULL) < 0) {
//       cout << "pcap_loop() failed: " << pcap_geterr(descr);
//       return 1;
//   }

    std::cout << "capture finished" << std::endl;

    return 0;
}
