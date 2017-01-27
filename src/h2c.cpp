/* h2c.cpp  */
#include "binchange.h"
#include "bmc.h"
//#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>
#include <errno.h>
#include "libad/libad.h"
#include <ncurses.h>


/*
int main(int argc, char *argv[])
{
  int i,sam,a;
  int chon[15];
  char buf;
  if (argc<2) 
  {
    printf("falsch\n");
    return -1;
  }
 
  sam=atoi(argv[1]);
  printf("Eingegebener Wert: %d\n",sam);
  itob(sam,chon);
  printf("Einge. Wert binär: ");
  for (i=0; i<16;i++) printf("%d",chon[i]);
  printf("\n");
  sam=0;
  sam=btoi(chon);
  printf("Rückgerechneter Wert: %d\n",sam);
  channel_set(chon,atoi(argv[2]),1);
  printf("Wert binär modifiziert : ");
  for (i=0; i<16;i++) printf("%d",chon[i]);
  printf("\n");

  return 0;
}
*/


int main ()
//int main (int argc, char *argv[])
{
  //char *driver;
  const char *driver = "lanbase:131.130.31.144";  /* BMCM - Gerät initiieren */
  int i=0,bef=0,state[15],chdi=0;
  int32_t adh=0, rc=0;
  uint32_t data=0, dataout=0, rng=3;
  float u[15];
  int32_t cnt;
  int32_t cha;
  struct ad_range_info info;
  struct ad_device_info devinfo;


  initscr(); 			/* Ncurses initiieren */
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1,COLOR_WHITE, COLOR_BLUE);
  init_pair(2,COLOR_GREEN, COLOR_BLACK);
  init_pair(3,COLOR_BLACK, COLOR_GREEN);

  attron(COLOR_PAIR(1));

/*
  if (argc <= 1)		 Wenn keine Angaben - usage*/
/*  {
    usage ();
    endwin();
    exit(1);
  }
*/

  adh = ad_open(driver);

  for (i=0;i<=15;i++) state[i]=0;
  chdi=2;
  dataout=0;

  rc = ad_set_line_direction(adh,AD_CHA_TYPE_DIGITAL_IO|chdi,0x00);
  rc = ad_discrete_out(adh,AD_CHA_TYPE_DIGITAL_IO|chdi,0,dataout);
  if (adh == -1)
    {
      if (ad_invalid_driver_version (errno))
        printw ("failed to open %s: invalid driver version\n", driver);
      else
        printw ("failed to open %s: err = %d\n", driver, errno);
    }

//  initiate_screen();
  
  //cha=1;

#if 0
  rc = ad_get_dev_info(adh,&devinfo);
  attron(COLOR_PAIR(2));
   cnt=devinfo.analog_in;
  printw ("%u\n ",devinfo.analog_in);
  printw ("%u\n ",devinfo.analog_out);
  printw ("%u\n ",devinfo.digital_io);
  printw ("%u\n ",devinfo.can);
  printw ("%u\n ",devinfo.counter);
  printw ("Press any key to start\n");
  getch();
#endif

//---------------File handling ------------

#if 0
   static const char filename[] = "doc/h2_config";
   FILE *file = fopen ( filename, "r" );
   if ( file != NULL )
   {
      printw ("h2_config Inhalt:\n");
      char line [10][ 25 ]; /* or other suitable maximum line size */
      int i=1,config[10];
      while ( fgets ( line[i], sizeof line[i], file ) != NULL ) /* read a line */
      {
         //fputs ( line, stdout ); /* write the line */
         //printw (line[i]);
	 //printw ("%i\n",i);
	 config[i] = atoi(line[i]);
	 printw("Config[%i]=%i\n",i,config[i]);
	 i=i+1;
         
      }
      fclose ( file );
   }
   else
   {
      perror ( filename ); /* why didn't the file open? */
   }
  //FILE *fp;
  //char h2_config[]="h2_config"; // write filenames

  //fp = fopen(h2_config,"w");
  //fclose(fp);
  printw ("Press any key to continue\n");
  getch();
#endif


//-----------------------------------------

/*    for (i=0;i < cnt; i++)
	    {
		    printw ("i: %2u/%2u\n",i,cnt);
		    rc = ad_get_range_info(adh, cha, i, &info);
		    printw("range: %f\n",info.max);// --- print-lösung suchen
	    }
	    getch();
	goto ende;
*/

  timeout(1000); // ein mal pro Sekunde:
  while((bef = getch()) != 10)
    {
        move(0,0);
        attron(COLOR_PAIR(2));
	refresh_val(rc,adh,data,u); //read values from analouge inputs (hsteuerung.h)
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(3));
        refresh_dout(bef,state,dataout,chdi); //Digital-Out Values schreiben (hsteuerung.h)
        attroff(COLOR_PAIR(3));
	move(18,0);
	printw("Key-Code: %d",bef);
	if (bef == 'm') // press m for menu
		{
		 menu();
		}
        refresh();
	//s
    }

  ende:
  dataout=0;
  rc = ad_discrete_out(adh,AD_CHA_TYPE_DIGITAL_IO|chdi,0,dataout);

  refresh();
  endwin();
  
  return 23;
}




