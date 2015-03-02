#include "RSLPF.h"

YCImage::YCImage(){
	this->height = 0;
	this->width = 0;
}

YCImage::YCImage(YCrCb** existing, const unsigned int Width, const unsigned int Height){
	this->data = existing;
	this->width = Width;
	this->height = Height;
}

void YCImage::toGrayscale(){
	unsigned int i, j;

	for (i = 0; i < this->width; i++){
		for (j = 0; j < this->height; j++){
			this->data[i][j].Cb = 0;
			this->data[i][j].Cr = 0;
		}
	}
}