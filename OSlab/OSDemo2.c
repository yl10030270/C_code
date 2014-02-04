#include <sndfile.h>

int main(){
	SF_INFO s_info;
	SNDFILE * sndfile;
	
	double temp[100];
	s_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	sndfile = sf_open("myfile", SFM_WRITE, &s_info);
	sf_write_double(sndfile, temp, 100);
	
	sf_close(sndfile);
	return 0;
}
