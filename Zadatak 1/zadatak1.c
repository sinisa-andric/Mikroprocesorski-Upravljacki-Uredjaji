#include <string.h>
#include <math.h>
#define MAX 10

int str2num(char *x)
{
	unsigned char i = 0;
	unsigned char n = 0;
	unsigned char mult = 1;
		
	if(*(x) == 'b')
	{
		mult = pow(2,strlen(x)-2);
		for(i = 1; i < strlen(x); i++)
		{
			if(*(x+i) == '1') n = n + mult;
			mult = mult/2;
		}		
	}
	else if(*(x) == 'H')
	{
		mult = pow(16,strlen(x)-2);
		for(i = 1; i < strlen(x); i++)
		{
			if(*(x+i) < 65 )
			{
				n = n + (*(x+i) - 48) * mult;
			}
			else
			{
				n = n + (*(x+i) - 55) * mult; // slova su 55 razlike
			}
			mult = mult/16;
		}
	}
	else
	{
		for(i = 0; i < strlen(x)-1; i++)
		{
			mult = mult*10;
		}

		for(i = 0; i < strlen(x); i++)
		{
			n = n + (*(x+i) - 48) * mult;
			mult = mult/10;
		}
	}

	return n;
}

char* num2str(int x)
{
	char str[MAX] = "";
	unsigned char i = 0;
	int n = 0;
	unsigned char m = 1;
	unsigned char size = 0;
	

	n = x;
	if(n == 0)
	{
		size = 1;
	}
	else
	{
		size = floor(log10(x))+1;
	}

	for(i = 0; i < size-1; i++)
	{
		m = m*10;
	}

	i = 0;
	while(size != 0)
	{
		str[i] = n/m + 48;
		n = n%m;
		i++;
		size--;
		m = m/10;
	}
	return str;
}

void izracunaj(char* op1, char* op2, char* rez, char operant)
{
	int a = 0;	
	int b = 0;
	int c = 0;
	
	a = str2num(op1);
	b = str2num(op2);

	switch(operant)
	{
		case '+': c = a + b;
				  break;
		case '-': c = a - b;
				  break;
		case '*': c = a * b;
				  break;
		case '/': if(b != 0)
				  {
				  	c = a / b;
				  }
				  else
				  {
				  	c = 1; // deljenje nulom nije dozvoljeno
				  }
				  break;
	}
	strcpy(rez, num2str(c));
}

void main(void)
{
	unsigned t = 0;

	char operand1[MAX] = "H0A";
	char operand2[MAX] = "b101";
	char rezultat[MAX] = "";
	char operacija = '+';
	izracunaj(operand1, operand2, rezultat, operacija);

	operacija = '-';
	izracunaj(operand1, operand2, rezultat, operacija);

	operacija = '*';
	izracunaj(operand1, operand2, rezultat, operacija);

	operacija = '/';
	izracunaj(operand1, operand2, rezultat, operacija);	
	
	t = t + 1; // zbog breakpoint-a
}
