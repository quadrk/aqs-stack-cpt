/*
    ------ FRAME_05 - set Frame Size  --------

    Explanation: This example shows how to set the maximum frame size
    automatically by giving the settings for the communication module

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

    Version:           3.1
    Design:            David Gascón
    Implementation:    Yuri Carmona
*/

#include <WaspFrame.h>

// define variable for maximum size
uint16_t maxSize;



void setup()
{
  // Init USB port
  USB.ON();
  USB.println(F("FRAME_05 example"));

  
  /////////////////////////////////////////////
  // 1. set frame size for XBee 802.15.4
  /////////////////////////////////////////////
  //  - XBee-PRO 802.15.4
  //  - Unicast 16-b addressing
  //  - XBee link-layer encryption Disabled
  //  - AES application-layer encryption Disabled
  maxSize = frame.getMaxSizeForXBee(XBEE_802_15_4, UNICAST_16B, DISABLED, DISABLED);  
  frame.setFrameSize(maxSize);

  
  USB.println(F("---------------------------------"));
  USB.print(F("Frame size:"));
  USB.println(frame.getFrameSize(), DEC); 
  USB.println(F("---------------------------------"));
  USB.println(F("For settings: \n- XBee-PRO 802_15_4\n- UNICAST_16B\n- XBee encryp Disabled\n- AES encryp Disabled"));
  USB.println();
 

  /////////////////////////////////////////////
  // 2. set frame size for XBee 868LP
  /////////////////////////////////////////////
  //  - XBee 868 LP
  //  - Unicast 64-b addressing (default)
  //  - XBee link-layer encryption Enabled
  //  - AES application-layer encryption Enabled
  maxSize = frame.getMaxSizeForXBee(XBEE_868LP, ENABLED, ENABLED);
  frame.setFrameSize(maxSize);
  
  USB.println(F("---------------------------------"));
  USB.print(F("Frame size:"));
  USB.println(frame.getFrameSize(), DEC); 
  USB.println(F("---------------------------------"));
  USB.println(F("For settings: \n- XBee-PRO 868LP\n- UNICAST_64B\n- XBee encryp Enabled\n- AES encryp Enabled"));  
  USB.println();


  /////////////////////////////////////////////
  // 3. set frame size for XBee ZigBee
  /////////////////////////////////////////////
  //  - XBee ZigBee
  //  - Broadcast addressing
  //  - XBee link-layer encryption Enabled
  //  - AES application-layer encryption Disabled
  maxSize = frame.getMaxSizeForXBee(ZIGBEE, BROADCAST, ENABLED, DISABLED);
  frame.setFrameSize(maxSize);

  USB.println(F("---------------------------------"));
  USB.print(F("Frame size:"));
  USB.println(frame.getFrameSize(), DEC); 
  USB.println(F("---------------------------------"));
  USB.println(F("For settings: \n- XBee ZigBee\n- UNICAST_64B\n- XBee encryp Enabled\n- AES encryp Disabled"));  
  USB.println();


  /////////////////////////////////////////////
  // 4. set frame size for XBee-PRO 900HP
  /////////////////////////////////////////////
  //  - XBee-PRO 900HP
  //  - Unicast 64-b addressing (default)
  //  - XBee link-layer encryption Disabled
  //  - AES application-layer encryption Enabled
  maxSize = frame.getMaxSizeForXBee(XBEE_900HP, DISABLED, ENABLED);
  frame.setFrameSize(maxSize);  

  USB.println(F("---------------------------------"));
  USB.print(F("Frame size:"));
  USB.println(frame.getFrameSize(), DEC); 
  USB.println(F("---------------------------------"));
  USB.println(F("For settings: \n- XBee-PRO 900HP\n- UNICAST_64B\n- XBee encryp Disabled\n- AES encryp Enabled"));  
  USB.println();


  /////////////////////////////////////////////
  // 5. set frame size for XBee-PRO DigiMesh
  /////////////////////////////////////////////
  //  - XBee-PRO DigiMesh
  //  - Broadcast addressing
  //  - XBee link-layer encryption Enabled
  //  - AES application-layer encryption Enabled
  maxSize = frame.getMaxSizeForXBee(DIGIMESH, BROADCAST, ENABLED, ENABLED);
  frame.setFrameSize(maxSize);

  USB.println(F("---------------------------------"));
  USB.print(F("Frame size:"));
  USB.println(frame.getFrameSize(), DEC); 
  USB.println(F("---------------------------------"));
  USB.println(F("For settings: \n- XBee-PRO DigiMesh\n- UNICAST_64B\n- XBee encryp Enabled\n- AES encryp Enabled")); 
  USB.println();


  /////////////////////////////////////////////
  // 6. set frame size for WiFi PRO 
  /////////////////////////////////////////////
  //  - Send packets to Meshlium
  //  - AES application-layer encryption Disabled
  maxSize = frame.getMaxSizeForWifi("https", "10.10.10.1", "443", DISABLED);
  frame.setFrameSize(maxSize);

  USB.println(F("---------------------------------"));
  USB.print(F("Frame size:"));
  USB.println(frame.getFrameSize(), DEC);
  USB.println(F("---------------------------------"));
  USB.println(F("For settings: \n- WIFI-PRO\n- https\n- AES encryption Disabled"));  
  USB.println();


  /////////////////////////////////////////////
  // 6. set frame size via parameter given by the user
  /////////////////////////////////////////////
  frame.setFrameSize(125);

  USB.print(F("\nframe size given by the user (125):"));
  USB.println(frame.getFrameSize(), DEC);


}


void loop()
{
  // do nothing
}

