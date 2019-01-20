#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


#include "variable.h"
#include "exchange.h"
#include "hexproc.h"
#include "picproc.h"



int Execute(char *Name,int (*Proc)(void),struct SegmentInfo *S){
	CurS=S;
	Message(Name);
	if(Proc==NULL){
		ExchErr|=0x20;
		Message("not available ");
	}
	else{
		if((*Proc)()==0)
			Message("O'k!\n");
		else
			Message("error\n");
	}

	return(ExchErr);
}

int main(int argc,char **argv){
	int Key,Err;
	char Str[256];

	if(argc<4){
		printf("\nSyntax : usbprog processor commands hexfile\n\n");
		printf("Commands: rp rd rc mk sa ea wp wd wc vp vd vc\n\n");/*fgets(Str,10,stdin);*/
		return(1);
	}


	if(SetParameters(argv[1])!=0){
		printf("\nUnknown processor type - %s.\n",argv[1]);/*fgets(Str,10,stdin);*/return(3);
	}

	if(InitUSBPort()!=0){
		printf("Can not open USB port.\n");/*fgets(Str,10,stdin);*/return(2);
	}

	if((flog=fopen("usbprog.log","at"))==NULL){
		printf("\nCan not open log file.\n");CloseUSBPort();/*fgets(Str,10,stdin);*/return(1);
	}

	fprintf(flog,"\n\nProgramming .. %s %s %s\n",argv[1],argv[2],argv[3]);

	printf("setvbuf = %d\n",setvbuf(stdout,NULL,_IONBF,0));

	for(;;){

		printf("\nConnect processor and check VPP = %s\n\n",VppStr);

		printf("Continue? [YN] : ");
		for(;;){
			Key=toupper(fgetc(stdin));
			if((Key=='Y')||(Key=='N'))
				break;
		}
		printf("\n");

		if(Key=='N')
			break;

		/*Test

		(*StartProc)();
		(*EnterProc)();

		Exchange();

		int i;

		for(i=0;i<1000;i++){
			SetPGD();
			PutDelay(12000);
			ClrPGD();
			Exchange();
		}

		(*ExitProc)();
		(*StopProc)();

		break;

		Test*/

		for(;;){
			if(ReadHex(argv[3])!=0){
				ExchErr|=0x10;
				break;
			}

			if(Execute("Starting ",StartProc,NULL)!=0)
				break;
		
			if((strstr(argv[2],"rp")!=NULL)||(strstr(argv[2],"mk")!=NULL)){
				if(Execute("Read Program ",ReadProc,Segment+CODE)!=0)
					break;
			}

			if(strstr(argv[2],"rd")!=NULL){
				if(Execute("Read Data ",ReadProc,Segment+EEDATA)!=0)
					break;
			}

			if((strstr(argv[2],"rc")!=NULL)||(strstr(argv[2],"mk")!=NULL)){
				if(Execute("Read Configuration ",ReadProc,Segment+CONFIG)!=0)
					break;
			}

			if(strstr(argv[2],"mk")!=NULL){
				if(Execute("Merge caLibration ",MKProc,NULL)!=0)
					break;
			}

			if(strstr(argv[2],"sa")!=NULL){
				if(Execute("Save All ",WriteHex,NULL)!=0)
					break;
			}

			if(strstr(argv[2],"ea")!=NULL){
				if(Execute("Erase All ",EAProc,NULL)!=0)
					break;
			}

			if(strstr(argv[2],"wp")!=NULL){
				if(Execute("Write Program ",WriteProc,Segment+CODE)!=0)
					break;
			}

			if(strstr(argv[2],"wd")!=NULL){
				if(Execute("Write Data ",WriteProc,Segment+EEDATA)!=0)
					break;
			}

			if((strstr(argv[2],"wp")!=NULL)||(strstr(argv[2],"vp")!=NULL)){
				if(Execute("Read Program ",ReadProc,Segment+CODE)!=0)
					break;
				if(Execute("Verify Program ",VerifyProc,Segment+CODE)!=0)
					break;
			}

			if((strstr(argv[2],"wd")!=NULL)||(strstr(argv[2],"vd")!=NULL)){
				if(Execute("Read Data ",ReadProc,Segment+EEDATA)!=0)
					break;
				if(Execute("Verify Data ",VerifyProc,Segment+EEDATA)!=0)
					break;
			}

			if(strstr(argv[2],"wc")!=NULL){
				if(Execute("Write Configuration ",WriteProc,Segment+CONFIG)!=0)
					break;
			}

			if((strstr(argv[2],"wc")!=NULL)||(strstr(argv[2],"vc")!=NULL)){
				if(Execute("Read Configuration ",ReadProc,Segment+CONFIG)!=0)
					break;
				if(Execute("Verify Configuration ",VerifyProc,Segment+CONFIG)!=0)
					break;
			}

			break;
		}

		Err=ExchErr;ExchErr=0;
		Execute("Stopping ",StopProc,NULL);
		ExchErr|=Err;

		if(ExchErr){
			printf("Disconnect processor. Press ENTER.");fgets(Str,10,stdin);
			break;
		}

	}

	if(ExchErr)
		Message("\nfailed\n\n");
	else
		Message("\nCongratulations!\n\n");

	CloseUSBPort();

	fclose(flog);

	return(ExchErr);
}
