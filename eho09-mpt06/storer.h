/*
 * storer.h
 *
 *  Created on: 27/05/2012
 *      Author: edi
 */

#ifndef STORER_H_
#define STORER_H_

#include "reader.h"

#define TREESIZE 18252
#define LEVELs 3
#define ALPHABET 26

typedef void *Pointer;
typedef struct NodeIntern{
	int type;
	int n;
	char c;
	Pointer l;
	Pointer r;
	pthread_mutex_t mutex;
}NodeIntern;

typedef struct Node{
	int type;
	Element e;
	pthread_mutex_t mutex;
}Node;

typedef struct Tree{
	pthread_mutex_t mutex;
	Pointer raiz;
}Tree;

typedef struct Forest{
	unsigned int NumTree[LEVELs];
}Forest;

Tree forestLvl1[ALPHABET + 1];
Tree forestLvl2[ALPHABET][ALPHABET];
Tree forestLvl3[ALPHABET][ALPHABET][ALPHABET];
//pthread_mutex_t searchsMutex[GETDELETERS];
unsigned int NumTree[LEVELs];

int search( Element []/*, Print*/, int, int, int );
NodeIntern *removeNode( char key[] );
void insert( Element e );
void inOrder( NodeIntern *p );
void travel( );
void setTrees( );
Tree *getTree( unsigned int *i, char *name );
void lockForest( );
#endif /* STORER_H_ */
