#include <pthread.h>  //Thread library
#include <sndfile.h>  //Audio file handling library
#include <stdlib.h>  
#include <stdio.h> 

//Data structure that will be passed to our worker function
struct workInfo
{
   double * data;
   double * output;
   size_t length;
   double scalefactor;
};

//Function that actually does the processing.
//It is also our thread entry point.
void * workerfunction(void *_data)
{
    int i =0;
    struct workInfo * workData = (struct workInfo*) _data;
 
    for(i =0; i < workData->length; i++)
    {
         workData->output[i] = workData->scalefactor*workData->data[i];
     }
}

//Function that waits on all the threads in the array.
void JoinAllMyThreads( pthread_t * threads, int size)
{
    int i =0;
     for(i =0; i < size; i ++)
     {
        pthread_join(threads[i],NULL);
      }
}
int main(int argc, char ** argv)
{
   int numThreads = atoi(argv[4]);   // Number of thread we want to help do our work
   int chunkSize = 1024; //Define how much data each thread will process at a time
   int i;
   size_t  bufferSize;   // the size of the dataBuffer
   double * dataBuffer; //Global
   double *outputBuffer; //global output buffer
   pthread_t *threads;  //Array of threads
   int * thread_result;  // Pointer to storage, for the thread results
   struct workInfo * threadWork; // pointer to array storing data structure to pass to threads
   int j; //second loop
    SNDFILE * infile;  // File pointer/File descriptor to the input file
    SNDFILE * outfile; // File pointer/File descriptor to the output file
    SF_INFO sf_info;   // Stores the properties of the sound file
    size_t numread;    // Total number of samples read
    size_t numframes;

    /* remember, data shared between threads should be on the heap. This includes the main thread */
    //setup threads array
    threads = (pthread_t * ) malloc(sizeof(pthread_t)*numThreads);  // Array for storing the thread data structures
    thread_result =(int *) malloc(sizeof(int) * numThreads);        // Array for storing the thread creation results

    //Setup input and output arrays

    bufferSize = numThreads * chunkSize;     // define the size of our buffers.  ChunkSize * number of threads
    dataBuffer = (double *) malloc(sizeof(double) * bufferSize);   //  Buffer to store enough data for all threads
    outputBuffer = (double *) malloc(sizeof(double)* bufferSize);  //  Buffer to store outputs from all threads

    threadWork=(struct workInfo *) malloc(sizeof(struct workInfo)*numThreads);  // Buffer to store parameters to apss to threads.
    
    // Open out files for reading and writing
    infile = sf_open(argv[1],SFM_READ, &sf_info);
    
    printf("%s \n",argv[1]);
    
    if(infile == NULL)
    {
    	perror("Could not open file");
    	//error(1);
    }

    outfile = sf_open(argv[2],SFM_WRITE, &sf_info);

     //Finally get down to work.  
    do 
    {
        int threadsToCreate;
        //Read our data
	numframes = bufferSize/sf_info.channels;
        numread = sf_read_double(infile,dataBuffer, numframes);
        threadsToCreate = numread * sf_info.channels / chunkSize;  //Number of threads we need
        
         //Loop to spawn threads
        for( i = 0; i < numThreads; i++)
        {
             //Setup struct for the thread, so it knows what to process and where to put the results
             threadWork[i].data = dataBuffer + chunkSize*i;
             threadWork[i].output = outputBuffer + chunkSize *i;
             threadWork[i].scalefactor = strtod(argv[3],NULL);    
             threadWork[i].length = chunkSize;      
              
             if(i < threadsToCreate)  //Only create threads 
                                      //if there is data to feed it.
             {                       
                thread_result[i] = pthread_create(threads+i, NULL, workerfunction,
                threadWork +i);
             }
             
        }
         //Main threads waits for all it's children before writing the data.
         JoinAllMyThreads(threads, threadsToCreate);

          //Write the data
         sf_write_double(outfile, outputBuffer, numread);
    }while(numread*sf_info.channels == bufferSize);
    
   //Should check if pointers are valid before releasing.
   

   //Clean up
   sf_close(outfile);
   sf_close(infile);
   free(dataBuffer);
   free(outputBuffer);
   free(threads);
   free(threadWork);
   free(thread_result);
     
}

