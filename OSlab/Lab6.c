//
//  main.c
//  Reordering
//
//  Created by Kim Lam on 2/7/2014.
//  Copyright (c) 2014 Kim Lam. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>   //needed for the memset command
#include <sndfile.h>
#include <stdio.h>

#define maxSize 2048
//We are storing our data in a linked list.
//BLLItem defines a node/Item on that list.
typedef struct bufferLinkedListItem
{
    size_t window_offset_begin;
    size_t length;
    size_t max_size;
    double * buffer;
    struct bufferLinkedListItem * next;
} BLLItem;
//We want to keep track of the current position of 
//out file, so we combine the file handle and the
//position in one structure.
typedef struct FileStruct
{
    void * sndfile; //A pointer to the actual sound file
    size_t currentPosition;  //CurrentStart position;
} FStruct;

//Structure to pass data to the work thread.
typedef struct WorkerThreadStructure
{
    //Shared locks, same address for all threads.
    sem_t * fileReadLock;  //Lock for file reading
    sem_t * queueLock;
    sem_t * outPositionCheckLock;
    //Will probably need more
    BLLItem * sharedQueue; //This is the shared queue, same address for all threads
    FStruct * inSndfile;  //Pointer to the input sndFile
    FStruct * outSndfile; //Pointer to the output sndFile
    double  scale;
} WTStruct;

//Factory function to create our link list items for us.
struct bufferLinkedListItem* BufferLinkedListItemFactory(size_t max_size)
{
    BLLItem * newBuffer =
    (BLLItem *) malloc(sizeof(BLLItem));
    memset(newBuffer,0,sizeof(BLLItem));
    newBuffer->max_size = max_size;
    newBuffer->buffer = (double *) malloc(sizeof(double)*max_size);
    return newBuffer;
    
}
//Function to free out list items. We free the items after the 
//data has been written to a file.
void FreeBufferLinkedListItem(BLLItem ** itemPtr)
{
    BLLItem * item;
    if(itemPtr)
    {
       item= *itemPtr;
       if(item->buffer)
       {
          free(item->buffer);
       }
        free(item);
        *itemPtr =0; //Good idea to reset to 0, accidental re-use will lead to crashes, rather than bugs.
    }
    return;
}
//Return the head of the linked list, it may change if we have to
//prepend data.
BLLItem * addToList(BLLItem * head,BLLItem * item)  //For better results a better insertion algorithm would work.
{
    BLLItem * current = head;
    if(head == NULL)  // Null Case
        return item;
    if(item->window_offset_begin  < head-> window_offset_begin) //Prepend case
    {
        item->next = head;
        return item;
    }
    do {
       if(current->next == NULL)  //End of list, append to the end
       {
           current->next = item;
           return head;
       }
        // Insert buffer at the right place
       if(item->window_offset_begin < current->next->window_offset_begin)  //Insert into the middle of the list
       {
           item->next = current->next;
           current->next = item;
           return head;
       }
        current = current->next;
           
    } while (1); // infinite loop
    
    return NULL;
}
//Head returns the new start of the list
//outQueue, if head.offset == start, then outQueue contains a list of contigous buffers from start.
//otherwise outQueue = null
//returns 1 if the head changed, 0 if there is not change. //should be a boolean.
int seperateList(BLLItem ** head, BLLItem ** outQueue, size_t start)
{
    BLLItem * cur;  //current position
    if((*head)->window_offset_begin != start)  //First item not the start of the queue
    {
        //printf("Not first buffer\n");
        *outQueue = NULL;
        return 0;
    }
    cur = *head;
    
    *outQueue = *head;   //Assign head to outqueue, as it's the start of a contigous chain of
                         //buffers.
    if(cur ->next == NULL)  //Only one item in queue
    {
        *head = NULL;   //All items tranfers to the outQueue, nothing left in the current queue.
        return 1;
    }
    
    //Condition in while checks if two queue items are contigous.
    while((cur->window_offset_begin+cur->length) == cur->next->window_offset_begin)
    {
        //If it is contigous, move to the next item.
         cur = cur->next;
        if(cur->next == NULL)  // contigous queue, we write it all to the output
        {
            //printf("Headis null\n");
            *head = NULL;
            return 1;
        }               
    }    
    *head = cur->next;  //The new head is the 1st discontinous point.
    cur->next = NULL; //Terminate the outqueue
    return 1;
}


//Read from the sound file.  returns the number of doubles read.
size_t readFromFile(BLLItem * item, FStruct * fd,sem_t * semaphore)
{
    sem_wait(semaphore);
    item->window_offset_begin = fd->currentPosition;
    item->length = sf_read_double(fd->sndfile,item->buffer, item->max_size);
    fd->currentPosition = fd->currentPosition + item->length;
    sem_post(semaphore);
    return item->length;
}

size_t writeToFile(BLLItem* localWriteQueue, sem_t * posLock, size_t * currentPosition, SNDFILE * sndfile , double scale)
{
            //Write sound file
	    size_t i;
            size_t numWritten=0;
            BLLItem * current;
            while(localWriteQueue)
            {
                current = localWriteQueue;
		double tempBuffer[maxSize];
		for(i = 0; i < localWriteQueue->length ; i++){
			tempBuffer[i] = localWriteQueue->buffer[i] * scale;
		}
                numWritten += sf_write_double(sndfile,tempBuffer,localWriteQueue->length);
                localWriteQueue = localWriteQueue->next;
		
                FreeBufferLinkedListItem(&current);
            }
            sem_wait(posLock); //update the current position
            (*currentPosition) += numWritten;
            sem_post(posLock);
            return numWritten;
}

//Just one type of worker function

void * workerFunction(void * dataStruct)
{
    WTStruct * threadContext = (WTStruct *) dataStruct;
    size_t numRead =0;
    int i =0;
    
    do {  //Infinite loop, exit condition will be in the form of a break statement.
        BLLItem * localWriteQueue = NULL;
        BLLItem * newItem = BufferLinkedListItemFactory(maxSize); //Make new buffer
        numRead = readFromFile(newItem,threadContext->inSndfile, threadContext->fileReadLock);  // readlock used in function
        if(numRead == 0)
        {
            FreeBufferLinkedListItem(&newItem);
            break;  //Exit when the end of the file has been reached.
        }
        //Do our processing
        for(i = 0; i < newItem->length; i++)
        {
            newItem->buffer[i] *=0.5;  //Half the volume
        }
        
        //Now add to the list.  If we don't design this carefully, we can get deadlocks!
        sem_wait(threadContext->queueLock);        
        threadContext->sharedQueue = addToList(threadContext->sharedQueue, newItem);        
        sem_wait(threadContext->outPositionCheckLock); //Protects the current position variable
        seperateList(&threadContext->sharedQueue, &localWriteQueue, threadContext->outSndfile->currentPosition);
        sem_post(threadContext->outPositionCheckLock);
        sem_post(threadContext->queueLock);
      
        if(localWriteQueue)  // There should never be more than one thread writing at a time.
        {            
           writeToFile(localWriteQueue, threadContext->outPositionCheckLock, 
                         &(threadContext->outSndfile->currentPosition), threadContext->outSndfile->sndfile, threadContext->scale);
        }
        
    } while (1);
    
    return NULL;
}


int main(int argc, const char * argv[])
{
    
    int numThreads =  atoi(argv[4]);
    pthread_t * threads = malloc(sizeof(pthread_t)*numThreads);
    SF_INFO sf_info;
    int i=0;
    
    //Setup semaphores, all semaphores unlocked initially.
    WTStruct * sharedContext = (WTStruct *) malloc(sizeof(WTStruct));
    sharedContext->fileReadLock = (sem_t *) malloc(sizeof(sem_t));
    sharedContext->outPositionCheckLock = (sem_t *) malloc(sizeof(sem_t));
    sharedContext->queueLock = (sem_t *) malloc(sizeof(sem_t));
    sharedContext->scale = strtod(argv[3], NULL);
    sem_init(sharedContext->fileReadLock,0,1);
    sem_init(sharedContext->outPositionCheckLock,0,1);
    sem_init(sharedContext->queueLock,0,1);
    
    //Setup file
    sharedContext->sharedQueue = NULL;
    sharedContext->inSndfile = (FStruct *)malloc(sizeof(FStruct));
    sharedContext->inSndfile->currentPosition =0;
    sharedContext->inSndfile->sndfile = sf_open(argv[1],SFM_READ,&sf_info);
    if(!sharedContext->inSndfile->sndfile)
    {
        printf("Could not open file\n");
        return 0;
    }
    sharedContext->outSndfile = (FStruct *)malloc(sizeof(FStruct));
    sharedContext->outSndfile->sndfile=sf_open(argv[2],SFM_WRITE,&sf_info);
    sharedContext->outSndfile->currentPosition =0;
    
    // insert code here...
    printf("Starting program!\n");
    
    for( i=0; i < numThreads; i++)
    {
        pthread_create(threads+i, NULL,  workerFunction, sharedContext);
    }
    for(i =0; i < numThreads; i++)
    {
        pthread_join(threads[i],NULL);
    }

    //This part is a bit ugly, because our exit condition is a bit awkward;
    if(sharedContext->sharedQueue != NULL)
    {        
        //Write remaining data                  
        size_t numWritten =0;
        printf("Queue is not empty\n");    
        numWritten=writeToFile(sharedContext->sharedQueue, sharedContext->outPositionCheckLock, 
                         &(sharedContext->outSndfile->currentPosition), sharedContext->outSndfile->sndfile , sharedContext->scale);
        printf("Samples Written by main thread = %ld\n",numWritten);
        printf("Notes this number will vary from run to run, due to thread timing\n");                
    }

    printf("Finished processing file");
    sf_close(sharedContext->inSndfile->sndfile);
    sf_close(sharedContext->outSndfile->sndfile);
    sem_destroy(sharedContext->fileReadLock);
    sem_destroy(sharedContext->queueLock);
    sem_destroy(sharedContext->outPositionCheckLock);
    free(sharedContext->fileReadLock);
    free(sharedContext->queueLock);
    free(sharedContext->outPositionCheckLock);
    free(threads);
    free(sharedContext);
    return 0;
}

