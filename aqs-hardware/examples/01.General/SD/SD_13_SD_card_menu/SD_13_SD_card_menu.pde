/*  
 *  ------ [SD_13] - SD card menu -------- 
 *  
 *  Explanation: Turn on the SD card. Delete specified filename
 *  in the case it exists. The first loop creates the file, but 
 *  the following loops will not because the file already exists. 
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
 *  Implementation:    Yuri Carmona
 */


void setup()
{
  // Open USB port
  USB.ON();
  USB.println(F("SD_13 example"));    

  // explore the SD contents
  SD.menu(20000);

  
  USB.println(F("\n\n*** Setup done ***"));
}


void loop()
{  
  // do nothing
}


