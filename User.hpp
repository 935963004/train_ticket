#ifndef USER_HPP
#define USER_HPP
#include "string.hpp"
#include "vector.hpp"
struct User
{
	tstring<20> password, email, phone;
	tstring<40> name;
	int privilege, id;
};
#endif