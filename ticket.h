#pragma once

struct Message
{
    char AccountType;
    char name[35];
    char details[150];
};

struct Ticket
{
    int ticketNumber;
    int accountNum;
    int ticketStatus;
    char ticketSubject[31];
    int messageCount;
    struct Message messages[20];
};

void newTickets(struct Ticket *, int, const struct Account *, int);

void activeTickets(struct Ticket *, int);

void closedTickets(struct Ticket *, int);

void allTickets(struct Ticket *, int);

void manageTickets(struct Ticket *, int, const struct Account *);

int loadTickets(struct Ticket tickets[], int);

int saveTickets(struct Ticket tickets[], int);
