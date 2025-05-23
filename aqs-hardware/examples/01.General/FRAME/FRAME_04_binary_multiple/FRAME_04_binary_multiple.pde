/*  
 *  ------ FRAME_04_binary_multiple - WaspFrame Binary multiple -------- 
 *  
 *  Explanation: This example creates a binary frame with a multiple field
 *  and shows it. 
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
 *  Design:            David Gascón 
 *  Implementation:    Joaquín Ruiz, Yuri Carmona
 */

#include <WaspFrame.h>
  
// define the Waspmote ID 
char moteID[] = "node_01";

// hypothetical GPS values
float gps_latitude  = 1.4465431234;
float gps_longitude = -0.8842734321;


void setup()
{
  // Init USB port
  USB.ON();  
  USB.println(F("Start program"));

  // init ACC
  ACC.ON();
  
  // set the Waspmote ID
  frame.setID(moteID);
}

void loop()
{
  // Create new frame (BINARY)
  frame.createFrame(BINARY);
  
  // set frame fields (multiple)
  frame.addSensor(SENSOR_ACC, ACC.getX(), ACC.getY(), ACC.getZ());
  
  // set frame fields (multiple)
  frame.addSensor(SENSOR_GPS, gps_latitude, gps_longitude);

  // Print frame
  frame.showFrame();

  delay(5000);
}

