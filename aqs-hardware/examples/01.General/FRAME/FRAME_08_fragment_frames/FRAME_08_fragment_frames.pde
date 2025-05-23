/*
    ------ FRAME_08 - fragment frames --------

    Explanation: This example shows how to fragment an original binary
    frame. ASCII frames are not permitted.
    The resulting fragments are stored in frame.bufferFragment and the
    length of this buffer is frame.lengthFragment

    Copyright (C) 2017 Libelium Comunicaciones Distribuidas S.L.
    http://www.libelium.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Version:           3.0
    Design:            David Gascón
    Implementation:    Yuri Carmona
*/


#include <WaspFrame.h>

// define the Waspmote ID
char waspmote_id[] = "node_01";


void setup()
{
  // init USB port
  USB.ON();
  USB.println(F("Start program"));
  USB.println(F("-------------------------------"));
  USB.println(F("Only BINARY frames can be "));
  USB.println(F("fragmented if required "));
  USB.println(F("-------------------------------"));
  USB.println();

  // set the Waspmote ID
  frame.setID(waspmote_id);
}


void loop()
{
  ///////////////////////////////////////////
  // 1. Create BINARY frame
  ///////////////////////////////////////////

  // create new frame (BINARY)
  frame.createFrame(BINARY);

  // set frame fields
  frame.addSensor(SENSOR_STR, "string1");
  frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel());
  frame.addSensor(SENSOR_STR, "string2");
  frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel());
  frame.addSensor(SENSOR_STR, "string3");
  frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel());
  frame.addSensor(SENSOR_STR, "string4");
  frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel());
  frame.addSensor(SENSOR_STR, "string5");
  frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel());
  frame.addSensor(SENSOR_STR, "string6");
  frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel());
  frame.addSensor(SENSOR_STR, "string7");
  frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel());
  frame.addSensor(SENSOR_STR, "string8");
  frame.addSensor(SENSOR_BAT, PWR.getBatteryLevel());

  // print frame
  USB.println(F("Original frame:"));
  frame.showFrame();
  USB.println();


  ///////////////////////////////////////////
  // 2. Create fragments
  ///////////////////////////////////////////


  USB.print(F("Maximum frame size:"));
  USB.println(80);

  while (frame.createFragmentHeader(80) > 0)
  {
    frame.generateFragment();
    USB.println(F("Fragment frame:"));
    USB.println(F("-------------------------------"));
    USB.printHexln(frame.bufferFragment, frame.lengthFragment);
    USB.println(F("-------------------------------"));
    USB.println();
  }
  USB.println();

  delay(5000);
}

