/*
 *  ------Waspmote WIFI_30b Example--------
 *
 *  Explanation: This example shows how to create a TCP 
 *  connection to comunicate with the Waspmote Wifi PC program. 
 *
 *  Copyright (C) 2013 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
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
 *  Version:                0.2
 *  Design:                 David Gascón
 *  Implementation:         Joaquin Ruiz
 */

// Include WIFI library 
#include <WaspWIFI.h>

// choose socket (SELECT USER'S SOCKET)
///////////////////////////////////////
uint8_t socket=SOCKET0;
///////////////////////////////////////

// WiFi AP settings (CHANGE TO USER'S AP)
/////////////////////////////////
#define ESSID "libelium_AP"
#define AUTHKEY "password"
/////////////////////////////////

// TCP server settings
/////////////////////////////////
#define IP_ADDRESS "2.139.174.70"
#define REMOTE_PORT 3000
#define LOCAL_PORT 2000
/////////////////////////////////

// define timeout for listening to messages
#define TIMEOUT 10000

// variable to measure time
unsigned long previous;

void setup()
{ 
  // Then switch on the WIFI module on the desired socket. 
  WIFI.ON(socket); 

  // 1. Configure the transport protocol (UDP, TCP, FTP, HTTP...) 
  WIFI.setConnectionOptions(CLIENT); 
  // 2. Configure the way the modules will resolve the IP address. 
  WIFI.setDHCPoptions(DHCP_ON);
  // 3. Configure the Authentication Key
  WIFI.setAuthKey(WPA1,AUTHKEY);
  // 4. Configure how to connect the AP 
  WIFI.setJoinMode(MANUAL);
  // 5. Store values
  WIFI.storeData(); 
} 

void loop()
{ 
  // 1. Switch ON the WiFi module
  WIFI.ON(socket);

  // 2. Join Network
  if (WIFI.join(ESSID)) 
  {    
    USB.println(F("Joined AP"));

    // 4. Call the function to create the TCP connection 
    if( WIFI.setTCPclient(IP_ADDRESS, REMOTE_PORT, LOCAL_PORT) ) 
    { 
      // 5. Reads an answer from the TCP connection (NOBLO means NOT BLOCKING)
      USB.println(F("Listen to TCP socket:"));
      previous=millis();
      while(millis()-previous<TIMEOUT)
      {
        if(WIFI.read(NOBLO)>0)
        {
          for(int j=0; j<WIFI.length; j++)
          {
            USB.print(WIFI.answer[j],BYTE);
          }
          USB.println();
        }
		// Condition to avoid an overflow (DO NOT REMOVE)
		if (millis() < previous)
		{
			previous = millis();	
		}
      }
      // Closes the TCP connection. 
      WIFI.close(); 
    }
    else
    {
      USB.println(F("TCP client NOT set"));
    }
  }
  else
  {
    USB.println(F("NOT Connected to AP"));
  } 
  
  WIFI.OFF();  
  USB.println(F("****************************"));
  delay(3000);    
  
}


