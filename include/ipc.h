#pragma once

#define ENDBYTE '\x07'
int create_socket(int display, int screen);
int connect_to_socket(int display, int screen);
int send_message(int sockfd, const char *message);
char* receive_message(int sockfd); // returned pointer must be free()'d by the caller
