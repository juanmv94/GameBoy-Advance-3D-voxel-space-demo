#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

	FILE *in,*out;
	unsigned char color_in[3];
	unsigned short color[3];
	unsigned short color_out;
	unsigned char *color_out_b=&color_out;

	if (argc!=3 || !(in=fopen(argv[1],"rb")) || !(out=fopen(argv[2],"wb+"))) {
		printf("Uso: %s in.bgr out.raw\n",argv[0]);
		return (EXIT_SUCCESS);
	}
	while (fread(color_in,3,1,in)==1)
	{
		color[0]=color_in[0];
		color[1]=color_in[1];
		color[2]=color_in[2];
		color_out=(color[2]>>3)|((color[1]&0xF8)<<2)|((color[0]&0xF8)<<7);
		fwrite(color_out_b+1,1,1,out);
		fwrite(color_out_b,1,1,out);
	}
	return (EXIT_SUCCESS);    
}

