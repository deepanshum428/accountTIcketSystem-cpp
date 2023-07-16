
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>

// Uses the time.h library to obtain current year information 
// Get the current 4-digit year from the system 
int currentYear(void) { 
	time_t currentTime = time(NULL); 
	return localtime(&currentTime)->tm_year + 1900; 
}

// As demonstrated in the course notes: https://ict.senecacollege.ca//~ipc144/pages/content/formi.html#buf 
// Empty the standard input buffer 
void clearStandardInputBuffer(void) { 
	while (getchar() != '\n') { 
		; // On purpose: do nothing 
	} 
}



int getInteger() {
	int temp, flag=1;
	char newline;
	do {
		temp = -200;
		scanf("%d%c", &temp, &newline);
		if (newline != '\n' || temp == -200) {
			printf("ERROR: Value must be an integer: ");
			clearStandardInputBuffer();
		}
		else {
			flag = 0;
		}
	} while (flag);
	return temp;
}

int getPositiveInteger() {
	int temp;

	do {
		temp = getInteger();
		if (temp < 1) {
			printf("ERROR: Value must be a positive integer greater than zero: ");
		}


	} while (temp < 1);
	return temp; 
}




int getIntFromRange(int min, int max) {
	int temp, flag = 1; 
	do {
		temp = getInteger();
		if (temp < min || temp > max) {
			printf("ERROR: Value must be between %d and %d inclusive: ", min, max);
		}
		else {
			flag = 0;
		}
	} while (flag);
	return temp;
}




double getDouble() {
	int  flag = 1;
	double temp;
	char newline;
	do {
		scanf("%lf%c", &temp, &newline);
		if (newline != '\n') {
			printf("ERROR: Value must be a double floating-point number: ");
			clearStandardInputBuffer();
		}
		else {
			flag = 0;
		}
	} while (flag);
	return temp;
}

double getPositiveDouble() {
	double temp;

	do {
		temp = getDouble();
		if (temp < 0.1) {
			printf("ERROR: Value must be a positive double floating-point number: ");
		}
	} while (temp < 0.1);
	return temp;
}



char getCharOption(char* selection) {
	char temp, overflow ;
	int ii, flag = 1;
	do {
		scanf("%c%c",&temp, &overflow);
		if (overflow != '\n') {
			clearStandardInputBuffer();
		}
		else {
			for (ii = 0; selection[ii] != '\0'; ii++) {
				if (temp == selection[ii]) {
					flag = 0;
				}
			}
		}
		if (flag) {
			printf("ERROR: Character must be one of [%s]: ",selection);
		}
	} while (flag);
	return temp;
}

void getCString(char* inString, int min, int max) {
	char temp[100], overflow;
	int ii, jj, flag = 1; 

	do {
		scanf("%99[^\n]%c", temp, &overflow);
		if (overflow != '\n') {
			clearStandardInputBuffer();
			if (min == max)
				printf("ERROR: String length must be exactly %d chars: ",min);
			else
				printf("ERROR: String length must be no more than %d chars: ", max);
		}
		else {
			for (ii = 0; temp[ii] != '\0'; ii++) {
				//getting size of array
			}
			if (min == max && ii != max)
				printf("ERROR: String length must be exactly %d chars: ", max);
			else if (min != max && ii > max) {
				printf("ERROR: String length must be no more than %d chars: ", max);
			}
			else if (min != max && ii < min)
				printf("ERROR: String length must be between %d and %d chars: ", min, max);
			else
				flag = 0; 
		}
	} while (flag);

	for (jj = 0; jj < ii; jj++) {
		inString[jj] = temp[jj];
	}
	inString[ii] = '\0';
}
