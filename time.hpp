#ifndef TIME_HPP
#define TIME_HPP
#include "string.hpp"
#include <string>
using namespace std;
struct mytime
{

	int hour, minute, date;

	mytime()
	{
		date = hour = minute = 0;
	}

	void set(string s)
	{
		if (s[0] == 'X' || s[0] == 'x') {
			date = hour = minute = 0;
			return;
		}
		date = 0;
		hour = (s[0] - '0') * 10 + s[1] - '0';
		minute = (s[3] - '0') * 10 + s[4] - '0';
	}

	bool operator<(const mytime &other)
	{
		int x = date * 1440 + hour * 60 + minute, y = other.date * 1440 + other.hour * 60 + other.minute;
		return x < y;
	}

	mytime &operator = (const mytime &other)
	{
		date = other.date;
		hour = other.hour;
		minute = other.minute;
		return *this;
	}

	int operator-(const mytime &other)
	{
		int x = date * 1440 + hour * 60 + minute, y = other.date * 1440 + other.hour * 60 + other.minute;
		return x - y;
	}
};

class mydate
{
private:
	
	int year, month, day;

public:

	mydate()
	{
		year = month = day = 0;
	}

	void set(string s)
	{
		if (s[0] == 'X' || s[0] == 'x') {
			year = month = day = 0;
			return;
		}
		year = (s[0] - '0') * 1000 + (s[1] - '0') * 100 + (s[2] - '0') * 10 + s[3] - '0';
		month = (s[5] - '0') * 10 + s[6] - '0';
		day = (s[8] - '0') * 10 + s[9] - '0';
	}

	mydate operator++()
	{
		if (day == 30 && (month == 4 || month == 6 || month == 9 || month == 11)) {
			++month;
			day = 1;
		}
		else if (day == 31 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10)) {
			++month;
			day = 1;
		}
		else if (year % 4 == 0 && month == 2 && day == 29) {
			++month;
			day = 1;
		}
		else if (year % 4 != 0 && month == 2 && day == 28) {
			++month;
			day = 1;
		}
		else
			++day;
		return *this;
	}

	string tos()
	{
		string str;
		str += to_string(year);
		str += '-';
		if (month < 10)
			str += '0';
		str += to_string(month);
		str += '-';
		if (day < 10)
			str += '0';
		str += to_string(day);
		return str;
	}
};
#endif