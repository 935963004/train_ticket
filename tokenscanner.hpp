#ifndef TOKENSCANNER_HPP
#define TOKENSCANNER_HPP
#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

class scanner
{
private:

	char *buffer;

public:

	scanner()
	{
		buffer = nullptr;
	}

	scanner(char *s)
	{
		buffer = s;
	}

	void set(char *s)
	{
		buffer = s;
	}

	string gets()
	{
		if (buffer == nullptr) {
			string tmp;
			cin >> tmp;
			return tmp;
		}
		while (buffer[0] == ' ' || buffer[0] == '\n')
			++buffer;
		string tmp;
		while (buffer[0] != ' ' && buffer[0] != '\n' && buffer[0] != '\0') {
			tmp += buffer[0];
			++buffer;
		}
		return tmp;
	}

	int geti()
	{
		if (buffer == nullptr) {
			int tmp;
			cin >> tmp;
			return tmp;
		}
		while (buffer[0] == ' ' || buffer[0] == '\n')
			++buffer;
		int tmp =  0;
		while (buffer[0] != ' ' && buffer[0] != '\n' && buffer[0] != '\0') {
			tmp = tmp * 10 + buffer[0] - '0';
			++buffer;
		}
		return tmp;
	}

	double getd()
	{
		if (buffer == nullptr) {
			double tmp;
			cin >> tmp;
			return tmp;
		}
		while (buffer[0] == ' ' || buffer[0] == '\n')
			++buffer;
		double tmp = 0;
		int cnt = 1, flag = 0;
		while (buffer[0] != ' ' && buffer[0] != '\n' && buffer[0] != '\0') {
			if (flag)
				cnt *= 10;
			if (buffer[0] == '.')
				flag = 1;
			else
				tmp = tmp * 10 + buffer[0] - '0';
			++buffer;
		}
		tmp /= cnt;
		return tmp;
	}

	char getc()
	{
		if (buffer == nullptr) {
			char c = getchar();
			while (c == ' ' || c == '\n' || c == '\0')
				c = getchar();
			return c;
		}
		char c;
		while (buffer[0] == ' ' || buffer[0] == '\n')
			++buffer;
		c = buffer[0];
		++buffer;
		return c;
	}
};
#endif
