/*
 *  ------ UT_11 Show Waspmote version ------
 *
 *  Explanation: this example shows how to display the Waspmote 
 *  version number: v12 or v15
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


void setup()
{
  USB.ON();
  USB.println(F("Start program"));

  Utils.showVersion();
}



void loop()
{
 // do nothing
}



