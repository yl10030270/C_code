/*
 * SemaphoreDemo.c
 *
 *  Created on: Jan 31, 2014
 *      Author: user1
 */
//
//Simple example to copy 1 file, to multiple output files
//
#include <pthread.h>
#include <semaphore.h>
#include <sndfile.h>
#include <stdio.h>
#include <stdlib.h>


//Data structure of our buffer
struct bufferStruct
{
	size_t length;
	size_t maxSize;
	double * buffer;
};

//Data structure passed into the writer
struct writerStruct
{
	int numReaders;
    sem_t ** writerBlocks;  //This will be an array
    sem_t ** readerBlocks;
    struct bufferStruct * sharedBuffer;
    SNDFILE * infile;
};
//Data structure passed into our reader
struct readerStruct
{
    sem_t * writerBlock;  //This will not be an array
    sem_t * readerBlock;
    struct bufferStruct * sharedBuffer;
    SNDFILE * outfile;
    double scale;
};

//Function for worker 1
void * writerWorkerFunction(void * _data)
{
	struct writerStruct * ws = (struct  writerStruct *) _data;
	int i;

	do
	{
	   for(i = 0; i< ws->numReaders; i++)
	   {
		   sem_wait(ws->writerBlocks[i]);   //Make sure we can write to the buffer
		                                 //Check that none of the readers need the buffer
		                                 //We could also do this with a counting semaphore
	   }
	   ws->sharedBuffer->length = sf_read_double(ws->infile, ws->sharedBuffer->buffer, ws->sharedBuffer->maxSize);


	   for(i = 0; i < ws->numReaders; i++)
	   {
		   sem_post(ws->readerBlocks[i]);
	   }
	} while(ws->sharedBuffer->length == ws->sharedBuffer->maxSize);
	return NULL;
}
//reader worker function.
void * readerWorkerFunction(void * _data)
{
	struct readerStruct * rs = (struct readerStruct *) _data;
        size_t i;
	do
	{
		//Write data directly from the buffer to the file.
		sem_wait(rs->readerBlock);
		double tempBuffer[2048];
		for(i = 0; i < rs->sharedBuffer->length ; i++){
			tempBuffer[i] = rs->sharedBuffer->buffer[i] * rs->scale;	
		}
		sem_post(rs->writerBlock);
		sf_write_double(rs->outfile,tempBuffer, rs->sharedBuffer->length);
		

	}while(rs->sharedBuffer->length == rs->sharedBuffer->maxSize);
	return NULL;
}
void JoinAllMyThreads(pthread_t *threads, int size)
{
   int i =0;
   for(i =0; i < size; i++)
   {
	   pthread_join(threads[i],NULL);
   }
}
int main(int argc, char** argv)
{
	int maxSize = 2048;
	int numOutput = atoi(argv[1]);
    	int i, status;
	struct bufferStruct * buffer;
	struct readerStruct ** readers;
	struct writerStruct * writer;
	sem_t ** readerSemaphores;
	sem_t ** writerSemaphores;
    	SNDFILE ** outSndFileArray;
    	SNDFILE * inSndFile;
    	pthread_t * threads;
    	SF_INFO sf_info;
	//Setup our buffer
	buffer = (struct bufferStruct *) malloc(100);
	buffer->buffer = (double *) malloc(sizeof(double)*maxSize);
	buffer->maxSize = maxSize;
	buffer->length =0;

	//Setup sndFiles;
	inSndFile = sf_open(argv[argc - 1], SFM_READ, &sf_info);
	if(!inSndFile)
	{
		printf("Could not open input file\n");
		return -1;
	}
	outSndFileArray = (SNDFILE**) malloc(sizeof(SNDFILE*)*numOutput);
	for( i=0; i < numOutput; i++)
	{
		char  filename[1024];
		sprintf(filename,argv[numOutput + 2 + i], i);  // Create unique filenames
		outSndFileArray[i] = sf_open(filename,SFM_WRITE,&sf_info);
		if(!outSndFileArray[i])
		{
			printf("Could not open output file\n");
			return -1;
		}
	}

	//setup semaphores
	readerSemaphores = (sem_t **)malloc(sizeof(sem_t*)*numOutput);
	writerSemaphores = (sem_t **)malloc(sizeof(sem_t*)*numOutput);

	//Create the semaphores.
	for(i = 0; i < numOutput; i++)
	{
		int result;
		readerSemaphores[i] = (sem_t *) malloc(sizeof(sem_t));
		writerSemaphores[i] = (sem_t *) malloc(sizeof(sem_t));
		result = sem_init(readerSemaphores[i], 0, 0);  //Set the reader semaphores to 0, the reader threads block initially till there is data
		if(result == -1)
		{
			printf("Could not obtain reader semaphore %d\n", i);
		}

		result = sem_init(writerSemaphores[i], 0, 1); //Set the writer semaphores to 1, so the reader can do the initial read.
		if(result == -1)
		{
			printf("Could not obtain writer semaphore %d\n", i);
		}
	}

	//Setup writer data structure, this will be passed to the writer function.
    writer = (struct writerStruct *) malloc(sizeof(struct writerStruct));
    writer->readerBlocks = readerSemaphores;
    writer->writerBlocks = writerSemaphores;
    writer->numReaders = numOutput;
    writer->sharedBuffer = buffer;
    writer->infile = inSndFile;

    readers = (struct readerStruct **) malloc(sizeof(struct readerStruct*)*numOutput);

    //Setup the struct for the readers, this will be passed to the reader threads
    for(i =0; i < numOutput; i++)
    {
    	readers[i] = (struct readerStruct *) malloc(sizeof(struct readerStruct));
    	readers[i]->outfile = outSndFileArray[i];
    	readers[i]->readerBlock = readerSemaphores[i];
    	readers[i]->writerBlock = writerSemaphores[i];
    	readers[i]->sharedBuffer = buffer;
	readers[i] ->scale =  strtod(argv[i + 2], NULL);
    }

    //Finally we spawn our threads

    threads = (pthread_t *) malloc(sizeof(pthread_t )*(numOutput+1));
    //create the writer thread
    status = pthread_create(threads+numOutput,NULL,writerWorkerFunction, writer);
    //Should really do error checking on the status
    //create the reader threads
    for(i =0; i < numOutput; i++)
    {
        status = pthread_create(threads+i,NULL,readerWorkerFunction, readers[i]);
    }

    JoinAllMyThreads(threads, numOutput+1);

    //Clean up, make sure we clean up everything correctly.
    for(i =0; i < numOutput; i++)
    {
    	sf_close(outSndFileArray[i]);
    	free(readers[i]);
    	sem_destroy(readerSemaphores[i]);
    	sem_destroy(writerSemaphores[i]);
    	free(readerSemaphores[i]);
    	free(writerSemaphores[i]);
    }
    free(writer);
    free(readers);
    free(readerSemaphores);
    free(writerSemaphores);
    free(threads);
    free(buffer->buffer);
    free(buffer);
    sf_close(inSndFile);
  return 0;
}
