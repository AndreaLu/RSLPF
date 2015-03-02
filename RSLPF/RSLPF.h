#ifndef RSLPF_H
#define RSLPF_H 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>
#define SEQUENCE_SIZE 200

#define PI 3.1415926525
#define BMPTYPE 19778

struct RGBcol
{
	unsigned char R;
	unsigned char B;
	unsigned char G;
	RGBcol() { R = 0; G = 0; B = 0; }
	RGBcol(const unsigned char r, const unsigned char g, const unsigned char b) { R = r; G = g; B = b; }
};
class BMPImage
{
private:
	unsigned int width;
	unsigned int height;
	RGBcol** data;
public:
	BMPImage();
	BMPImage(const unsigned int width, const unsigned int height, const RGBcol color);
	void Clear(const RGBcol color);
	void Clear(); //clear black
	void toFile(const char* filename);
	void set(const unsigned int x, const unsigned int y, const RGBcol color); //unsigned int per evitare check inutili
	void drawLine(const int x0, const int y0, //const int invece di unsigned int per permettere di iniziare le righe anche fuori dall'immagine
		const  int x1, const  int y1, const RGBcol color);
	bool fromFile(const char* filename);
	void negative();
	void contrastEmphasis(const unsigned char min, const unsigned char max);
	void binarize(const unsigned char threshold);
	void toGrayscale();

};
class Program
{
public:
	Program(const char* src); //Inizializza il programma leggendo una sequenza da un file testuale
	Program(); //Inizializza il programma generando una sequenza randomica di SEQUENCE_SIZE valori
	void displaySequence(); //Crea un'immagine bmp 200x100 con il grafico della sequenza e la apre
private:
	std::vector<int> sequence;
	BMPImage* buffer;
};
class Sequence{
private:
	float* data;
	unsigned int lenght;
public:
	Sequence();
	Sequence(float* existing);
	Sequence& operator* (Sequence& h);  //Convoluzione di due sequenze
	Sequence& operator^ (Sequence& h);	//Prodotto membro-a-membro di due sequenze
	Sequence& operator+ (Sequence& h);  //Somma membro-a-membro di due sequenze
	void dft();
	void fft();
};
struct YCrCb{							//1 Byte di luminanza, 2 Byte con segno di crominanza
	unsigned char Y;
	signed char Cr;
	signed char Cb;
	YCrCb() { Y = 0; Cr = 0; Cb = 0; }
	YCrCb(const unsigned char _Y, const signed char _Cr, const signed char _Cb) { Y = _Y; Cr = _Cr; Cb = _Cb; }
};
//Invece di avere YCrCb image bisogna sviluppare una classe image che possa essere utilizzata con entrambi gli standard

#endif