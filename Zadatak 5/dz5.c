#include <at89c51rc2.h>

unsigned char brojac50ms;
unsigned char sekunda;

void obrada_prekida_tajmera_0(void) interrupt 1
{
	TH0 = 0x4B;
	TL0 = 0xFD;
	brojac50ms++;
	if(brojac50ms >= 20)
	{
		sekunda = 1;
		brojac50ms = 0;
	}
    // ciklus 1 sec.
    // 12*(1/(11059200)) = 1.085 us
    // 50ms	50000 / 1.085 = 46083 
    // 65536 - 46083 = 19453
    // 19453 -> 4BFD 0x format, nibble hi u THO, nibble low u TLO
}

void inicijalizacija_tajmera(void)
{
	TMOD = 0x01;
	TH0 = 0x4B;
	TL0 = 0xFD;
	TR0 = 1;
	ET0 = 1;
	EA = 1;
	brojac50ms = 0;
	sekunda = 0;
}

void main(void)
{
	// start        = P0_0,
    // stop         = P0_1
	// program 1    = P0_2,
    // program 2    = P0_3, 
    // program 3    = P0_4, 
    // program 4    = P0_5
	

	// inicijalizacija
	unsigned char start = 0;

	// za program 3 i 4
	unsigned char niz_stanja[6] = {0x81, 0x42, 0x24, 0x18, 0x24, 0x42};
    unsigned char niz_stanja2[7] = {0x03, 0x06, 0x0C, 0x30, 0x60, 0xC0, 0x81}; // ili bez 0x81
	unsigned char iterator_stanja = 0;

    inicijalizacija_tajmera();

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
            P2 = 0x00;
		}
		// brzina 1
		if(P0_6 == 0)
		{
			//brzina = 10;
		}
		// brzina 2
		if(P0_7 == 0)
		{
			//brzina = 5;
		}
		// start
		if(start == 1)
		{
			// program 1
			if(P0_2 == 0)
			{
				if(sekunda){
				    if(P2 == 0x00)
				    {
				    	P2 = 0xFF; // ukljuci sve
				    }
				    else
				    {
				    	P2 = 0x00; // iskljuci sve
				    }	
                    sekunda = 0;
			    }
            }
			// program 2
			if(P0_3 == 0)
			{
				if(sekunda)
				{
				    P2 = P2 << 1; // ukljucuj na levo
				    if(P2 == 0x00)
				    {
				    	P2 = 0x01; // ponovi ciklus
			    	}
                    sekunda = 0;
                }
			}
			// program 3
			if(P0_4 == 0)
			{
				if(sekunda)
				{
					P2 = niz_stanja[iterator_stanja];
					iterator_stanja++;
					if(iterator_stanja > 5)
					{
						iterator_stanja = 0;
					}
					sekunda = 0;
				}
			}
			// program 4
			if(P0_5 == 0)
			{
				if(sekunda)
                {
                    P2 = niz_stanja2[iterator_stanja]; // iteracija kroz stanja iz predefinisanog niza
				    iterator_stanja++;
			    	if(iterator_stanja > 6)
				    {
				    	iterator_stanja = 0;
				    }
                    
                    /*
                    u slucaju da ne radi iznad
                        P2=0x0F;
                        delay(brzina);	
                        P2=0xF0;
                        delay(brzina);
                    */
                   sekunda = 0;
                }	
			}	
		}
		else
		{
			P2 = 0x00; // kraj
		} 	
	}				
}