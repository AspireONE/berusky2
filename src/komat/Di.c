/*
  rutiny pro obsluhu mysi a klavesnice
*/

#include "3d_all.h"

byte      key[POCET_KLAVES];
byte      _a_klavesa;
byte		  key_pressed;
MYSI_INFO mi;

int zapni_kurzor(void)
{
 ShowCursor(TRUE);
 return(TRUE);
}

int vypni_kurzor(void)
{
 ShowCursor(FALSE);
 return(TRUE);
}
