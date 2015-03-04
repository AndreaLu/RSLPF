#include "RSLPF.h"


int main()
{
	/*Program prog;
	prog.displaySequence();
	system("pause");*/

	BMPImage prova;
	prova.fromFile("amici_miei1.bmp");
	prova.toGrayscale();
	prova.binarize(80);
	prova.toFile("test1.bmp");
	system("test1.bmp");
	system("pause");
}