#include<iostream>
#include<pcap.h>
#include<cstdio>

using namespace std;

static int packetCount = 0;

void packetHandler(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet) {
  cout << ++packetCount << " packet(s) captured\tlen: " << pkthdr->len <<" \ttime: " <<pkthdr->ts.tv_usec <<endl;
} 

int main(int argc, char *argv[]) {
    // if (--argc)
    //     for(int i; i<argc; i++)
    //         std::cout << "Argument[" <<i <<"]: " << argv[i+1] <<std::endl;
    // std::cout << std::endl << "Number of Arguments: " << argc <<std::endl;
    if (argc < 2){
        std::cout << "Enter pcap file \n";
        return 1;
    }

    char *dev;
    pcap_if_t *alldevs;
    pcap_t *descr;
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t * pcap = pcap_open_offline(argv[1], errbuf);
    if (pcap == NULL) {
        cout << "pcap_open_offline() failed: " << errbuf << endl;
        return 1;
    }
    struct pcap_pkthdr *header;
    const u_char *data;
    u_int packetCount = 0;

    struct sniff_ip {
        u_char ip_vhl;      /* version << 4 | header length >> 2 */
        u_char ip_tos;      /* type of service */
        u_short ip_len;     /* total length */
        u_short ip_id;      /* identification */
        u_short ip_off;     /* fragment offset field */
        #define IP_RF 0x8000        /* reserved fragment flag */
        #define IP_DF 0x4000        /* dont fragment flag */
        #define IP_MF 0x2000        /* more fragments flag */
        #define IP_OFFMASK 0x1fff   /* mask for fragmenting bits */
        u_char ip_ttl;      /* time to live */
        u_char ip_p;        /* protocol */
        u_short ip_sum;     /* checksum */
        struct in_addr ip_src;
        struct in_addr ip_dst; /* source and dest address */
        struct sniff_tcp {
            u_short th_sport;   /* source port */
            u_short th_dport;   /* destination port */
            u_int32_t th_seq;       /* sequence number */
            u_int32_t th_ack;       /* acknowledgement number */
        };
    } ip;


    while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0){
        printf("Packet # %i\n", ++packetCount);
 
        // Show the size in bytes of the packet
        printf("Packet size: %ld bytes\n", header->len);
 
        // Show a warning if the length captured is different
        if (header->len != header->caplen)
            printf("Warning! Capture size different than packet size: %ld bytes\n", header->len);
 
        // Show Epoch Time
        printf("Epoch Time: %ld:%ld seconds\n", header->ts.tv_sec, header->ts.tv_usec);

        // ip = (struct sniff_ip*)(data + SIZE_ETHERNET);
        // std::cout <<ip.ip_src;

        /*// loop through the packet and print it as hexidecimal representations of octets
        // We also have a function that does this similarly below: PrintData()
        for (u_int i=0; (i < header->caplen ) ; i++)
        {
            // Start printing on the next after every 16 octets
            if ( (i % 16) == 0) printf("\n");
 
            // Print each octet as hex (x), make sure there is always two characters (.2).
            printf("%.2x ", data[i]);
        }*/
 
        // Add two lines between packets
        printf("\n\n");
    }

    return 0;
}
