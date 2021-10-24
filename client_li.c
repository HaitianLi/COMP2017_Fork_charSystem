/*
 * @Author: Haitian Li
 * @Date: 2021-05-14 09:53:28
 * @LastEditors: Haitian Li
 * @LastEditTime: 2021-05-18 17:46:58
 * @Description: Assignment4
 */
#include "server.h"

void error_print(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
}

void write_log(char *info, int mode) {
    if (mode == 0) {
        FILE *fp = fopen("./li.txt", "w");
        fprintf(fp, "log: %s\n", info);
        fclose(fp);

    } else {
        FILE *fp = fopen("./li.txt", "a");
        fprintf(fp, "log: %s\n", info);
        fclose(fp);
    }
}

int main(int argc, char** argv) {


    
    // printf("Opening...\n");
    //argv "./TheDomain/_RD"
    int count = 0;
    while (1) {
        int fd = open("./AnotherDomain/li_RD", O_RDONLY);
        unsigned char buffer[2048] = {0};
        int ret = read(fd, buffer, 2048);
        if (ret > 0) {
            //printf("got %d\n", buffer[0]);
            if (buffer[0] == 3) {
                unsigned char send_info[buffer_max];
                memset(send_info, 0, buffer_max);

                unsigned char send_from[256];

                int j = 0;
                int k = 2;
                for (int i = 0; i < ascii_name; i++) {
                    send_from[j] = buffer[k];
                    if (send_from[j] == 0) {
                        send_from[j] = '\0';
                    }
                    j++;
                    k++;
                }

                k = 258;
                j = 2;
                unsigned char send_message[1790];

                //say have 1790 bytes
                for (int i = 0; i < 1790; i++) {
                    send_message[i] = buffer[k];
                    j++;
                    k++;
                }
                
                char temp[3000];
                sprintf(temp,"message from: %s \nMessage: WR is %s\n", send_from, send_message);
                if (count == 0) {
                    count++;
                    write_log(temp, write_mode);
                } else {
                    write_log(temp, append_mode);
                }

            } else if (buffer[0] == 4) {
                //printf("send 4\n");
                unsigned char send_info[buffer_max];
                memset(send_info, 0, buffer_max);

                unsigned char send_from[256];

                int j = 0;
                int k = 2;
                for (int i = 0; i < ascii_name; i++) {
                    send_from[j] = buffer[k];
                    if (send_from[j] == 0) {
                        send_from[j] = '\0';
                    }
                    j++;
                    k++;
                }

                k = 258;
                j = 2;
                unsigned char send_message[1790];

                //say have 1790 bytes
                for (int i = 0; i < 1790; i++) {
                    send_message[i] = buffer[k];
                    j++;
                    k++;
                }
                
                char temp[3000];
                sprintf(temp,"message from: %s \nMessage: WR is %s\n", send_from, send_message);
                if (count == 0) {
                    count++;
                    write_log(temp, write_mode);
                } else {
                    write_log(temp, append_mode);
                }

            } else if (buffer[0] == 5) {
                int fd_wr = open("./AnotherDomain/li_WR", O_WRONLY);
                char temp[2048];
                temp[0] = 6;
                temp[1] = 0;
                int write_to = write(fd_wr, temp, 2048);
                close(fd_wr);
            }
        }
        
        close(fd);
    }


    return 0;
}
