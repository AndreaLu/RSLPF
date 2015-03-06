#include "RSLPF.h"


int main()
{
	/*Program prog;
	prog.displaySequence();
	system("pause");*/

	BMPImage prova;
	prova.fromFile("amici_miei.bmp");
	prova.toFile("prova.bmp");
	system("pause");
}