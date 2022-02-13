#include <at89c51rc2.h>

unsigned char counterForSec = 0;		 
unsigned char elapsed1sec = 0;

unsigned char trenutnoStanjeSoftP01 = 1;
unsigned char trenutnoStanjeSoftP01_zaMain = 1;					  //pali/gasi 
unsigned char prethodnoStanjeSoftP01_zaMain = 1;
unsigned char prethodnoStanjeHardP01 = 1;
unsigned char trenutnoStanjeHardP01 = 1;
unsigned char brPonavljanjaP01 = 0;

unsigned char trenutnoStanjeSoftP02 = 1;
unsigned char trenutnoStanjeSoftP02_zaMain = 1;					  //programi
unsigned char prethodnoStanjeSoftP02_zaMain = 1;
unsigned char prethodnoStanjeHardP02 = 1;
unsigned char trenutnoStanjeHardP02 = 1;
unsigned char brPonavljanjaP02 = 0;

unsigned char trenutnoStanjeSoftP03 = 1;
unsigned char trenutnoStanjeSoftP03_zaMain = 1;					   //brzine
unsigned char prethodnoStanjeSoftP03_zaMain = 1;
unsigned char prethodnoStanjeHardP03 = 1;
unsigned char trenutnoStanjeHardP03 = 1;
unsigned char brPonavljanjaP03 = 0;


unsigned char radi = 0;

unsigned char program = 0x01;
unsigned char brzina = 0;

void interrupt_timer_0(void) interrupt 1
{

//BRZINA
	TH0 = 0xEE;						  
	TL0 = 0;
	counterForSec++;
	if(brzina == 0)
	{
	 	if(counterForSec == 200)
		{
		 	counterForSec = 0;
			elapsed1sec = 1;
		}
	}
	else
	{
		if(counterForSec == 100)
		{
			counterForSec = 0;
			elapsed1sec = 1;
		}	
	}

//P0.1															
	trenutnoStanjeHardP01 = P0_1;
	if(prethodnoStanjeHardP01 == trenutnoStanjeHardP01)	 
	{
		brPonavljanjaP01++;
		if(brPonavljanjaP01 > 5-1)							   	
		{
			trenutnoStanjeSoftP01 = trenutnoStanjeHardP01;		//ako je stanje isto 4 puta za redom, prihvatamo ga, postaje softversko
			brPonavljanjaP01 = 0;
		}
	}
	else
	{														   //doslo je do promene
		brPonavljanjaP01 = 0;
	}

	prethodnoStanjeHardP01 = trenutnoStanjeHardP01;				//namestamo prethodno stanje za sledeci prekid

//P0.2
	trenutnoStanjeHardP02 = P0_2;
	if(prethodnoStanjeHardP02 == trenutnoStanjeHardP02)	 
	{
		brPonavljanjaP02++;
		if(brPonavljanjaP02 > 5-1)							   	
		{
			trenutnoStanjeSoftP02 = trenutnoStanjeHardP02;		//ako je stanje isto 4 puta za redom, prihvatamo ga, postaje softversko
			brPonavljanjaP02 = 0;
		}
	}
	else
	{														   //doslo je do promene
		brPonavljanjaP02 = 0;
	}

	prethodnoStanjeHardP02 = trenutnoStanjeHardP02;				//namestamo prethodno stanje za sledeci prekid

//P0.3
	trenutnoStanjeHardP03 = P0_3;
	if(prethodnoStanjeHardP03 == trenutnoStanjeHardP03)	 
	{
		brPonavljanjaP03++;
		if(brPonavljanjaP03 > 5-1)							   	
		{
			trenutnoStanjeSoftP03 = trenutnoStanjeHardP03;		//ako je stanje isto 4 puta za redom, prihvatamo ga, postaje softversko
			brPonavljanjaP03 = 0;
		}
	}
	else
	{														   //doslo je do promene
		brPonavljanjaP03 = 0;
	}

	prethodnoStanjeHardP03 = trenutnoStanjeHardP03;				//namestamo prethodno stanje za sledeci prekid

}

void main(void)
{
	unsigned char niz_stanja1[2] = {0x00, 0xFF};
	unsigned int duzina1 = 0;
	unsigned char niz_stanja2[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	unsigned int duzina2 = 0;
	unsigned char niz_stanja3[6] = {0x81, 0x42, 0x24, 0x18, 0x24, 0x42};
	unsigned int duzina3 = 0;
	unsigned char niz_stanja4[4] = {0x88, 0x44, 0x22, 0x11};
	unsigned int duzina4 = 0;	

	//inicijalizacija
	counterForSec = 0;

	TH0 = 0xEE;						  
	TL0 = 0;
	TMOD = 0x01;
	ET0 = 1;
	EA = 1;
	TR0 = 1;

	while(1)
	{
		//provera ulaza
		//logika
		//osvezavanje izlaza
		//komunikacija
		trenutnoStanjeSoftP01_zaMain = trenutnoStanjeSoftP01;		    //za main
		trenutnoStanjeSoftP02_zaMain = trenutnoStanjeSoftP02;
		trenutnoStanjeSoftP03_zaMain = trenutnoStanjeSoftP03;
		//trenutnoStanjeSoftP04_zaMain = P0_4;

		if(prethodnoStanjeSoftP01_zaMain > trenutnoStanjeSoftP01_zaMain)	//opadajuca ivica?
		{
			radi = radi ? 0 : 1;											//menjaj stanje radi/ne radi
			if(radi)
			{			
				counterForSec = 0;											//ako radi, podesi timer
			    TH0 = 0xEE;						  
				TL0 = 0;
			}
		}

		if(prethodnoStanjeSoftP03_zaMain > trenutnoStanjeSoftP03_zaMain)
		{
		 	brzina = brzina ? 0 : 1;
		}

		if(prethodnoStanjeSoftP02_zaMain > trenutnoStanjeSoftP02_zaMain)
		{
		 	program++;
			if(program == 0x05)
				program = 0x01;
		}

		prethodnoStanjeSoftP01_zaMain = trenutnoStanjeSoftP01_zaMain;		//za sledecu iteraciju
		prethodnoStanjeSoftP02_zaMain = trenutnoStanjeSoftP02_zaMain;
		prethodnoStanjeSoftP03_zaMain = trenutnoStanjeSoftP03_zaMain;

		if(radi)
		{ 
			if(program == 0x01)
			{
				if(elapsed1sec == 1)
				{
					if(duzina1 == 2)
						duzina1 = 0;
					P2 = niz_stanja1[duzina1];
					duzina1++;
					elapsed1sec = 0;
				}
			}
			else if(program == 0x02)
			{
			 	if(elapsed1sec == 1)
				{
					if(duzina2 == 8)
						duzina2 = 0;
					P2 = niz_stanja2[duzina2];
					duzina2++;
				 	elapsed1sec	= 0;
		   		}
			}
			else if(program == 0x03)
			{
			 	if(elapsed1sec == 1)
				{
					if(duzina3 == 6)
						duzina3 = 0;
					P2 = niz_stanja3[duzina3];
					duzina3++;
				 	elapsed1sec	= 0;
		   		}
			}
			else if(program == 0x04)
			{
			 	if(elapsed1sec == 1)
				{
					if(duzina4 == 4)
						duzina4 = 0;
					P2 = niz_stanja4[duzina4];
					duzina4++;
				 	elapsed1sec	= 0;
		   		}
			}
		}

		else
		{
			P2 = 0;
		}
	}
}