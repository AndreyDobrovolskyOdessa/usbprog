#ifndef __PICPROG_H

#define __PICPROG_H 1

#define MAXCODELEN 0x10000
#define MAXCONFIGLEN 0x100
#define MAXEEDATALEN 0x1000


struct SegmentInfo {
	unsigned short int *Buf;
	unsigned short int *RBuf;
	unsigned short int *Used;
	unsigned int Addr;
	unsigned int Len;
	unsigned int Size;
	void (*Select)(void);
	void (*Read)(unsigned short *);
	void (*Write)(int);

};

enum SegmentName {CODE=0,CONFIG,EEDATA,MAXSEGMENTNUM};


#endif
