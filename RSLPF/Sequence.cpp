#include "RSLPF.h"

Sequence::Sequence(){
	this->lenght = 0;
}

Sequence::Sequence(float* source)
{
	this->data = source;
	this->lenght = sizeof(source) / sizeof (float);
}

Sequence Sequence::operator * (Sequence h)
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
