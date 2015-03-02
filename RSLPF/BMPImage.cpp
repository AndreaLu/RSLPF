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
		this->data[i][j] = color;
}
void BMPImage::Clear()
{
	RGBcol black = RGBcol(0, 0, 0);
	for (int i = 0; i < this->width; i++)
	for (int j = 0; j < this->height; j++)
		this->data[i][j] = black;
}

void BMPImage::toFile(const char* filename)
{
	HANDLE file;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER fileInfo;
	DWORD write = 0;

	file = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);   //Sets up the new bmp to be written to

	fileHeader.bfType = BMPTYPE;                                                                         //Sets our type to BM or bmp
	fileHeader.bfSize = sizeof(fileHeader.bfOffBits) + sizeof(RGBTRIPLE);                              //Sets the size equal to the size of the header struct
	fileHeader.bfReserved1 = 0;                                                                        //sets the reserves to 0
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);                        //Sets offbits equal to the size of file and info header

	fileInfo.biSize = sizeof(BITMAPINFOHEADER);
	fileInfo.biWidth = this->width;
	fileInfo.biHeight = this->height;
	fileInfo.biPlanes = 1;
	fileInfo.biBitCount = 24;
	fileInfo.biCompression = BI_RGB;
	fileInfo.biSizeImage = 640 * 480 * (24 / 8);
	fileInfo.biXPelsPerMeter = 2400;
	fileInfo.biYPelsPerMeter = 2400;
	fileInfo.biClrImportant = 0;
	fileInfo.biClrUsed = 0;

	WriteFile(file, &fileHeader, sizeof(fileHeader), &write, NULL);
	WriteFile(file, &fileInfo, sizeof(fileInfo), &write, NULL);
	unsigned int W;
	unsigned int H;
	W = this->width; H = this->height;
	LPVOID pointerR,pointerG,pointerB;

	for (unsigned int y = H - 1; y >= 0; y--) {
		for (unsigned int x = 0; x < W; x++) {
			pointerR = &(this->data[x][y].R);
			pointerG = &(this->data[x][y].G);
			pointerB = &(this->data[x][y].B);
			WriteFile(file, pointerB, 1, &write, NULL);
			WriteFile(file, pointerG, 1, &write, NULL);
			WriteFile(file, pointerR, 1, &write, NULL);
		}
		if (y == 0) break;
	}

	CloseHandle(file);
}

void BMPImage::set(const unsigned int x, const unsigned int y, const RGBcol color)
{
	if (x >= this->width || y >= this->height) return;
	this->data[x][y] = color;							//No need to check if unsigned < 0
}

int sign(int a)
{
	if (a >= 0) return(1);
	if (a < 0) return(0);
}
void BMPImage::drawLine(const int x0, const int y0,
						const int x1, const int y1, 
						const RGBcol color)
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
void BMPImage::fromFile(const char* filename)
{
	HANDLE file;
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER fileInfo;
	DWORD filePosition = 0;

	file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (file == INVALID_HANDLE_VALUE){
		std::cout << "Unable to open file." << std::endl;
		system("pause");
		return;
	}

	ReadFile(file, &fileHeader, sizeof(BITMAPFILEHEADER), &filePosition, NULL);
	ReadFile(file, &fileInfo, sizeof(BITMAPINFOHEADER), &filePosition , NULL);

	if (fileHeader.bfType != BMPTYPE || fileInfo.biBitCount != 24 )												//Controlla che il file in questione sia un BMP 24-bit
	{
		std::cout << "Unknown BMP format." << std::endl;
		system("pause");
		return;
	}

	this->height = fileInfo.biHeight;
	this->width = fileInfo.biWidth;
	this->data = new RGBcol*[fileInfo.biWidth];

	for (int i = 0; i < fileInfo.biWidth; i++)									//Alloca la memoria per contenere l'immagine
	{
		this->data[i] = new RGBcol[fileInfo.biHeight];
	}

	DWORD32 x, y = fileInfo.biHeight - 1;
	BYTE temp;
	
	while (y + 1 > 0)															//Necessario in quanto unsigned
	{
		x = 0;
		while (x < fileInfo.biWidth)
		{
			ReadFile(file, &temp, 1, &filePosition, NULL);
			this->data[x][y].B = temp;
			ReadFile(file, &temp, 1, &filePosition, NULL);
			this->data[x][y].G = temp;
			ReadFile(file, &temp, 1, &filePosition, NULL);
			this->data[x][y].R = temp;
			x++;
		}
		y--;
	}

	CloseHandle(file);
}
void BMPImage::negative()
{
	for (DWORD32 x = 0; x < this->width; x++)
	{
		for (DWORD32 y = 0; y < this->height; y++)
		{
			this->data[x][y].R = 255 - this->data[x][y].R;
			this->data[x][y].G = 255 - this->data[x][y].G;
			this->data[x][y].B = 255 - this->data[x][y].B;
		}
	}
}
void BMPImage::contrastEmphasis(const BYTE min, const BYTE max)
{

	if (max < min) return;														//Non fa niente con dati errati

	for (DWORD32 x = 0; x < this->width; x++)
	{
		for (DWORD32 y = 0; y < this->height; y++)
		{
			if (this->data[x][y].R < min) this->data[x][y].R = 0;
			else if (this->data[x][y].R > max) this->data[x][y].R = 255;
			else this->data[x][y].R *= 255 / (max - min);
			
			if (this->data[x][y].G < min) this->data[x][y].G = 0;
			else if (this->data[x][y].G > max) this->data[x][y].G = 255;
			else this->data[x][y].G *= 255 / (max - min);

			if (this->data[x][y].B < min) this->data[x][y].B = 0;
			else if (this->data[x][y].B > max) this->data[x][y].B = 255;
			else this->data[x][y].B *= 255 / (max - min);
		}
	}
}
void BMPImage::binarize(const BYTE threshold)
{
	for (DWORD32 x = 0; x < this->width; x++)
	{
		for (DWORD32 y = 0; y < this->height; y++)
		{
			if (this->data[x][y].R < threshold) this->data[x][y].R = 0;
			else this->data[x][y].R = 255;

			if (this->data[x][y].G < threshold) this->data[x][y].G = 0;
			else this->data[x][y].G = 255;
			
			if (this->data[x][y].B < threshold) this->data[x][y].B = 0;
			else this->data[x][y].B = 255;
		}
	}
}

void BMPImage::toGrayscale()
{
	BYTE Y;

	for (DWORD32 x = 0; x < this->width; x++)
	{
		for (DWORD32 y = 0; y < this->height; y++)
		{
			Y = (BYTE) 0.21 * (this->data[x][y].R) + 0.72 * (this->data[x][y].R) + 0.07 * (this->data[x][y].R);
			this->data[x][y].R = Y;
			this->data[x][y].G = Y;
			this->data[x][y].B = Y;
		}
	}
}