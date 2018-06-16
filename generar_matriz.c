#include <stdio.h>
#include <stdlib.h>
#include "parametros.h"

int main(int argc, char** argv) {

	FILE *out,*out2;
	unsigned char valor=0;
	int step,alt,altura;
	
	short i,x;
	char *xb=&x;
	
	//Creamos matriz de alturas
	
	if (!(out=fopen("matrizalturas.raw","wb+"))) {
		puts("Error escritura");
		return (EXIT_SUCCESS);
	}
	
	for (altura=minaltura;altura<(minaltura+stepaltura*numalturas);altura+=stepaltura)
	{
		step=minstep;
		while (step<=maxstep)
		{
			alt=(-altura+valor+((step*varbaja)>>8))*MODE5_HEIGHT/((step*varalta+step*varbaja)>>8);
			if (alt>MODE5_HEIGHT) fputc(MODE5_HEIGHT,out); else if (alt<0) fputc(0,out); else fputc(alt,out);
			if ((valor++)==0) step++;
		}
	}
	fclose(out);
	
	//Creamos matriz x
	
	if (!(out2=fopen("matrizx.raw","wb+"))) {
		puts("Error escritura");
		return (EXIT_SUCCESS);
	}
	
	for (step=minstep;step<=maxstep;step++)
	{
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=i*(step*2+1)/MODE5_WIDTH-step;
			fputc(xb[1],out2);
			fputc(xb[0],out2);
		}
	}
	fclose(out2);
	return (EXIT_SUCCESS);    
}

