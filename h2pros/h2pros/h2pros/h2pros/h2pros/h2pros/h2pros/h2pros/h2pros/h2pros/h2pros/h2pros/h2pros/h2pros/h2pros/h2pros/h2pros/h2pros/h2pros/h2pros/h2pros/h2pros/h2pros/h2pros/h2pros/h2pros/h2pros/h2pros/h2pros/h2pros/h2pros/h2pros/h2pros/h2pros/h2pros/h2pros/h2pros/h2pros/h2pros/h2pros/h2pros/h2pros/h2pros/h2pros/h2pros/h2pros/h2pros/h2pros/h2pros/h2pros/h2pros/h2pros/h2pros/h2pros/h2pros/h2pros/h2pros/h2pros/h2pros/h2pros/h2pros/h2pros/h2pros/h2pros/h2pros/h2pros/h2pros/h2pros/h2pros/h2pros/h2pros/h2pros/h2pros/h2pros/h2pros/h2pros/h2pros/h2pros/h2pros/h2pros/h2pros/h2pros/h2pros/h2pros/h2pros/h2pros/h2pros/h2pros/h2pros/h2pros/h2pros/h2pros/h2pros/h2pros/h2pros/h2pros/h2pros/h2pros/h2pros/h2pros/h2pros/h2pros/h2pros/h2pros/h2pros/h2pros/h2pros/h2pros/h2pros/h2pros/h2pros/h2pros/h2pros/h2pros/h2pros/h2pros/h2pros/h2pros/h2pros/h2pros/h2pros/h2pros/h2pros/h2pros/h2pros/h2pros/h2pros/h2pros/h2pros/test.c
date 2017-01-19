// test.c
// wandelt das erste Argument in eine Binärzahl
// schreibt dann an der nten Stelle "1" n ... zweites Argument


#include "binchange.h"


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


