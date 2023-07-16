#pragma once

struct Person {
	char name[31];
	int year;
	double income;
	char country[31];
};

struct UserLogin {
	char userName[11];
	char password[9];
};

struct Account {
	int acctNum;
	char type;

	struct Person person;
	struct UserLogin login;
};

void getAccount(struct Account*);
void getPerson(struct Person*);
void getUserLogin(struct UserLogin*);

void updateAccount(struct Account*);
void updatePerson(struct Person* );
void updateUserLogin(struct UserLogin*);

int loadAccounts(struct Account accounts[], int);
int saveAccounts(struct Account accounts[], int);
