#include "storer.h"

unsigned int NumTree[LEVELs];

int search( Element buffer[]/*, Print ptbuf*/, int id, int wakePrint, int type/*id, uint64_t ind*/ ){
		unsigned int side = 0;
		unsigned int i = 0;
		int k;
		Tree *t = getTree( &i,buffer[id].name );
		NodeIntern **p = &t->raiz;
		NodeIntern **father = p;
		Pointer aux;
		pthread_mutex_lock(&t->mutex);
		if ( *p != NULL) {
			while( !(*p)->type  ){
				father = p;
				if( buffer[id].name[(*p)->n] > (*p)->c ){
					side = 0;
					p = &(*p)->r;
				}else{
					side = 1;
					p = &(*p)->l;
				}
			}
		}


		if( strcmp( buffer[id].name, ((Node *)*p)->e.name) == 0 ){
			/*if(type == GET){
				strcpy( e.number, ((Node *)*p)->e.number );
			}*/
			strcpy( buffer[id].name, ((Node *)*p)->e.number );
			/*pthread_mutex_lock(&searchsMutex[id]);
			pthread_mutex_unlock(&searchsMutex[(id + 1) % GETERS]);*/
			/*if( ind > printBuffer.e[printBuffer.iBegin].iget ){
				pthread_mutex_lock( &ptbuf.stopGets );
				printBuffer.stopGetCount++;
				if( printBuffer.stopGetCount != GETERS )
					pthread_mutex_unlock( &printBuffer.stopGets );
				else
					printBuffer.stopCount = 0;
			}*/
			while(id != geTurn);
			if(type == GET){
				printf( "%s %s\n", buffer[id].name, buffer[id].number );
			}else{
				aux = *father;
				if( father != p ){
					if( side ){
						*father = (*father)->r;
					}else{
						*father = (*father)->l;
					}
					free( *p );
				}else{
					t->raiz = NULL;
					/*if( i )
						i--;
					NumTree[i]--;*/
				}
				free( aux );
			}
			if(!wakePrint)
				/*pthread_barrier_wait(&getPrint);
			else*/
				geTurn = (geTurn + 1) % BS;
			pthread_mutex_unlock(&t->mutex);
			return 0;
		}else{
			pthread_mutex_unlock(&t->mutex);
			if( type == GET ){
				for(k = id + 1; k < GETDELETERS; k++){
					if(!strcmp( buffer[k].name, buffer[id].name )){
						printf( "%s %s\n", buffer[k].name, buffer[k].number );
						return 0;
					}
				}
			}
		}
	return 1;
}

NodeIntern *removeNode( char *key ){
	unsigned int side = 0;
	unsigned int i = 0;
	Tree *t = getTree( &i,key );
	NodeIntern **p = &t->raiz;
	NodeIntern **father = p;
	Pointer aux;
	pthread_mutex_lock(&t->mutex);
	if ( *p != NULL) {
		while( !(*p)->type  ){
			father = p;
			if( key[(*p)->n] > (*p)->c ){
				side = 0;
				p = &(*p)->r;
			}else{
				side = 1;
				p = &(*p)->l;
			}
		}
	}

	if( strcmp(key, ((Node *)*p)->e.name) == 0 ){
		aux = *father;
		if( father != p ){
			if( side ){
				*father = (*father)->r;
			}else{
				*father = (*father)->l;
			}
			free( *p );
		}else{
			t->raiz = NULL;
			/*if( i )
				i--;
			NumTree[i]--;*/
		}
		free( aux );
	}else{
		pthread_mutex_unlock(&t->mutex);
		return NULL;
	}
	pthread_mutex_unlock(&t->mutex);
	return (*father);
}

Tree *getTree( unsigned int *i, char name[] ){
	//unsigned int j = 0;
	int fim = 1;
	int aux2 = name[*i] - 'A';
	int aux[LEVELs +1];
	aux[0] = name[*i] - 'a';
	while( fim && *i < LEVELs ){/*arrumar maiusculas*/
		//treeInd += aux1 + pot;
		if( aux[*i] >= 0 && aux[*i] <= 25  ){
			(*i)++;
		}else{
			if( aux2 >= 0 && aux2 <= 25 ){
				(*i)++;
				aux[*i] = aux2;
			}else{
				fim = 0;
			}
		}

		aux2 = name[*i] - 'A';
		aux[*i] = name[*i] - 'a';
	}

	if( *i == 3 ){
		return (&forestLvl3[aux[0]][aux[1]][aux[2]]);
	}
	if( *i == 2 ){
		return (&forestLvl2[aux[0]][aux[1]]);
	}
	if( *i == 1 ){
		return (&forestLvl1[aux[0] + 1]);
	}
	return (&forestLvl1[0]);
}

void insert( Element e ){
	unsigned int j,i = 0;
	Tree *p = getTree( &i,e.name );
	NodeIntern **father;
	Node *newNode;
	NodeIntern *newNodeInter;
	//pthread_mutex_lock( &p->mutex );
	pthread_mutex_lock(&p->mutex);
	NodeIntern **next = &(p->raiz);
	char aux;
	j = i;
	if( *next == NULL ){
		newNode = ( Node* )malloc( sizeof( Node ) );
		strcpy( newNode->e.name, e.name );
		strcpy( newNode->e.number, e.number );
		newNode->type = 1;
		*next = newNode;
		pthread_mutex_unlock(&p->mutex);
		/*if( i )
			i--;
		NumTree[i]++;diminuir no delete*/
		return;
	}

	while( !( *next )->type  ){
		if( e.name[( *next )->n] > ( *next )->c ){
			next = &( *next )->r;
		}else{
			next = &( *next )->l;
		}
	}

	while( e.name[i] == ( (Node *)*next )->e.name[i] ) i++;
	aux = ( (Node *)*next )->e.name[i];
	father = next = &(p->raiz);
	while( ( *next )->type == 0 ){
		if( (*next )->n <= i ){
			father = next;
			if( e.name[( *next )->n] > ( *next )->c ){
				next = &( *next )->r;
			}else{
				next = &( *next )->l;
			}
		}else{
			//next = father;
			break;
		}
	}

	newNodeInter = ( NodeIntern* )malloc( sizeof( NodeIntern ) );
	newNodeInter->n = i;
	newNode = ( Node* )malloc( sizeof( Node ) );
	strcpy( newNode->e.name, e.name );
	strcpy( newNode->e.number, e.number );
	newNode->type = 1;
	newNodeInter->type = 0;
	if( (*next)->type ){
		if( e.name[i] >= ( (Node *)*next )->e.name[i] ){
			newNodeInter->r = newNode;
			newNodeInter->l = *next;
			newNodeInter->c = ( (Node *)*next )->e.name[i] ;
		}else{
			newNodeInter->c = e.name[i];
			newNodeInter->r = *next;
			newNodeInter->l = newNode;
		}
	}else{
		if( e.name[i] >= aux ){
			newNodeInter->r = newNode;
			newNodeInter->l = *next;
			newNodeInter->c = aux;
		}else{
			newNodeInter->r = *next;
			newNodeInter->l = newNode;
			newNodeInter->c = e.name[i];
		}
		//printf("-%c-\n", aux );
	}

	*next = newNodeInter;
	pthread_mutex_unlock( &p->mutex );
	/*while( *newNode != NULL ){
		i = j;
		//pthread_mutex_lock( &(*newNode)->mutex );
		father = newNode;
		while( (*newNode)->e.name[i] == e.name[i] )
			i++;
		if( i >= (*newNode)->n ){
			if( (*newNode)->e.name[i] >= e.name[i]  ){
				newNode = &(*newNode)->l;
			}else{
				newNode = &(*newNode)->r;
			}
		}else {
			aux = ( Pointer )malloc( sizeof( Node ) );
			aux->n = i;
			strcpy( aux->e.name, e.name );
			strcpy( aux->e.number, e.number );
			if( (*newNode)->e.name[i] > e.name[i] ){
				aux->l = (*newNode)->l;
				aux->r = NULL;
				if( (*newNode)->l != NULL){
					while( (*newNode)->l->e.name[i] == e.name[i] ) i++;
					(*newNode)->l->n = i;
				}

				(*newNode)->l = aux;
			}else{
				aux->r = (*newNode)->r;
				aux->l = NULL;
				if( (*newNode)->r != NULL){
					while( (*newNode)->l->e.name[i] == e.name[i] ) i++;
					(*newNode)->r->n = i;
				}
				(*newNode)->r = aux;
			}
			return;
		}

		//pthread_mutex_unlock( &(*father)->mutex );
	}

	*newNode = ( Pointer )malloc( sizeof( Node ) );
	//pthread_mutex_lock(&(*newNode)->mutex);
	(*newNode)->n = i;
	strcpy( (*newNode)->e.name, e.name );
	strcpy( (*newNode)->e.number, e.number );
	(*newNode)->l = NULL;
	(*newNode)->r = NULL;
	//pthread_mutex_unlock(&(*newNode)->mutex);*/

	/*Pointer *father = NULL;
	//pthread_mutex_unlock( &forest[treeInd].mutex );
	while( *newNode != NULL ){
		//pthread_mutex_lock( &(*newNode)->mutex );
		if( (*newNode)->e.name[(*newNode)->n] == e.name[(*newNode)->n] ){
			while( (*newNode)->e.name[i] == e.name[i] )
				i++;
			//father = newNode;
			aux = ( Pointer )malloc( sizeof( Node ) );
			aux->n = i;
			strcpy( aux->e.name, e.name );
			strcpy( aux->e.number, e.number );
			aux->l = NULL;
			aux->r = NULL;
			if( e.name[i] < (*newNode)->e.name[i] ){//arrumar maiusculas
				i = j;
				if( (*newNode)->l != NULL ){
					while((*newNode)->l->e.name[i] == e.name[i] )
						i++;
					(*newNode)->l->n = i;
					if( e.name[i] > (*newNode)->l->e.name[i] ){
						aux->l = (*newNode)->l;
						aux->r = NULL;
					}else{
						aux->r = (*newNode)->l;
						aux->l = NULL;
					}
				}
				printf("oi\n");
				(*newNode)->l = aux;

			}else{
				if( (*newNode)->r != NULL ){
					i = j;
					while((*newNode)->r->e.name[i] == e.name[i] )
						i++;
					(*newNode)->r->n = i;
					if( e.name[i] > (*newNode)->r->e.name[i] ){
						aux->l = (*newNode)->r;
						aux->r = NULL;
					}else{
						aux->r = (*newNode)->r;
						aux->l = NULL;
					}
				}
				(*newNode)->r = aux;
			}
			//pthread_mutex_unlock( &(*father)->mutex );
			return;
		}else{
			father = newNode;
			if( (*newNode)->e.name[(*newNode)->n] < e.name[(*newNode)->n] ){
				newNode = &(*newNode)->r;
			}else{
				newNode = &(*newNode)->l;
			}
		}
		//pthread_mutex_unlock( &(*father)->mutex );
	}*/


}

void inOrder( NodeIntern *next ){
	if( next->type  ){
		printf("%s %s\n", (*(Node*)next).e.name,(*(Node *)next).e.number );
		return;
	}
	//printf("%d\n", next->n);
	inOrder( next->l );
	inOrder( next->r );

}

void travel( ){
	unsigned int i,j,l;
	int aux, lv1 = ALPHABET + 1;

	if( forestLvl1[0].raiz != NULL )
		inOrder( forestLvl1[0].raiz );
	pthread_mutex_unlock( &forestLvl1[0].mutex );
	for( l = 1; l < lv1; l++ ){
		aux = l - 1;
		if( forestLvl1[l].raiz != NULL ){
			inOrder( forestLvl1[l].raiz );
		}
		pthread_mutex_unlock( &forestLvl1[l].mutex );
		for( j = 0; j < 26; j++ ){
			if( forestLvl2[aux][j].raiz != NULL ){
				inOrder( forestLvl2[aux][j].raiz);
			}
			pthread_mutex_unlock( &forestLvl2[aux][j].mutex );
		}

		for( i = 0; i < ALPHABET; i++){
			for( j = 0; j < ALPHABET; j++ ){
				if( forestLvl3[aux][i][j].raiz != NULL ){
					inOrder( forestLvl3[aux][i][j].raiz );
				}
				pthread_mutex_unlock( &forestLvl3[aux][i][j].mutex );
			}
		}

	}

	/*for(k = 0; k < LEVELs; k++){
		for( j = i = 0; i < NumTree[k]; j++ ){
			if( p[k][j].raiz != NULL ){
				inOrder( p[k][j].raiz );
				i++;
			}
			pthread_mutex_unlock(&forest[j].mutex); //lock no print
		}
	}*/
}

void setTrees( ){
	NumTree[0] = 0;
	NumTree[1] = 0;
	NumTree[2] = 0;

	//pthread_mutex_lock(&searchsMutex[1]);
	//pthread_mutex_lock(&searchsMutex[2]);

}

void lockForest( ){
	int i,k,j;
	for( i = 0; i <= ALPHABET; i++ ){
		pthread_mutex_lock( &forestLvl1[i].mutex );
	}

	for( i = 0; i < ALPHABET; i++ ){
		for( j = 0; j < ALPHABET; j++ )
			pthread_mutex_lock( &forestLvl2[i][j].mutex );
	}

	for( i = 0; i < ALPHABET; i++ ){
		for( j = 0; j < ALPHABET; j++ )
			for( k = 0; k < ALPHABET; k++ )
				pthread_mutex_lock( &forestLvl3[i][j][k].mutex );
	}
}
