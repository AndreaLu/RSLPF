#include "RSLPF.h"


int main()
{
	/*Program prog;
	prog.displaySequence();
	system("pause");*/

	BMPImage prova;
	prova.fromFile("amici_miei.bmp");
	prova.toGrayscale();
	system("pause");
	prova.binarize(80);
	system("pause");
	prova.toFile("test1.bmp");
	system("test1.bmp");
	system("pause");
}