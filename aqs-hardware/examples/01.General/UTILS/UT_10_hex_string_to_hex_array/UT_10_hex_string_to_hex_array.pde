/*
 *  ------ [UT_10] - Hex string to hex array ------
 *
 *  Explanation: this example shows how to convert a number from a
 *  string representation to the long integer type depending on the 
 *  base used to define the number as a string message
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
 *  Version:          3.0
 *  Design:           David Gascón
 *  Implementation:   Yuri Carmona
 */

// define variables
char     buffer1[] = "48656C6C6F20576173706D6F7465";
uint8_t  buffer2[200];
char     buffer3[200];
uint16_t size;


void setup()
{
  USB.ON();
  USB.println(F("Start program"));

  ///////////////////////////////////////////////////////////////////
  // 1. String to Array 
  // From 'buffer1' to 'buffer2'. The ascii representation of 
  // hexadecimal digits is converted to array of bytes:
  // From "48656C..." to {0x48 0x65 0x6C ...}
  ///////////////////////////////////////////////////////////////////
  size = Utils.str2hex(buffer1, buffer2, sizeof(buffer2));


  USB.println(F("----------------------------------------------------------------------"));

  USB.print(F("Input string with hex digits: \""));
  USB.print(buffer1);
  USB.println(F("\""));
  
  USB.print(F("Output array (ASCII): "));
  USB.println(buffer2, size);
  
  USB.print(F("Output array of bytes (HEX): "));
  for (int i=0; i<size; i++)
  {
    USB.print(F(" 0x"));
    USB.printHex(buffer2[i]);
  }  

  USB.println(F("\n----------------------------------------------------------------------"));

  
  ///////////////////////////////////////////////////////////////////
  // 2. Array to String
  // From 'buffer2' to 'buffer3'. The bytes in the array are converted
  // to hexadecimal digits:
  // From {0x48 0x65 0x6C ...} to "48656C..."
  ///////////////////////////////////////////////////////////////////
  Utils.hex2str(buffer2, buffer3, size);
  
  USB.print(F("Input array of bytes (HEX): "));
  for (int i=0; i<size; i++)
  {
    USB.print(F(" 0x"));
    USB.printHex(buffer2[i]);
  }

  USB.print(F("\nOutput string : \""));
  USB.print(buffer3);
  USB.println(F("\""));
  
  USB.println(F("----------------------------------------------------------------------"));

}



void loop()
{
 // do nothing
}



