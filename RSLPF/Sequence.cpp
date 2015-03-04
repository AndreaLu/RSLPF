#include "RSLPF.h"

Sequence::Sequence(){
	this->length = 0;
	this->data = NULL;
}
Sequence::Sequence(unsigned int size)
{
	length = size;
	data = new float[size];
	for (unsigned int i = 0; i < size; i++)
		data[i] = 0;
}
Sequence::Sequence(Sequence& seq)
{
	this->length = seq.length;
	this->data = new float[this->length];
	for (unsigned int i = 0; i < this->length; i++)
		this->data[i] = seq.data[i];
}

Sequence& Sequence::operator* (Sequence& h)
{
	unsigned int totalLength = (length + h.length - 1);
	Sequence returnV(totalLength);

	for (unsigned int i = 0; i < totalLength; i++) {
		returnV.data[i] = 0;
		for (unsigned int j = 0; j <= i; j++)						
			returnV.data[i] += data[j] * h.data[i - j];
	}
	return(returnV);
}

Sequence& Sequence::operator^ (Sequence& h){//questa funzione non va bene, andrebbe fatto che non può funzionare tra sequenze di dimensioni differenze in quanto non ha un senso matematico
	Sequence returnV(min(length,h.length));

	for (unsigned int i = 0; i < returnV.length; i++)
		returnV.data[i] = data[i] * h.data[i];
	
	return(returnV);
}

Sequence& Sequence::operator+ (Sequence& h){  //questa funzione non va bene, andrebbe fatto che non può funzionare tra sequenze di dimensioni differenze in quanto non ha un senso matematico
	Sequence returnV(min(length,h.length));

	for (unsigned int i = 0; i < returnV.length; i++){
		returnV.data[i] = data[i] + h.data[i];
	}

	return(returnV);
}

void Sequence::normalize(){
	float maxV = 0;
	unsigned int i;

	for (i = 0; i < this->length; i++){
		maxV = max(maxV, abs(this->data[i]));
	}

	if (maxV == 0) return;			//Sequenza di zeri

	for (i = 0; i < this->length; i++){
		this->data[i] = this->data[i] / maxV;
	}
}

void Sequence::lpf(const float sampleRate, const float cut_off_frequency){
	unsigned int i;
	
	float coeff_a = 1 / (abs(sampleRate) + abs(cut_off_frequency));
	float coeff_b = abs(sampleRate) * coeff_a;

	for (i = 1; i < this->length; i++){
		this->data[i] = coeff_a * this->data[i] + coeff_b * this->data[i - 1];
	}
}