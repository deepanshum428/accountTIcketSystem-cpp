
#pragma once
#include "account.h"
#include "ticket.h"

struct AccountTicketingData
{
    struct Account *accounts;   // array of accounts
    const int ACCOUNT_MAX_SIZE; // maximum elements for account array
    struct Ticket *tickets;     // array of tickets
    const int TICKET_MAX_SIZE;  // maximum elements for ticket array
};

void displayAccountDetailHeader();
void displayAccountDetailRecord(const struct Account*);

void applicationStartup(struct AccountTicketingData *data);
int menuLogin(const struct Account a[], int);
void menuAgent(struct AccountTicketingData *, const struct Account *);
int findAccountIndexByAcctNum(int, const struct Account*, int, int);
void displayAllAccountRecords(const struct Account*, int);
void pauseExecution();
