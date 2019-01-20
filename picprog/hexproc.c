#include <stdio.h>

#include "variable.h"

int HexError;

int HexNibble(char *C){
	if(((*C)>='0')&&((*C)<='9'))
		return((*C)-'0');
	if(((*C)>='A')&&((*C)<='F'))
		return((*C)-'A'+10);
	HexError=1;
	return(0);
}

int HexByte(char *P){
	return((HexNibble(P)*0x10)+HexNibble(P+1));
}

int HexWord(char *P){
	return((HexByte(P)*0x100)+HexByte(P+2));
}


unsigned char *GetBufAddr(unsigned long A){
	int i;

	for(i=0;i<MAXSEGMENTNUM;i++){
		if((Segment[i].Size!=0)&&(A>=Segment[i].Addr)&&(A<(Segment[i].Addr+Segment[i].Size)))
			return(((unsigned char *)(Segment[i].Buf))+(A-Segment[i].Addr));
	}
	return(NULL);
}

unsigned char *GetUsedAddr(unsigned long A){
	int i;

	for(i=0;i<MAXSEGMENTNUM;i++){
		if((Segment[i].Size!=0)&&(A>=Segment[i].Addr)&&(A<(Segment[i].Addr+Segment[i].Size)))
			return(((unsigned char *)(Segment[i].Used))+(A-Segment[i].Addr));
	}
	return(NULL);
}

#define STRLEN 256

#define HEXBUFLEN 0x10

int ReadHex(char *Name){
	FILE *f;
	char Str[STRLEN];
	int i,Len,HexCmd,RetVal=0;
	unsigned char CS;
	unsigned long Offset;
	static unsigned long HexAddr=0;
	unsigned char HBuf[HEXBUFLEN],*Ptr;
	unsigned long Addr;

	Addr=0;HexError=0;
	printf("Reading HEX ");
	if((f=fopen(Name,"rt"))==NULL){
		printf("Can not open - %s\n",Name);return(1);
	}

	for(;;){
		if(fgets(Str,STRLEN,f)==NULL){
			if(feof(f))
				break;
			printf("read error.\n");RetVal=2;break;
		}
		if(Str[0]!=':')
			continue;
		Len=HexByte(Str+1);
		Offset=HexWord(Str+3);
		HexCmd=HexByte(Str+7);
		if(Len>HEXBUFLEN){
			printf("too many bytes in string.\n\n%s\n",Str);RetVal=3;break;
		}
		CS=(unsigned char)(Len+(Offset/0x100)+(Offset&0xff)+HexCmd);
		for(i=0;i<Len;i++){
			HBuf[i]=HexByte(Str+9+(i*2));
			CS+=HBuf[i];
		}
		CS+=HexByte(Str+9+(i*2));
		if(HexError){
			printf("hex digit error.\n\n%s\n",Str);RetVal=5;break;
		}
		if(CS!=0){
			printf("checksum error.\n\n%s\n",Str);RetVal=7;break;
		}
		if(HexCmd==0x01)
			break;
		if(HexCmd==0x04){
			Addr&=0xffff;
			Addr+=Offset<<16;
		}
		else if(HexCmd==0x00){
			HexAddr&=0xffff0000;
			HexAddr+=Offset;
			for(i=0;i<Len;i++,HexAddr++){
				Ptr=GetBufAddr(HexAddr);
				if(Ptr==NULL){
					printf("address out of range.\n\n%s\n",Str);RetVal=11;break;
				}
				*Ptr=HBuf[i];
				Ptr=GetUsedAddr(HexAddr);
				*Ptr=1;
			}
			if(i!=Len)
				break;
		}
		else{
			printf("unknown command.\n\n%s\n",Str);RetVal=6;break;;
		}
	}

	fclose(f);
	printf("O'k!\n");
	return(RetVal);
}

#define OUTNAME "out.hex"



unsigned long HexAddrHigh;
int HexCheckSum;

void PutHexNibble(unsigned char N,char *P){
	N&=0x0f;
	*P=(N<10)?('0'+N):('A'+N-10);
}

void PutHexByte(unsigned char N,char *P){
	HexCheckSum+=N;
	PutHexNibble(N,P+1);
	N>>=4;
	PutHexNibble(N,P);
}

void CodeHighAddr(char *Str){
	HexCheckSum=0;
	Str[0]=':';
	PutHexByte(2,Str+1);
	PutHexByte(0,Str+3);
	PutHexByte(0,Str+5);
	PutHexByte(4,Str+7);
	PutHexByte((unsigned char)(HexAddrHigh>>8),Str+9);
	PutHexByte((unsigned char)HexAddrHigh,Str+11);
	PutHexByte((unsigned char)(-HexCheckSum),Str+13);
	Str[15]='\n';
	Str[16]=0;
}

void CodeData(char *Str,unsigned long Addr,unsigned char *Buf,int Len){
	int i;

	HexCheckSum=0;

	Str[0]=':';
	PutHexByte((unsigned char)Len,Str+1);
	PutHexByte((unsigned char)(Addr>>8),Str+3);
	PutHexByte((unsigned char)Addr,Str+5);
	PutHexByte(0,Str+7);
	for(i=0;i<Len;i++)
		PutHexByte(Buf[i],Str+9+(2*i));
	PutHexByte((unsigned char)(-HexCheckSum),Str+9+(2*i));
	Str[11+(2*i)]='\n';
	Str[12+(2*i)]=0;
	
}

int WriteHexSegment(FILE *f,struct SegmentInfo *Seg){
	long i,BlockStart,Len;
	unsigned long NewAddr;
	char Str[2*HEXBUFLEN+20];

	for(i=0;;){
		for(;(!((unsigned char *)Seg->Used)[i])&&(i<Seg->Size);i++);
		if(i==Seg->Size)
			break;
		BlockStart=i;
		for(i++;(((unsigned char *)Seg->Used)[i])&&(i<Seg->Size)&&(((i+(Seg->Addr))%HEXBUFLEN)!=0);i++);
		Len=i-BlockStart;
		NewAddr=Seg->Addr+BlockStart;
		if(HexAddrHigh!=(NewAddr>>16)){
			HexAddrHigh=NewAddr>>16;
			CodeHighAddr(Str);
			if(fputs(Str,f)==EOF)
				return(1);
		}
		CodeData(Str,NewAddr,((unsigned char *)(Seg->RBuf))+BlockStart,Len);
		if(fputs(Str,f)==EOF)
			return(2);
	}
	return(0);
}


int WriteHex(void){
	FILE *f;
	int i;

	if((f=fopen(OUTNAME,"wt"))==NULL){
		printf("can not open file %s\n",OUTNAME);return(1);
	}


	HexAddrHigh=0xffffffff;
	

	for(i=0;i<MAXSEGMENTNUM;i++){
		if(WriteHexSegment(f,Segment+i)!=0){
			fclose(f);return(2);
		}
	}

	if(fputs(":00000001FF\n",f)==EOF){
		fclose(f);return(3);
	}

	fclose(f);
	return(0);
}

