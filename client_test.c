/*
 * @Author: Haitian Li
 * @Date: 2021-05-14 09:53:28
 * @LastEditors: Haitian Li
 * @LastEditTime: 2021-05-17 09:19:55
 * @Description: Assignment4
 */
#include "server.h"

void error_print(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}



void deal_connect() {
    
}


int main(int argc, char** argv) {


    
    // printf("Opening...\n");
    int fd = open("./TheDomain/bob_WR", O_WRONLY);
    if (fd == -1) {
        error_print("open gevent error");
    }

    
    char buffer[2048] = {0};
    
    buffer[0] = 1;
    buffer[1] = 0;
    int k = 2;
    int j = 0;
    //char client_id[256] = "kkkk";
    char client_domain[1790] = "message from jack";

    // for (int i = 0; i < 256; i++) {
    //     buffer[k] = client_id[j];
    //     if (buffer[k] == '\0') {
    //         buffer[k] = 0;
    //     }
    //     j++;
    //     k++;
    // }
    // k = 258;
    // j = 0;

    for (int i = 0; i < 1790; i++) {
        buffer[k] = client_domain[j];
        if (buffer[k] == '\0') {
            buffer[k] = 0;
        }
        j++;
        k++;
    }

    int read_res = write(fd, buffer, 2048);
    if (read_res < 0) {
        error_print("write gevent error");
    }

    close(read_res);

    return 0;
}