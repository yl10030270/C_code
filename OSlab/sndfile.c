#include <sndfile.h>
#include <stdio.h>
#define arraySize 1024

int main()
{
	double buffer[arraySize];  //buffer for storing data
    SNDFILE * inputfile, outputfile;  // file pointers to reference files
    SF_INFO sf_info;   // sound file data structure describing file properties
    size_t numread;  // stores the number of frames read
    size_t numframes; // number of frames to read;

    inputfile = sf_open("in.wav", SFM_READ, &sf_info);  //Open file for reading
    if(inputfile == NULL)
    {
    	perror("Could not open file");
    	error(1);
    }
    outputfile = sf_open("out.wav",SFM_WRITE,&sf_info);

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
