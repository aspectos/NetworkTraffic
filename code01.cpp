#include <iostream>
#include <pcap.h>

using namespace std;

static int packetCount = 0;

void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
  cout << ++packetCount << " packet(s) captured\tlen: " << pkthdr->len <<" \ttime: " <<pkthdr->ts.tv_usec <<endl;
} 

int main() {
  char *dev;
  pcap_if_t *alldevs;
  pcap_t *descr;
  char errbuf[PCAP_ERRBUF_SIZE];

  // dev = pcap_lookupdev(errbuf);
  // if (dev == NULL) {
  if (pcap_findalldevs(&alldevs, errbuf) == -1) {
      cout << "pcap_lookupdev() failed: " << errbuf << endl;
      return 1;
  }
  dev = alldevs->name;
  // dev = alldevs;
  descr = pcap_open_live(dev, BUFSIZ, 0, -1, errbuf);
  if (descr == NULL) {
      cout << "pcap_open_live() failed: " << errbuf << endl;
      return 1;
  }

  if (pcap_loop(descr, 10, packetHandler, NULL) < 0) {
      cout << "pcap_loop() failed: " << pcap_geterr(descr);
      return 1;
  }

  cout << "capture finished" << endl;

  return 0;
}
