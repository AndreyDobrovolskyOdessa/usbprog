#include <stdio.h>
#include <string.h>

#include "exchange.h"
#include "ft245pins.h"

#define STRLEN 80

void ShowExchByte(unsigned char B){
	printf("\n");
	printf("_PwrUp=%d ",(Byte&_PWRUP)?1:0);
	printf("PwrDn=%d ",(Byte&PWRDN)?1:0);
	printf("VppEn=%d ",(Byte&VPP)?1:0);
	printf("_MCLR=%d ",(Byte&_MCLR)?1:0);
	printf("PGC=%d ",(Byte&PGC)?1:0);
	printf("PGDUp=%d ",(Byte&PGDUP)?1:0);
	printf("_PGDDn=%d ",(Byte&_PGDDN)?1:0);
	printf("\n\n");
}

int main(void){
	char Str[STRLEN];

	if(InitUSBPort()!=0){
		printf("Can not open USB port.\n\n");fgets(Str,STRLEN,stdin);return(1);
	}


	for(;;){
		ShowExchByte(Byte);
		printf("Commands : exit pu pd\n\nCmd : ");
		fgets(Str,STRLEN,stdin);
		if(strcmp(Str,"exit\n")==0)
			break;
		else if(strcmp(Str,"pu\n")==0){
			PowerUpEn();PutDelay(100000);Exchange();
		}
		else if(strcmp(Str,"pd\n")==0){
			PowerUpDis();PutDelay(1000);
			PowerDownEn();PutDelay(100000);
			PowerDownDis();PutDelay(10000);
			Exchange();
		}
		printf("\nExchErr=%d\n",ExchErr);
	}

	CloseUSBPort();

	return(0);
}