#ifndef __VARIABLE_H

#define __VARIABLE_H 1

#include <stdio.h>
#include "picprog.h"

extern struct SegmentInfo Segment[MAXSEGMENTNUM];

extern struct SegmentInfo *CurS;
/*
extern unsigned long Addr;
*/
extern char VppStr[];

extern int (*StartProc)(void);
extern int (*StopProc)(void);
extern int (*EnterProc)(void);
extern int (*ExitProc)(void);
extern int (*EAProc)(void);
extern int (*MKProc)(void);

extern int (*ReadProc)(void);
extern int (*WriteProc)(void);
extern int (*VerifyProc)(void);

extern FILE *flog;

extern void Message(char *Str);

#endif