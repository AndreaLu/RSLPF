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
	BYTE R;
	BYTE B;
	BYTE G;
	RGBcol() { R = 0; G = 0; B = 0; }
	RGBcol(const BYTE r, const BYTE g, const BYTE b) { R = r; G = g; B = b; }
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
	void fromFile(const char* filename);
	void negative();
	void contrastEmphasis(const BYTE min, const BYTE max);
	void binarize(const BYTE threshold);
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

	Sequence operator * (Sequence h){
		unsigned int totalLenght = (this->lenght + h.lenght - 1);
		unsigned int i, j;
		float* y = new float[totalLenght];

		for (i = 0; i < totalLenght; i++)
		{
			y[i] = 0;
			for (j = 0; j <  ; j++)
			{
				y[i] += this->data[i] * h.data[j - i];
			}
		}
	
	}
};

#endif