#include "RSLPF.h" 

BMPImage::BMPImage()
{
	//Crea un'immagine di 0x0 pixel
	this->width = 0;
	this->height = 0;
}
BMPImage::BMPImage(const unsigned int width, const unsigned int height, const RGBcol color)
{
	this->width = width;
	this->height = height;
	this->data = new RGBcol[width*height];
	for (unsigned int i = 0; i < width*height; i++)
		this->data[i] = color;
}

void BMPImage::Clear(const RGBcol color)
{

	for (int j = 0; j < this->height*this->width; j++)
		this->data[j] = color;
}
void BMPImage::Clear()
{
	RGBcol black = RGBcol(0, 0, 0);
	for (int j = 0; j < this->height*this->width; j++)
		this->data[j] = black;
}
RGBcol BMPImage::get(unsigned int x, unsigned int y)
{
	return(this->data[x + (this->height - y)*this->width]);	
}
void BMPImage::set(const unsigned int x, const unsigned int y, const RGBcol color)
{
	if (x >= this->width || y >= this->height) return;
	this->data[(this->height - y)*this->width + x] = color;
}
void BMPImage::drawLine(const int x0, const int y0, const int x1, const int y1, const RGBcol color)
{
	int x = x0, y = y0;
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2, e2;
	for (;;) {
		this->set(x, y, color);
		if (x == x1 && y == y1) break;
		e2 = err;
		if (e2 > -dx) { err -= dy; x += sx; }
		if (e2 < dy) { err += dx; y += sy; }
	}
}

bool BMPImage::fromFile(const char* fileName){
	HANDLE file;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER fileInfo;
	unsigned long filePosition = 0;

	file = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE){
		std::cout << "Impossibile aprire il file per la lettura." << std::endl;
		return(false);
	}

	ReadFile(file, &fileHeader, sizeof(BITMAPFILEHEADER), &filePosition, NULL);
	ReadFile(file, &fileInfo, sizeof(BITMAPINFOHEADER), &filePosition , NULL);

	if (fileHeader.bfType != BMPTYPE || fileInfo.biBitCount != 24)												//Controlla che il file in questione sia un BMP 24-bit
	{
		std::cout << "Formato BMP incompatibile, utilizza 24bit non compresso." << std::endl;
		return(false);
	}

	this->height = fileInfo.biHeight;
	this->width = fileInfo.biWidth;

	unsigned int i;

	//Alloca la memoria per contenere l'immagine
	this->data = new RGBcol[fileInfo.biHeight*fileInfo.biWidth];
	ReadFile(file, this->data, 3 * fileInfo.biHeight*fileInfo.biWidth, &filePosition, NULL);
	CloseHandle(file);
	return(true);
}
bool BMPImage::toFile(const char* fileName){
	HANDLE file;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER fileInfo;
	unsigned long filePosition = 0;

	file = CreateFileA(fileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);   //Sets up the new bmp to be written to

	if (file == INVALID_HANDLE_VALUE){
		std::cout << "Impossibile aprire il file per la scrittura." << std::endl;
		return(false);
	}

	fileHeader.bfType = BMPTYPE;                                                                         //Sets our type to BM or bmp
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 3 * (this->width * this->height);                             //Sets the size equal to the size of the header struct
	fileHeader.bfReserved1 = 0;                                                                        //sets the reserves to 0
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);                     //Sets offbits equal to the size of file and info header

	fileInfo.biSize = sizeof(BITMAPINFOHEADER);
	fileInfo.biWidth = this->width;
	fileInfo.biHeight = this->height;
	fileInfo.biPlanes = 1;
	fileInfo.biBitCount = 24;
	fileInfo.biCompression = BI_RGB;
	fileInfo.biSizeImage = 0; //This may be set to zero for BI_RGB bitmaps
	fileInfo.biXPelsPerMeter = 0;
	fileInfo.biYPelsPerMeter = 0;
	fileInfo.biClrImportant = 0;
	fileInfo.biClrUsed = 0;

	WriteFile(file, &fileHeader, sizeof(fileHeader), &filePosition, NULL);
	WriteFile(file, &fileInfo, sizeof(fileInfo), &filePosition, NULL);

	WriteFile(file, this->data, this->width*this->height * 3, &filePosition, NULL);

	CloseHandle(file);
	return(true);
}

void BMPImage::negative()
{
	RGBcol col;
	unsigned int x, y;
	for (x = 0; x < this->width; x++)
	{
		for (y = 0; y < this->height; y++)
		{
			col = this->get(x, y);
			col.R = 255 - col.R; col.B = 255 - col.B; col.G = 255 - col.G;
			this->set(x, y, col);
		}
	}
}
void BMPImage::contrastEmphasis(const unsigned char min, const unsigned char max)
{
	 
}
void BMPImage::binarize(const unsigned char threshold)
{
	RGBcol col;
	for (unsigned int i = 0; i < this->width*this->height; i++)
	{
		col = this->data[i];
		col.R >= threshold ? col.R = 255 : col.R = 0;
		col.G >= threshold ? col.G = 255 : col.G = 0;
		col.B >= threshold ? col.B = 255 : col.B = 0;
		this->data[i] = col;
	}
}
void BMPImage::toGrayscale()
{
}
void BMPImage::transpose()
{
	BMPImage newimg = BMPImage(height, width, RGBcol(0, 0, 0));
	for (unsigned int x = 0; x < width;x++)
	for (unsigned int y = 0; y < height; y++)
		newimg.set(y, x, this->get(x, y) );
	newimg.toFile("C:\\Tests\\stocazz.bmp");

}