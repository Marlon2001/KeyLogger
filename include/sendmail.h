#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <locale.h>
#include "header.h"
#include "base64.h"

#pragma comment(lib, "Ws2_32.lib")

char *MailHeader(const char* from, const char* to, const char* subject);
SOCKET ConnectToServer(const char* server_address);

int sendEmail(const char* de, const char* para, const char* assunto, const char* mensagem){
    char *header = MailHeader(de, para, assunto);
    SOCKET connected_fd = ConnectToServer(SMTP_ADDRESS);

    if(connected_fd != -1){
        int recvd = 0;
        char recv_buff[5000];
        int sdsd;
        
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        
        char buff[1000];
        strcpy(buff, "EHLO ");
        strcat(buff, DOMAIN);
        strcat(buff, "\r\n");
        send(connected_fd, buff, strlen(buff), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, buff);

        char _cmd2[1000];
        strcpy(_cmd2, "AUTH LOGIN\r\n");
        send(connected_fd, _cmd2, strlen(_cmd2), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd2);
        
        char *_cmd3 = base64_encode(UID, strlen(UID));
        strcat(_cmd3, "\r\n");
        send(connected_fd, _cmd3, strlen(_cmd3), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd3);

        char *_cmd4 = base64_encode(PWD, strlen(PWD));
        strcat(_cmd4, "\r\n");
        send(connected_fd, _cmd4, strlen(_cmd4), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd4);

        char _cmd5[1000];
        strcpy(_cmd5, "mail from: ");
        strcat(_cmd5, de);
        strcat(_cmd5, "\r\n");
        send(connected_fd, _cmd5, strlen(_cmd5), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd5);

        char _cmd6[1000];
        strcpy(_cmd6, "rcpt to: ");
        strcat(_cmd6, para);
        strcat(_cmd6, "\r\n");
        send(connected_fd, _cmd6, strlen(_cmd6), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd6);

        char _cmd7[1000];
        strcpy(_cmd7, "DATA\r\n");
        send(connected_fd, _cmd7, strlen(_cmd7), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd7);

        char to[1000];
        strcpy(to, "To: ");
        strcat(to, para);
        strcat(to, "\r\n");
        send(connected_fd, to, strlen(to), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, to);

        char from[1000];
        strcpy(from, "From: ");
        strcat(from, de);
        strcat(from, "\r\n");
        send(connected_fd, from, strlen(from), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, from);

        char subject[1000];
        strcpy(subject, "Subject: ");
        strcat(subject, assunto);
        strcat(subject, "\r\n");
        send(connected_fd, subject, strlen(subject), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, subject);

        // send(connected_fd, header, strlen(header), 0);
        // sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        // recvd += sdsd;
        // printf("%i:\t\n%s\n", sdsd, header);

        char _cmd8[1000];
        strcpy(_cmd8, mensagem);
        strcat(_cmd8, "\r\n");
        send(connected_fd, _cmd8, sizeof(_cmd8), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd8);

        char _cmd9[1000];
        strcpy(_cmd9, ".\r\n");
        send(connected_fd, _cmd9, sizeof(_cmd9), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd9);

        char _cmd10[1000];
        strcpy(_cmd10, "quit\r\n");
        send(connected_fd, _cmd10, sizeof(_cmd10), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd10);

        printf("\n\n%s\r\n", &recv_buff);
        free(header);
        closesocket(connected_fd);
    }
    
    return (EXIT_SUCCESS);
}

char *MailHeader(const char* from, const char* to, const char* subject){
    time_t now;
    time(&now);
    
    char* mail_header = NULL;

    char date_buff[26];
    char Branding[6 + strlen(date_buff) + 2]; 
    char Sender[6 + strlen(from) + 2]; 
    char Recip[4 + strlen(to) + 2]; 
    char Subject[9 + strlen(subject) + 2];

    strftime(date_buff, (33), "%a , %d %b %Y %H:%M:%S", localtime(&now));
    sprintf(Branding, "Date: %s\r\n", date_buff);
    sprintf(Recip, "To: %s\r\n", to);
    sprintf(Sender, "From: %s\r\n", from);
    sprintf(Subject, "Subject: %s\r\n", subject);

    int mail_header_length = strlen(Branding) + strlen(Sender) + strlen(Recip) + strlen(Subject) + 10;

    mail_header = (char *) malloc(mail_header_length *sizeof(char));

    memcpy(&mail_header[0], &Branding, strlen(Branding));
    memcpy(&mail_header[0 + strlen(Branding)], &Sender, strlen(Sender));
    memcpy(&mail_header[0 + strlen(Branding) + strlen(Sender)], &Recip, strlen(Recip));
    memcpy(&mail_header[0 + strlen(Branding) + strlen(Sender) + strlen(Recip)], &Subject, strlen(Subject));

    return mail_header;
}

SOCKET ConnectToServer(const char* server_address) {
    WSADATA wsaData;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(server_address);
    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Erro ao iniciar o WSA.\n");
        return -1;
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (socket_fd == INVALID_SOCKET || socket_fd == SOCKET_ERROR) {        
        printf("Erro ao criar o socket: %i", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    if(connect(socket_fd, (SOCKADDR *)&addr, sizeof(addr)) < 0){
        printf("Erro na conexao.\n");
        return -1;
    }else{
        printf("Socket conectado.\n");
    }
    
    return socket_fd;
}