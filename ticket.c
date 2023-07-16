#include <stdio.h>
#include "account.h"
#include "ticket.h"
#include "commonHelpers.h"
#include "accountTicketingUI.h"

/* 
	This method create a new ticket for provided customer's account number if the size of the ticket array is not full
 */
void newTickets(struct Ticket *tickets, int ticket_max, const struct Account *accounts, int account_max)
{
	int i, max_ticket_num = 0;
	char confirm;
	for (i = 0; i < ticket_max; i++)
		if (max_ticket_num < tickets[i].ticketNumber)
			max_ticket_num = tickets[i].ticketNumber;
	for (i = 0; i < ticket_max && tickets[i].ticketNumber != 0; i++) // checking is there any blank ticket position
	{
	}
	if (i >= ticket_max)
	{
		printf("\nERROR: Ticket listing is FULL, call ITS Support!");
		return;
	}
	printf("\nEnter the account#: ");
	tickets[i].accountNum = getInteger();
	int res = findAccountIndexByAcctNum(tickets[i].accountNum, accounts, account_max, 0);
	if (res == -1)
	{
		printf("\nERROR: Invalid account number!\n\n");
		tickets[i].accountNum = 0;
		return;
	}
	if (accounts[res].type == 'A')
	{
		printf("\nERROR: Agent accounts can't have tickets!\n");
		tickets[i].accountNum = 0;
		return;
	}
	printf("\n");
	displayAccountDetailHeader();
	displayAccountDetailRecord(&accounts[res]);
	printf("\nAdd a new ticket for this customer? ([Y]es|[N]o): ");
	confirm = getCharOption("YyNn");
	if (confirm == 'Y' || confirm == 'y')
	{

		tickets[i].ticketNumber = max_ticket_num + 1;
		printf("\nNew Ticket (Ticket#:%06d)\n", tickets[i].ticketNumber);
		printf("----------------------------------------\n");
		printf("Enter the ticket SUBJECT (30 chars. maximum): ");
		getCString(tickets[i].ticketSubject, 1, 30);
		printf("\nEnter the ticket message details (150 chars. maximum). Press the ENTER key to submit:\n");
		getCString(tickets[i].messages[0].details, 1, 150);

		printf("\n*** New ticket created! ***\n");

		strcpy(tickets[i].messages[0].name, accounts[res].person.name);
		tickets[i].messages[0].AccountType = 'C';
		tickets[i].messageCount = 1;
		tickets[i].ticketStatus = 1;
	}
}

/* 
	displays active new tickets those have status and message count 1.
	And prompt for ticket number to view messages
 */
void activeTickets(struct Ticket *tickets, int ticket_max)
{
	int ticketNum, i, found = 0;
	printf("\n");
	do
	{
		printf("------ ----- --------------- ------ ------------------------------ --------\n");
		printf("Ticket Acct# Full Name       Status Subject                        Messages\n");
		printf("------ ----- --------------- ------ ------------------------------ --------\n");
		for (i = 0; i < ticket_max; i++)
		{
			if (tickets[i].ticketNumber != 0 && tickets[i].ticketStatus == 1)
			{
				printf("%06d %d %-16s%s %-34s%2d\n", tickets[i].ticketNumber, tickets[i].accountNum, tickets[i].messages[0].name, "ACTIVE", tickets[i].ticketSubject, tickets[i].messageCount);
			}
		}
		printf("------ ----- --------------- ------ ------------------------------ --------\n");
		printf("\n");
		printf("Enter the ticket number to view the messages or\n0 to return to previous menu: ");
		ticketNum = getInteger();
		if (ticketNum != 0)
		{
			found = 0;
			for (i = 0; i < ticket_max; i++)
			{
				if (tickets[i].ticketNumber == ticketNum && tickets[i].ticketStatus == 1)
				{
					found = 1;
					printf("\n================================================================================\n");
					printf("%06d (%s) Re: %s\n", tickets[i].ticketNumber, "ACTIVE", tickets[i].ticketSubject);
					printf("================================================================================\n");
					for (int j = 0; j < tickets[i].messageCount; j++)
					{
						printf("%s (%s):\n", tickets[i].messages[j].AccountType == 'A' ? "AGENT" : "CUSTOMER", tickets[i].messages[j].name);
						printf("   %s\n\n", tickets[i].messages[j].details);
					}
				}
			}
			if (found == 0)
				printf("\nERROR: Invalid ticket number.\n\n");
			pauseExecution();
		}
	} while (ticketNum != 0);
}

/*
	displays closed tickets those have status 0.
	And prompt for ticket number to view messages
 */
void closedTickets(struct Ticket *tickets, int ticket_max)
{
	int ticketNum, i, found = 0;
	printf("\n");
	do
	{
		printf("------ ----- --------------- ------ ------------------------------ --------\n");
		printf("Ticket Acct# Full Name       Status Subject                        Messages\n");
		printf("------ ----- --------------- ------ ------------------------------ --------\n");
		for (int i = 0; i < ticket_max; i++)
		{
			if (tickets[i].ticketNumber != 0 && tickets[i].ticketStatus == 0)
			{
				printf("%06d %d %-16s%s %-34s%2d\n", tickets[i].ticketNumber, tickets[i].accountNum, tickets[i].messages[0].name, tickets[i].ticketStatus ? "ACTIVE" : "CLOSED", tickets[i].ticketSubject, tickets[i].messageCount);
			}
		}
		printf("------ ----- --------------- ------ ------------------------------ --------\n");
		printf("\n");
		printf("Enter the ticket number to view the messages or\n0 to return to previous menu: ");
		ticketNum = getInteger();
		if (ticketNum != 0)
		{
			found = 0;
			for (i = 0; i < ticket_max; i++)
			{
				if (tickets[i].ticketNumber == ticketNum && tickets[i].ticketStatus == 0)
				{
					found = 1;
					printf("\n================================================================================\n");
					printf("%06d (%s) Re: %s\n", tickets[i].ticketNumber, "ACTIVE", tickets[i].ticketSubject);
					printf("================================================================================\n");
					for (int j = 0; j < tickets[i].messageCount; j++)
					{
						printf("%s (%s):\n", tickets[i].messages[j].AccountType == 'A' ? "AGENT" : "CUSTOMER", tickets[i].messages[j].name);
						printf("   %s\n\n", tickets[i].messages[j].details);
					}
				}
			}
			if (found == 0)
				printf("\nERROR: Invalid ticket number.\n\n");
			pauseExecution();
		}
	} while (ticketNum != 0);
}

/*
	displays all tickets if ticket number is not 0.
	And prompt for ticket number to view messages
 */
void allTickets(struct Ticket *tickets, int ticket_max)
{
	int ticketNum, i, found = 0;
	printf("\n");
	do
	{
		printf("------ ----- --------------- ------ ------------------------------ --------\n");
		printf("Ticket Acct# Full Name       Status Subject                        Messages\n");
		printf("------ ----- --------------- ------ ------------------------------ --------\n");
		for (int i = 0; i < ticket_max; i++)
		{
			if (tickets[i].ticketNumber != 0 && tickets[i].ticketStatus == 1 && tickets[i].messageCount == 1)
			{
				printf("%06d %d %-16s%s %-34s%2d\n", tickets[i].ticketNumber, tickets[i].accountNum, tickets[i].messages[0].name, tickets[i].ticketStatus == 1 ? "ACTIVE" : "CLOSED", tickets[i].ticketSubject, tickets[i].messageCount);
			}
		}
		printf("------ ----- --------------- ------ ------------------------------ --------\n");
		printf("\n");
		printf("Enter the ticket number to view the messages or\n0 to return to previous menu: ");
		ticketNum = getInteger();
		if (ticketNum != 0)
		{
			found = 0;
			for (i = 0; i < ticket_max; i++)
			{
				if (tickets[i].ticketNumber == ticketNum && tickets[i].ticketStatus == 1 && tickets[i].messageCount == 1)
				{
					found = 1;
					printf("\n================================================================================\n");
					printf("%06d (%s) Re: %s\n", tickets[i].ticketNumber, "ACTIVE", tickets[i].ticketSubject);
					printf("================================================================================\n");
					for (int j = 0; j < tickets[i].messageCount; j++)
					{
						printf("%s (%s):\n", tickets[i].messages[j].AccountType == 'A' ? "AGENT" : "CUSTOMER", tickets[i].messages[j].name);
						printf("   %s\n\n", tickets[i].messages[j].details);
					}
				}
			}
			if (found == 0)
			{
				printf("\nERROR: Invalid ticket number.\n\n");
			}
			pauseExecution();
		}
	} while (ticketNum != 0);
}

/*
	Manage all tickets.
	And prompt for ticket number to add messages, change ticket status closed to active or active to closed
 */
void manageTickets(struct Ticket *tickets, int ticket_max, const struct Account *account)
{
	printf("\nEnter ticket number: ");
	int tempTicketNum = getInteger();
	int i, option;
	char str[151];
	for (i = 0; i < ticket_max && tickets[i].ticketNumber != tempTicketNum; i++)
	{
	}
	if (i >= ticket_max)
	{
		printf("\nERROR: Invalid ticket number.\n\n");
		pauseExecution();
		return;
	}
	do
	{
		printf("\n----------------------------------------\n");
		printf("Ticket %06d - Update Options", tickets[i].ticketNumber);
		printf("\n----------------------------------------");
		printf("\nStatus  : %s", tickets[i].ticketStatus == 1 ? "ACTIVE" : "CLOSED");
		printf("\nSubject : %s", tickets[i].ticketSubject);
		printf("\nAcct#   : %d", tickets[i].accountNum);
		printf("\nCustomer: %s", tickets[i].messages[0].name);
		printf("\n----------------------------------------\n");

		printf("1) Add CUSTOMER message\n");
		printf("2) Add AGENT message\n");
		printf("3) Close ticket\n");
		printf("4) Re-activate ticket\n");
		printf("0) Done\n");
		printf("Selection: ");
		option = getIntFromRange(0, 4);

		switch (option)
		{
		case 1:
			if (tickets[i].ticketStatus == 1)
			{
				if (tickets[i].messageCount < 20)
				{
					printf("Enter the ticket message details (150 chars. maximum). Press the ENTER key to submit:\n");
					getCString(str, 2, 150);
					strcpy(tickets[i].messages[tickets[i].messageCount].details, str);
					strcpy(tickets[i].messages[tickets[i].messageCount].name, tickets[i].messages[0].name);
					tickets[i].messages[tickets[i].messageCount].AccountType = 'C';
					tickets[i].messageCount++;
				}
				else
				{
					printf("ERROR: Message limit has been reached, call ITS Support!");
				}
			}
			else
			{
				printf("ERROR: Ticket is closed - new messages are not permitted.");
			}

			break;

		case 2:
			if (tickets[i].ticketStatus == 1)
			{
				if (tickets[i].messageCount < 20)
				{
					printf("Enter the ticket message details (150 chars. maximum). Press the ENTER key to submit:\n");
					getCString(str, 2, 150);
					strcpy(tickets[i].messages[tickets[i].messageCount].details, str);
					strcpy(tickets[i].messages[tickets[i].messageCount].name, account->person.name);
					tickets[i].messages[tickets[i].messageCount].AccountType = 'A';
					tickets[i].messageCount++;
				}
				else
				{
					printf("ERROR: Message limit has been reached, call ITS Support!");
				}
			}
			else
			{
				printf("ERROR: Ticket is closed - new messages are not permitted.");
			}
			break;

		case 3:
			if (tickets[i].ticketStatus == 1)
			{
				printf("\nAre you sure you want to Close? ([Y]es|[N]o): ");
				str[0] = getCharOption("YyNn");
				if (str[0] == 'Y' || str[0] == 'y')
				{
					tickets[i].ticketStatus = 0;
					printf("\n*** Ticket closed! ***\n");
				}
			}
			else
			{
				printf("\nERROR: Ticket is already closed!\n");
			}
			break;

		case 4:
			if (tickets[i].ticketStatus == 0)
			{
				printf("\nAre you sure you want to RE-ACTIVATE this closed ticket? ([Y]es|[N]o): ");
				str[0] = getCharOption("YyNn");
				if (str[0] == 'Y' || str[0] == 'y')
				{
					tickets[i].ticketStatus = 1;
					printf("\n*** Ticket re-activated! ***\n");
				}
			}
			else
			{
				printf("\nERROR: Ticket is already active!\n");
			}
			break;
		}
	} while (option != 0);
	printf("\n");
}

/*
	load tickets data from tickets.txt file to AccountTicketingData's ticket member variable
 */
int loadTickets(struct Ticket tickets[], int tickets_size)
{
	FILE *ticketFile = fopen("tickets.txt", "r");
	char end = 0;
	int ticketsCount = 0;
	if (ticketFile)
	{
		for (int i = 0; i < tickets_size && end != EOF; i++) // geting ticket data of size (ticket_size)
		{
			char pipe = 0;
			fscanf(ticketFile, "%d", &tickets[i].ticketNumber);
			fscanf(ticketFile, "%c", &pipe);

			fscanf(ticketFile, "%d", &tickets[i].accountNum);
			fscanf(ticketFile, "%c", &pipe);

			fscanf(ticketFile, "%d", &tickets[i].ticketStatus);
			fscanf(ticketFile, "%c", &pipe);

			fscanf(ticketFile, "%[^|]s", tickets[i].ticketSubject);
			fscanf(ticketFile, "%c", &pipe);

			fscanf(ticketFile, "%d", &tickets[i].messageCount);
			fscanf(ticketFile, "%c", &pipe);

			for (int j = 0; j < tickets[i].messageCount; j++)
			{
				fscanf(ticketFile, "%c", &tickets[i].messages[j].AccountType);
				fscanf(ticketFile, "%c", &pipe);

				fscanf(ticketFile, "%[^|]s", tickets[i].messages[j].name);
				fscanf(ticketFile, "%c", &pipe);

				fscanf(ticketFile, "%[^|]s", tickets[i].messages[j].details);
				fscanf(ticketFile, "%c%c", &pipe, &end);
			}
			if (pipe == '|') // increment ticketCount if line is not blank
				ticketsCount++;
		}
	}
	fclose(ticketFile);
	return ticketsCount;
}

/*
	save ticket data to tickets.txt file
 */
int saveTickets(struct Ticket tickets[], int tickets_size)
{
	FILE *ticketFile = fopen("tickets.txt", "w");
	int ticketsCount = 0;
	if (ticketFile)
	{
		for (int i = 0; i < tickets_size; i++) // saving ticket data of size (ticket_size)
		{
			if (tickets[i].ticketNumber != 0)
			{
				char pipe = '|';
				fprintf(ticketFile, "%d", tickets[i].ticketNumber);
				fprintf(ticketFile, "%c", pipe);

				fprintf(ticketFile, "%d", tickets[i].accountNum);
				fprintf(ticketFile, "%c", pipe);

				fprintf(ticketFile, "%d", tickets[i].ticketStatus);
				fprintf(ticketFile, "%c", pipe);

				fprintf(ticketFile, "%s", tickets[i].ticketSubject);
				fprintf(ticketFile, "%c", pipe);

				fprintf(ticketFile, "%d", tickets[i].messageCount);
				fprintf(ticketFile, "%c", pipe);

				for (int j = 0; j < tickets[i].messageCount; j++)
				{
					fprintf(ticketFile, "%c", tickets[i].messages[j].AccountType);
					fprintf(ticketFile, "%c", pipe);

					fprintf(ticketFile, "%s", tickets[i].messages[j].name);
					fprintf(ticketFile, "%c", pipe);

					fprintf(ticketFile, "%s", tickets[i].messages[j].details);
					fprintf(ticketFile, "%c", pipe);
				}
				fprintf(ticketFile, "%c", '\n');
				ticketsCount++;
			}
		}
	}
	fclose(ticketFile);
	return ticketsCount;
}
