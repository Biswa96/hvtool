// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) Biswapriyo Nath
// This file is part of hvtool project.

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/reboot.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <linux/vm_sockets.h>

#define BUFF_SIZE 0x1000
#define DEFAULT_PORT 54321

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(expression) \
    (__extension__ \
        ({ long int __result; \
            do __result = (long int) (expression); \
            while (__result == -1L && errno == EINTR); \
            __result; }))
#endif

int main() {
    int ret = 0;

    const int sockfd = socket(AF_VSOCK, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    const struct timeval timeout = { .tv_sec = 30, .tv_usec = 0 };
    ret = setsockopt(sockfd, AF_VSOCK,
        SO_VM_SOCKETS_CONNECT_TIMEOUT, &timeout, sizeof timeout);
    if (ret < 0) {
        perror("setsockopt");
        close(sockfd);
        return 1;
    }

    struct sockaddr_vm addr = { 0 };
    addr.svm_family = AF_VSOCK;
    addr.svm_port = DEFAULT_PORT;
    addr.svm_cid = VMADDR_CID_HOST;
    ret = connect(sockfd, (struct sockaddr *)&addr, sizeof addr);
    if (ret < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    const pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        close(sockfd);
        return 1;
    }

    if (pid == 0) {
        execlp("/init_script.sh", "/init_script.sh", (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }

    char msg[BUFF_SIZE];

    while(true) {
        ret = TEMP_FAILURE_RETRY(recv(sockfd, msg, BUFF_SIZE, 0));
        if(ret > 0) {
            msg[ret] = '\0';
            printf("%s\n", msg);
        }
        else if (ret == 0) {
            printf("server closing...\n");
            break;
        }
        else {
            perror("recv");
            break;
        }
    };

    close(sockfd);
    sync();
    reboot(RB_POWER_OFF);
    return 0;
}
