# HardyDarty
A project developed by the University of Cincinnati Rocketry Club to create a Boosted Dart with capabilities reaching Mach 1 as a lead-up to a full dual-deployment rocket. Our Avionics team was tasked with creating a custom electronics bay from choosing the parts to the 3D printed bay. This README is a synopsis of our coding, wiring, CAD, and final launch process leading to our launch on November 23rd, 2025 at the WSR club in Dayton.

# Final Launch
![DSC06829](https://github.com/user-attachments/assets/95dc577c-87c9-4418-a1ba-0a8d518f72cb)
Harty Darty reached apogee at 9,241ft, reaching Mach 1.03 with 27 Gs of force. Great turnout for the entire project and all those who participated! Team lead by Project Manager [Sam Theil](https://www.linkedin.com/in/sam-theil/), Avionics Lead [Jesse Dominguez](https://www.linkedin.com/in/jdz29/), and Structures Lead [Evan Langenderfer](https://www.linkedin.com/in/evan-langenderfer/). Check out their LinkedIns to see the amazing projects they have been up to!

# Goals
Create a fully functional avionics bay within scope, with primary features including data logging, apogee detection, reading sensors and continuity, as well as working with pyrotechnics. 
## High Priority Items


| Logging | Goals |
| --- | --- |
| Current Location | Be able to understand where the rocket is in terms of position, on the pad, in flight, or has landed; using barometric pressure |
| Current Stage | Determine 1st stage burn, 2nd coasting, and final descent|
| Pyro channel continuity | Continuous checks to ensure activity |
| Motor Burn Status | General statistic |
| Parachute Deployment | General statistic but also great for confirming deployment in logs |
| Errors | Anything that goes wrong during flight can be logged to be fixed and improved upon |
| Battery Voltage | Ensuring that data is being read properly and is not due to dying voltage |

**Other Goals**
| Goal | Purpose |
| --- | --- |
| Mach Transition Time | A critical mission for this boosted-dart was achieving Mach 1 capabilities so tests for continuity was critical in development |
| Trigger Messages | When a change occurs in anything we deemed loggable (or Mach 1), record to the outputted txt or cvg file |
| Safety and Abort Systems | With safetey always being at the forefront, if there was something critically wrong the system should abort |
| Online Transferable logging information | With our ESP, we could host a Wi-Fi connection and connect to the local host to download the recorded data after flight|


<img width="468" height="637" alt="image" src="https://github.com/user-attachments/assets/cfbb1845-fe44-4fd5-bb5f-46ae7c1f0b4a" />
Primary Code Development was [Loring Teuteuberg](https://www.linkedin.com/in/loring-teuteberg/),[Rania Maaraba](https://www.linkedin.com/in/rania-maaraba-403987290/), and [Jesse Dominguez](https://www.linkedin.com/in/jdz29/). CAD development was handled by Jonah Ulcer and Jesse Dominguez at Design Fusion, Cincinnati Ohio. Wiring development and non-custom avionics utilising the Blue Raven by Featherweight Altimiters, created by Hayden Mays. Final Code pushed in _'development'_ branch. 

# Initial Wiring
<img width="1370" height="858" alt="hayden_mays_wiring" src="https://github.com/user-attachments/assets/6cc99760-8070-4816-9567-b6b8bf7858ea" />
Courtesy of Hayden Mays, this was the wiring developed for Harty Darty's custom avionics bay. Wiring was later executed by Loring Teuteuberg.

# CAD Development
<img width="282" height="931" alt="jonah_uler_3dmodeling" src="https://github.com/user-attachments/assets/4b1ed595-2bf4-45d7-bfd8-822befa1878d" />
Courtesy of Jonah Ulcer in TinkerCAD as well as Siemanns' NX software. The electronics bay fit like a glove around the desired components and allowed for space for wiring. 

### Packages
Developed in C++ using PlatformIO in VSC with Adafruit LSM6DS, barometer MS5611 SPI on a Seeed ESP32S3.


## Resources
Rob Tilaart's [code for the MS5611 SPI on GitHub](https://github.com/RobTillaart/MS5611_SPI)
Adafruit's Adafruit [LSM6DS code on GitHub](https://github.com/adafruit/Adafruit_LSM6DS) as well as their [online documentation](https://docs.arduino.cc/libraries/ms5611_spi/) on the Arduino website
