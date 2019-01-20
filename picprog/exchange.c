#include "ftd2xx.h"

#include "ft245pins.h"

HANDLE hPort;

#define MINBLOCKLEN 62
#define BLOCKNUM 0x100
#define MAXBLOCKLEN (MINBLOCKLEN*BLOCKNUM)


FT_HANDLE ftPort;




unsigned char TBuf[MAXBLOCKLEN+0x100],RBuf[MAXBLOCKLEN+0x100];
unsigned long Len=0;
unsigned char Byte;

void InitByte(void){Byte=(_PWRUP | _MCLR);}

void PowerUpEn(void){Byte&=~_PWRUP;}
void PowerUpDis(void){Byte|=_PWRUP;}

void PowerDownEn(void){Byte|=PWRDN;}
void PowerDownDis(void){Byte&=~PWRDN;}

void ClrMCLR(void){Byte|=_MCLR;}
void SetMCLR(void){Byte&=~_MCLR;}

void VppEn(void){Byte|=VPP;}
void VppDis(void){Byte&=~VPP;}

void SetPGC(void){Byte|=PGC;}
void ClrPGC(void){Byte&=~PGC;}

void SetPGD(void){Byte|=PGDUP;Byte|=_PGDDN;}
void ClrPGD(void){Byte&=~PGDUP;Byte&=~_PGDDN;}

void DisPGD(void){Byte&=~PGDUP;Byte|=_PGDDN;}


struct DataPosInfo{
	unsigned int Pos;
	unsigned short *Data;
	int BitCnt;
};

#define MAXDATAPOSINFO (MAXBLOCKLEN/16)

struct DataPosInfo DataPosFIFO[MAXDATAPOSINFO];

int Head=0,Tail=0;

int NextDataPos(int Cur){
	return(((++Cur)==MAXDATAPOSINFO)?0:Cur);
}


#define CMDLEN 6
#define DATALEN 16

void ExtractAllData(void){
	struct DataPosInfo *DPtr;

	for(;Head!=Tail;Head=NextDataPos(Head)){
		for(DPtr=DataPosFIFO+Head;(DPtr->BitCnt>0)&&(DPtr->Pos<Len);(DPtr->Pos)+=2,(DPtr->BitCnt)--){
			if(RBuf[DPtr->Pos]&PGDIN)
				*(DPtr->Data)|=1<<(DATALEN-2);
			*(DPtr->Data)>>=1;
		}
		if((DPtr->Pos)>=Len){
			DPtr->Pos-=Len;
			break;
		}
	}
}

int ExchErr=0;

void _Exchange(void){
	unsigned int Done;

	if(Len==0)
		return;

	if(FT_Write(ftPort,TBuf,Len,&Done)!=FT_OK)
		ExchErr|=1;
	if(FT_Read(ftPort,RBuf,Len,&Done)!=FT_OK)
		ExchErr|=2;
	if(Done!=Len)
		ExchErr|=4;
	
	
	ExtractAllData();

	Len=0;
}


void PutByte(void){
	TBuf[Len++]=Byte;
	if(Len>=MAXBLOCKLEN)
		_Exchange();
}

void PutBytes(int N){
	while(N--)
		PutByte();
}

void Exchange(void){
	while(Len>0)
		PutByte();
}

void FreeBuf(unsigned int N){
	if(N<=MAXBLOCKLEN){
		if((MAXBLOCKLEN-Len)<N)
			Exchange();
	}
}

void PutCommand(int Cmd){
	int i;

	for(i=0;i<CMDLEN;i++,Cmd>>=1){
		SetPGC();
		if(Cmd&1)
			SetPGD();
		else
			ClrPGD();
		PutByte();
		ClrPGC();PutByte();
	}
}


void PutData(int Data){
	int i;

	for(i=0,Data<<=1;i<DATALEN;i++,Data>>=1){
		SetPGC();
		if(Data&1)
			SetPGD();
		else
			ClrPGD();
		PutByte();
		ClrPGC();PutByte();
	}
}


void GetData(unsigned short *A){
	int i;

	DataPosFIFO[Tail].Pos=Len+4;
	DataPosFIFO[Tail].Data=A;*A=0;
	DataPosFIFO[Tail].BitCnt=DATALEN-2;
	Tail=NextDataPos(Tail);
	DisPGD();PutByte();
	for(i=0;i<DATALEN;i++){
		SetPGC();PutByte();
		ClrPGC();PutByte();
	}
	/*ClrPGD();PutByte();*/
}

#define BR 57600
#define DELAY_DIVIDER ((57600*4)/BR)

int InitUSBPort(void){

	FT_STATUS St;

	St=FT_Open(0,&ftPort);
	if(St!=FT_OK)
		return(1);

	St=FT_SetUSBParameters(ftPort,64*BLOCKNUM,0);
	if(St!=FT_OK)
		return(2);

	St=FT_SetLatencyTimer(ftPort,2);
	if(St!=FT_OK)
		return(2);

	St=FT_SetBitMode(ftPort,(unsigned char)~PGDIN,0x04);
	if(St!=FT_OK)
		return(3);
/*
	St=FT_SetBaudRate(ftPort,BR);
	if(St!=FT_OK)
		return(4);
*/
	St=FT_SetDivisor(ftPort,1);
	if(St!=FT_OK)
		return(4);

	InitByte();PutByte();
	Exchange();

	return(ExchErr);

}


void CloseUSBPort(void){
	FT_Close(ftPort);
}

#define N1	64
#define T1L	36
#define T1H	57

#define N2	256
#define T2L	210
#define T2H	250

#define N3	MAXBLOCKLEN
#define T3L	30000
#define T3H	55000


void PutCmdDelay(int Cmd,int DelayLen){
	int WholeN;
	long N;
	
	WholeN=DelayLen/T3H;
	DelayLen%=T3H;
	
	if(DelayLen>T3L){
		WholeN++;N=0; /* Warning */
	}
	else if(DelayLen>=T2H){
		N=DelayLen-T2H;N*=N3-N2;N/=T3L-T2H;N+=N2;
	}
	else if(DelayLen>T2L){
		N=N2; /* Warning */
	}
	else if(DelayLen>=T1H){
		N=DelayLen-T1H;N*=N2-N1;N/=T2L-T1H;N+=N1;
	}
	else if(DelayLen>T1L){
		N=N1; /* Warning */
	}
	else{
		N=DelayLen;N*=N1;N/=T1L;
	}
	
	if(Cmd!=(-1)){
		Exchange();
		PutCommand(Cmd);
		N-=CMDLEN*2;
	}

	PutBytes(WholeN*N3+(int)N);
}


void PutDelay(int DelayLen){
	PutCmdDelay(-1,DelayLen);
}
