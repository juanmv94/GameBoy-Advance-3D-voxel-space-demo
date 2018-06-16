#include "gba.h"
#include "gfx.h"
#include "parametros.h"
#define IWRAM_CODE __attribute__((section(".iwram"), long_call))

#define dim_mapa 1024
#define dim_mapa_log2 10

#define INC_STEP ((step>>6)+2) //1	//Incremento en el step para optimizar

#define INI_X 160
#define INI_Y 190
#define INI_DX 500
#define INI_DY 960

#define getcolor(x,y) 	rawcolor[x+(y<<dim_mapa_log2)]
#define getprof(x,y)	rawprof[x+(y<<dim_mapa_log2)]


typedef void (*r)(void);

int *tempgfxp;
u16 *VRAM=BACK_BUFFER;
u8 **punterosalturas;

u16 tecla_pulsada=0;

int posx=0,posy=0,posz=2;
u8 posr=0;

IWRAM_CODE void actualiza_punterosalturas()
{
	int i,sum=((maxstep-minstep+1)<<8);
	punterosalturas[0]=matrizalturas;
	for (i=1;i<numalturas;i++)
	{
		punterosalturas[i]=punterosalturas[i-1]+sum;
	}
}

IWRAM_CODE void DrawVerticalLine(u8 x,u8 y,u8 y2,u16 color)
{
	u16 *base= VRAM+x;
	u16 *ini;
	u16 *fin= base+tempgfxp[y2];
	for(ini=base+tempgfxp[y]; ini<fin; ini+=MODE5_WIDTH)
	{
		*ini=color;
	}
}
/*
int fx(int x, int y) //rotacion
{
	switch (posr)
	{
		case 1:
		return (-y/2 + x)&(dim_mapa-1);
		case 2:
		return (-y + x/2)&(dim_mapa-1);
		case 3:
		return -y&(dim_mapa-1);
		case 4:
		return (-y - x/2)&(dim_mapa-1);
		case 5:
		return (-y/2 - x)&(dim_mapa-1);
		case 6:
		return -x&(dim_mapa-1);
		case 7:
		return (y/2 - x)&(dim_mapa-1);
		case 8:
		return (y - x/2)&(dim_mapa-1);
		case 9:
		return y&(dim_mapa-1);
		case 10:
		return (y + x/2)&(dim_mapa-1);
		case 11:
		return (y/2 + x)&(dim_mapa-1);
		default:
		return x&(dim_mapa-1);
	}
}

int fy(int x, int y) //rotacion
{
	switch (posr)
	{
		case 1:
		return (y + x/2)&(dim_mapa-1);
		case 2:
		return (y/2 + x)&(dim_mapa-1);
		case 3:
		return x&(dim_mapa-1);
		case 4:
		return (-y/2 + x)&(dim_mapa-1);
		case 5:
		return (-y + x/2)&(dim_mapa-1);
		case 6:
		return -y&(dim_mapa-1);
		case 7:
		return (-y - x/2)&(dim_mapa-1);
		case 8:
		return (-y/2 - x)&(dim_mapa-1);
		case 9:
		return -x&(dim_mapa-1);
		case 10:
		return (y/2 - x)&(dim_mapa-1);
		case 11:
		return (y - x/2)&(dim_mapa-1);
		default:
		return y&(dim_mapa-1);
	}
}
*/

IWRAM_CODE void mueve_pantalla_step(int step)
{
			if (((REG_DISPSTAT)&1)==0) return;
			if (tecla_pulsada&KEY_B)
			{REG_BG2Y=INI_DY+(step<<1);return;}
			else if (tecla_pulsada&KEY_A)
			{REG_BG2Y=INI_DY-(step<<1);return;}
			if (tecla_pulsada&KEY_DOWN)
			{REG_BG2X=INI_DX-(step);REG_BG2Y=INI_DY-(step>>2);REG_BG2PA=INI_X+(step>>7);REG_BG2PD=INI_Y+(step>>7);}
			else if (tecla_pulsada&KEY_UP)
			{REG_BG2X=INI_DX+(step);REG_BG2Y=INI_DY+(step>>2);REG_BG2PA=INI_X-(step>>7);REG_BG2PD=INI_Y-(step>>7);}
			if (tecla_pulsada&KEY_LEFT){if (tecla_pulsada&KEY_UP) REG_BG2X=INI_DX; else REG_BG2X=INI_DX-step;}
			else if (tecla_pulsada&KEY_RIGHT){ if (tecla_pulsada&KEY_DOWN) REG_BG2X=INI_DX; else REG_BG2X=INI_DX+step;}
}

IWRAM_CODE void render0()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=x&(dim_mapa-1);
			yf=y&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render1()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=(-y/2 + x)&(dim_mapa-1);
			yf=(y + x/2)&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render2()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=(-y + x/2)&(dim_mapa-1);
			yf=(y/2 + x)&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render3()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=-y&(dim_mapa-1);
			yf=x&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render4()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=(-y - x/2)&(dim_mapa-1);
			yf=(-y/2 + x)&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render5()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=(-y/2 - x)&(dim_mapa-1);
			yf=(-y + x/2)&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render6()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=-x&(dim_mapa-1);
			yf=-y&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render7()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=(y/2 - x)&(dim_mapa-1);
			yf=(-y - x/2)&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render8()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=(y - x/2)&(dim_mapa-1);
			yf=(-y/2 - x)&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render9()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=y&(dim_mapa-1);
			yf=-x&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render10()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=(y + x/2)&(dim_mapa-1);
			yf=(y/2 - x)&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

IWRAM_CODE void render11()
{
	u8 alturas[MODE5_WIDTH];
	int step,anchostep,i,x,y,xf,yf,alt,tmpalturas;
	u16 *punterox;
	memset(alturas,0,MODE5_WIDTH);
    for (step=minstep;step<maxstep;step+=INC_STEP)
	{
		tmpalturas=(step-minstep);
		mueve_pantalla_step(tmpalturas);
		punterox=matrizx+MODE5_WIDTH*tmpalturas;	//Solo para version rapida
		tmpalturas<<=8;					//*256		//Solo para version rapida
		for (i=0;i<MODE5_WIDTH;i++)
		{
			x=posx+punterox[i];
			y=posy+step;
			
			///////único cambio//
			xf=(y/2 + x)&(dim_mapa-1);
			yf=(y - x/2)&(dim_mapa-1);
			//////////////////
			
			alt=punterosalturas[posz][tmpalturas+getprof(xf,yf)];	//Leemos de la matriz de alturas
			if (alt>alturas[i])
			{
				DrawVerticalLine(i, alt, alturas[i], getcolor(xf,yf));
				alturas[i]=alt;
			}
		}
	}
}

const r rend[12]={&render0,&render1,&render2,&render3,&render4,&render5,&render6,&render7,&render8,&render9,&render10,&render11};

void incr_a()
{
	int tmpx=posx, tmpy=posy;
	switch (posr)
	{
		case 1:
		case 4:
		case 7:
		case 10:
		posx=(3*tmpy+4*tmpx)/5;
		posy=(4*tmpy-3*tmpx)/5;
		break;
		case 2:
		case 5:
		case 8:
		case 11:
		posx=(tmpy+2*tmpx)/2;
		posy=(2*tmpy-tmpx)/2;
		break;
		default:
		posx=(4*tmpx+2*tmpy)/5;
		posy=(4*tmpy-2*tmpx)/5;
		break;
	}
	posr++;
	if (posr>11) posr=0;
}

void decr_a()
{
	int tmpx=posx, tmpy=posy;
	switch (posr)
	{
		case 1:
		case 4:
		case 7:
		case 10:
		posx=(2*tmpx-tmpy)/2;
		posy=(2*tmpy+tmpx)/2;
		break;
		case 2:
		case 5:
		case 8:
		case 11:
		posx=(-3*tmpy+4*tmpx)/5;
		posy=(4*tmpy+3*tmpx)/5;
		break;
		default:
		posx=(-2*tmpy+4*tmpx)/5;
		posy=(4*tmpy+2*tmpx)/5;
		break;
	}
	posr--;
	if (posr>11) posr=11;
}

IWRAM_CODE int main()
{
	char x,y;
	int i;
	
	//Se puede hacer constante
	tempgfxp=malloc(4*(MODE5_HEIGHT+1));
	for (i=0;i<=MODE5_HEIGHT;i++) tempgfxp[i]=(MODE5_WIDTH*(MODE5_HEIGHT-i));
	
	punterosalturas=malloc(numalturas*4);
	actualiza_punterosalturas();
	REG_DISPCNT_L = BG2_ENABLE | MODE5;
	
	REG_BG2PA=INI_X;	//escalado horizontal;
	REG_BG2PD=INI_Y;	//escalado vertical;
	REG_BG2X=INI_DX;	//desp horizontal
	REG_BG2Y=10;		//desp vert
	while (1)
	{
		memset(VRAM,0x62,MODE5_WIDTH*MODE5_HEIGHT);	//memcpy(VRAM,rawback,MODE5_WIDTH*MODE5_HEIGHT);
		rend[posr]();
		while (!((REG_DISPSTAT)&1));
		REG_BG2PA=INI_X;	//escalado horizontal;
		REG_BG2PD=INI_Y;	//escalado vertical;
		REG_BG2X=INI_DX;	//desp horizontal
		REG_BG2Y=INI_DY;	//desp vert
		REG_DISPCNT_L^=BACK_BUFFER_FLAG;
		
		VRAM= (VRAM==BACK_BUFFER) ? FRONT_BUFFER : BACK_BUFFER;
		
		while ((tecla_pulsada=INPUT)==0);

		if (tecla_pulsada&KEY_L) {incr_a();}
		if (tecla_pulsada&KEY_R) {decr_a();}
			
		if ((tecla_pulsada&KEY_B)&&(posz>0)){ posz-=1;}
		else if ((tecla_pulsada&KEY_A)&&(posz<(numalturas-1))) { posz+=1;}
		if (tecla_pulsada&KEY_DOWN){ posy-=3;}
		else if (tecla_pulsada&KEY_UP){ posy+=3;}
		if (tecla_pulsada&KEY_LEFT){ posx-=2;}
		else if (tecla_pulsada&KEY_RIGHT){ posx+=2;}
		
	}
	//while (1);
}
