#include<iostream>
#include<pcap.h>
#include<chrono>
#include<vector>

#include<captureFuncs.h>

int main(int argc, char *argv[]) {
    if(argc < 3){
        std::cout <<"Usage: "<<argv[0] <<" {filename} {seconds}\n\n";
        return 1;
    }
    std::string strFilename = argv[1], strTime = argv[2];

    PcapLib myCap;

    int i;
    for(auto& wIfName: myCap.getIfNames())
        std::cout <<++i <<". " <<wIfName <<std::endl;
    
    myCap.captureInit(myCap.mvwIfNames[0].c_str(), strFilename);
    myCap.captureFor(std::stoi(strTime), [](int iCount){
        if(iCount)
            std::cout <<iCount <<" packets captured. (in main lambda)\n";
    });
    return 0;
}
