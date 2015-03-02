#include "RSLPF.h"

Sequence::Sequence(){
	this->lenght = 0;
}

Sequence::Sequence(float* source)
{
	this->data = source;
	this->lenght = sizeof(source) / sizeof (float);
}

