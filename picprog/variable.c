#include <stdio.h>

#include "picprog.h"

unsigned short int CodeBuf[MAXCODELEN];
unsigned short int CodeRBuf[MAXCODELEN];
unsigned short int CodeUsed [MAXCODELEN]={0};

unsigned short int ConfigBuf[MAXCONFIGLEN];
unsigned short int ConfigRBuf[MAXCONFIGLEN];
unsigned short int ConfigUsed [MAXCONFIGLEN]={0};

unsigned short int EEDataBuf[MAXEEDATALEN];
unsigned short int EEDataRBuf[MAXEEDATALEN];
unsigned short int EEDataUsed [MAXEEDATALEN]={0};



struct SegmentInfo Segment[MAXSEGMENTNUM]={
	{CodeBuf,CodeRBuf,CodeUsed,0,0,0,NULL,NULL,NULL},
	{ConfigBuf,ConfigRBuf,ConfigUsed,0,0,0,NULL,NULL,NULL},
	{EEDataBuf,EEDataRBuf,EEDataUsed,0,0,0,NULL,NULL,NULL}
};

struct SegmentInfo *CurS;
/*
unsigned long Addr;
*/
char VppStr[256]="";

int (*StartProc)(void)=NULL;
int (*StopProc)(void)=NULL;
int (*EnterProc)(void)=NULL;
int (*ExitProc)(void)=NULL;
int (*EAProc)(void)=NULL;
int (*MKProc)(void)=NULL;

int (*ReadProc)(void)=NULL;
int (*WriteProc)(void)=NULL;
int (*VerifyProc)(void)=NULL;

FILE *flog;

void Message(char *Str){
	printf("%s",Str);
	fprintf(flog,"%s",Str);
}


