#include "RSLPF.h"
Program::Program(const char* src) //Inizializza il programma leggendo una sequenza da un file testuale
{
	//Leggi il file testuale
	std::ifstream input_file(src);
	std::string line;
	if (!input_file.is_open())
	{
		std::cout << "Impossibile leggere il file testuale con la sequenza in ingresso!" << std::endl;
		return;
	}
	this->buffer = new BMPImage(400, 255, RGBcol(255, 255, 255));
}

Program::Program()
{
	//Genera la sequenza randomica e la salva in un file
	srand(0);
	std::ofstream output_file;
	output_file.open("file.txt");
	if (!output_file.is_open()) {
		std::cout << "Impossibile creare un file in uscita!" << std::endl;
		return;
	}
	char buff[5];
	int val;
	for (int i = 1; i <= SEQUENCE_SIZE; i++) {
		val = rand() % 255;
		this->sequence.push_back(val);
		_itoa_s(val, buff, 10);
		output_file << buff;
		if (i != SEQUENCE_SIZE)
			output_file << std::endl;
	}
	output_file.close();

	this->buffer = new BMPImage(400,255, RGBcol(255, 255, 255));

	//Faccio una prova: disegno una riga rossa e un quadrato blu
	float step = 2 * PI / 480;
	int x0, y0, x1, y1;
	for (float angle = step; angle <= step + 2 * PI; angle += step)
	{
		x0 = 200 + cos(angle - step) * 70;
		y0 = 127 + sin(angle - step) * 70;
		x1 = 200 + cos(angle) * 70;
		y1 = 127 + sin(angle) * 70;
		this->buffer->drawLine(x0, y0, x1, y1, RGBcol(255, 0, 0));
		this->buffer->drawLine(200, 127, x1, y1, RGBcol(255, 0, 0));
	}

}

void Program::displaySequence()
{
	//Salva l'immagine in BMP
	this->buffer->toFile("test.bmp");
	system("test.bmp");
}