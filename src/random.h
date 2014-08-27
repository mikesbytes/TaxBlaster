#ifndef RANDOM_H
#define RANDOM_H

class Random
{
public:
	Random();
	~Random();
	
	unsigned long getRand();
	int getRand(const long& lowerLimit, const long& upperLimit);
	
private:
	unsigned long sX;
	unsigned long sY;
	unsigned long sZ;
};

#endif // RANDOM_H
