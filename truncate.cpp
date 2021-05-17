#include<iostream>
#include<pcap.h>
#include<cstdio>
#include<regex>

// using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 3){
        std::cout <<"Usage: "<<argv[0] <<" {filename} {cap}\nThis trancate\n";
        return 1;
    }
    std::string strFilename = argv[1];
    auto strOutFilename = std::regex_replace (strFilename,std::regex(".pcap"),"") + ".t.pcap";
    int iCap = atoi(argv[2]);

    char* dev;
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t* pcap = pcap_open_offline(strFilename.c_str(), errbuf);
    if (pcap == NULL) {
        std::cout << "pcap_open_offline() failed: " << errbuf << std::endl;
        return 1;
    }

    pcap_dumper_t* dumper = pcap_dump_open(pcap,strOutFilename.c_str());
    if (dumper == NULL) {
        std::cout << "pcap_dump_open() failed: " << pcap_geterr(pcap)
            << "\n filename = " << strOutFilename << std::endl;
        return 1;
    }

    struct pcap_pkthdr* header;
    const u_char* data;
    u_int packetCount = 0;

    packetCount = pcap_loop(pcap, iCap, &pcap_dump, (u_char*)dumper);
    if (packetCount == PCAP_ERROR) {
        std::cout << "pcap_loop() failed: " << pcap_geterr(pcap);
        return 1;
    }

    return 0;
}
