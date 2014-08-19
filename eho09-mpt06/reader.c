#include "storer.h"

void *put(void *arg){
	Element e;
	//int wakePrint = 0;

	pthread_mutex_lock(&putBuffer.mutex);
	pthread_mutex_lock(&putBuffer.gandalf);
	while(!putBuffer.length && !finish)
		pthread_cond_wait(&putBuffer.feanor, &putBuffer.gandalf);
	pthread_mutex_unlock(&putBuffer.gandalf);

	while(putBuffer.length || !finish) {

		pthread_mutex_lock(&putBuffer.lengthMutex);
		putBuffer.length--;
		pthread_mutex_unlock(&putBuffer.lengthMutex);
		pthread_cond_signal(&putBuffer.condition);

		memset(e.name, '\0', NS);
		strcpy(e.name, putBuffer.e[putBuffer.iBegin].name);
		strcpy(e.number, putBuffer.e[putBuffer.iBegin].number);
		if(putBuffer.e[putBuffer.iBegin].ts == printBuffer.e[printBuffer.iBegin].tsPut) {
			putBuffer.iBegin = (putBuffer.iBegin + 1) % BS;
			puts("PUT WAKE PRINT");
		} else {
			putBuffer.iBegin = (putBuffer.iBegin + 1) % BS;
			puts("PUT DONT WAKE PRINT");
			pthread_mutex_unlock(&putBuffer.mutex);
		}
		printf("LENGTH: %d\n", putBuffer.length);
		insert( e );
		pthread_mutex_lock(&putBuffer.im);
		putBuffer.executed++;
		pthread_mutex_unlock(&putBuffer.im);
		pthread_mutex_lock(&putBuffer.mutex);
		pthread_mutex_lock(&putBuffer.gandalf);
		while(!putBuffer.length && !finish)
			pthread_cond_wait(&putBuffer.feanor, &putBuffer.gandalf);
		pthread_mutex_unlock(&putBuffer.gandalf);
	}
	puts("PUTS FIM ...");
	pthread_mutex_unlock(&putBuffer.mutex);
	pthread_exit( NULL );
	return (arg);
}

void *getDel(void *arg){
	Element e;
	int wakePrint = 0, id/* = *((int *)arg)*/;
	int type;

	pthread_mutex_lock( &getDelBuffer.mutex );
	pthread_mutex_lock(&getDelBuffer.gandalf);
	while(!getDelBuffer.length && !finish)
		pthread_cond_wait(&getDelBuffer.feanor, &getDelBuffer.gandalf);
	pthread_mutex_unlock(&getDelBuffer.gandalf);

	while(getDelBuffer.length || !finish) {

		pthread_mutex_lock(&getDelBuffer.lengthMutex);
		getDelBuffer.length--;
		pthread_mutex_unlock(&getDelBuffer.lengthMutex);
		pthread_cond_signal(&getDelBuffer.condition);

		memset(e.name, '\0', NS);
		strcpy( eBuffer[id].name, getDelBuffer.e[getDelBuffer.iBegin].name );
		type = getDelBuffer.e[getDelBuffer.iBegin].type;
		//strcpy( e.name, getDelBuffer.e[getDelBuffer.iBegin].name );
		/*if(getBuffer.iBegin == printBuffer.e[printBuffer.iBegin].iget) {
			getBuffer.iBegin = (getBuffer.iBegin + 1) % BS;
			wakePrint= 1;
		} else {
			getBuffer.iBegin = (getBuffer.iBegin + 1) % BS;
			pthread_mutex_unlock( &getBuffer.mutex );
		}*/
		id = getDelBuffer.iBegin;
		if(getDelBuffer.e[getDelBuffer.iBegin].ts == printBuffer.e[printBuffer.iBegin].tsGetDel)
			wakePrint= 1;
		getDelBuffer.iBegin = (getDelBuffer.iBegin + 1) % BS;
		pthread_mutex_unlock( &getDelBuffer.mutex );
		//while( search( e, printBuffer, id, i ) );
		while( search( eBuffer/*, printBuffer*/, id, wakePrint, type ) );
		pthread_mutex_lock(&getDelBuffer.im);
		getDelBuffer.executed++;
		pthread_mutex_unlock(&getDelBuffer.im);
		wakePrint =0;
		pthread_mutex_lock( &getDelBuffer.mutex );
		pthread_mutex_lock(&getDelBuffer.gandalf);
		while(!getDelBuffer.length && !finish)
			pthread_cond_wait(&getDelBuffer.feanor, &getDelBuffer.gandalf);
		pthread_mutex_unlock(&getDelBuffer.gandalf);
	}
	puts("GET FIM ...");
	pthread_mutex_unlock( &getDelBuffer.mutex );
	pthread_exit( NULL );
	return (arg);
}

/*void *delete(void *arg){
	Name e;
	NodeIntern *ni;
	//int wakePrint = 0;

	pthread_mutex_lock( &deleteBuffer.mutex );
	pthread_mutex_lock(&deleteBuffer.gandalf);
	while(!deleteBuffer.length && !finish)
		pthread_cond_wait(&deleteBuffer.fëanor, &deleteBuffer.gandalf);
	pthread_mutex_unlock(&deleteBuffer.gandalf);

	while(deleteBuffer.length || !finish) {

		pthread_mutex_lock(&deleteBuffer.lengthMutex);
		deleteBuffer.length--;
		pthread_mutex_unlock(&deleteBuffer.lengthMutex);
		pthread_cond_signal(&deleteBuffer.condition);

		memset(e.name, '\0', NS);
		strcpy( e.name, deleteBuffer.e[deleteBuffer.iBegin].name );
		if(deleteBuffer.e[deleteBuffer.iBegin].ts == printBuffer.e[printBuffer.iBegin].tsDel) {
			deleteBuffer.iBegin = (deleteBuffer.iBegin + 1) % BS;
		} else {
			deleteBuffer.iBegin = (deleteBuffer.iBegin + 1) % BS;
			pthread_mutex_unlock( &deleteBuffer.mutex );
		}
		do{
			ni = removeNode(e.name);
		}while( ni != NULL );
		pthread_mutex_lock(&deleteBuffer.im);
		deleteBuffer.executed++;
		pthread_mutex_unlock(&deleteBuffer.im);

		pthread_mutex_lock( &deleteBuffer.mutex );
		while(!deleteBuffer.length && !finish);
	}
	puts("DELETE FIM ...");
	pthread_mutex_unlock( &deleteBuffer.mutex );
	pthread_exit( NULL );
	return (arg);
}*/

void *print(void *arg){
	//PrintElement e;

	//pthread_mutex_lock( &printBuffer.mutex );
	pthread_mutex_lock(&printBuffer.gandalf);
	while(!printBuffer.length && !finish)
		pthread_cond_wait(&printBuffer.feanor, &printBuffer.gandalf);
	pthread_mutex_unlock(&printBuffer.gandalf);

	while(printBuffer.length || !finish) {
		//e.idel = printBuffer.e[printBuffer.iBegin].idel;
		//e.iget = printBuffer.e[printBuffer.iBegin].iget;
		//e.iput = printBuffer.e[printBuffer.iBegin].iput;
		//pthread_mutex_lock(&printBuffer.lengthMutex);
		//pthread_mutex_unlock(&printBuffer.lengthMutex);
		//pthread_mutex_unlock( &printBuffer.mutex );
		/*if(printBuffer.e[printBuffer.iBegin].idel != -1)
			pthread_barrier_wait(&delPrint);
		if(printBuffer.e[printBuffer.iBegin].iget != -1)
			pthread_barrier_wait(&getPrint);
		if(printBuffer.e[printBuffer.iBegin].iput != -1)
			pthread_barrier_wait(&putPrint);*/
		while(putBuffer.executed < printBuffer.e[printBuffer.iBegin].iput);
		while(getDelBuffer.executed < printBuffer.e[printBuffer.iBegin].igetDel);
		//while(deleteBuffer.executed < printBuffer.e[printBuffer.iBegin].idel);
		printBuffer.iBegin = (printBuffer.iBegin + 1) % BS;
		printBuffer.length--;
		lockForest( );
		pthread_mutex_unlock(&putBuffer.mutex);
		//pthread_mutex_unlock(&deleteBuffer.mutex);
		printf("Início agenda\n");
		travel( );
		printf("Fim agenda\n");
		//pthread_mutex_unlock(&getDelBuffer.mutex);
		geTurn++;
		//pthread_mutex_unlock( &printBuffer.stopGets );
		//pthread_mutex_lock( &printBuffer.mutex );
		pthread_mutex_lock(&printBuffer.gandalf);
		while(!printBuffer.length && !finish)
			pthread_cond_wait(&printBuffer.feanor, &printBuffer.gandalf);
		pthread_mutex_unlock(&printBuffer.gandalf);
	}
	puts("PRINT FIM ...");
	pthread_mutex_unlock( &printBuffer.mutex );
	pthread_exit( NULL );
	return (arg);
}

void reader(char *str , int readerId, int bufferId){
	int aux, i;
	char cmd[CMDSIZE];
	Element e;

	sscanf(str, "%s", cmd );
	if( !strcmp( cmd,"PUT" ) ){
		sscanf(str, "%*s %s %s", e.name, e.number);

		pthread_mutex_lock(&readerMutex);
		while(readerId != priority[bufferId])
			pthread_cond_wait(&readerCondition, &readerMutex);
		pthread_mutex_unlock(&readerMutex);

		pthread_mutex_lock(&putBuffer.lengthMutex);
		while(putBuffer.length >= BS);
			pthread_cond_wait(&putBuffer.condition, &putBuffer.lengthMutex);
		putBuffer.length++;
		pthread_mutex_unlock(&putBuffer.lengthMutex);
		pthread_cond_signal(&putBuffer.feanor);

		aux = putBuffer.inserted % BS;
		strcpy( putBuffer.e[aux].name, e.name );
		strcpy( putBuffer.e[aux].number, e.number );
		timeStamp++;
		putBuffer.e[aux].ts = timeStamp;
		putBuffer.inserted++;

		pthread_mutex_lock(&readerMutex);
		priority[bufferId] = (priority[bufferId] + 1) % READERS;
		pthread_mutex_unlock(&readerMutex);
		pthread_cond_signal(&readerCondition);

	/*} else if( !strcmp( cmd,"DELETE" ) ){
		sscanf(str, "%*s %s", e.name );

		pthread_mutex_lock(&readerMutex);
		while(readerId != priority[bufferId])
			pthread_cond_wait(&readerCondition, &readerMutex);
		pthread_mutex_unlock(&readerMutex);

		pthread_mutex_lock(&deleteBuffer.lengthMutex);
		while(deleteBuffer.length >= BS);
			pthread_cond_wait(&deleteBuffer.condition, &deleteBuffer.lengthMutex);
		deleteBuffer.length++;
		pthread_mutex_unlock(&deleteBuffer.lengthMutex);
		pthread_cond_signal(&deleteBuffer.fëanor);

		aux = deleteBuffer.inserted % BS;
		strcpy( deleteBuffer.e[aux].name, e.name );
		timeStamp++;
		deleteBuffer.e[aux].ts = timeStamp;
		deleteBuffer.inserted++;

		pthread_mutex_lock(&readerMutex);
		priority[bufferId] = (priority[bufferId] + 1) % READERS;
		pthread_mutex_unlock(&readerMutex);
		pthread_cond_signal(&readerCondition);
*/
	} else if( !strcmp(cmd,"PRINT") ){

		pthread_mutex_lock(&readerMutex);
		while(readerId != priority[bufferId])
			pthread_cond_wait(&readerCondition, &readerMutex);
		pthread_mutex_unlock(&readerMutex);

		pthread_mutex_lock(&printBuffer.lengthMutex);
		while(printBuffer.length >= BS);
			pthread_cond_wait(&printBuffer.condition, &printBuffer.lengthMutex);
		printBuffer.length++;
		pthread_mutex_unlock(&printBuffer.lengthMutex);
		pthread_cond_signal(&printBuffer.feanor);

		aux = printBuffer.inserted % BS;
		//printBuffer.e[aux].idel = deleteBuffer.inserted;
		printBuffer.e[aux].igetDel = getDelBuffer.inserted;
		printBuffer.e[aux].iput = putBuffer.inserted;
		printBuffer.e[aux].tsPut = putBuffer.e[putBuffer.inserted % BS].ts;
		printBuffer.e[aux].tsGetDel = getDelBuffer.e[getDelBuffer.inserted % BS].ts;
		//printBuffer.e[aux].tsDel = deleteBuffer.e[deleteBuffer.inserted % BS].ts;
		printBuffer.inserted++;

		pthread_mutex_lock(&readerMutex);
		priority[bufferId] = (priority[bufferId] + 1) % READERS;
		pthread_mutex_unlock(&readerMutex);
		pthread_cond_signal(&readerCondition);
	} else {
		if( !strcmp( cmd,"GET" ) )
			i = GET;
		else if( !strcmp( cmd,"DELETE" ) )
			i = DEL;
		else return;
		sscanf(str, "%*s %s", e.name );

		pthread_mutex_lock(&readerMutex);
		while(readerId != priority[bufferId])
			pthread_cond_wait(&readerCondition, &readerMutex);
		pthread_mutex_unlock(&readerMutex);

		pthread_mutex_lock(&getDelBuffer.lengthMutex);
		while(getDelBuffer.length >= BS);
			pthread_cond_wait(&getDelBuffer.condition, &getDelBuffer.lengthMutex);
		getDelBuffer.length++;
		pthread_mutex_unlock(&getDelBuffer.lengthMutex);
		pthread_cond_signal(&getDelBuffer.feanor);

		aux = getDelBuffer.inserted % BS;
		getDelBuffer.e[aux].type = i;
		strcpy( getDelBuffer.e[aux].name, e.name );
		timeStamp++;
		getDelBuffer.e[aux].ts = timeStamp;
		getDelBuffer.inserted++;

		pthread_mutex_lock(&readerMutex);
		priority[bufferId] = (priority[bufferId] + 1) % READERS;
		pthread_mutex_unlock(&readerMutex);
		pthread_cond_signal(&readerCondition);
	}
}

void *readerFirst(void *arg) {
	Buffer *b = (Buffer *)arg;
	char *first, begin[CMD];
	int i, id = 0;

	pthread_barrier_wait(&barrier);
	while(b->size[id]) {
		first = b->str[id];
		i = strcspn(b->str[id], "\n");
		b->strP = first + i;
		if(i < b->size[id])
			b->strP += 1;
		strcpy(begin, end);

		pthread_barrier_wait(&ctrl);

		first[i] = '\0';
		strcat(begin, first);
		reader(begin, 0, id);

		pthread_mutex_lock(&b->mutex);
		while(b->turn[id])
			pthread_cond_wait(&b->condition[0], &b->mutex);
		pthread_mutex_unlock(&b->mutex);

		while(strlen(b->strP)) {
			i = strcspn(b->strP, "\n");
			first = b->strP;
			b->strP += i + 1;

			pthread_mutex_lock(&b->mutex);
			b->turn[id] = 1;
			pthread_mutex_unlock(&b->mutex);
			pthread_cond_signal(&b->condition[1]);

			first[i] = '\0';
			reader(first, 0, id);

			pthread_mutex_lock(&b->mutex);
			while(b->turn[id])
				pthread_cond_wait(&b->condition[0], &b->mutex);
			pthread_mutex_unlock(&b->mutex);
		}
		pthread_mutex_lock(&b->mutex);
		b->turn[id] = 1;
		pthread_mutex_unlock(&b->mutex);
		pthread_cond_signal(&b->condition[1]);

		id = (id+1) % BUFFERS;
		pthread_barrier_wait(&barrier);
	}
	pthread_exit(NULL);
	return (arg);
}

void *readerLast(void *arg) {
	Buffer *b = (Buffer *)arg;
	char *last;
	int i, id = 0;

	pthread_barrier_wait(&barrier);
	while(b->size[id]) {
		if(b->size[id] < BUFSIZE)

			pthread_barrier_wait(&ctrl);

		else {

			pthread_barrier_wait(&ctrl);

			last = strrchr(b->strP,'\n');
			if(last) {
				strcpy(end, last+1);
				b->strP[last+1-b->strP] = '\0';
			} else {
				strcpy(end, b->strP);
				b->strP[0] = '\0';
			}
		}
		while(strlen(b->strP)) {
			i = strcspn(b->strP, "\n");
			last = b->strP;
			b->strP += i + 1;

			pthread_mutex_lock(&b->mutex);
			b->turn[id] = 0;
			pthread_mutex_unlock(&b->mutex);
			pthread_cond_signal(&b->condition[0]);

			last[i] = '\0';
			reader(last, 1, id);

			pthread_mutex_lock(&b->mutex);
			while(!b->turn[id])
				pthread_cond_wait(&b->condition[1], &b->mutex);
			pthread_mutex_unlock(&b->mutex);
		}

		pthread_mutex_lock(&b->mutex);
		b->turn[id] = 0;
		pthread_mutex_unlock(&b->mutex);
		pthread_cond_signal(&b->condition[0]);

		id = (id+1) % BUFFERS;
		pthread_barrier_wait(&barrier);
	}
	pthread_exit(NULL);
	return (arg);
}

int main() {
	int i, j;
	pthread_t readers[READERS], threads[WORKERS];
	Buffer buffer;

	timeStamp = 0;
	finish = 0;
	geTurn = 0;
	end[0] ='\0';
	/*readerMutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	readerCondition = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	putBuffer.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	putBuffer.lengthMutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	putBuffer.gandalf = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	putBuffer.im = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	putBuffer.condition = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	putBuffer.feanor = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	getDelBuffer.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	getDelBuffer.lengthMutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	getDelBuffer.gandalf = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	getDelBuffer.im = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	getDelBuffer.condition = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	getDelBuffer.feanor = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	printBuffer.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	printBuffer.lengthMutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	printBuffer.gandalf = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	printBuffer.im = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	printBuffer.condition = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
	printBuffer.feanor = (pthread_cond_t)PTHREAD_COND_INITIALIZER;*/
	pthread_barrier_init(&barrier, NULL, READERS+1);
	pthread_barrier_init(&ctrl, NULL, READERS);
	/*pthread_mutex_init(&buffer.mutex, NULL);
	pthread_cond_init (&buffer.condition[0], NULL);
	pthread_cond_init (&buffer.condition[1], NULL);*/
	pthread_create(&readers[0], NULL, readerFirst, (void *)&buffer);
	pthread_create(&readers[1], NULL, readerLast, (void *)&buffer);
	//deleteBuffer.iBegin = 0;
	getDelBuffer.iBegin = 0;
	putBuffer.iBegin = 0;
	printBuffer.iBegin = 0;
	//deleteBuffer.inserted = 0;
	getDelBuffer.inserted = 0;
	putBuffer.inserted = 0;
	printBuffer.inserted = 0;
	//deleteBuffer.executed = 0;
	getDelBuffer.executed = 0;
	putBuffer.executed = 0;
	//deleteBuffer.length = 0;
	getDelBuffer.length = 0;
	putBuffer.length = 0;
	printBuffer.length = 0;
	printBuffer.e[0].tsPut = 0;
	printBuffer.e[0].tsGetDel = 0;
	//printBuffer.e[0].tsDel = 0;
	for( j = i = 0; i < PRINTERS; ++i, j++ )
		pthread_create( &threads[j], NULL, print, (void *)i );
	for( i = 0; i < PUTERS; ++i,j++ )
		pthread_create( &threads[j], NULL, put, (void *)i );
	for( i = 0; i < GETDELETERS; ++i, j++ )
		pthread_create( &threads[j], NULL, getDel, (void *)i );
	/*for( i = 0; i < DELETERS; ++i, j++ )
		pthread_create( &threads[j], NULL, delete, (void *)i );*/

	i = 1;
	while(!feof(stdin)) {
		i = (i+1) % BUFFERS;
		buffer.size[i] = fread(buffer.str[i], sizeof(char), BUFSIZE, stdin);
		buffer.str[i][buffer.size[i]] = '\0';
		buffer.turn[i] = 1;
		priority[i] = 0;
		//printf("STRING: .%s.\tSIZE: %d\n", buffer.str[i], buffer.size[i]);
		//fflush(stdout);
		/*
		 *  MANTER O ENTER MAGICO LIDO QUANDO: ./__AGENDA < teste
		 *  fim de arquivo e lido < BUFSIZE
		 */
		pthread_barrier_wait(&barrier);
	}
	buffer.size[(i+1) % BUFFERS] = 0;
	pthread_barrier_wait(&barrier);
	/*pthread_join(readers[0], NULL);
	pthread_join(readers[1], NULL);*/
	pthread_mutex_lock(&putBuffer.gandalf);
	pthread_mutex_lock(&getDelBuffer.gandalf);
	//pthread_mutex_lock(&deleteBuffer.gandalf);
	pthread_mutex_lock(&printBuffer.gandalf);
	finish = 1;
	pthread_mutex_unlock(&putBuffer.gandalf);
	pthread_mutex_unlock(&getDelBuffer.gandalf);
	//pthread_mutex_unlock(&deleteBuffer.gandalf);
	pthread_mutex_unlock(&printBuffer.gandalf);
	pthread_cond_signal(&putBuffer.feanor);
	pthread_cond_signal(&getDelBuffer.feanor);
	//pthread_cond_signal(&deleteBuffer.fëanor);
	pthread_cond_signal(&printBuffer.feanor);
	finish = 1;
	puts("FINISHING ...");
	pthread_barrier_destroy(&barrier);
	pthread_barrier_destroy(&ctrl);
	/*pthread_mutex_destroy(&buffer.mutex);
	pthread_cond_destroy(&buffer.condition[0]);
	pthread_cond_destroy(&buffer.condition[1]);*/
	pthread_exit(NULL);
}
