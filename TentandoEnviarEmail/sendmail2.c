#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>
#include <string.h>
#include <time.h>
#include <base64.h>
#include <locale.h>
#include "para.h"

char * MailHeader(const char* from, const char* to, const char* subject, const char* mime_type, const char* charset);
int ConnectToServer(const char* server_address);
const char* GetIPAddress(const char* target_domain);

int main(){
	setlocale(LC_ALL, "Portuguese");
    char *header = MailHeader(FROM, TO, "Olá, esta é umm mensagem de teste.", "text/plain", "ISO-8859-1");
    printf("%s", header);
    int connected_fd = ConnectToServer(smtp_domain_name);
    
    if(connected_fd != -1){
        int recvd = 0;
        char recv_buff[4768];
        int sdsd;
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;

        char buff[1000];
        strcpy(buff, "EHLO");
        strcat(buff, domain);
        strcat(buff, "\r\n");
        
        send(connected_fd, buff, strlen(buff), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;

        char _cmd2[1000];
        strcpy(_cmd2, "AUTH LOGIN\r\n");
        int dfdf = send(connected_fd, _cmd2, strlen(_cmd2), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        
        char *_cmd3;
        b64_encodef(UID, _cmd3);
        strcat(_cmd3, "\r\n");
        send(connected_fd, _cmd3, strlen(_cmd3), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;

        char *_cmd4;
        b64_encodef(PWD, _cmd4);
        strcat(_cmd4, "\r\n");
        send(connected_fd, _cmd4, strlen(_cmd4), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;

        char _cmd5[1000];
        strcpy(_cmd5, "MAIL FROM: ");
        strcat(_cmd5, FROM);
        strcat(_cmd5, "\r\n");
        send(connected_fd, _cmd5, strlen(_cmd5), 0);
        char skip[1000];
        sdsd = recv(connected_fd, skip, sizeof(skip), 0);

        char _cmd6[1000];
        strcpy(_cmd6, "RCPT TO: ");
        strcat(_cmd6, TO);
        strcat(_cmd6, "\r\n");
        send(connected_fd, _cmd6, strlen(_cmd6), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;

        char _cmd7[1000];
        strcpy(_cmd7, "DATA\r\n");
        send(connected_fd, _cmd7, strlen(_cmd7), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;

        send(connected_fd, header, strlen(header), 0);
        char _cmd8[1000];
        strcpy(_cmd8, "Ola eu estou testando meu smtp\r\n");
        send(connected_fd, _cmd8, sizeof(_cmd8), 0);
        char _cmd9[1000];
        strcpy(_cmd9, "\r\n.\r\n.");
        send(connected_fd, _cmd9, sizeof(_cmd9), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;

        char _cmd10[1000];
        strcpy(_cmd10, "QUIT\r\n");
        send(connected_fd, _cmd10, sizeof(_cmd10), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("0)%s\r\n", &recv_buff);
    }    
    free(header);
    closesocket(connected_fd);
    
    getchar();
    return (EXIT_SUCCESS);
}

char *MailHeader(const char* from, const char* to, const char* subject, const char* mime_type, const char* charset){
    time_t now;
    time(&now);
    
    char *app_brand = "Application for Tests.";
    char* mail_header = NULL;

    // Criando vetores que conterão cada linha do cabeçalho
    char date_buff[26]; 
    char Branding[6 + strlen(date_buff) + 12 + strlen(app_brand) + 2]; // <-- Guardará DATA, HORA e o cabeçalho X-Mailer (aplicação que rascunhou e enviou o e-mail)
    char Sender[6 + strlen(from) + 2]; // <-- Remetente
    char Recip[4 + strlen(to) + 2]; // <-- Destinatário
    char Subject[9 + strlen(subject) + 2]; // <-- Assunto
    char mime_data[33 + strlen(mime_type) + 10 + strlen(charset) + 4]; // <-- Outros dados do cabeçalho

    // Preenchendo os vetores do header com os seus respectivos valores
    strftime(date_buff, (33), "%a , %d %b %Y %H:%M:%S", localtime(&now));
    sprintf(Branding, "DATE: %s\r\nX-Mailer: %s\r\n", date_buff, app_brand);
    sprintf(Sender, "FROM: %s\r\n", from);
    sprintf(Recip, "TO: %s\r\n", to);
    sprintf(Subject, "Subject: %s\r\n", subject);
    sprintf(mime_data, "MIME-Version: 1.0\r\nContent-Type: %s; charset=%s\r\n\r\n", mime_type, charset);

    // Pegando o tamanho de todos os vetores
    int mail_header_length = strlen(Branding) + strlen(Sender) + strlen(Recip) + strlen(Subject) + strlen(mime_data) + 10;

    // Criando um vetor dinamicamente que guardará todo o cabeçalho 
    mail_header = (char *) malloc(mail_header_length *sizeof(char));

    memcpy(&mail_header[0], &Branding, strlen(Branding));
    memcpy(&mail_header[0 + strlen(Branding)], &Sender, strlen(Sender));
    memcpy(&mail_header[0 + strlen(Branding) + strlen(Sender)], &Recip, strlen(Recip));
    memcpy(&mail_header[0 + strlen(Branding) + strlen(Sender) + strlen(Recip)], &Subject, strlen(Subject));
    memcpy(&mail_header[0 + strlen(Branding) + strlen(Sender) + strlen(Recip) + strlen(Subject)], &mime_data, strlen(mime_data));

    return mail_header;
}

int ConnectToServer(const char* server_address){
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    struct sockaddr_in addr; // Struct que contem os ip do servidor de email
    memset(&addr, 0, sizeof(addr)); // Copiando o '0' para todos os membros da structure 'addr'
    addr.sin_family=AF_INET; // 'AF_INET" para endereços IPv4
    addr.sin_port=htons(25); // Porta do 'SMTP'
    
    // const char* ip = GetIPAddress(smtp_domain_name);

    addr.sin_addr.s_addr = inet_addr("182.50.145.3");
    // addr.sin_addr.s_addr = ; // Endereço ip do servidor de email da ásia
    if(connect(socket_fd, (struct sockaddr *)&addr, sizeof(addr))==1){
        printf("sucesso na conexão");
    }else{
        printf("fracasso na conexão");
    };
    
    return socket_fd;
}

const char* GetIPAddress(const char* target_domain){
    const char *target_ip;
    struct in_addr host_address;
    struct hostent *hostinfo=gethostbyname(target_domain);
    host_address = *(struct in_addr *) hostinfo->h_addr_list[0];
    target_ip = inet_ntoa(host_address);
    
    return target_ip;
}