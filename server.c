#include "server.h"

//myfork, from csapp book.
void error_print(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    //I dont thing a error have to be exit since it is a server
    //it can just fail the current process, but continues running for next one
    //exit(1);
}

pid_t Fork() {

    pid_t pid;

    if ((pid = fork()) < 0) {
        error_print("Fork error");
    }

    return pid;
}

//signale handler for SIGUSR1, once get sigusr
//use wait the take care the reaped child process
void signal_handler(int sig) {
    //printf("exist with handler\n");
    wait(NULL);
}

void write_log(char *info, int mode) {
    if (mode == 0) {
        FILE *fp = fopen("server.txt", "w");
        fprintf(fp, "log: %s\n", info);
        fclose(fp);

    } else {
        FILE *fp = fopen("server.txt", "a");
        fprintf(fp, "log: %s\n", info);
        fclose(fp);
    }
}
void ping_handler(int sig) {
    //printf("ping sender shoud exit");
    exit(0);
}

void client_handler(char *buffer) {
    //signal(SIGCHLD,SIG_IGN)
    //let init take care exited child
    signal(SIGCHLD, SIG_IGN);

    //init the domain and client WR RD fifo
    time_t start;
    time_t stop;
    start = time(NULL);
    DIR *dp;
    struct dirent *drip;

    char client_id[ascii_name];
    memset(client_id, 0, ascii_name);
    char client_domain[ascii_name];
    memset(client_domain, 0, ascii_name);
    
    //first 2 byte for type, so skep
    int k = 2;
    int j = 0;
    for (int i = 0; i <ascii_name; i++) {
        if ((buffer[k] != 0)) {
            client_id[j] = buffer[k];
            j++;
            k++;
        }
    }
    client_id[j] = '\0';

    //ascii_name(name) + 2(type)
    k = 258;
    j = 0;
    for (int i = 0; i <ascii_name; i++) {
        if ((buffer[k] != 0)) {
            client_domain[j] = buffer[k];
            j++;
            k++;
        }
    }
    client_domain[j] = '\0';

    //init temp name for different pip, domain
    char current_file[temp_file] = "./";
    char copy_current_file[temp_file];
    strcat(current_file, client_domain);
    //it wont make a new one if already exist
    mkdir(current_file, 0777);
    
    char domain_folder[temp_file];
    char client_WR[temp_file];
    char client_RD[temp_file];
    strcpy(domain_folder, current_file);
    strcat(domain_folder, "/");

    char filedir[ascii_name];
    strcpy(filedir, current_file);
    
    strcat(current_file, "/");
    strcpy(copy_current_file, current_file);

    strcat(current_file, client_id);
    strcat(copy_current_file, client_id);
    
    strcat(current_file, "_WR");
    strcat(copy_current_file, "_RD");

    strcpy(client_WR, current_file);
    strcpy(client_RD, copy_current_file);

    //make fifo, if exist it will not make a new one
    int check_fifo_wr = mkfifo(current_file, 0777);
    int check_fifo_rd = mkfifo(copy_current_file, 0777);
    char info[buffer_max] = {};
    if (check_fifo_rd != -1 && check_fifo_wr != -1) {
        ;//fprintf(stderr, "Coneected with client %s in domain %s\n", client_id, client_domain);
        //write_log(client_id, client_domain);
        
        sprintf(info,"Connected: client: %s -- domain: %s\n", client_id, client_domain);
        write_log(info, append_mode);
    } else {
        
        sprintf(info,"client: %s -- domain: %s Connected, please do not connect again\n", client_id, client_domain);
        write_log(info, append_mode);
    }
    
    
    //multiplexing fd, open with flag O-RDWR(EOF handle)
    //since the slight different between pip and fifo
    
    int check_client_wr = open(current_file, O_RDWR | O_NONBLOCK);

    if (check_client_wr == -1) {
        //this error when the file is not exist.
        error_print("client wr error");
    }


    //init the value use for select
    fd_set wr_fd;
    int nfds = check_client_wr + 1;
    long delay_sec = 1;
    struct timeval tv;

    //fork a chile process, send to the RD the ping every 15 sec forever
    //it will end when the parent(client handler killed)
    //it become a orphan process, and re parent and handled by init
    pid_t ping_sender = Fork(); 

    if (ping_sender == 0) {
        signal(SIGUSR2, ping_handler);
        while (1) {
            sleep(15);
            int ping_send = open(client_RD, O_WRONLY);
            //printf("this should not print after kill %s\n", client_id);
            //it only return -1 when chientRD not exist
            //its the time when handler get killed
            //handler will delete the fifo, make it return -1
            
            if (ping_send == -1) {
                //if is success exit since I control it to do so
                exit(success_exit);
            }
            unsigned char ping_buffer[buffer_max];
            ping_buffer[0] = ping_code;
            ping_buffer[1] = byte_zero;
            int write_to = write(ping_send, ping_buffer, buffer_max);
            //printf("%s send ping success\n", client_id);
            close(ping_send);
            if (write_to == -1) {
                exit(success_exit);
            }
        }
        exit(success_exit);
    }
    
    //variable keep tracking the ping and pong
    int count_ping_time = 0;
    int found_pong = 0;

    while (1) {
        //every cycle it count the time
        //every 15 sec should check if we got pong back
        //if not kill the handler, delete fifo
        stop = time(NULL);
        if (stop - start >= 15) {
            //printf("check here\n");
            if (count_ping_time == 0) {
                count_ping_time++;
                start = time(NULL);
            } else {
                if (found_pong != 0) {
                    found_pong = 0;
                    start = time(NULL);
                } else {
                    remove(client_RD);
                    remove(client_WR);
                    remove(client_domain);

                    char temp[temp_file] = {};
                    sprintf(temp, "No pong reply from %s, process been cleaned\n", client_id);
                    write_log(temp, append_mode);

                    pid_t ppid;
                    ppid = getppid();
                    kill(ping_sender, SIGUSR2);
                    kill(ppid, SIGUSR1);
                    exit(success_exit);
                }
            }
        }

        //use select keep read from client_wr
        //it have timout 1 sec
        FD_ZERO(&wr_fd);
        FD_SET(check_client_wr, &wr_fd);

        tv.tv_sec = delay_sec;
        tv.tv_usec = 0;

        int ret = select(nfds, &wr_fd, NULL, NULL, &tv);
        
        

        //this part for the signal interupted, in UNIX system
        //it will interrupt the select by an errorno EINTR
        //we could simplely ingnor that error and make the process continue

        if (ret == -1) {
            //https://unix.stackexchange.com/questions/509375/what-is-interrupted-system-call
            if (errno == EINTR) {
                continue;
            } else {
                error_print("select() failed: ");
            }
        } else if (ret) {
            
            if (FD_ISSET(check_client_wr, &wr_fd)) {
                unsigned char buffer[buffer_max] = {0};
                int res_wr = read(check_client_wr, buffer, buffer_max);
                // for (int i = 0; i < 50; i++) {
                //     printf("%d", buffer[i]);
                // }
                // printf("\n");

                if (res_wr > 0) {
                    int clientHandler_operation = buffer[1] + buffer[0];
                    //printf("from %s, operation %d\n",client_id, clientHandler_operation);
                    
                    if (clientHandler_operation == say_code) {
                        //printf("send say\n");
                        if ((dp = opendir(domain_folder)) == NULL) {
                            error_print("open dir error");
                        }
                        //traverse the files in current file.
                        while ((drip = readdir(dp))!= NULL) {
                            //ignore current and parent file
                            if (strcmp(drip->d_name, ".") == 0 ||
                                strcmp(drip->d_name, "..") == 0) {
                                    continue;
                                }
                            
                            //make the path
                            char tempfile[buffer_max];
                            strcpy(tempfile, domain_folder);
                            strcat(tempfile, drip->d_name);
                            
                            if (strcmp(tempfile, client_RD) == 0) {
                                //skep the current client rd
                                continue;
                            } else {
                                
                                if (strstr(tempfile, "RD") != NULL) {
                                    //fork this process to a child process
                                    //so the client wr will never blocked
                                    //by waiting writing, or errors(client down)
                                    pid_t pid = Fork();
                                    if (pid == 0) {
                                
                                        int temp_rd = open(tempfile, O_WRONLY);
                                        
                                        
                                        if (temp_rd == -1) {
                                            //if file not exist
                                            printf("open rd failed\n");
                                        }

                                        //build up sending package
                                        unsigned char send_info[buffer_max];
                                        memset(send_info, 0, buffer_max);

                                        send_info[0] = receive_code;
                                        send_info[1] = byte_zero;

                                        int j = 0;
                                        int k = 2;
                                        for (int i = 0; i < ascii_name; i++) {
                                            send_info[k] = client_id[j];
                                            if (send_info[k] == '\0') {
                                                send_info[k] = 0;
                                            }
                                            j++;
                                            k++;
                                        }

                                        k = 258;
                                        j = 2;
                                        //say have 1790 bytes
                                        for (int i = 0; i < 1790; i++) {
                                            send_info[k] = buffer[j];
                                            j++;
                                            k++;
                                        }
                                        
                                        int write_to = write(temp_rd, send_info, buffer_max);
                        
                                        
                                        if (write_to == -1 ){
                                            printf("This is not a open client\n");
                                        }
                                        
                                        close(temp_rd);
                                        
                                        //every time the write finish
                                        //exit the process
                                        //also let handler clean it up
                                        
                                        exit(success_exit);

                                    }
                                    
                                }
                            }
                            
                        }
                        closedir(dp);
                    } else if (clientHandler_operation == say_cont_code) {

                        //basic same logic as say
                        //different is offset 2047 have terminal code
                        //it will send in order
                        //since all write process write with block mode
                        //which means it will always in queue

                        if ((dp = opendir(domain_folder)) == NULL) {
                            error_print("open dir error");
                        }
                        while ((drip = readdir(dp))!= NULL) {
                            if (strcmp(drip->d_name, ".") == 0 ||
                                strcmp(drip->d_name, "..") == 0) {
                                    continue;
                                }
                            
                            char tempfile[buffer_max];
                            strcpy(tempfile, domain_folder);
                            strcat(tempfile, drip->d_name);
                            if (strcmp(tempfile, client_RD) == 0) {
                                
                                continue;
                            } else {
                                
                                if (strstr(tempfile, "RD") != NULL) {
                                
                                    pid_t pid = Fork();
                                    if (pid == 0) {
                                        int temp_rd = open(tempfile, O_WRONLY);
                                        //printf("tempfile = %s\n", tempfile);
                                        //printf("inside %s %s\n", client_id, tempfile);
                                        if (temp_rd == -1) {
                                            fprintf(stderr, "%s\n",strerror(errno));
                                        }
                                        unsigned char send_info[buffer_max];
                                        memset(send_info, 0, buffer_max);

                                        send_info[0] = receive_cont_code;
                                        send_info[1] = byte_zero;
                                        int j = 0;
                                        int k = 2;

                                        for (int i = 0; i < ascii_name; i++) {
                                            send_info[k] = client_id[j];
                                            if (send_info[k] == '\0') {
                                                send_info[k] = 0;
                                            }
                                            j++;
                                            k++;
                                        }

                                        k = 258;
                                        j = 2;
                                        for (int i = 0; i < 1790; i++) {
                                            send_info[k] = buffer[j];
                                            j++;
                                            k++;
                                        }
                                        send_info[2047] = buffer[2047];
                                        
                                        write(temp_rd, send_info, buffer_max);
                                        //printf("sned from = %s %s\n",client_id, client_domain );

                                        close(temp_rd);
                                        //clean up
                                        exit(success_exit);
                                        
                                    }
                                    
                                }
                            }
                            
                        }
                        closedir(dp);
                    } else if (clientHandler_operation == pong_code) {
                        
                        //we got pong, change the tracking variable
                        //printf("we got pong from %s\n", client_id);
                        found_pong = 1;

                    } else if (clientHandler_operation == 7) {
                        //printf("shoud disconnect\n");
                        remove(client_RD);
                        remove(client_WR);
                        //if after delete the fifo, the domain is empty
                        //delete the domain folder
                        remove(client_domain);
                        char temp[temp_file] = {};
                        sprintf(temp, "%s disconnected, process been clean\n", client_id);
                        write_log(temp, append_mode);
                        //printf("%s", temp);
                        break;
                    } else {
                        fprintf(stderr, "not a valid command\n");
                    }
                    
                }
            }
        }
        
    }
    
    //handler exit
    //send signal, cleaned by gervent
    pid_t ppid;
    ppid = getppid();
    kill(ping_sender, SIGUSR2);
    kill(ppid, SIGUSR1);
    exit(success_exit);
    
}


int main(int argc, char** argv) {

    //init value
    int read_res;
    mkfifo("gevent", 0777);
    //printf("mkfifo = %d\n", i);
    int fd = open("gevent", O_RDWR);
    signal(SIGUSR1, signal_handler);
    //fprintf(stderr, "Gevent fifo made.\n");

    write_log("Server Start.\n", write_mode);
    write_log("gevent fifo made.\n", append_mode);

    while (1) {
        char buffer[buffer_max] = {0};
        //read with RDWR, it should stop here before anything could read
        read_res = read(fd, buffer, buffer_max);
        
        if (read_res > 0) {

            int global_process = buffer[0];
            if (global_process == 0) {

                pid_t pid = Fork();
                if (pid == 0) {
                    //close dupplicate 
                    close(read_res);
                    //start handler
                    client_handler(buffer);

                }               
            } else {
                write_log("Invalid command, should be connect informaiton\n", append_mode);
            }
        }
    }
    close(read_res);

    return 0;
}
