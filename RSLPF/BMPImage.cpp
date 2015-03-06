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
	this->data = new RGBcol*[width];
	for (int i = 0; i < width; i++) {
		this->data[i] = new RGBcol[height];
		for (int j = 0; j < height; j++)
			this->data[i][j] = color;
	}
}

void BMPImage::Clear(const RGBcol color)
{
	for (int i = 0; i < this->width;i++)
	for (int j = 0; j < this->height; j++)
		this->data[j][i] = color;
}
void BMPImage::Clear()
{
	RGBcol black = RGBcol(0, 0, 0);
	for (int i = 0; i < this->width; i++)
	for (int j = 0; j < this->height; j++)
		this->data[j][i] = black;
}

void BMPImage::set(const unsigned int x, const unsigned int y, const RGBcol color)
{
	if (x >= this->width || y >= this->height) return;
	this->data[y][x] = color;							//No need to check if unsigned < 0
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
	this->data = new RGBcol*[fileInfo.biHeight];
	for (i = 0; i < fileInfo.biHeight; i++)
		this->data[i] = new RGBcol[fileInfo.biWidth];

	//Leggo il contenuto dell'immagine
	
	i = fileInfo.biHeight - 1;
	while (i){
		ReadFile(file, this->data[i], 3 * fileInfo.biWidth, &filePosition, NULL);
		i--;
	}
	ReadFile(file, this->data[0], 3 * fileInfo.biWidth, &filePosition, NULL);

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
	fileInfo.biXPelsPerMeter = 2835;
	fileInfo.biYPelsPerMeter = 2835;
	fileInfo.biClrImportant = 0;
	fileInfo.biClrUsed = 0;

	WriteFile(file, &fileHeader, sizeof(fileHeader), &filePosition, NULL);
	WriteFile(file, &fileInfo, sizeof(fileInfo), &filePosition, NULL);

	unsigned int i = this->height - 1;

	while (i){
		WriteFile(file, this->data[i], this->width * 3, &filePosition, NULL);
		i--;
	}
	WriteFile(file, this->data[0], this->width * 3, &filePosition, NULL);

	CloseHandle(file);
	return(true);
}

void BMPImage::negative()
{
	unsigned int x, y;
	for (x = 0; x < this->width; x++)
	{
		for (y = 0; y < this->height; y++)
		{
			this->data[y][x].R = 255 - this->data[y][x].R;
			this->data[y][x].G = 255 - this->data[y][x].G;
			this->data[y][x].B = 255 - this->data[y][x].B;
		}
	}
}
void BMPImage::contrastEmphasis(const unsigned char min, const unsigned char max)
{

	if (max < min) return;														//Non fa niente con dati errati
	unsigned int x, y;
	for (x = 0; x < this->width; x++)
	{
		for (y = 0; y < this->height; y++)
		{
			if (this->data[y][x].R < min) this->data[y][x].R = 0;
			else if (this->data[y][x].R > max) this->data[y][x].R = 255;
			else this->data[y][x].R *= 255 / (max - min);
			
			if (this->data[y][x].G < min) this->data[y][x].G = 0;
			else if (this->data[y][x].G > max) this->data[y][x].G = 255;
			else this->data[y][x].G *= 255 / (max - min);

			if (this->data[y][x].B < min) this->data[y][x].B = 0;
			else if (this->data[y][x].B > max) this->data[y][x].B = 255;
			else this->data[y][x].B *= 255 / (max - min);
		}
	}
}
void BMPImage::binarize(const unsigned char threshold)
{
	unsigned int x, y;
	for (x = 0; x < this->width; x++)
	{
		for (y = 0; y < this->height; y++)
		{
			if (this->data[y][x].R < threshold) this->data[y][x].R = 0;
			else this->data[y][x].R = 255;

			if (this->data[y][x].G < threshold) this->data[y][x].G = 0;
			else this->data[y][x].G = 255;
			
			if (this->data[y][x].B < threshold) this->data[y][x].B = 0;
			else this->data[y][x].B = 255;
		}
	}
}
void BMPImage::toGrayscale()
{
	unsigned char luma;
	unsigned int x, y;

	for (x = 0; x < this->width; x++)
	{
		for (y = 0; y < this->height; y++)
		{
			luma = (unsigned char) 0.2126 * (this->data[y][x].R) + 0.7152 * (this->data[y][x].G) + 0.0722 * (this->data[y][x].B);
			this->data[y][x].R = luma;
			this->data[y][x].G = luma;
			this->data[y][x].B = luma;
		}
	}
}