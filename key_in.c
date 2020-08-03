#include "key_in.c"



unsigned char key_in(){
	unsigned char in, in2;
	
	in=~PIN;
	
	while(1){
		in2=~PIN;
		if(in==in2)break;
		in=in2;
	}
	if(!(in)){
		return 0;
	}

	return in;

}
