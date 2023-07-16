#include <stdio.h>
#include "account.h"
#include "commonHelpers.h"
#include "accountTicketingUI.h"

/*
	Create new account of auto generated account number (max account number +1) if array size is not full and nothing returns
 */
void getAccount(struct Account *inAccount)
{
	printf("Account Data: (Account# %d)\n", inAccount->acctNum);
	printf("----------------------------------------\n");
	printf("Enter the account type (A=Agent | C=Customer): ");
	inAccount->type = getCharOption("AC");
	getPerson(&inAccount->person);
	if (inAccount->type == 'A')
	{
		getUserLogin(&inAccount->login);
	}
}

/*
	Function to add personal data to Person object by getting name, birthday year, income and country name as input and nothing returns
 */
void getPerson(struct Person *inPerson)
{
	printf("\nPerson Data Input\n");
	printf("----------------------------------------\n");
	printf("Enter the person's full name (30 chars max): ");
	getCString(inPerson->name, 1, 30);
	printf("Enter birth year (current age must be between 18 and 110): ");
	inPerson->year = getIntFromRange(currentYear() - 110, currentYear() - 18);
	printf("Enter the household Income: $");
	inPerson->income = getPositiveDouble();
	printf("Enter the country (30 chars max.): ");
	getCString(inPerson->country, 1, 30);
	for (int i = 0; inPerson->country[i] != '\0' && i < 31; i++)
	{
		if (inPerson->country[i] >= 'a' && inPerson->country[i] <= 'z')
		{
			inPerson->country[i] = inPerson->country[i] - 32;
		}
	}
}

/*
	Function to add user login data to UserLogin object by getting username and a strong password as input and nothing returns
 */
void getUserLogin(struct UserLogin *inLogin)
{
	int correctLoginUserName = 0;
	int containsSpaces = 0;
	printf("\nUser Login Data Input\n");
	printf("----------------------------------------\n");
	do
	{
		correctLoginUserName = 0;
		printf("Enter user login (10 chars max): ");
		getCString(inLogin->userName, 1, 10);
		for (int i = 0; inLogin->userName[i] != 0 && i < 10 && containsSpaces != 1; i++)
		{
			if (inLogin->userName[i] == ' ')
			{
				correctLoginUserName = -1;
				containsSpaces = 1;
			}
		}

	} while (correctLoginUserName != 0);

	if (containsSpaces == 1)
		printf("ERROR: The user login must NOT contain whitespace characters.\n");

	int valid = 0;
	int lower = 0;
	int upper = 0;
	int digit = 0;
	int symbol = 0;
	char pw[8 + 1];
	do
	{
		printf("Enter the password (must be 8 chars in length): ");
		getCString(pw, 8, 8);

		for (int i = 0; i <= 8; i++)
		{
			if (pw[i] <= 'z' && pw[i] >= 'a') // increment lower count if password contains lower case characters
				lower++;
			if (pw[i] <= 'Z' && pw[i] >= 'A') // increment upper count if password contains upper case characters
				upper++;
			if (pw[i] <= '9' && pw[i] >= '0') // increment digit count if password contains digits
				digit++;
			if (pw[i] == '!' || pw[i] == '@' || pw[i] == '#' || pw[i] == '$' || pw[i] == '^' || pw[i] == '&' || pw[i] == '*') // increment symbol count if password contains any of the symbol
				symbol++;
		}

		if (lower == 2 && upper == 2 && digit == 2 && symbol == 2) // set valid as 1 if lower, upper, digit, and symbol count is equal to 2
			valid = 1;
		else
		{
			printf("SECURITY: Password must contain 2 of each:\n");
			printf("\t  Digit: 0-9\n");
			printf("\t  UPPERCASE character\n");
			printf("\t  lowercase character\n");
			printf("\t  symbol character: !@#$^&*\n");
			lower = 0;
			upper = 0;
			digit = 0;
			symbol = 0;
		}
	} while (valid == 0);

	strcpy(inLogin->password, pw);
}

/*
	Update account's details of account address and nothing returns
 */
void updateAccount(struct Account *account)
{
	int choice;
	do
	{
		printf("\nUpdate Account: %05d (%s)\n----------------------------------------\n", account->acctNum, account->person.name);
		printf("1) Update account type (current value: %c)\n", account->type);
		printf("2) Person\n");
		printf("3) Login\n");
		printf("0) Done\n");
		printf("Selection: ");
		choice = getIntFromRange(0, 3);
		if (choice == 1)
		{
			printf("\nEnter the account type (A=Agent | C=Customer): ");
			account->type = getCharOption("AC");
			if (account->type == 'C')
			{
				account->login.userName[0] = '\0';
				account->login.password[0] = '\0';
			}
			else
			{
				printf("\nAgent type accounts require a user login.  Please enter this information now:\n");
				getUserLogin(&account->login);
			}
		}
		else if (choice == 2)
		{
			updatePerson(&account->person);
		}
		else if (choice == 3)
		{
			if (account->type == 'C')
			{
				printf("\nERROR:  Customer account types don't have user logins!\n");
			}
			else
			{
				updateUserLogin(&account->login);
			}
		}
		else
		{
			printf("\n");
		}
	} while (choice);
}

/*
	Update account's personal details of given address of type person and nothing returns
 */
void updatePerson(struct Person *person)
{

	int choice;
	double temp;

	do
	{
		printf("\nPerson Update Options\n");
		printf("----------------------------------------\n");
		printf("1) Full name (current value: %s)\n", person->name);
		printf("2) Household Income (current value: $%.2lf)\n", person->income);
		printf("3) Country (current value: %s)\n", person->country);
		printf("0) Done\n");
		printf("Selection: ");
		choice = getIntFromRange(0, 3);
		switch (choice)
		{
		case 1:
			printf("\nEnter the person's full name (30 chars max): ");
			getCString(person->name, 1, 30);
			break;
		case 2:
			printf("\nEnter the household Income: $");
			temp = (double)getPositiveDouble();
			person->income = temp;
			break;
		case 3:
			printf("\nEnter the country (30 chars max.): ");
			getCString(person->country, 1, 30);
			for (int i = 0; person->country[i] != '\0' && i < 31; i++)
			{
				if (person->country[i] >= 'a' && person->country[i] <= 'z')
				{
					person->country[i] = person->country[i] - 32;
				}
			}
			break;
		}

	} while (choice);
}

/*
	Update account's UserLogin details of given address of type UserLogin and nothing returns
 */
void updateUserLogin(struct UserLogin *login)
{

	int choice;
	do
	{
		printf("\nUser Login: %s - Update Options\n", login->userName);
		printf("----------------------------------------\n");
		printf("1) Password\n");
		printf("0) Done\n");
		printf("Selection: ");
		choice = getIntFromRange(0, 1);
		if (choice)
		{
			int valid = 0;
			int lower = 0;
			int upper = 0;
			int digit = 0;
			int symbol = 0;
			char pw[8 + 1];
			printf("\n");
			do
			{
				printf("Enter the password (must be 8 chars in length): ");
				getCString(pw, 8, 8);

				for (int i = 0; i <= 8; i++)
				{
					if (pw[i] <= 'z' && pw[i] >= 'a')
						lower++;
					if (pw[i] <= 'Z' && pw[i] >= 'A')
						upper++;
					if (pw[i] <= '9' && pw[i] >= '0')
						digit++;
					if (pw[i] == '!' || pw[i] == '@' || pw[i] == '#' || pw[i] == '$' || pw[i] == '^' || pw[i] == '&' || pw[i] == '*')
						symbol++;
				}

				if (lower == 2 && upper == 2 && digit == 2 && symbol == 2)
					valid = 1;
				else
				{
					printf("SECURITY: Password must contain 2 of each:\n");
					printf("\t  Digit: 0-9\n");
					printf("\t  UPPERCASE character\n");
					printf("\t  lowercase character\n");
					printf("\t  symbol character: !@#$^&*\n");
					lower = 0;
					upper = 0;
					digit = 0;
					symbol = 0;
				}
			} while (valid == 0);

			strcpy(login->password, pw);
		}
	} while (choice);
}

/*
	load accounts data from accounts.txt file to AccountTicketingData's accounts member variable(Account Array)
 */
int loadAccounts(struct Account accounts[], int accounts_size)
{
	FILE *accountFile = fopen("accounts.txt", "r");
	int accountCount = 0;
	if (accountFile) // if file is not NULL
	{
		for (int i = 0; i < accounts_size && accounts[i].acctNum != EOF; i++) // geting account data of size (account_size)
		{
			char tilt = 0;
			fscanf(accountFile, "%d", &accounts[i].acctNum);

			fscanf(accountFile, "%c", &tilt);
			fscanf(accountFile, "%c", &accounts[i].type);

			fscanf(accountFile, "%c", &tilt);
			fscanf(accountFile, "%[^~]s", accounts[i].person.name);

			fscanf(accountFile, "%c", &tilt);
			fscanf(accountFile, "%d", &accounts[i].person.year);

			fscanf(accountFile, "%c", &tilt);
			fscanf(accountFile, "%lf", &accounts[i].person.income);

			fscanf(accountFile, "%c", &tilt);

			if (accounts[i].type == 'C')
			{
				fscanf(accountFile, "%[^\n]s", accounts[i].person.country);
			}
			else
			{
				fscanf(accountFile, "%[^~]s", accounts[i].person.country);

				fscanf(accountFile, "%c", &tilt);
				fscanf(accountFile, "%[^~]s", accounts[i].login.userName);

				fscanf(accountFile, "%c", &tilt);
				fscanf(accountFile, "%[^\n]s", accounts[i].login.password);
			}
			if (tilt == '~')
				accountCount++;
		}
	}
	fclose(accountFile);
	return accountCount;
}
/*
	save accounts data to accounts.txt file from AccountTicketingData's accounts
 */
int saveAccounts(struct Account accounts[], int accounts_size)
{
	FILE *accountFile = fopen("accounts.txt", "w");
	int accountCount = 0;
	if (accountFile) // if file is not NULL
	{
		for (int i = 0; i < accounts_size; i++) // saving account data of size (account_size)
		{
			if (accounts[i].acctNum != 0)
			{
				char tilt = '~';
				fprintf(accountFile, "%d", accounts[i].acctNum);

				fprintf(accountFile, "%c", tilt);
				fprintf(accountFile, "%c", accounts[i].type);

				fprintf(accountFile, "%c", tilt);
				fprintf(accountFile, "%s", accounts[i].person.name);

				fprintf(accountFile, "%c", tilt);
				fprintf(accountFile, "%d", accounts[i].person.year);

				fprintf(accountFile, "%c", tilt);
				fprintf(accountFile, "%.2lf", accounts[i].person.income);

				fprintf(accountFile, "%c", tilt);

				if (accounts[i].type == 'C')
				{
					fprintf(accountFile, "%s\n", accounts[i].person.country);
				}
				else
				{
					fprintf(accountFile, "%s", accounts[i].person.country);

					fprintf(accountFile, "%c", tilt);
					fprintf(accountFile, "%s", accounts[i].login.userName);

					fprintf(accountFile, "%c", tilt);
					fprintf(accountFile, "%s\n", accounts[i].login.password);
				}
				accountCount++;
			}
		}
	}
	fclose(accountFile);
	return accountCount;
}
