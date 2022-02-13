/* taster:
0-start
1-stop
2-program1
3-program2
4-program3
5-program4
6-brzina1
7-brzina2
*/
			

void delay(int brzina){

	unsigned int i,j;

	if(brzina==1)
	{
		for(i=0; i<400; i++){
		for(j=0;j<400; j++);
		}
	}

	if(brzina==2)
	{
		for(i=0; i<280; i++){
		for(j=0;j<280; j++);
		}	
	}
}

void main(void)
{
	unsigned int brzina=1;
	unsigned int program=1;
	unsigned int radi=0;
	unsigned char pom1, pom2, pom3, pom4, pom5;
	P2=0x00;


while(1){
	if(P0_0==0){		//start
			radi=1;
			P2=0x00;
			pom3=0xFF;
		}
	if(P0_1==0){		//stop
			radi=0;
			P2=0x00;
		}
		delay(brzina);
	if(radi==1){
			if(P0_2==0){		    //program 1
					program=1;
					P2=0x00;
					pom3=0xFF;
			}
			if(P0_3==0){			//program 2
					program=2;
					P2=0x01;
		
			}
			if(P0_4==0){			//program 3
					program=3;
					pom1=0x80;
					pom2=0x01;
					P2=pom1|pom2;	
			}
			if(P0_5==0){		   	//program 4
					program=4;
					pom4=0xAA;
					P2=0x00;
					pom5=0x55;
			}

			if(P0_6 == 0){		   	//brzina 1
					brzina=1;
			}

			if(P0_7==0){		   	//brzina 2
					brzina=2;
			}

			if(program==1)
			{
				  P2=P2^pom3;
			}
			if(program==2)
			{
				delay(brzina);
				P2=	P2<<1;

				if(P2==0){
				
					P2=0x01;
				}
			}
			if(program==3)
			{
				pom2=pom2<<1;
				pom1=pom1>>1;
	
					if(pom1==0x01)
					{
						pom1=0x80;
						pom2=0x01;
					}
					 if(pom1==0x10)
					{
					pom2=pom2<<1;
					pom1=pom1>>1;
					}

					P2=pom2|pom1;
			}
			if(program==4)
			{
					P2=0x55;
					delay(brzina);	
					P2=0xAA;
					delay(brzina);
			}
	}

} 
}
