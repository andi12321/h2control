/*
bmc.cpp
library für Steuerungs-relevante Funktionen
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "libad/libad.h"
#include <ncurses.h>
#include "binchange.h"

#if 0
void initiate_screen()
{
  
}
#endif

#if 0
int menu()
{
  char mbef;
  mvprintw(0,50,"--MENU--");
  mvprintw(1,60,"Hydrogenation-Cycle program [h]");
  mvprintw(2,60,"pressure-program (empty)");
  mvprintw(3,60,"practical demonstration (empty)");
  while((mbef=getch()) != 10){
	  if (mbef == 'h'){
		  //hydrogenation;
		  mvprintw(4,60,"pressed h");

		  break;
		  }
	  }
  // hier gehört dann ein getch her!!
}
#endif



// 		Usage - Beschreibung des Inputs
int usage ()
{
  printw ("  dfdf usage: ex1 <driver> [ -r <range> ] <cha1> .. <chan>\n"
          "  <driver>     string to pass to ad_open()\n"
          "               - will prompt for name\n"
          "  <range>      range number of analog input\n"
          "  <cha1>       number of first analog input to read\n"
          "  <chan>       number of last analog input to read\n");
  return 0;
}

#if 0
//		gelesene Werte aus Analog IN schreiben
int refresh_val (int32_t rc,int32_t adh,uint32_t data,float u[])
{
int i=0,x=0,y=0;
int32_t cha=0;
uint32_t rng=3;

 
  mvprintw (0,0,"Analog IN");

  for (i=0; i<=15;i++)
    {
      cha = i;
      rc = ad_discrete_in(adh, AD_CHA_TYPE_ANALOG_IN|cha, rng, &data);
      if (rc == 0)
        rc = ad_sample_to_float (adh, AD_CHA_TYPE_ANALOG_IN|cha, rng, data, &u[i]);
      if (rc == 0)
      {
	getyx(stdscr,y,x);
        mvprintw (i+1,0,"cha %2d:", cha);
	attron(A_BOLD);
        mvprintw (i+1,10,"%7.3f V", u[i]);
	attroff(A_BOLD);
      }

      else
        printw("Fail\n");
    }
#if 0
  mvprintw (20,0,"+--- V1 --- M1 ---< Elektrolyseur\n\
|\n\
|\n\
+--- V2 --- M2 ---< Buffer\n\
|\n\
|\n\
+--- V3 --- M3 ---< Tank ... T1\n\
|\n\
|\n\
+--- V4 --- M4 ---< Brennstoffzelle\n");
#endif

  return 0;
}
#endif

#if 0
//		Digitale Ausgänge schreiben/aktualisieren
int refresh_dout (int bef, int state[],uint32_t dataout, int chdi)
{
int rc=0,adh=0,i=0;

mvprintw (0,20,"Digital OUT");
if (state[bef-48]==0) state[bef-48]=1;
else  state[bef-48]=0;
	//for (i=0;i<=15;i++) printw("%d",state[i]);
	//printw("\n");
	dataout=btoi(state);
        //printw("Bef: %d\n",bef);
	//printw("\n");
	//printw("Dataout: %d\n",dataout);
        rc = ad_discrete_out(adh,AD_CHA_TYPE_DIGITAL_IO|chdi,0,dataout);
        for (i=0; i<=15;i++) 
	{ 
	  if (state[i]==0) mvprintw (i+1,20,"Channel %2d: %d\n",i, state[i]);
	  if (state[i]==1) mvprintw (i+1,20,"Channel %2d: %d ON",i,state[i]);
	}
	//printw("RC: %d\n ",rc);
  return 0;
}
#endif
