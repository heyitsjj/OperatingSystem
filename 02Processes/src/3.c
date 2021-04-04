/**** multi-converter.c ****/

#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


#define _XOPEN_SOURCE 700 

/* Nb of converter pcs */
#define NB_CONVERTERS 5

/* Identifier for the converter in euros */
#define EUR_CONV 0
/* Identifier for the converter in pounds sterling */
#define GBP_CONV 1 
/* Identifier for the converter in US dollars */
#define USD_CONV 2 
/* Identifier for the converter in japanese yen */
#define JPY_CONV 3 
/* Identifier for the converter in chinese yuan */
#define CNY_CONV 4 

/* Conversion rate euro */
#define EUR 1			
/* Conversion rate pound sterling */
#define GBP 0.8388
/* Conversion rate US dollar */
#define USD 1.1255
/* Conversion rate yen */
#define JPY 114.096
/* Conversion rate yuan */
#define CNY 7.4986


/* Converts from a currency to another.
	input_currency :	currency to convert from
	target_currency :	currency to convert to
	input_amount :		amount to convert */
double convert(char* input_currency, char* target_currency, double input_amount);


/* Determines a currency string identifier, given its integer identifier */
char* determine_currency(int curr_nb);

/* Displays the result of a conversion */
void display_result(int target_currency, double conversion_result);

/* Converts from any to EUR */
double convertfrom(char* input_currency, double input_amount) {
	if (strcmp(input_currency, "GBP") == 0)
		return (input_amount / GBP);
	else if (strcmp(input_currency, "USD") == 0)
		return (input_amount / USD);
	else if (strcmp(input_currency, "JPY") == 0)
		return (input_amount / JPY);
	else if (strcmp(input_currency, "CNY") == 0)
		return (input_amount / CNY);
	else if (strcmp(input_currency, "EUR") == 0)
		return input_amount;
	else {
		perror("Unknown currency");
		exit(1);
	}
}


/* Converts from EUR to any */
double convertto(char* target_currency, double input_amount) {
	if (strcmp(target_currency, "GBP") == 0)
		return (input_amount * GBP);
	else if (strcmp(target_currency, "USD") == 0)
		return (input_amount * USD);
	else if (strcmp(target_currency, "JPY") == 0)
		return (input_amount * JPY);
	else if (strcmp(target_currency, "CNY") == 0)
		return (input_amount * CNY);
	else if (strcmp(target_currency, "EUR") == 0)
		return input_amount;
	else {
		perror("Unknown currency");
		exit(1);
	}
}


/* Converts from one currency to another.
	input_currency :	currency to convert from
	target_currency :	currency to convert to
	input_amount :		amount to convert */
double convert(char* input_currency, char* target_currency, double input_amount) {
	return convertto(target_currency, convertfrom(input_currency, input_amount));
}


/* Determines a currency string identifier, given its integer identifier */
char* determine_currency(int curr_nb) {
	switch(curr_nb) {
		case EUR_CONV: return "EUR";
		case GBP_CONV: return "GBP";
		case USD_CONV: return "USD";
		case JPY_CONV: return "JPY";
		case CNY_CONV: return "CNY";
		default: perror("Unknown currency"); exit(1);
	}
}


void display_result(int target_currency, double conversion_result) {
    printf("\t %s %.3f\n", determine_currency(target_currency), conversion_result);
}


int main(int argc, char **argv)
{
    double amt = atof(argv[2]);
    printf("Conversion for: %s %.3f\n", argv[1], amt);
    for (int i = 0; i < 5; i++) {
        if (fork() == 0) {
            display_result(i, convert(argv[1], determine_currency(i), amt));
            exit(1);
        }
    }
    while (wait(NULL) > 0) {
        // do noting
    }
    printf("End of conversion\n");
    return EXIT_SUCCESS;
}
