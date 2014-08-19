#ifndef READER_H_
#define READER_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define NS 61
#define NBS 21
#define BS 200
#define BUFFERS 2
#define BUFSIZE 20
#define CMDSIZE 7
#define READERS 2
//#define FIM i + READERS
#define CMD 1024
#define PUTERS 4
/*#define GETERS 3
#define DELETERS 2*/
#define GETDELETERS 4
#define PRINTERS 1
#define WORKERS PUTERS + GETDELETERS + PRINTERS
#define GET 0
#define DEL 1

typedef struct Buffer {
	char str[BUFFERS][BUFSIZE+1], *strP;
	int size[BUFFERS], turn[BUFFERS];
	pthread_cond_t condition[READERS];
	pthread_mutex_t mutex;
}Buffer;

typedef struct Element{
	char name[NS];
	char number[NBS];
	uint64_t ts;
}Element;

typedef struct Name{
	char name[NS];
	int type;
	uint64_t ts;
}Name;

typedef struct Indexs{
	uint64_t iput, igetDel, tsPut, tsGetDel;
}Indexs;

typedef struct Put{
	pthread_mutex_t mutex, lengthMutex, gandalf, im;
	pthread_cond_t condition, feanor;
	int length, iBegin/*, iEnd*/;
	uint64_t inserted, executed;
	Element e[BS];
}Put;

typedef struct GetDel{
	pthread_mutex_t mutex, lengthMutex, gandalf, im;
	pthread_cond_t condition, feanor;
	int length, iBegin/*, iEnd*/;
	uint64_t inserted, executed;
	Name e[BS];
}GetDel;


/*typedef struct Delete{
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER,
		lengthMutex = PTHREAD_MUTEX_INITIALIZER,
		gandalf = PTHREAD_MUTEX_INITIALIZER,
		im = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t condition = PTHREAD_COND_INITIALIZER,
		fëanor = PTHREAD_COND_INITIALIZER;
	int length, iBegin;
	uint64_t inserted, executed;
	Name e[BS];
}Delete;*/

typedef struct Print{
	pthread_mutex_t mutex, lengthMutex, gandalf, im;
	pthread_cond_t condition, feanor;
	int length, iBegin/*, iEnd*/;
	uint64_t inserted;
	Indexs e[BS];
}Print;

/*typedef struct PutBuffer{
	pthread_mutex_t mutex;
	Element elements[BS];
}PutBuffer;

typedef struct GetBuffer{
	pthread_mutex_t mutex;
	GetElement getElements[BS];
}GetBuffer;

typedef struct DelBuffer{
	pthread_mutex_t mutex;
	DelElement delElements[BS];
}DelBuffer;

typedef struct PrintBuffer{
	pthread_mutex_t mutex;
	PrintElement printElements[BS];
}PrintBuffer;

PutBuffer pbuf;
GetBuffer gbuf;
PrintBuffer ptbuf;
DelBuffer dbuf;*/
Put putBuffer;
GetDel getDelBuffer;
//Delete deleteBuffer;
Print printBuffer;

Element eBuffer[GETDELETERS];
int priority[BUFFERS], finish, geTurn;
uint64_t timeStamp;
char end[CMD];

pthread_mutex_t readerMutex;
pthread_cond_t readerCondition;
pthread_barrier_t barrier, ctrl;

#endif
