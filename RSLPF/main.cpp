#include "RSLPF.h"


int main()
{
	/*Program prog;
	prog.displaySequence();
	system("pause");*/

	BMPImage prova;
	prova.fromFile("C:\\Users\\Emanuele\\Desktop\\photo_2015-03-01_16-42-44.bmp");
	prova.toGrayscale();
	prova.toFile("test1.bmp");
	system("test1.bmp");
	system("pause");
}