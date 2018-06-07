#include <iostream>
#include <cstdio>
#include <cstring>
#include "backend.hpp"
bool equal(const char *a, const char *b)
{
	if (strlen(a) != strlen(b))
		return false;
	for (int i = 0; i < strlen(a); ++i)
		if (a[i] != b[i])
			return false;
	return true;
}
int main()
{
	backend tmp;
	tmp.init();
	char op[50];
	while (true) {
		scanf("%s", op);
		if (equal(op, "register"))
			printf("%d\n", tmp.regist());
		else if (equal(op, "login"))
			printf("%d\n", tmp.login());
		else if (equal(op, "query_profile")) {
			int x;
			scanf("%d", &x);
			printf("%s\n", tmp.query_profile(x));
		}
		else if (equal(op, "modify_profile"))
			printf("%d\n", tmp.modify_profile());
		else if (equal(op, "modify_privilege")) {
			int id1, id2, pri;
            		scanf("%d%d%d", &id1, &id2, &pri);
			printf("%d\n", tmp.modify_privilege(id1, id2, pri));
		}
		else if (equal(op, "add_train"))
			printf("%d\n", tmp.add_train());
		else if (equal(op, "modify_train"))
			printf("%d\n", tmp.modify_train());
		else if (equal(op, "sale_train"))
			printf("%d\n", tmp.sale_train());
		else if (equal(op, "delete_train"))
			printf("%d\n", tmp.delete_train());
		else if (equal(op, "query_train"))
			printf("%s\n", tmp.query_train());
		else if (equal(op, "query_ticket")) {
			char *s = tmp.query_ticket();
			//printf("%s\n", s);
			int len = strlen(s);
			for (int i = 0; i < len; ++i)
				printf("%c", s[i]);
			printf("\n");
		}
		else if (equal(op, "buy_ticket"))
			printf("%d\n", tmp.buy_ticket());
		else if (equal(op, "query_order"))
			printf("%s\n", tmp.query_order());
		else if (equal(op, "refund_ticket"))
			printf("%d\n", tmp.refund_ticket());
		else if (equal(op, "query_transfer"))
			printf("%s\n", tmp.query_transfer());
		else if (equal(op, "clean")) {
			tmp.clean();
			printf("1\n");
		}
		else if (equal(op, "exit")) {
			printf("BYE\n");
			break;
		}
	}
	return 0;
}
