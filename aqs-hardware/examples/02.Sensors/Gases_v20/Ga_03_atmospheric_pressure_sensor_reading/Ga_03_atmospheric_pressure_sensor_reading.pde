/*  
 *  ------ [Ga_3] - Atmospheric Pressure Sensor for Gases v20-------- 
 *  
 *  Explanation: Turn on the Gases Board v20 and read the atmospheric
 *  pressure sensor every second, printing the result through the USB
 *  
 *  Copyright (C) 2012 Libelium Comunicaciones Distribuidas S.L. 
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
 *  Version:           0.1 
 *  Design:            David Gascón 
 *  Implementation:    
 */

#include <WaspSensorGas_v20.h>

//Variable to store the read atmospheric pressure value
float pressureVal;

void setup()
{
  //Turn on the USB and print a start message
  USB.ON();
  USB.println(F("start"));
  delay(100);
}
 
void loop()
{
  // Turn on the sensor board
  SensorGasv20.ON();
  
  // Turn on the atmospheric pressure sensor and wait for stabilization and
  // sensor response time
  SensorGasv20.setSensorMode(SENS_ON, SENS_PRESSURE);
  delay(20); 
 
  // Read the sensor
  pressureVal = SensorGasv20.readValue(SENS_PRESSURE);
  
  // Turn off the atmospheric pressure sensor
  SensorGasv20.setSensorMode(SENS_OFF, SENS_PRESSURE);
    
  // Turn off the sensor board
  SensorGasv20.OFF();
  
  // Print the result through the USB
  USB.print(F("Pressure: "));
  USB.print(pressureVal);
  USB.println(F(" kPa"));
  
  delay(1000);
}
