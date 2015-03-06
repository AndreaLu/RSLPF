#ifndef RSLPF_H
#define RSLPF_H 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <Windows.h>
#define SEQUENCE_SIZE 200

#define PI 3.1415926525
#define BMPTYPE 19778

struct RGBcol
{
	unsigned char B;
	unsigned char G;
	unsigned char R;
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
	void set(const unsigned int x, const unsigned int y, const RGBcol color); //unsigned int per evitare check inutili
	void drawLine(const int x0, const int y0, const  int x1, const  int y1, const RGBcol color);  //const int invece di unsigned int per permettere di iniziare le righe anche fuori dall'immagine 
	bool fromFile(const char* fileName);
	bool toFile(const char* fileName);
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
	unsigned int length;
public:
	Sequence();
	Sequence(unsigned int length);
	Sequence(Sequence& existing);
	Sequence& operator* (Sequence& h);  //Convoluzione di due sequenze
	Sequence& operator^ (Sequence& h);	//Prodotto membro-a-membro di due sequenze
	Sequence& operator+ (Sequence& h);  //Somma membro-a-membro di due sequenze
	void dft();
	void fft();
	void normalize();					//Tutti i valori [-1, 1]
	void lpf(const float sampleRate, const float cut_off_frequency);
};
//Basta con sta struttura Y Cr Cb
#endif