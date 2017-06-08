#include <stdio.h>
#include <stdbool.h>
#include <math.h>



void channel_set(int b[],int chan, bool oo)
{
  if (b[chan]!=oo)
  {
    b[chan]=oo;
  }
}

void itob(int a,int b[])		/* Integer to Binary-Array 0-15=1-32768 */
{
  int i,buf;
  buf=a;
  for (i=15; i >= 0; i--)
    {
      if (buf>= pow(2,i))
      {
        b[i]=1;
	buf=buf-pow(2,i);
      }
      else b[i]=0;
    }
}
#if 0
char *get_state(int a)		/* Integer to Binary-Array 0-15=1-32768 */
{
  int i,bufi,b[16];
  char stringreturn[19];
  buf=a;
  for (i=15; i >= 0; i--)
    {
      if (buf>= pow(2,i))
      {
        b[i]=1;
	buf=buf-pow(2,i);
      }
      else b[i]=0;
    }
  stringreturn=sprintf("%i%i%i%i %i%i%i%i %i%i%i%i %i%i%i%i",b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8],b[9],b[10],b[11],b[12],b[13],b[14],b[15],b[16])
  return stringreturn;
}
#endif

int btoi(int b[])		/* Binary-Array 0-15=1-32768  to Integer*/
{
  int i,buf,a;
  a=0;
  for (i=15; i >= 0; i--)
    {
      if (b[i]==1)
      {
        a=a+pow(2,i);
      }
    }
  return a;  
}


