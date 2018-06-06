#ifndef TRIANSTATION_HPP
#define TRAINSTATION_HPP
#include "string.hpp"
#include "vector.hpp"
#include "time.hpp"
struct Station
{
	tstring<40> name;
	double price[5];
	tstring<5> timeArrive, timeStart, timeStopover;
	mytime ta, ts;
};

struct Train
{
	tstring<40> name;
	tstring<20> id, priceName[5];
	int stationNum, priceNum;
	Station sta[60];
	char catalog;
	bool isSaled = false;
};
#endif