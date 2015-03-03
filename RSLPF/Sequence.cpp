#include "RSLPF.h"

Sequence::Sequence(){
	this->lenght = 0;
}

Sequence::Sequence(float* source)
{
	this->data = source;
	this->lenght = sizeof(source) / sizeof (float);
}

Sequence& Sequence::operator* (Sequence& h)
{
	unsigned int totalLenght = (this->lenght + h.lenght - 1);
	unsigned int i, j;
	float* y = new float[totalLenght];

	for (i = 0; i < totalLenght; i++)
	{
		y[i] = 0;
		for (j = 0; j <= i; j++)						
		{
			y[i] += this->data[j] * h.data[i - j];
		}
	}
	Sequence return_V(y);
	return return_V;
}

Sequence& Sequence::operator^ (Sequence& h){
	unsigned int i;
	float* y = new float[min(this->lenght, h.lenght)];

	for (i = 0; i < min(this->lenght, h.lenght); i++){
		y[i] = this->data[i] * h.data[i];
	}
	
	Sequence return_V(y);
	return return_V;
}

Sequence& Sequence::operator+ (Sequence& h){
	unsigned int i;
	float* y = new float[min(this->lenght, h.lenght)];

	for (i = 0; i < min(this->lenght, h.lenght); i++){
		y[i] = this->data[i] + h.data[i];
	}

	Sequence return_V(y);
	return return_V;
}

void Sequence::normalize(){
	float maxV = 0;
	unsigned int i;

	for (i = 0; i < this->lenght; i++){
		maxV = max(maxV, abs(this->data[i]));
	}

	if (maxV == 0) return;			//Sequenza di zeri

	for (i = 0; i < this->lenght; i++){
		this->data[i] = this->data[i] / maxV;
	}
}