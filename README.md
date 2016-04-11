CARE-Drone Control Software
Version 0.0.0.1

TABLE OF CONTENTS
SECTION I.......................Executive Summary
SECTION II......................Software Components
SECTION III.....................Operation Description
SECTION IV......................Class Hierarchy
SECTION V.......................Process of Execution
SECTION VI......................Extending the SPI Bus
SECTION VII.....................Software License



SECTION I.......................Executive Summary
The CARE-Drone Control Software comprises several software components
that work together in order to implement the CARE-Drone's control
algorithm. Some software components will also serve to implement
critical features of the drone, such as motor control and 
flight data readouts to the operator. The control software
is written in C, C++, and C# depending on the component and the
component's target platform. 

SECTION II......................Software Components
The CARE-Drone's Control Software is composed of several components.

1: The Flight Control Software, sometimes referred to as simply the
"control software." The control software sits as firmware on the
physical flight computer, and is the actual implementation of the 
control algorithm.

2: The Control Panel. The control panel is a C#/.NET application
targeted for 64 bit Windows platforms. The control panel, during
flight, serves as the flight data readout to the operator. 
During debugging, the control panel can also issue manual commands
to the flight computer.

3: The ESC Firmware. The ESC firmware is a firmware package written
in C that is installed in the EEPROM of the MCU's of the drone's
ESCs. This firmware serves as an extension of the flight control
software and serves the purpose of independent motor control.

4: The Ground Station Software. The ground station software is the
software package loaded on the Arduino unit of the ground station. 
The ground station serves as the receiving point to receive wireless
data from the Drone and to relay it to the Control Panel.


SECTION III.....................Operation Description
SECTION IV......................Class Hierarchy
SECTION V.......................Process of Execution
SECTION VI......................Extending the SPI Bus


SECTION VII.....................Software License
Copyright 2016  Aaron Burns,
                Joshua Donaway,
                Matthew Love,
                Department of Engineering, Harding University

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.