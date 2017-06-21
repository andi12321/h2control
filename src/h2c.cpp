/* h2c.cpp  */
#include "binchange.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "libad/libad.h"
#include <ncurses.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sys/time.h>
#include <bitset>
#include <sstream>
#include <chrono>
#include <thread>
using namespace std;

int state[15];

//-----------------------------------------------------------------------------------------------------------
//---------------------------------  Function Defs   --------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------

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
            mvprintw (j+1,10,"%7.3f V", u[j]);
        }

        else
            printw("Fail\n");
    }


    return 0;
}

//		Digitale Ausgänge schreiben/aktualisieren
int refresh_dout (int bef, int funcstate[], uint32_t dataout, int chdi)
{
    int rc=0,adh=0,i=0, key;

    mvprintw (0,20,"Digital OUT");
    if (funcstate[bef-97]==0) funcstate[bef-97]=1;
    else  funcstate[bef-97]=0;
    dataout=btoi(funcstate);
    rc = ad_discrete_out(adh,AD_CHA_TYPE_DIGITAL_IO|chdi,0,dataout);
    for (i=0; i<=15;i++) 
    { 
        key = i + 65;
        if (funcstate[i]==0) mvprintw (i+1,20,"Channel %1c: %d\n",(char) key, funcstate[i]);
        if (funcstate[i]==1) mvprintw (i+1,20,"Channel %1c: %d ON", (char) key, funcstate[i]);
    }
    return dataout;
}

//-----------------------------------------------------------------------------------------------------------
//---------------------------------  Main Part   ------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
int main (int argc, char* argv[])
{
    printf("check");
    if (argc < 2) {
        // Tell the user how to run the program
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        /* "Usage messages" are a conventional way of telling the user
         * how to run a program if they enter the command incorrectly.
         */
        return 1;
    }
    
    std::string command ("cp ");
    std::string temp_filename ("");
    temp_filename += argv[1];           // character
    temp_filename += "_temp";           // character
    
    command += argv[1];         // c-string
    command += " ";         // string
    command += temp_filename;           // character
    
    std::cout << command << std::endl;
    system(command.c_str());
    
    using namespace std::this_thread; // sleep_for, sleep_until
    using namespace std::chrono; // nanoseconds, system_clock, seconds
    const char *driver = "lanbase:131.130.31.144";  /* BMCM - Gerät initiieren */
    int i=0,bef=0, chdi=0, saving=0, error, refreshtime=1000 ;
    int32_t adh=0, rc=0;
    uint32_t data=0, dataout=0, rng=3, print_state=0;
    float u[15];
    float time_now;
    int32_t cnt;
    int32_t cha;
    struct ad_range_info info;
    struct ad_device_info devinfo;
    struct timeval current_time;
    struct timespec time_before, time_after;
    //FILE *temp_file = fopen (temp_filename.c_str(), "a+");
    FILE *fp = fopen("doc/data", "w+");
    int cycles, elements;
    string filename;
    struct procedure_datastructure{
        int run[20], dur[20];
        int state[20][15];
    } procedure;
    //char c;
    //std::cin >>c ;
    //std::cout <<"the value of '"<<c<<"' is "<<int(c)<<'\n';


    string line;
    ifstream temp(temp_filename.c_str());
    while (getline(temp, line))
    {
        cout << "READ ---------------" << line << endl;
        switch(line[0])
        {
        case 'G':   
        cycles = atoi(line.substr(2,4).c_str());
        cout << "Cycles: " << cycles << endl;
        break;

        case 'F':   
        filename = line.substr(2,40);
        cout << "Filename: " << filename << "|" << endl;
        break;

        case 'E':
        elements = atoi(line.substr(2,3).c_str());
        cout << "Elements: " << elements << endl;
        break;
        
        case 'R':
        procedure.run[atoi(line.substr(48,1).c_str())]=atoi(line.substr(2,5).c_str());
        procedure.dur[atoi(line.substr(48,1).c_str())]=atoi(line.substr(8,4).c_str());

        procedure.state[atoi(line.substr(48,1).c_str())][0] = atoi(line.substr(13,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][1] = atoi(line.substr(14,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][2] = atoi(line.substr(15,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][3] = atoi(line.substr(16,1).c_str());

        procedure.state[atoi(line.substr(48,1).c_str())][4] = atoi(line.substr(18,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][5] = atoi(line.substr(19,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][6] = atoi(line.substr(20,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][7] = atoi(line.substr(21,1).c_str());

        procedure.state[atoi(line.substr(48,1).c_str())][8] = atoi(line.substr(23,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][9] = atoi(line.substr(24,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][10] = atoi(line.substr(25,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][11] = atoi(line.substr(26,1).c_str());

        procedure.state[atoi(line.substr(48,1).c_str())][12] = atoi(line.substr(28,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][13] = atoi(line.substr(29,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][14] = atoi(line.substr(30,1).c_str());
        procedure.state[atoi(line.substr(48,1).c_str())][15] = atoi(line.substr(31,1).c_str());
        

        for (i=0;i<=15;i++)
        {
            cout <<  procedure.state[atoi(line.substr(48,1).c_str())][i];
        }
        cout << endl;                   
        break;
        }

    }



//-----------------------------------------------------------------------------------------------------------
//---------------------------------  Main Loop   ------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
    for (int cycle_counter=1; cycle_counter<=cycles;cycle_counter++)
    {
        cout << "Cycle " << cycle_counter << endl;
        for (int element_counter=1; element_counter<=elements;element_counter++)
        {
            cout << "Element...  " << element_counter << endl;
            for (int unit_counter=0; unit_counter<=procedure.run[element_counter]; unit_counter++)
            {
                cout << "Unit " << unit_counter << " @ " << procedure.dur[element_counter] << endl; 
            }
        }
    }



#if 0
    char line[100];
    //char a[1],b[1],c[1],d[1];
    if (fgets (line, 100, temp_file) != NULL)
        {
        printf("%c%c%c%c\n",line[1],line[2],line[3],line[4]);
        }
#endif    
    //printf("Dur: %i\n", duration);
    //printf("%c\n", c);
    return 0;

    initscr(); 			/* Ncurses initiieren */
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1,COLOR_WHITE, COLOR_BLUE);
    init_pair(2,COLOR_GREEN, COLOR_BLACK);
    init_pair(3,COLOR_BLACK, COLOR_GREEN);

    attron(COLOR_PAIR(2));
    adh = ad_open(driver);

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


//-----------------------------------------------------------------------------------------------------------
//---------------------------------  Main Loop   ------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
    timeout(984); // ein mal pro Sekunde:
    while((bef = getch()) != 27)
    {
        refresh_val(rc,adh,data,u); //read values from analouge inputs (bmc.cpp)

        print_state = refresh_dout(bef,state,dataout,chdi); //Digital-Out Values schreiben (bmc.cpp)
        mvprintw(18,0,"<A-P> ...... trigger Valves \n<Space> .... trigger Saving to file \n<ESC> ...... Exit");
        if (saving == 1)
        {
            gettimeofday(&current_time, NULL);
            fprintf(fp,"%i.%i",current_time.tv_sec, current_time.tv_usec/10000);
            fprintf(fp, "%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f ", u[0],u[1],u[2],u[3],u[4],u[5],u[6],u[7],u[8],u[9],u[10],u[11],u[12],u[13],u[14],u[15]);
            fprintf(fp, "%i%i%i%i %i%i%i%i %i%i%i%i %i%i%i%i\n", state[0], state[1], state[2], state[3], state[4], state[5], state[6], state[7], state[8], state[9], state[10], state[11], state[12], state[13], state[14], state[15]);
            fflush(fp);
            mvprintw(17, 0, "SAVING @  ");
            mvprintw(17, 10, argv[1]);
        }
        if (bef == 32) // press Space for Saving
        {
            if (saving == 0)
            {
                attroff(COLOR_PAIR(2));
                attron(COLOR_PAIR(3));
                saving=1;
            }
            else if (saving == 1)
            {
                attroff(COLOR_PAIR(3));
                attron(COLOR_PAIR(2));
                mvprintw(17,0,"                                                           ");
                saving=0;
            }
        }
        else {}
        refresh();

    }

ende:
    dataout=0;
    rc = ad_discrete_out(adh,AD_CHA_TYPE_DIGITAL_IO|chdi,0,dataout);
    refresh();
    endwin();
    fclose(fp);
    printw("Closing!");
    return 23;
}

