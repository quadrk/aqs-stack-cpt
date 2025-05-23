/*
 *  ------ [Ut_03] Waspmote reading unique serial ID number --------
 *
 *  Explanation: This example shows how to read the unique serial ID number
 *
 *  Copyright (C) 2017 Libelium Comunicaciones Distribuidas S.L. 
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
 *  Version:           3.1
 *  Design:            David Gascón 
 *  Implementation:    Marcos Yarza
 */
 
 
void setup()
{
  // Init USB
  USB.ON();
  
  // Show '_serial_id' stored by the API when powering up
  USB.print(F("Global variable '_serial_id':"));
  USB.printHex(_serial_id[0]);
  USB.printHex(_serial_id[1]);
  USB.printHex(_serial_id[2]);
  USB.printHex(_serial_id[3]);
  USB.printHex(_serial_id[4]);
  USB.printHex(_serial_id[5]);
  USB.printHex(_serial_id[6]);
  USB.printHex(_serial_id[7]);
  USB.println();
  delay(1000);
}

void loop()
{
  // Reading the serial number
  Utils.readSerialID();
  
  USB.print(F("Waspmote serial ID: "));
  USB.printHex(_serial_id[0]);
  USB.printHex(_serial_id[1]);
  USB.printHex(_serial_id[2]);
  USB.printHex(_serial_id[3]);
  USB.printHex(_serial_id[4]);
  USB.printHex(_serial_id[5]);
  USB.printHex(_serial_id[6]);
  USB.printHex(_serial_id[7]);
  USB.println();
  delay(1000);
}

