/*
 * @Author: Haitian Li
 * @Date: 2021-05-14 09:53:28
 * @LastEditors: Haitian Li
 * @LastEditTime: 2021-05-17 22:11:46
 * @Description: Assignment4
 */
#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#define ascii_name 256
#define buffer_max 2048
//should be greater than 256
#define temp_file 300
#define ping_code 5
#define byte_zero 0
#define say_code 1
#define say_cont_code 2
#define receive_code 3
#define receive_cont_code 4
#define pong_code 6
#define success_exit 0
#define write_mode 0
#define append_mode 1
#endif
