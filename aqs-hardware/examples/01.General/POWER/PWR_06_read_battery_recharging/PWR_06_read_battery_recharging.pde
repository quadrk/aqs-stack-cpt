/*  
 *  ------ [PWR_6] Read battery recharging -------- 
 *  
 *  Explanation: This example shows how to read the recharging state
 *  of the battery (1=recharging; 0=not recharging)
 *  Besides, it shows how to read the current recharged (by the solar 
 *  panel only)
 *
 *  WARNING: This example is suitable for rechargeable batteries only. 
 *  Non-rechargeable batteries always show a plain discharge curve, 
 *  so it is not possible to know how much power is left.
 *  
 *  Copyright (C) 2016 Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 *  
 *  Version:           3.0
 *  Design:            David Gascon 
 *  Implementation:    Marcos Yarza
 */

char WARNING_MESSAGE[] =\
"\n-------------------------------------------------------------------\n"\
"WARNING: This example is suitable for Waspmote v15 only\n"\
"-------------------------------------------------------------------\n";

bool chargeState;
uint16_t chargeCurrent;


void setup()
{
  // init USB
  USB.ON();
  USB.println(F("PWR_6 example"));

  // show warning
  USB.println( WARNING_MESSAGE );
}



void loop()
{
  // get charging state and current
  chargeState = PWR.getChargingState();
  chargeCurrent = PWR.getBatteryCurrent(); 
  
  
  // Show the battery charging state. This is valid for both USB and Solar panel
  // If any of those ports are used --> the charging state will be true
  USB.print(F("Battery charging state: "));
  if (chargeState == true)
  {
    USB.println(F("Battery is charging"));
  }
  else
  {
    USB.println(F("Battery is not charging"));
  }

  // Show the battery charging current (only from solar panel)
  USB.print(F("Battery charging current (only from solar panel): "));
  USB.print(chargeCurrent, DEC);
  USB.println(F(" mA"));

  USB.println();  
  delay(5000);
}

