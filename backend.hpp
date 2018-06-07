#ifndef BACKEND_HPP
#define BACKEND_HPP
#include <iostream>
#include "User.hpp"
#include "Ticket.hpp"
#include "Train.hpp"
#include "DB.hpp"
#include "string.hpp"
#include "vector.hpp"
#include <string>
#include "time.hpp"
#include "tokenscanner.hpp"
class backend
{
private:

	DB<int, User> userDB;
	DB<tstring<40>, Ticket, cmp> ticketDB;
	DB<tstring<20>, Train, cmp> trainDB;
	DB<tstring<100>, int, cmp> routeDB;
	DB<tstring<30>, tic, cmp> ticket_leftDB;
	DB<tstring<20>, char, cmp> staDB;

public:

	void init()
	{
		userDB.init("user");
		ticketDB.init("ticket");
		trainDB.init("train");
		routeDB.init("route");
		ticket_leftDB.init("ticket_left");
		staDB.init("sta");
		tstring<40> s;
		if (!ticketDB.find(s).second) {
			Ticket tmp;
			tmp.ticketID = 0;
			ticketDB.insert(s, tmp);
		}
	}
	
	int regist(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		User user;
		user.name = Scanner.gets();
		user.password = Scanner.gets();
		user.email = Scanner.gets();
		user.phone = Scanner.gets();
		user.id = userDB.size() + 2018;
		if (user.id == 2018)
			user.privilege = 2;
		else
			user.privilege = 1;
		bool flag = userDB.insert(user.id, user);
		if (flag)
			return user.id;
		else
			return 0;
	}

	int login(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		int userID = Scanner.geti();
		tstring<20> pwd;
		pwd = Scanner.gets();
		auto tmp = userDB.find(userID);
		if (!tmp.second)
			return 0;
		if (tmp.first->password != pwd) {
			delete tmp.first;
			return 0;
		}
		delete tmp.first;
		return 1;
	}

	char* query_profile(int userID)
	{
		auto tmp = userDB.find(userID);
		char *ret;
		if (!tmp.second) {
			ret = new char[2];
			ret[0] = '0';
			ret[1] = '\0';
			return ret;
		}
		string res;
		res += (string)tmp.first->name;
		res += ' ';
		res += (string)tmp.first->email;
		res += ' ';
		res += (string)tmp.first->phone;
		res += ' ';
		res += to_string(tmp.first->privilege);
		int len = res.size();
		ret = new char[len + 1];
		for (int i = 0; i < len; ++i)
			ret[i] = res[i];
		ret[len] = '\0';
		delete tmp.first;
		return ret;
	}

	int modify_profile(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		User user;
		user.id = Scanner.geti();
		user.name = Scanner.gets();
		user.password = Scanner.gets();
		user.email = Scanner.gets();
		user.phone = Scanner.gets();
		auto tmp = userDB.find(user.id);
		if (!tmp.second || tmp.first->password != user.password) {
			delete tmp.first;
			return 0;
		}
		userDB.modify(user.id, user);
		delete tmp.first;
		return 1;
	}

	int modify_privilege(int ID1, int ID2, int pri)
	{
		auto tmp1 = userDB.find(ID1), tmp2 = userDB.find(ID2);
		if (!tmp1.second) {
			if (tmp2.second)
				delete tmp2.first;
			return 0;
		}
		else if (!tmp2.second) {
			delete tmp1.first;
			return 0;
		}
		if (tmp1.first->privilege == 1 || (tmp2.first->privilege == 2 && pri == 1)) {
			delete tmp1.first;
			delete tmp2.first;
			return 0;
		}
		tmp2.first->privilege = pri;
		userDB.modify(ID2, *(tmp2.first));
		delete tmp1.first;
		delete tmp2.first;
		return 1;
	}

	int add_train(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		Train train;
		train.id = Scanner.gets();
		train.name = Scanner.gets();
		train.catalog = Scanner.getc();
		train.stationNum = Scanner.geti();
		train.priceNum = Scanner.geti();
		for (int i = 0; i < train.priceNum; ++i)
			train.priceName[i] = Scanner.gets();
		for (int i = 0; i < train.stationNum; ++i) {
			train.sta[i].name = Scanner.gets();
			train.sta[i].timeArrive = Scanner.gets();
			train.sta[i].timeStart = Scanner.gets();
			train.sta[i].timeStopover = Scanner.gets();
			train.sta[i].ta.set(train.sta[i].timeArrive);
			train.sta[i].ts.set(train.sta[i].timeStart);
			if (i > 0) {
				if (train.sta[i].ta < train.sta[i - 1].ts)
					train.sta[i].ta.date = train.sta[i - 1].ts.date + 1;
				else
					train.sta[i].ta.date = train.sta[i - 1].ts.date;
				if (train.sta[i].ts < train.sta[i].ta)
					train.sta[i].ts.date = train.sta[i].ta.date + 1;
				else
					train.sta[i].ts.date = train.sta[i].ta.date;
			}
			for (int j = 0; j < train.priceNum; ++j) {
				string str = Scanner.gets();
				while (str[0] < '0' || str[0] > '9')
					str.erase(0, 1);
				train.sta[i].price[j] = stod(str);
			}
		}
		return trainDB.insert(train.id, train);
	}

	int sale_train(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		tstring<20> trainID;
		trainID = Scanner.gets();
		auto tmp = trainDB.find(trainID);
		if (!tmp.second)
			return 0;
		if (tmp.first->isSaled) {
			delete tmp.first;
			return 0;
		}
		for (int i = 0; i < tmp.first->stationNum; ++i) {
			staDB.insert(tmp.first->sta[i].name, 1);
			for (int j = i + 1; j < tmp.first->stationNum; ++j) {
				tstring<100> str;
				str += (string)tmp.first->sta[i].name;
				str += (string)tmp.first->sta[j].name;
				str += (string)trainID;
				routeDB.insert(str, 1);
			}
		}
		tmp.first->isSaled = true;
		trainDB.modify(trainID, *(tmp.first));
		delete tmp.first;
		return 1;
	}

	char* query_train(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		tstring<20> trainID;
		trainID = Scanner.gets();
		auto tmp = trainDB.find(trainID);
		char *ret;
		if (!tmp.second) {
			ret = new char[2];
			ret[0] = '0';
			ret[1] = '\0';
			return ret;
		}
		if (!tmp.first->isSaled) {
			delete tmp.first;
			ret = new char[2];
			ret[0] = '0';
			ret[1] = '\0';
			return ret;
		}
		string str;
		str += (string)trainID;
		str += ' ';
		str += (string)tmp.first->name;
		str += ' ';
		str += tmp.first->catalog;
		str += ' ';
		str += to_string(tmp.first->stationNum);
		str += ' ';
		str += to_string(tmp.first->priceNum);
		for (int i = 0; i < tmp.first->priceNum; ++i) {
			str += ' ';
			str += tmp.first->priceName[i];
		}	
		for (int i = 0; i < tmp.first->stationNum; ++i) {
			str += '\n';
			str += (string)tmp.first->sta[i].name;
			str += ' ';
			str += (string)tmp.first->sta[i].timeArrive;
			str += ' ';
			str += (string)tmp.first->sta[i].timeStart;
			str += ' ';
			str += (string)tmp.first->sta[i].timeStopover;
			for (int j = 0; j < tmp.first->priceNum; ++j) {
				str += " $";
				str += to_string(tmp.first->sta[i].price[j]);
			}
		}
		int len = str.size();
		ret = new char[len + 1];
		for (int i = 0; i < len; ++i)
			ret[i] = str[i];
		ret[len] = '\0';
		delete tmp.first;
		return ret;
	}

	int delete_train(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		tstring<20> trainID;
		trainID = Scanner.gets();
		auto tmp = trainDB.find(trainID);
		if (!tmp.second)
			return 0;
		if (tmp.first->isSaled) {
			delete tmp.first;
			return 0;
		}
		trainDB.erase(trainID);
		delete tmp.first;
		return 1;
	}

	int modify_train(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		Train train;
		train.id = Scanner.gets();
		auto tmp = trainDB.find(train.id);
		if (!tmp.second)
			return 0;
		if (tmp.first->isSaled) {
			delete tmp.first;
			return 0;
		}
		train.name = Scanner.gets();
		train.catalog = Scanner.getc();
		train.stationNum = Scanner.geti();
		train.priceNum = Scanner.geti();
		for (int i = 0; i < train.priceNum; ++i)
			train.priceName[i] = Scanner.gets();
		bool flag = false;
		for (int i = 0; i < train.stationNum; ++i) {
			train.sta[i].name = Scanner.gets();
			train.sta[i].timeArrive = Scanner.gets();
			train.sta[i].timeStart = Scanner.gets();
			train.sta[i].timeStopover = Scanner.gets();
			train.sta[i].ta.set(train.sta[i].timeArrive);
			train.sta[i].ts.set(train.sta[i].timeStart);
			if (i > 0) {
				if (train.sta[i].ta < train.sta[i - 1].ts)
					train.sta[i].ta.date = train.sta[i - 1].ts.date + 1;
				else
					train.sta[i].ta.date = train.sta[i - 1].ts.date;
				if (train.sta[i].ts < train.sta[i].ta)
					train.sta[i].ts.date = train.sta[i].ta.date + 1;
				else
					train.sta[i].ts.date = train.sta[i].ta.date;
			}
			for (int j = 0; j < train.priceNum; ++j) {
				string str = Scanner.gets();
				while (str[0] < '0' || str[0] > '9')
					str.erase(0, 1);
				train.sta[i].price[j] = stod(str);
			}
		}
		delete tmp.first;
		return trainDB.modify(train.id, train);;
	}

	char* query_ticket(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		tstring<20> loc1, loc2;
		loc1 = Scanner.gets();
		loc2 = Scanner.gets();
		tstring<10> date, cata;
		date = Scanner.gets();
		cata = Scanner.gets();
		tstring<100> route1, route2;
		route1 += (string)loc1;
		route1 += (string)loc2;
		route2 += (string)loc1;
		route2 += (string)loc2;
		tstring<20> trainID;
		trainID.len = 20;
		for (int i = 0; i < 20; ++i)
			trainID.m[i] = -128;
		route1 += (string)trainID;
		for (int i = 0; i < 20; ++i)
			trainID.m[i] = 127;
		route2 += (string)trainID;
		sjtu::vector<tstring<100>> v;
		routeDB.findRange(route1, route2, v);
		char *ret;
		if (!v.size()) {
			ret = new char[2];
			ret[0] = '0';
			ret[1] = '\0';
			return ret;
		}
		string ans;
		int cnt = 0;
		for (int i = 0; i < v.size(); ++i) {
			string str;
			str += '\n';
			for (int j = loc1.len + loc2.len; j < v[i].len; ++j)
				trainID.m[j - loc1.len - loc2.len] = v[i].m[j];
			trainID.len = v[i].len - loc1.len - loc2.len;
			auto tmp = trainDB.find(trainID);
			if (!tmp.second)
				continue;
			bool flag = false;
			for(int j = 0; j < cata.len; ++j)
				if (tmp.first->catalog == cata.m[j]) {
					flag = true;
					break;
				}
			if (!flag) {
				delete tmp.first;
				continue;
			}
			int from, to;
			for (int j = 0; j < tmp.first->stationNum; ++j) {
				if (tmp.first->sta[j].name == loc1) {
					from = j;
					str += (string)trainID;
					str += ' ';
					str += (string)loc1;
					str += ' ';
					if (tmp.first->sta[j].ts.date) {
						mydate md;
						md.set(date);
						++md;
						str += md.tos();
					}
					else
						str += (string)date;
					str += ' ';
					str += (string)tmp.first->sta[j].timeStart;
				}
				if (tmp.first->sta[j].name == loc2) {
					to = j;
					str += ' ';
					str += (string)loc2;
					str += ' ';
					if (tmp.first->sta[j].ta.date) {
						mydate md;
						md.set(date);
						++md;
						str += md.tos();
					}
					else
						str += (string)date;
					str += ' ';
					str += (string)tmp.first->sta[j].timeArrive;
					break;
				}
			}
			for (int j = 0; j < tmp.first->priceNum; ++j) {
				str += ' ';
				str += (string)tmp.first->priceName[j];
				tstring<30> t;
				t += (string)tmp.first->id;
				t += (string)date;
				auto tmp2 = ticket_leftDB.find(t);
				int tic_left = 2000;
				if (!tmp2.second) {
					tic tt;
					ticket_leftDB.insert(t, tt);
				}
				else {
					for (int k = from + 1; k <= to; ++k)
						if (tmp2.first->a[j][k] < tic_left)
							tic_left = tmp2.first->a[j][k];
					delete tmp2.first;
				}
				str += ' ';
				str += to_string(tic_left);
				str += ' ';
				double price = 0;
				for (int k = from + 1; k <= to; ++k)
					price += tmp.first->sta[k].price[j];
				str += to_string(price);
			}
			delete tmp.first;
			++cnt;
			ans += str;
		}
		ans = to_string(cnt) + ans;
		int len = ans.size();
		ret = new char[len + 1];
		for (int i = 0; i < len; ++i)
			ret[i] = ans[i];
		ret[len] = '\0';
		return ret;
	}

	mypair<mytime, tstring<20>> qtt(tstring<20> &loc1, tstring<20> &loc2, tstring<10> &date, tstring<10> &cata, mytime &mt)
	{
		tstring<60> route1, route2;
		route1 += loc1;
		route1 += loc2;
		route2 += loc1;
		route2 += loc2;
		tstring<20> trainID;
		trainID.len = 20;
		for (int i = 0; i < 20; ++i)
			trainID.m[i] = -128;
		route1 += trainID;
		for (int i = 0; i < 20; ++i)
			trainID.m[i] = 127;
		route2 += trainID;
		sjtu::vector<tstring<100>> v;
		routeDB.findRange(route1, route2, v);
		tstring<20> ansid;
		mytime ans;
		ans.date = 100;
		for (int i = 0; i < v.size(); ++i) {
			for (int j = loc1.len + loc2.len; j < v[i].len; ++j)
				trainID.m[j - loc1.len - loc2.len] = v[i].m[j];
			trainID.len = v[i].len - loc1.len - loc2.len;
			auto tmp = trainDB.find(trainID);
			if (!tmp.second)
				continue;
			bool flag = false;
			for(int j = 0; j < cata.len; ++j)
				if (cata[j] == tmp.first->catalog) {
					flag = true;
					break;
				}
			if (!flag)
				continue;
			mytime t;
			t = mt;
			for (int j = 0; j < tmp.first->stationNum; ++j) {
				if (tmp.first->sta[j].name == loc1 && tmp.first->sta[j].ts < mt)
					++t.date;
				if (tmp.first->sta[j].name == loc2) {
					t.hour = tmp.first->sta[j].ta.hour;
					t.minute = tmp.first->sta[j].ta.minute;
					t.date += tmp.first->sta[j].ta.date;
					break;
				}
			}
			if (t < ans) {
				ans = t;
				ansid = trainID;
			}
			delete tmp.first;
		}
		return mypair<mytime, tstring<20>>(ans, ansid);
	}

	char* query_transfer(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		tstring<20> loc1, loc2;
		loc1 = Scanner.gets();
		loc2 = Scanner.gets();
		tstring<10> date, cata;
		date = Scanner.gets();
		cata = Scanner.gets();
		tstring<20> lloc, rloc;
		for (int i = 0; i < 20; ++i) {
			lloc.m[i] = -128;
			rloc.m[i] = 127;
		}
		lloc.len = rloc.len = 20;
		sjtu::vector<tstring<20>> v;
		staDB.findRange(lloc, rloc, v);
		tstring<20> id1, id2, loc;
		mytime ans;
		ans.date = 100;
		for (int i = 0; i < v.size(); ++i) {
			mytime tmp;
			auto tmp1 = qtt(loc1, v[i], date, cata, tmp);
			auto tmp2 = qtt(v[i], loc2, date, cata, tmp1.first);
			if (tmp2.first < ans) {
				ans = tmp2.first;
				id1 = tmp1.second;
				id2 = tmp2.second;
				loc = v[i];
			}
		}
		char *ret;
		if (ans.date == 100) {
			ret = new char[3];
			ret[0] = '-';
			ret[1] = '1';
			ret[2] = '\0';
			return ret;
		}
		string str;
		auto res = trainDB.find(id1);
		
		int from, to;
		mytime foo;
		for (int i = 0; i < res.first->stationNum; ++i) {
			if (res.first->sta[i].name == loc1) {
				from = i;
				str += (string)id1;
				str += ' ';
				str += (string)loc1;
				str += ' ';
				if (res.first->sta[i].ts.date) {
					mydate md;
					md.set(date);
					++md;
					str += md.tos();
				}
				else
					str += (string)date;
				str += ' ';
				str += (string)res.first->sta[i].timeStart;
			}
			if (res.first->sta[i].name == loc) {
				to = i;
				str += ' ';
				str += loc;
				str += ' ';
				if (res.first->sta[i].ta.date) {
					mydate md;
					md.set(date);
					++md;
					str += md.tos();
				}
				else
					str += (string)date;
				str += ' ';
				str += res.first->sta[i].timeArrive;
				foo = res.first->sta[i].ta;
				break;
			}
		}
		for (int i = 0; i < res.first->priceNum; ++i) {
			str += ' ';
			str += (string)res.first->priceName[i];
			str += ' ';
			tstring<30> t;
			t += res.first->id;
			t += date;
			auto res2 = ticket_leftDB.find(t);
			int ticket_left = 2000;
			if (!res2.second) {
				tic tt;
				ticket_leftDB.insert(t, tt);
			}
			else {
				for (int j = from + 1; j <= to; ++j)
					if (res2.first->a[i][j] < ticket_left)
						ticket_left = res2.first->a[i][j];
				delete res2.first;
			}
			str += to_string(ticket_left);
			str += ' ';
			double price = 0;
			for (int j = from + 1; j <= to; ++j)
				price += res.first->sta[j].price[i];
			str += to_string(price);
		}
		str += '\n';
		res = trainDB.find(id2);
		for (int i = 0;i < res.first->stationNum; ++i) {
			if (res.first->sta[i].name == loc) {
				from = i;
				str += (string)id2;
				str += ' ';
				str += (string)loc;
				str += ' ';
				if (res.first->sta[i].ts < foo)
					++res.first->sta[i].ts.date;
				if (res.first->sta[i].ts.date) {
					mydate md;
					md.set(date);
					++md;
					str += md.tos();
				}
				else
					str += (string)date;
				str += ' ';
				str += (string)res.first->sta[i].timeStart;
			}
			if (res.first->sta[i].name == loc2) {
				to = i;
				str += ' ';
				str += loc;
				str += ' ';
				if (res.first->sta[i].ts < foo)
					++res.first->sta[i].ts.date;
				if (res.first->sta[i].ta.date) {
					mydate md;
					md.set(date);
					++md;
					str += md.tos();
				}
				else
					str += (string)date;
				str += ' ';
				str += res.first->sta[i].timeArrive;
				break;
			}
		}
		for (int i = 0; i < res.first->priceNum; ++i) {
			str += ' ';
			str += (string)res.first->priceName[i];
			str += ' ';
			tstring<30> t;
			t += res.first->id;
			t += date;
			auto res2 = ticket_leftDB.find(t);
			int ticket_left = 2000;
			if (!res2.second) {
				tic tt;
				ticket_leftDB.insert(t, tt);
			}
			else {
				for (int j = from + 1; j <= to; ++j)
					if (res2.first->a[i][j] < ticket_left)
						ticket_left = res2.first->a[i][j];
				delete res2.first;
			}
			str += to_string(ticket_left);
			str += ' ';
			double price = 0;
			for (int j = from + 1; j <= to; ++j)
				price += res.first->sta[j].price[i];
			str += to_string(price);
		}
		int len = str.size();
		ret = new char[len + 1];
		for (int i = 0; i < len; ++i)
			ret[i] = str[i];
		ret[len] = '\0';
		return ret;
	}

	int buy_ticket(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		int id, num;
		id = Scanner.geti();
		num = Scanner.geti();
		tstring<20> trainID;
		trainID = Scanner.gets();
		tstring<40> loc1, loc2;
		loc1 = Scanner.gets();
		loc2 = Scanner.gets();
		tstring<10> date;
		date = Scanner.gets();
		tstring<20> ticket_kind;
		ticket_kind = Scanner.gets();
		auto tmp1 = userDB.find(id);
		if (!tmp1.second)
			return 0;
		delete tmp1.first;
		auto tmp2 = trainDB.find(trainID);
		if (!tmp2.second)
			return 0;
		int from = -1, to = -1, t = -1;
		for (int i = 0; i < tmp2.first->stationNum; ++i) {
			if (tmp2.first->sta[i].name == loc1)
				from = i;
			if (tmp2.first->sta[i].name == loc2)
				to = i;
		}
		for(int i = 0; i < tmp2.first->priceNum; ++i)
			if (tmp2.first->priceName[i] == ticket_kind) {
				t = i;
				break;
			}
		if (from == -1 || to == -1 || from >= to || t == -1) {
			delete tmp2.first;
			return 0;
		}
		tstring<30> str;
		str += (string)trainID;
		str += (string)date;
		auto tmp3 = ticket_leftDB.find(str);
		if (!tmp3.second) {
			tic tmp;
			ticket_leftDB.insert(str, tmp);
			tmp3 = ticket_leftDB.find(str);
		}
		int left = 2000;
		for (int i = from + 1; i <= to; ++i)
			if (tmp3.first->a[t][i] < left)
				left = tmp3.first->a[t][i];
		if (left < num) {
			delete tmp2.first;
			delete tmp3.first;
			return 0;
		}
		for (int i = from + 1; i <= to; ++i)
			tmp3.first->a[t][i] -= num;
		ticket_leftDB.modify(str, *(tmp3.first));
		tstring<40> ts, foo;
		auto tmp4 = ticketDB.find(foo);
		ts += to_string(id);
		ts += (string)date;
		ts += tmp2.first->catalog;
		int cnt = ++tmp4.first->ticketID;
		ticketDB.modify(foo, *(tmp4.first));
		ts += fill_to(to_string(cnt), 9);
		Ticket ticket;
		ticket.trainID = tmp2.first->id;
		ticket.ticketNum = num;
		ticket.date = date;
		ticket.start = from;
		ticket.end = to;
		ticket.kind = t;
		ticket.ticketID = cnt;
		ticketDB.insert(ts, ticket);
		delete tmp2.first;
		delete tmp3.first;
		delete tmp4.first;
		return 1;
	}

	char* query_order(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		int id = Scanner.geti();
		tstring<10> date;
		date = Scanner.gets();
		char cata = Scanner.getc();
		tstring<40> t1, t2;
		t1 += to_string(id);
		t1 += (string)date;
		t1 += cata;
		t2 += to_string(id);
		t2 += (string)date;
		t2 += cata;
		t1 += fill_to(to_string(0), 9);
		t2 += fill_to(to_string(1e9 - 1), 9);
		sjtu::vector<Ticket> v;
		ticketDB.findRangeData(t1, t2, v);
		char *ret;
		if (!v.size()) {
			ret = new char[2];
			ret[0] = '0';
			ret[1] = '\0';
			return ret;
		}
		string str;
		str += to_string(v.size());
		for (int i = 0; i < v.size(); ++i) {
			str += '\n';
			auto tmp = trainDB.find(v[i].trainID);
			str += (string)v[i].trainID;
			str += ' ';
			str += (string)tmp.first->sta[v[i].start].name;
			str += ' ';
			str += (string)v[i].date;
			str += ' ';
			str += (string)tmp.first->sta[v[i].start].timeStart;
			str += ' ';
			str += (string)tmp.first->sta[v[i].end].name;
			str += ' ';
			str += (string)v[i].date;
			str += ' ';
			str += (string)tmp.first->sta[v[i].end].timeArrive;
			for (int j = 0; j < tmp.first->priceNum; ++j) {
				str += ' ';
				str += (string)tmp.first->priceName[j];
				str += ' ';
				if (j == v[i].kind)
					str += to_string(v[i].ticketNum);
				else
					str += '0';
				str += ' ';
				double price = 0;
				for (int k = v[i].start + 1; k <= v[i].end; ++k)
					price += tmp.first->sta[k].price[j];
				str += to_string(price);
			}
			delete tmp.first;
		}
		int len = str.size();
		ret = new char[len + 1];
		for (int i = 0; i < len; ++i)
			ret[i] = str[i];
		ret[len] = '\0';
		return ret;
	}

	int refund_ticket(char *s = nullptr)
	{
		scanner Scanner;
		Scanner.set(s);
		int id = Scanner.geti(), num = Scanner.geti();
		tstring<20> trainID, ticket_kind;
		trainID = Scanner.gets();
		tstring<40> loc1, loc2;
		loc1 = Scanner.gets();
		loc2 = Scanner.gets();
		tstring<10> date;
		date = Scanner.gets();
		ticket_kind = Scanner.gets();
		auto tmp = trainDB.find(trainID);
		if (!tmp.second)
			return 0;
		char cata = tmp.first->catalog;
		tstring<40> ticket1, ticket2;
		ticket1 += to_string(id);
		ticket1 += date;
		ticket1 += cata;
		ticket2 += to_string(id);
		ticket2 += date;
		ticket2 += cata;
		ticket1 += fill_to(to_string(0), 9);
		ticket2 += fill_to(to_string(1e9 - 1), 9);
		sjtu::vector<Ticket> v;
		ticketDB.findRangeData(ticket1, ticket2, v);
		if (!v.size()) {
			delete tmp.first;
			return 0;
		}
		int cnt = 0;
		for (int i = 0; i < v.size(); ++i)
			if (v[i].trainID == trainID && tmp.first->sta[v[i].start].name == loc1 && tmp.first->sta[v[i].end].name == loc2 && tmp.first->priceName[v[i].kind] == ticket_kind)
				cnt += v[i].ticketNum;
		if (num > cnt) {
			delete tmp.first;
			return 0;
		}
		for (int i = 0; i < v.size(); ++i) {
			if (v[i].trainID == trainID && tmp.first->sta[v[i].start].name == loc1 && tmp.first->sta[v[i].end].name == loc2 && tmp.first->priceName[v[i].kind] == ticket_kind) {
				if (num >= v[i].ticketNum) {
					tstring<40> ticket;
					ticket += to_string(id);
					ticket += date;
					ticket += cata;
					ticket += fill_to(to_string(v[i].ticketID), 9);
					num -= v[i].ticketNum;
					ticketDB.erase(ticket);
					tstring<30> t;
					t += tmp.first->id;
					t += date;
					auto tt = ticket_leftDB.find(t);
					for (int j = v[i].start + 1; j <= v[i].end; ++j)
						tt.first->a[v[i].kind][j] += v[i].ticketNum;
					ticket_leftDB.modify(t, *(tt.first));
					delete tt.first;
				}
				else {
					tstring<40> ticket;
					ticket += to_string(id);
					ticket += (string)date;
					ticket += cata;
					ticket += fill_to(to_string(v[i].ticketID), 9);
					v[i].ticketNum -= num;
					ticketDB.modify(ticket, v[i]);
					tstring<30> t;
					t += tmp.first->id;
					t += date;
					auto tt = ticket_leftDB.find(t);
					for (int j = v[i].start + 1; j <= v[i].end; ++j)
						tt.first->a[v[i].kind][j] += num;
					ticket_leftDB.modify(t, *(tt.first));
					delete tt.first;
					num = 0;
				}
				if (num == 0)
					break;
			}
		}
		delete tmp.first;
		return 1;
	}

	void clean()
	{
		userDB.trunc();
		ticketDB.trunc();
		trainDB.trunc();
		ticket_leftDB.trunc();
		routeDB.trunc();
		staDB.trunc();
	}
};
#endif
