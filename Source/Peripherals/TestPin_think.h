/**********************************************************************************
 Dateiname: TestPin_think.h

 Begonnen:  15.06.2012, M. Suter

 Dises Modul stellt einen Testpin für genaue Zeitmessung mit Oszilloskop
 zur Verfügung.

 Prozessor Kinetis K70 von freescale

 Port E11 ist auf dem TWR-K70-Board Pin 2 der Stifleiste J6
 Auf dem I-300-Board wird Port A4 verwendet


 Aenderungen:
 ************************************************************************************/
#pragma once

#include "CommonTypes.h"

////////////////////////////////////////////////////////////////////////////////////
void InitLED_FW(void);
/* diese Prozedur muss einmal vor der erten Benutzung des Moduls aufgerufen werden. */

////////////////////////////////////////////////////////////////////////////////////
void SetLED_FW(void);
void ClearLED_FW(void);
void ToggleLED_FW(void);


void SetLED_Err(void);
void ClearLED_Err(void);
void ToggleLED_Err(void);

void InitPeripherals(void);
Bool GetPushButton(void);
// return true, if push button is pressed.

