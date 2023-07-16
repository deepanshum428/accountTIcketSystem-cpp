
#include <stdio.h>
#include "accountTicketingUI.h"
#include "commonHelpers.h"
#include "ticket.h"

void displayAccountDetailHeader()
{
	printf("Acct# Acct.Type Full Name       Birth Income      Country    Login      Password\n");
	printf("----- --------- --------------- ----- ----------- ---------- ---------- --------\n");
}

void displayAccountDetailRecord(const struct Account *inAccount)
{
	char pw[9];
	int ii;
	for (ii = 0; inAccount->login.password[ii] != '\0'; ii++)
	{
		if (ii % 2)
			pw[ii] = '*';
		else
			pw[ii] = inAccount->login.password[ii];
	}
	pw[ii] = '\0';
	printf("%05d %-9s %-15s %5d %11.2lf %-10s %-10s %8s\n", inAccount->acctNum,
		   ((inAccount->type == 'C') ? "CUSTOMER" : "AGENT"), inAccount->person.name,
		   inAccount->person.year, inAccount->person.income, inAccount->person.country,
		   inAccount->login.userName, pw);
}

void applicationStartup(struct AccountTicketingData *data)
{
	int result;
	do
	{
		result = menuLogin(data->accounts, data->ACCOUNT_MAX_SIZE);
		if (result != -1)
		{
			printf("\n");
			menuAgent(data, &data->accounts[result]);
			printf("\nSaving session modifications...\n");
			printf("   %d account(s) saved.\n", saveAccounts(data->accounts, data->ACCOUNT_MAX_SIZE));
			printf("   %d ticket(s) saved.\n", saveTickets(data->tickets, data->TICKET_MAX_SIZE));
			
			printf("### LOGGED OUT ###\n\n");
		}

	} while (result != -1);
	printf("\n==============================================\n");
	printf("Account Ticketing System - Terminated\n");
	printf("==============================================\n\n");
}

int menuLogin(const struct Account account[], int maxElements)
{
	int choice;
	char confirm = 'n';
	int found = 0, quit = 0, tempAcct;
	do
	{
		printf("==============================================\n");
		printf("Account Ticketing System - Login\n");
		printf("==============================================\n");
		printf("1) Login to the system\n");
		printf("0) Exit application\n");
		printf("----------------------------------------------\n\n");
		printf("Selection: ");
		choice = getIntFromRange(0, 1);
		if (choice)
		{
			int attempt = 3;
			char username[51];
			char password[51];
			do
			{
				printf("\nEnter the account#: ");
				tempAcct = getPositiveInteger();
				printf("User Login        : ");
				getCString(username, 2, 50);
				printf("Password          : ");
				getCString(password, 2, 50);
				tempAcct = findAccountIndexByAcctNum(tempAcct, account, maxElements, 0);
				if (tempAcct != -1 && account[tempAcct].type == 'A' && strcmp(account[tempAcct].login.userName, username) == 0 && strcmp(account[tempAcct].login.password, password) == 0)
				{
					found = 1;
				}
				else
				{
					attempt--;
					printf("INVALID user login/password combination! [attempts remaining:%d]\n", attempt);
				}
			} while (attempt != 0 && found != 1);

			if (attempt == 0)
			{
				printf("\nERROR:  Access Denied.\n\n");
				pauseExecution();
			}
			else
			{
				found = 1;
			}
		}
		else
		{
			printf("\nAre you sure you want to exit? ([Y]es|[N]o): ");
			confirm = getCharOption("yYnN");
			if (confirm == 'y' || confirm == 'Y')
				quit = 1;
			else
				printf("\n");
		}
	} while (!found && !quit);

	return (quit) ? -1 : tempAcct;
}

void menuAgent(struct AccountTicketingData *data, const struct Account *loggedUser)
{
	int choice, freeIndex, ii, acctNum;
	char permit;
	do
	{
		printf("AGENT: %s (%d)\n", loggedUser->person.name, loggedUser->acctNum);
		printf("==============================================\n");
		printf("Account Ticketing System - Agent Menu\n");
		printf("==============================================\n");
		printf("1) Add a new account\n");
		printf("2) Modify an existing account\n");
		printf("3) Remove an account\n");
		printf("4) List accounts: detailed view\n");
		printf("----------------------------------------------\n");
		printf("5) List new tickets\n");
		printf("6) List active tickets\n");
		printf("7) List closed tickets\n");
		printf("8) Add a new ticket\n");
		printf("9) Manage a ticket\n");
		printf("----------------------------------------------\n");
		printf("0) Logout\n");
		printf("\n");
		printf("Selection: ");
		freeIndex = -1;
		for (ii = 0; ii < data->ACCOUNT_MAX_SIZE && freeIndex == -1; ii++)
		{
			if (data->accounts[ii].acctNum == 0)
				freeIndex = ii;
		}
		choice = getIntFromRange(0, 9);
		switch (choice)
		{
		case 1:
			if (freeIndex == -1)
			{
				printf("\nERROR: Account listing is FULL, call ITS Support!\n\n");
				pauseExecution();
			}
			else
			{
				int max = 0;
				for (int i = 0; i < data->ACCOUNT_MAX_SIZE; i++)
				{
					if (max < data->accounts[i].acctNum)
						max = data->accounts[i].acctNum;
				}
				data->accounts[freeIndex].acctNum = max + 1;
				printf("\n");
				getAccount(&data->accounts[freeIndex]);
				printf("\n*** New account added! ***\n\n");
				pauseExecution();
			}
			break;
		case 2:
			printf("\nEnter the account#: ");
			acctNum = findAccountIndexByAcctNum(0, data->accounts, data->ACCOUNT_MAX_SIZE, 1);
			if (acctNum != -1)
			{
				updateAccount(&data->accounts[acctNum]);
			}
			else
			{
				printf("\nERROR: Cannot find account by that number.\n\n");
				pauseExecution();
			}
			break;
		case 3:
			printf("\nEnter the account#: ");
			acctNum = findAccountIndexByAcctNum(0, data->accounts, data->ACCOUNT_MAX_SIZE, 1);
			if (acctNum != -1)
			{
				if (data->accounts[acctNum].acctNum != loggedUser->acctNum)
				{
					printf("\n");
					displayAccountDetailHeader();
					displayAccountDetailRecord(&data->accounts[acctNum]);
					printf("\nAre you sure you want to remove this record? ([Y]es|[N]o): ");
					permit = getCharOption("YN");
					if (permit == 'Y')
					{
						int ticketCount = 0;
						for (int i = 0; i < data->TICKET_MAX_SIZE; i++)
						{
							if (data->tickets[i].accountNum == data->accounts[acctNum].acctNum)
							{
								if (data->tickets[i].ticketNumber != 0)
								{
									data->tickets[i].ticketNumber = 0;
									data->tickets[i].accountNum = 0;
									ticketCount++;
								}
							}
						}

						data->accounts[acctNum].acctNum = 0;

						printf("\n*** Account Removed! (%d ticket removed) ***\n", ticketCount);
					}
					printf("\n");
					pauseExecution();
				}
				else
				{
					printf("\nERROR: You can't remove your own account!\n\n");
					pauseExecution();
				}
			}
			else
			{
				printf("\nERROR: Cannot find account by that number.\n\n");
				pauseExecution();
			}
			break;
		case 4:
			printf("\n");
			displayAllAccountRecords(data->accounts, data->ACCOUNT_MAX_SIZE);
			printf("\n");
			pauseExecution();
			// may need a \n
			break;

		case 5:
			allTickets(data->tickets, data->TICKET_MAX_SIZE);
			printf("\n");
			break;

		case 6:
			activeTickets(data->tickets, data->TICKET_MAX_SIZE);
			printf("\n");
			break;

		case 7:
			closedTickets(data->tickets, data->TICKET_MAX_SIZE);
			printf("\n");
			break;

		case 8:
			newTickets(data->tickets, data->TICKET_MAX_SIZE, data->accounts, data->ACCOUNT_MAX_SIZE);
			printf("\n");
			pauseExecution();
			break;

		case 9:
			manageTickets(data->tickets, data->TICKET_MAX_SIZE, loggedUser);
			break;
		}
	} while (choice);
}

int findAccountIndexByAcctNum(int findNum, const struct Account *accounts, int maxElements, int prompt)
{
	int found = -1, ii;
	if (prompt)
	{
		findNum = getPositiveInteger();
	}
	for (ii = 0; ii < maxElements && found == -1; ii++)
	{
		if (accounts[ii].acctNum == findNum)
		{
			found = ii;
		}
	}
	return ((found != -1) ? found : -1);
}

void displayAllAccountRecords(const struct Account *accounts, int maxAccounts)
{
	displayAccountDetailHeader();
	int ii;
	for (ii = 0; ii < maxAccounts; ii++)
	{
		if (accounts[ii].acctNum != 0)
		{
			displayAccountDetailRecord(&accounts[ii]);
		}
	}
}

// Pause execution until user enters the enter key
void pauseExecution(void)
{
	printf("<< ENTER key to Continue... >>");
	clearStandardInputBuffer();
	printf("\n");
}
