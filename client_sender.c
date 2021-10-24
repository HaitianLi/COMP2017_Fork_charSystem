/*
 * @Author: Haitian Li
 * @Date: 2021-05-14 09:53:28
 * @LastEditors: Haitian Li
 * @LastEditTime: 2021-05-18 18:25:07
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
    //argv "./TheDomain/bob_WR"
    int fd = open(argv[3], O_WRONLY);
    if (fd == -1) {
        printf("sender?\n");
        error_print("open gevent error");
    }

    
    char buffer[2048] = {0};
    
    buffer[0] = atol(argv[1]);
    //printf("2222 = %d\n", buffer[0]);
    buffer[1] = 0;
    if (buffer[0] == 1) {
        int k = 2;
        int j = 0;
        //char client_id[256] = "kkkk";
        char client_domain[1790];
        sprintf(client_domain, "message from %s\n", argv[3]);
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

    } else if (buffer[0] == 2) {
        //printf("sender = %s\n", argv[3]);
        int k = 2;
        int j = 0;
        //char client_id[256] = "kkkk";
        char client_domain[1790];
        sprintf(client_domain, "Cont message from %s\nOrder %ld\n", argv[3], atol(argv[2]));
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

    } else if (buffer[0] == 7) {
        //printf("here");
        write(fd, buffer, 2048);
    }

    close(fd);

    return 0;
}
