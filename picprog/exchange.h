#ifndef __EXCHANGE_H

#define __EXCHANGE_H 1

/*
extern HANDLE hPort;
*/


extern int InitUSBPort(void);
extern void CloseUSBPort(void);


extern void InitByte(void);

extern void PowerUpEn(void);
extern void PowerUpDis(void);

extern void PowerDownEn(void);
extern void PowerDownDis(void);

extern void ClrMCLR(void);
extern void SetMCLR(void);

extern void VppEn(void);
extern void VppDis(void);

extern void SetPGC(void);
extern void ClrPGC(void);

extern void SetPGD(void);
extern void ClrPGD(void);

extern void DisPGD(void);


extern void PutByte(void);
extern void PutBytes(int N);
extern void Exchange(void);
extern void FreeBuf(unsigned int N);

extern void PutCommand(int Cmd);
extern void PutData(int Data);
extern void GetData(unsigned short *A);
extern void PutCmdDelay(int Cmd,int DelayLen);
extern void PutDelay(int DelayLen);

extern int ExchErr;
extern unsigned char Byte;

#endif
