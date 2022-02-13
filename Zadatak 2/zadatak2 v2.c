#include <at89c51rc2.h>

void delay(unsigned char speed)
{
	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char k = 0;
	unsigned char z = 200*speed;
	
	for(i=0; i<speed; i++)
	{
		for(j=0; j<z; j++)
		{	
			for(k=0; k<z; k++)
			{
			}
		}
	}
}

void main(void)
{
	// start        = P0_0,
    // stop         = P0_1,
	// program 1    = P0_2,
    // program 2    = P0_3, 
    // program 3    = P0_4, 
    // program 4    = P0_5,
	// brzina 1     = P0_6,
    // brzina 2     = P0_7

	// inicijalizacija
	unsigned char start = 0;
	unsigned char program = 0;
	unsigned char brzina = 20;

	// za program 3
	unsigned char niz_stanja[6] = {0x81, 0x42, 0x24, 0x18, 0x24, 0x42};
   	 unsigned char niz_stanja2[7] = {0x03, 0x06, 0x0C, 0x30, 0x60, 0xC0, 0x81}; // ili bez 0x81
	unsigned char iterator_stanja = 0;

	// za program 4
	unsigned char proces = 0; // proces je ukljuci za 1, iskljuci za 0;

   	P2 = 0x00;
	
	while(1)
	{
		// pokretanje
		if(P0_0 == 0)
		{
			start = 1;
		}
		// zaustavljanje
		if(P0_1 == 0)
		{
			start = 0;
			iterator_stanja = 0;
			brzina = 20;
		}
		// brzina 1
		if(P0_6 == 0)
		{
			brzina = 10;
		}
		// brzina 2
		if(P0_7 == 0)
		{
			brzina = 5;
		}
		// proces
		if(start == 1)
		{
			// program 1
			if(P0_2 == 0)
			{
				delay(brzina);
				if(P2 == 0x00)
				{
					P2 = 0xFF; // ukljuci sve
				}
				else
				{
					P2 = 0x00; // iskljuci sve
				}	
			}
			// program 2
			if(P0_3 == 0)
			{
				delay(brzina);
				P2 = P2 << 1; // ukljucuj na levo
				if(P2 == 0x00)
				{
					P2 = 0x01; // ponovi ciklus
				}
			}
			// program 3
			if(P0_4 == 0)
			{
				delay(brzina);
				P2 = niz_stanja[iterator_stanja];
				iterator_stanja++;
				if(iterator_stanja > 5)
				{
					iterator_stanja = 0;
				}
			}
			// program 4
			if(P0_5 == 0)
			{
				delay(brzina);
                P2 = niz_stanja2[iterator_stanja];
				iterator_stanja++;
				if(iterator_stanja > 6)
				{
					iterator_stanja = 0;
				}

                /*
                ako ne radi prvi
                P2=0x0F;
					delay(brzina);	
					P2=0xF0;
					delay(brzina);
                */
				
			}	
		}
		else
		{
			P2 = 0x00; // kraj
		} 	
	}				
}
