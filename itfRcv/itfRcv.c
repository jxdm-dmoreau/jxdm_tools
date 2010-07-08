#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
 #include <unistd.h>

int main(int argc, char *argv[])
{
    int ret = 0;
    int ifIndex = 0;
    int fd      = 0;

    /* Open socket */
    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd < 0) {
        printf("Error socket\n");
        exit(-1);
    }
    ifIndex = if_nametoindex(argv[1]);
    struct sockaddr_ll  sll;

    memset(&sll, 0, sizeof(sll));
    sll.sll_family    = AF_PACKET;
    printf("ifIndex=%d\n", ifIndex);
    sll.sll_ifindex   = ifIndex;
    sll.sll_protocol  = htons(ETH_P_ALL);
    /* bind this socket too this physical  interface */
    ret = bind(fd, (struct sockaddr *) &sll, sizeof(sll)) ;
    if (ret < 0) {
        printf("Bind error\n");
        exit(-1);
    }

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    char buffer[8192];
    int size = 0;
    while(1) {
        /* reading select */
        tv.tv_sec = 10;
        ret = select(fd + 1, &rfds, NULL, NULL, &tv);
        if (ret == 1) {
            //while((size = read(fd, buffer, 8192)) > 0) {
            while((size = recvfrom(fd, buffer, 8192, 0, NULL, NULL)) > 0) {

                printf("Caught a packet %s (%d bytes)\n", buffer, size);
            }
        } else {
            printf("Nothing to read\n");
        }
    }


    return 0;
}
