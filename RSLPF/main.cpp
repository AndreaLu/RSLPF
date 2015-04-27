#include "RSLPF.h"


int main(int argc, char* argv[])
{
	BMPImage prova;
	prova.fromFile("C:\\Tests\\lena.bmp");
	prova.transpose();
}