#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* exit */
#include <unistd.h>
#include <sndfile.h>

#define arraySize 1024

void childProcessWorkFunction(const char * inputfilename, const char * outputfilename, double scale);

void childProcessWorkFunction(const char * inputfilename, const char * outputfilename , double scale)
{
	int i = 0;	
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
	       
	       for(i=0; i < numread * sf_info.channels ; i++){
			buffer[i] *= scale;
		}

	       sf_writef_double(outputfile, buffer, numread);  //Write to the file
	    }
	    sf_close(inputfile);
	    sf_close(outputfile);
}



int main(int argc, char ** argv)
{
	int lastForkReturn;
	int num_process = atoi(argv[1]) ;
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
		char * filename = argv[i + 2 + num_process];
		double volumn = strtod(argv[i+2],NULL);
		//sprintf(filename,"out%d.wav", i);   //create unique output name for each child
		childProcessWorkFunction(argv[argc-1],filename , volumn);  //Call out worker
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
