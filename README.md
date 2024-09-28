## WREN (Recliner Linear Motor Controller) SDK

DitroniX WREN, is an IoT Linear Motor Controller SDK.  It was initially designed to easily, and safely, retrofit (plug and play), to industry standard Electric Reclining Chairs, with continued use of existing push buttons (or remote hand controller), through isolated input and outputs, whilst also allowing simultaneous electrical control from Smart Home Automation.

The WREN 12S SDK V2  example code maybe used for Board Bring Up and Test Input / Output Ports, or used as a baseline for your custom code.

This Arduino IDE example (Settings. Board: ESP8266 12S Dev Module), basically exercises the RGB LED, Temperature Sensor, Voltage Sensor, Safety Circuit, Inputs and DC Motor Outputs.

You can also refer to Wiki for ESP-12S GPIO and I2C GPIO Expander (MCP23017). for control and monitor of the board functionality.   WREN 12S SDK Connections

![Display-Type-B](https://github.com/DitroniX/WREN-Recliner-Linear-Motor-Controller/blob/main/Datasheets%20and%20Information/WREN%2012S%20SDK%20V2%20Component%20Overview.jpg?raw=true)

Example UART Output
~~~c++

DitroniX WREN 12S SDK 2.0 Initialized

Scanning I2C	Found Devices:  (0x18)	 (0x20)	Found 2 Device(s).
Connected to Digital Temperature Sensor MCP9808	Test PCB Temp: 28.3750°C
Test RGB LEDs White Blue Green Red
Motor 1 Cycle Check, Motor 2 Cycle Check
Waiting for Controller Input

WREN Motor Driver 1A High
PCB Temp: 28.3750°C	ADC Raw Value: 535	 DC Input Voltage: 24.28 V

WREN Motor Driver 1B High
PCB Temp: 28.6250°C	ADC Raw Value: 536	 DC Input Voltage: 24.37 V

WREN Motor Driver 2A High
PCB Temp: 28.6875°C	ADC Raw Value: 535	 DC Input Voltage: 24.28 V

WREN Motor Driver 2B High
PCB Temp: 28.7500°C	ADC Raw Value: 536	 DC Input Voltage: 24.37 V


DitroniX WREN 12S SDK 2.0 Initialized

Scanning I2C	Found Devices:  (0x18)	 (0x20)	Found 2 Device(s).
Connected to Digital Temperature Sensor MCP9808	Test PCB Temp: 30.7500°C
Test RGB LEDs White Blue Green Red
Motor 1 Cycle Check, Motor 2 Cycle Check
Waiting for Controller Input

WREN Motor Driver Safety Fault Condition - DC Input (Low or High) or Back EMF Surge Voltage: 7.14 V
> RESTARTING ESP


DitroniX WREN 12S SDK 2.0 Initialized

Scanning I2C	Found Devices:  (0x18)	 (0x20)	Found 2 Device(s).
Connected to Digital Temperature Sensor MCP9808	Test PCB Temp: 30.8125°C
Test RGB LEDs White Blue Green Red
Motor 1 Cycle Check, Motor 2 Cycle Check
Waiting for Controller Input

Main DC Power Fail or Safety Fuse Blown.  Shutting Down ESP. Reset device to Wake Up
~~~

Tags: Accessibility, Amazon Alexa, DC Motor Controller, Designed and Made in Great Britain, Developer Community, DitroniX, Electric Recliner, Epressif, ESP-12S, ESP8266, Google Home, Hacker Projects, Home Automation, Home Cinema, IoT, Linear DC Motor Controller, Mobility, Mobility Chair Controller, MQTT, Open Source, Riser Recliner, SDK, SDK Developer Community, Smart Device, Smart Recliner, STEM, Voice Commands, Voice Controlled Switch, Wifi Remote Control, Wireless Remote Control, WREN


## **Further Information**

Additional information, and other technical details on this project, maybe found in the related repository pages.

**Repository Folders**

 - **Code** *(Code examples for Arduino  IDE and PlatformIO)*
 -  **Datasheets and Information** *(Component Datasheets, Schematics, Board Layouts, Photos, Technical Documentation)*
 - **Certification** *(Related Repository Project or Part, Certification Information)*

**Repository Tabs**

 - **Wiki** *(Related Repository Wiki pages and Technical User Information)*
 - **Discussions** *(Related Repository User Discussion Forum)*
 - **Issues** *(Related Repository Technical Issues and Fixes)*

***

We value our Customers, Users of our designs and STEM Communities, all over the World . Should you have any other questions, or feedback to share to others, please feel free to:

* Visit the related [Project](https://github.com/DitroniX?tab=repositories) *plus the related* **Discussions** and **Wiki** Pages.  See tab in each separate repository.
* **Project Community Information** can be found at https://www.hackster.io/DitroniX
* [DitroniX.net Website - Contact Us](https://ditronix.net/contact/)
* **Twitter**: [https://twitter.com/DitroniX](https://twitter.com/DitroniX)
* [Supporting the STEM Projects - BuyMeACoffee](https://www.buymeacoffee.com/DitroniX)
*  **LinkedIN**: [https://www.linkedin.com/in/g8puo/](https://www.linkedin.com/in/g8puo/)

***Dave Williams, Maidstone, UK.***

Electronics Engineer | Software Developer | R&D Support | RF Engineering | Product Certification and Testing | STEM Ambassador

## STEM

**Supporting [STEM Learning](https://www.stem.org.uk/)**

Life is one long exciting learning curve, help others by setting the seed to knowledge.

![DitroniX Supporting STEM](https://hackster.imgix.net/uploads/attachments/1606838/stem_ambassador_-_100_volunteer_badge_edxfxlrfbc1_bjdqharfoe1_xbqi2KUcri.png?auto=compress%2Cformat&w=540&fit=max)
Dave Williams | G8PUO | DitroniX 1981-2024 | ditronix.net
