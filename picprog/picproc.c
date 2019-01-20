#include <stdio.h>
#include <string.h>

#include "picprog.h"
#include "variable.h"
#include "exchange.h"

int Start16f628(void){
	InitByte();ClrPGD();PutByte();
	Exchange();
	return(ExchErr);
}

int Stop16f628(void){
	InitByte();PutByte();
	Exchange();
	return(ExchErr);
}

int Start16f877a(void){
	InitByte();ClrPGD();PutByte();
	PowerUpEn();PutByte();
	PutDelay(100000);
	Exchange();
	return(ExchErr);
}

int Stop16f877a(void){
	PowerUpDis();PutByte();
	PutDelay(1000);
	PowerDownEn();PutByte();
	PutDelay(100000);
	PowerDownDis();PutByte();
	PutDelay(1000);
	InitByte();PutByte();
	Exchange();
	return(ExchErr);
}

int Enter16f628(void){
	ClrPGD();ClrPGC();PutByte();
	PowerUpEn();PutByte();
	PutDelay(12000);

	VppEn();PutByte();
	PutDelay(100000);
	Exchange();
	return(ExchErr);
}

int Exit16f628(void){
	PowerUpDis();PutByte();
	PutDelay(1000);
	PowerDownEn();PutByte();
	PutDelay(100000);
	VppDis();PutByte();
	PutDelay(1000);
	PowerDownDis();PutByte();
	PutDelay(10000);
	Exchange();
	return(ExchErr);
}

int Enter16f877a(void){
	VppEn();PutByte();
	Exchange();
	return(ExchErr);
}

int Exit16f877a(void){
	VppDis();PutByte();
	Exchange();
	return(ExchErr);
}

void IncAddr(void){
	PutCommand(0x06);
}

void SelectConfig(void){
	PutCommand(0x00);
	PutData(0x3fff);
}

void ReadProgramWord(unsigned short *A){
	PutCommand(0x04);
	GetData(A);
}

void WriteProgramWord(int W){
	PutCommand(0x02);
	PutData(W);
	PutCommand(0x08);
	PutDelay(8000);
}

void WriteProgramWord10f206(int W){
	PutCommand(0x02);
	PutData(W);
	PutCmdDelay(0x08,2000);
	PutCommand(0x0e);
	PutDelay(200);
}

void ReadDataWord(unsigned short *A){
	PutCommand(0x05);
	GetData(A);
}

void WriteDataWord(int W){
	PutCommand(0x03);
	PutData(W);
	PutCommand(0x08);
	PutDelay(8000);
}



int EA16f628(void){
	int i;
	
	if((*EnterProc)()==0){
		SelectConfig();
		for(i=0;i<7;i++)
			IncAddr();
		PutCommand(0x01);
		PutCommand(0x07);
		PutCommand(0x08);
		PutDelay(8000);
		PutCommand(0x01);
		PutCommand(0x07);
	}

	(*ExitProc)();

	return(ExchErr);
}

int EA16f628a(void){

	if((*EnterProc)()==0){
		SelectConfig();
		PutCommand(0x09);
		PutDelay(8000);
		PutCommand(0x0b);
		PutDelay(8000);
	}

	(*ExitProc)();

	return(ExchErr);
}

int EA12f629(void){
	int i;
	
	if((*EnterProc)()==0){
		SelectConfig();
		for(i=0;i<7;i++)
			IncAddr();
		PutCommand(0x09);
		PutDelay(8000);
		PutCommand(0x0b);
		PutDelay(8000);
	}

	(*ExitProc)();

	return(ExchErr);
}


int EA10f200(void){
	if((*EnterProc)()==0){
		PutCommand(0x09);
		PutDelay(10000);
	}

	(*ExitProc)();

	return(ExchErr);
}

#define KM12f629 0x3e00
#define CFGOFF12f629 7

int MK12f629(void){
	int A;

	A=Segment[CODE].Len-1;
	if((Segment[CODE].Used[A])&&((Segment[CODE].Buf[A]&0x3c00)==0x3400)){
		if((Segment[CODE].RBuf[A]&0x3c00)!=0x3400)
			Message("Warning - OSCCAL word is not RETLW, substitution skipped ");
		else
			Segment[CODE].Buf[A]=Segment[CODE].RBuf[A];
	}

	if(Segment[CONFIG].Used[CFGOFF12f629]){
		Segment[CONFIG].Buf[CFGOFF12f629]&=~KM12f629;
		Segment[CONFIG].Buf[CFGOFF12f629]|=Segment[CONFIG].RBuf[CFGOFF12f629]&KM12f629;
/*		Segment[CONFIG].RBuf[CFGOFF12f629]=Segment[CONFIG].Buf[CFGOFF12f629];*/
	}

	return(0);
}


int MK10f200(void){
	int A;

	A=Segment[CODE].Len-1;
	if((Segment[CODE].Used[A])&&((Segment[CODE].Buf[A]&0xf00)==0xc00)){
		if((Segment[CODE].RBuf[A]&0xf00)!=0xc00)
			Message("Warning - OSCCAL word is not MOVLW, substitution skipped ");
		else
			Segment[CODE].Buf[A]=Segment[CODE].RBuf[A];
	}
	return(0);
}


int ReadSegment(void){
	long i,LastUsed;

	if((*EnterProc)()==0){
		for(LastUsed=(CurS->Len)-1;LastUsed>=0;LastUsed--){
			if(CurS->Used[LastUsed])
				break;
		}
		(*(CurS->Select))();
		for(i=0;(i<=LastUsed)&&(!ExchErr);i++){
			if((i&0xf)==0)
				printf(".");
			if(CurS->Used[i])
				(*(CurS->Read))((CurS->RBuf)+i);
			IncAddr();
		}
	}

	(*ExitProc)();

	return(ExchErr);

}

int WriteSegment(void){
	long i,LastUsed;

	if((*EnterProc)()==0){
		for(LastUsed=(CurS->Len)-1;LastUsed>=0;LastUsed--){
			if(CurS->Used[LastUsed])
				break;
		}
		(*(CurS->Select))();
		for(i=0;(i<=LastUsed)&&(!ExchErr);i++){
			if((i&0xf)==0)
				printf(".");
			if(CurS->Used[i])
				(*(CurS->Write))(CurS->Buf[i]);
			IncAddr();
		}
	}

	(*ExitProc)();

	return(ExchErr);

}

int VerifySegment(void){
	unsigned int i;

	for(i=0;i<CurS->Len;i++){
		if(CurS->Used[i]){
			if(CurS->Buf[i]!=CurS->RBuf[i]){
				fprintf(flog,"\nerror A=%08x D=%04x R=%04x ",((CurS->Addr)/sizeof(short int))+i,CurS->Buf[i],CurS->RBuf[i]);
				ExchErr|=8;
			}
		}
	}

	return(ExchErr);
}


void Empty(void){

}

int SetParameters(char *PName){
	int i;

	Segment[CODE].Addr=0;
	Segment[CONFIG].Addr=0x2000;
	Segment[EEDATA].Addr=0x2100;

	strcpy(VppStr,"10.0 - 13.0");

	StartProc=Start16f628;
	StopProc=Stop16f628;
	EnterProc=Enter16f628;
	ExitProc=Exit16f628;

	ReadProc=ReadSegment;
	WriteProc=WriteSegment;
	VerifyProc=VerifySegment;

	EAProc=EA16f628a;

	Segment[CODE].Select=Empty;
	Segment[CODE].Read=ReadProgramWord;
	Segment[CODE].Write=WriteProgramWord;

	Segment[CONFIG].Select=SelectConfig;
	Segment[CONFIG].Read=ReadProgramWord;
	Segment[CONFIG].Write=WriteProgramWord;

	Segment[EEDATA].Select=Empty;
	Segment[EEDATA].Read=ReadDataWord;
	Segment[EEDATA].Write=WriteDataWord;

	if(strcmp(PName,"16f628")==0){
		strcpy(VppStr,"10.0 - 13.5");

		Segment[CODE].Len=0x800;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0x80;

		EAProc=EA16f628;
	}
	else if(strcmp(PName,"16f628a")==0){
		strcpy(VppStr,"10.0 - 13.5");

		Segment[CODE].Len=0x800;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0x80;
	}
	else if(strcmp(PName,"16f648a")==0){
		strcpy(VppStr,"10.0 - 13.5");

		Segment[CODE].Len=0x1000;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0x100;
	}
	else if(strcmp(PName,"12f629")==0){
		strcpy(VppStr,"10.0 - 13.5");

		Segment[CODE].Len=0x400;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0x80;

		EAProc=EA12f629;
		MKProc=MK12f629;
	}
	else if(strcmp(PName,"12f675")==0){
		strcpy(VppStr,"10.0 - 13.5");

		Segment[CODE].Len=0x400;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0x80;

		EAProc=EA12f629;
		MKProc=MK12f629;
	}
	else if(strcmp(PName,"16f676")==0){
		strcpy(VppStr,"10.0 - 13.5");

		Segment[CODE].Len=0x400;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0x80;

		EAProc=EA12f629;
		MKProc=MK12f629;
	}
	else if(strcmp(PName,"12f683")==0){
		Segment[CODE].Len=0x800;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0x100;
	}
	else if(strcmp(PName,"16f688")==0){
		Segment[CODE].Len=0x1000;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0x100;
	}
	else if(strcmp(PName,"16f690")==0){
		Segment[CODE].Len=0x1000;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0x100;
	}
	else if(strcmp(PName,"16f887")==0){
		strcpy(VppStr,"10.0 - 12.0");

		Segment[CODE].Len=0x2000;
		Segment[CONFIG].Len=0x9;
		Segment[EEDATA].Len=0x100;

		EAProc=EA10f200;
	}
	else if(strcmp(PName,"10f206")==0){
		strcpy(VppStr,"12.5 - 13.5");

		Segment[CODE].Len=0x200;
		Segment[CONFIG].Addr=0xfff;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0;

		Segment[CODE].Select=IncAddr;
		Segment[CONFIG].Select=Empty;
		
		Segment[CODE].Write=WriteProgramWord10f206;
		Segment[CONFIG].Write=WriteProgramWord10f206;
		
		MKProc=MK10f200;
		EAProc=EA10f200;
	}
	else if(strcmp(PName,"10f200")==0){
		strcpy(VppStr,"12.5 - 13.5");

		Segment[CODE].Len=0x100;
		Segment[CONFIG].Addr=0xfff;
		Segment[CONFIG].Len=8;
		Segment[EEDATA].Len=0;

		Segment[CODE].Select=IncAddr;
		Segment[CONFIG].Select=Empty;

		Segment[CODE].Write=WriteProgramWord10f206;
		Segment[CONFIG].Write=WriteProgramWord10f206;

		MKProc=MK10f200;
		EAProc=EA10f200;
	}

	else
		return(1);

	for(i=0;i<MAXSEGMENTNUM;i++){
		Segment[i].Addr*=sizeof(short int); 
		Segment[i].Size=Segment[i].Len * sizeof(short int);
	}

	return(0);
}
