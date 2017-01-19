#include <stdio.h>
#include <stdbool.h>
#include <math.h>


void itob(int ,int *);
int btoi(int *);
void channel_set(int *,int ,bool );

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


