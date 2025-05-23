/*
 *  ------ [UT_09] - Analog pins -------- 
 *
 *  Explanation: This example shows how to set up the power supply lines
 *  in the Waspmote I/O sockets
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
 *  Implementation:    Yuri Carmona
 */

// define variables
int analog1;
int analog2;
int analog3;
int analog4;
int analog5;
int analog6;
int analog7;

void setup()
{
  USB.ON();
  USB.println(F("UT_09 example"));
  USB.println(F("------------------------------------------"));
  USB.println(F("Analog output (0 - 3.3V): from 0 to 1023"));
  USB.println(F("------------------------------------------"));
}


void loop()
{
  // read analog pins
  analog1 = analogRead(ANALOG1);
  analog2 = analogRead(ANALOG2);
  analog3 = analogRead(ANALOG3);
  analog4 = analogRead(ANALOG4);
  analog5 = analogRead(ANALOG5);
  analog6 = analogRead(ANALOG6);
  analog7 = analogRead(ANALOG7);
  
  // show values
  USB.print(F("ANALOG1: "));
  USB.println(analog1);
  USB.print(F("ANALOG2: "));
  USB.println(analog2);
  USB.print(F("ANALOG3: "));
  USB.println(analog3);
  USB.print(F("ANALOG4: "));
  USB.println(analog4);
  USB.print(F("ANALOG5: "));
  USB.println(analog5);
  USB.print(F("ANALOG6: "));
  USB.println(analog6);
  USB.print(F("ANALOG7: "));
  USB.println(analog7);

  USB.println(F("------------------------------------------"));
  delay(1000);
}


