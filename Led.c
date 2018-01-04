#include "SmartM_8051.h"

VOID LedInit(VOID)
{
  /* P2.5,P2.6,P2.7Òı½ÅÍÆÍìÊä³ö */
  P2M1&=~((1<<5)|(1<<6)|(1<<7));
  P2M0|=  (1<<5)|(1<<6)|(1<<7);
}

VOID Led1(BOOL bIsOn)
{
	 if(bIsOn)P2_5 =0;
	 else     P2_5 =1;
}

VOID Led2(BOOL bIsOn)
{
	 if(bIsOn)P2_6 =0;
	 else     P2_6 =1;
}

VOID Led3(BOOL bIsOn)
{
	 if(bIsOn)P2_7 =0;
	 else     P2_7 =1;
}

