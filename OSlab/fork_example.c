#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* exit */
#include <unistd.h>
#include <sndfile.h>
#define num_process 2
#define arraySize 1024
void childProcessWorkFunction(const char * inputfilename, const char * outputfilename);

void childProcessWorkFunction(const char * inputfilename, const char * outputfilename)
{
	double buffer[arraySize];  //buffer for storing data
	    SNDFILE * inputfile, *outputfile;  // file pointers to reference files
	    SF_INFO sf_info;   // sound file data structure describing file properties
	    size_t numread;  // stores the number of frames read
	    size_t numframes; // numnber of frames to read;

	    inputfile = sf_open(inputfilename, SFM_READ, &sf_info);  //Open file for reading
	    if(inputfile == NULL)
	    {
	    	perror("Could not open file");
	    	error(1);
	    }
	    outputfile = sf_open(outputfilename,SFM_WRITE,&sf_info);

	    numframes = arraySize/sf_info.channels;  //Find number of frames, buffersize/channels
	    numread = numframes;
	    while(numread == numframes)
	    {
	       numread = sf_readf_double(inputfile, buffer, numframes);  //read from the files
	       sf_writef_double(outputfile, buffer, numread);  //Write to the file
	    }
	    sf_close(inputfile);
	    sf_close(outputfile);
}



int main()
{
	int lastForkReturn;
	int * IDArray = (int *) malloc(sizeof(int)*num_process);  //allocate space for out array
	int i = 0;

	for(i =0; i < num_process; i++)
	{
	   lastForkReturn = IDArray[i] = fork();   //Create a child process
	   if(IDArray[i] == 0)  //If it's a child do not continue with the loop
	   {
		   break;

	   }
	}
	if(lastForkReturn ==0)
	{
		char filename[1024];
		sprintf(filename,"out%d.wav", i);   //create unique output name for each child
		childProcessWorkFunction("In.wav",filename);  //Call out worker
	}
	else
	{
		for(i = 0;i < num_process; i++)
		{
			int status;  //ignoring statuses for now
			waitpid(IDArray[i], &status,WUNTRACED | WCONTINUED);  //Wait for child processes
		}

	}
}
