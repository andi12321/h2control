/* h2c.cpp  */
#include "binchange.h"
//#include "bmc.h"
//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
#include <errno.h>
#include "libad/libad.h"
#include <ncurses.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sys/time.h>
#include <bitset>
using namespace std;


#if 0
int main();
int menu();
int refresh_val (int32_t rc,int32_t adh,uint32_t data,float u[]);
int refresh_dout (int bef, int state[],uint32_t dataout, int chdi);
#endif



//-----------------------------------------------------------------------------------------------------------
//---------------------------------  Function Defs   --------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
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
    return 0;
}
//		gelesene Werte aus Analog IN schreiben
int refresh_val (int32_t rc,int32_t adh,uint32_t data,float u[])
{
    int j=0,x=0,y=0;
    int32_t cha=0;
    uint32_t rng=3;
    mvprintw (0,0,"Analog IN");

    for (j=0; j<=15;j++)
    {
        cha = j;
        rc = ad_discrete_in(adh, AD_CHA_TYPE_ANALOG_IN|cha, rng, &data);
        if (rc == 0)
            rc = ad_sample_to_float (adh, AD_CHA_TYPE_ANALOG_IN|cha, rng, data, &u[j]);
        if (rc == 0)
        {
            getyx(stdscr,y,x);
            mvprintw (j+1,0,"cha %2d:", cha);
            attron(A_BOLD);
            mvprintw (j+1,10,"%7.3f V", u[j]);
            attroff(A_BOLD);
        }

        else
            printw("Fail\n");
    }


    return 0;
}

//		Digitale Ausgänge schreiben/aktualisieren
int refresh_dout (int bef, int funcstate[],uint32_t dataout, int chdi)
{
    int rc=0,adh=0,i=0, key;

    mvprintw (0,20,"Digital OUT");
    if (funcstate[bef-97]==0) funcstate[bef-97]=1;
    else  funcstate[bef-97]=0;
    //for (i=0;i<=15;i++) printw("%d",state[i]);
    //printw("\n");
    dataout=btoi(funcstate);
    //printw("Bef: %d\n",bef);
    //printw("\n");
    rc = ad_discrete_out(adh,AD_CHA_TYPE_DIGITAL_IO|chdi,0,dataout);
    for (i=0; i<=15;i++) 
    { 
        key = i + 65;
        if (funcstate[i]==0) mvprintw (i+1,20,"Channel %1c: %d\n",(char) key, funcstate[i]);
        if (funcstate[i]==1) mvprintw (i+1,20,"Channel %1c: %d ON", (char) key, funcstate[i]);
    }
    //printw("RC: %d\n ",rc);
    return dataout;
}

//-----------------------------------------------------------------------------------------------------------
//---------------------------------  Main Part   ------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
int main ()
    //int main (int argc, char *argv[])
{
    //char *driver;
    const char *driver = "lanbase:131.130.31.144";  /* BMCM - Gerät initiieren */
    int i=0,bef=0,state[15],chdi=0, saving=0;
    int32_t adh=0, rc=0;
    uint32_t data=0, dataout=0, rng=3, print_state=0;
    float u[15];
    int32_t cnt;
    int32_t cha;
    struct ad_range_info info;
    struct ad_device_info devinfo;
    static const char filename[] = "doc/data";
    FILE *fp = fopen ( filename, "w+" );
    //time_t current_time;
    //char* c_time_string;
    struct timeval current_time;
    //char string[19];

    // ofstream outfile;
    //outfile.open("doc/h2.dat");
    //outfile << "dingsda" << endl;



    initscr(); 			/* Ncurses initiieren */
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1,COLOR_WHITE, COLOR_BLUE);
    init_pair(2,COLOR_GREEN, COLOR_BLACK);
    init_pair(3,COLOR_BLACK, COLOR_GREEN);

    attron(COLOR_PAIR(1));
    adh = ad_open(driver);

    for (i=0;i<=15;i++) {state[i]=0; fprintf(fp, "%i %i\n", i, state[i]); fflush(fp);}
    //usleep(50000);
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


//-----------------------------------------------------------------------------------------------------------
//---------------------------------  Main Loop   ------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
    timeout(1000); // ein mal pro Sekunde:
    while((bef = getch()) != 27)
    {
        move(0,0);
        attron(COLOR_PAIR(2));
        for (i=0;i<=15;i++) {state[i]=0; fprintf(fp, "b %i %i\n", i, state[i]); fflush(fp);}
        refresh_val(rc,adh,data,u); //read values from analouge inputs (bmc.cpp)
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(3));

        print_state = refresh_dout(bef,state,dataout,chdi); //Digital-Out Values schreiben (bmc.cpp)
        for (i=0;i<=15;i++) {fprintf(fp, "a %i %i\n", i, state[i]); fflush(fp);}
        string string_state = to_string(print_state);
        printw("Dataout: %i%i%i%i %i%i%i%i %i%i%i%i %i%i%i%i\n", state[0], state[1], state[2], state[3], state[4], state[5], state[6], state[7], state[8], state[9], state[10], state[11], state[12], state[13], state[14], state[15]);
        //string = get_state(print_state);
        //printw(get_state(print_state));
        attroff(COLOR_PAIR(3));
        move(18,0);
        printw("Key-Code: %d",bef);
        mvprintw(19,0,"%7.3f",u[2]);
        mvprintw(20,0,"%i",dataout);
        if (saving == 1)
        {
            //current_time = time(NULL);
            gettimeofday(&current_time, NULL); 
            //c_time_string = ctime(&current_time);
            //outfile << "time" << endl;
            i = 2016;
            std::cout << "Input the number for " << u[0] << " =";
            //fprintf(fp, "%i.%ld ,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%d\n", current_time.tv_sec, current_time.tv_usec,u[0],u[1],u[3],u[4],u[5],u[6],u[7],u[8],u[9],u[10],u[11],u[12],u[13],u[14],u[15],string_state.c_str());
            fprintf(fp, "%i.%ld ,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f\n", current_time.tv_sec, current_time.tv_usec,u[0],u[1],u[3],u[4],u[5],u[6],u[7],u[8],u[9],u[10],u[11],u[12],u[13],u[14]);
            fflush(fp);
        }
        if (bef == 32) // press Space for menu
        {
            if (saving == 0)
            {
                printw("now saving");
                saving=1;
            }
            else if (saving == 1)
            {
                printw("saving stopped");
                saving=0;
            }
        }
        refresh();
    }

ende:
    dataout=0;
    rc = ad_discrete_out(adh,AD_CHA_TYPE_DIGITAL_IO|chdi,0,dataout);

    refresh();
    endwin();
    fclose(fp);
    return 23;
}

