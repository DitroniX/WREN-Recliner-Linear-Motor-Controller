/*
  Dave Williams, DitroniX 2019-2021 (ditronix.net)
  WREN 12S DC Linear Motor Controller ESP12S SDK 2.0
  PCA v2.03 - Test Code Firmware v2.8 - 4th March 2022
  
  Simplified Board Bring Up and Test Input/Output Ports (The WREN 'Hello World')
  
  The purpose of this test code is to cycle through the various main functions of the board as part of bring up testing.
 
  This test code is OPEN SOURCE and although is is not intended for real work use, it may be freely used, or modified as needed.

  The code assumes standard single, or dual, motor configuration.  No Wifi Enabled as this is not required for initial bring up.
  
  SFLAG is a hardware logic controlled Alert Flag, normally Low, unless of a (non-hardware latched), Safety Alert. This is monitoring both Driver
  inputs and sensing the ENx internal collectors of the Overcurrent and Thermal Protection transistors, to implement over current protection.  

  SFLAG would benefit on an interrupt, in a working environment.  Simply check for a high pulse to trigger an alert event.

  V2.5 Updated SFLAG monitoring. Commented out motor test on start up as part of safety.
  V2.6 Updated to flash RED LED upon low voltage or no main DC supply.  Assume powered via UART.
  V2.7 Updated info relating to Library for MCP23017.  Use Adafruit version 1.3.0 and NOT the later library version 2.02 (as this seems to have a problem)
  V2.7 Updated info relating to Library for MCP23017.
 */

 /*
  * References to Supporting Libraries (Add to Managed Libraries)
  * MCP9808 Precision I2C Temperature Sensor - Adafruit_MCP9808
  * https://learn.adafruit.com/adafruit-mcp9808-precision-i2c-temperature-sensor-guide
  * MCP23017 I2c Port Expander - Adafruit_MCP23017 (USE Version 1.3.0)
  * https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
  * GPIO 0 to 15 for the GPIOA0..GPIOA7, GPIOB0..GPIOB7 (i.e. pin 12 is GPIOB4).
  */

/*
 Test LED Sequence
 * White - Check Temp Sensor or I2C
 * RGB Blue, Green and Red Self Test
 * Blue Long - Motor 1 Cycle Test
 * Green Long - Motor 2 Cycle Test
 * Green Heart Beat - Checking for Controller Input
 * Blue - Input Detected and Motor On
 * Flashing Red for Bounds Alert and Safety
 */

#include "Arduino.h"
#include "time.h"
#include "Adafruit_MCP9808.h"
#include <Adafruit_MCP23X17.h>  //GPIOs 16
#include "Wire.h" 

// Create Adafruit objects
Adafruit_MCP9808 TempSensor = Adafruit_MCP9808();
Adafruit_MCP23X08 I2C_IO;

// Configure WREN GPIO

// **************** INPUTS ****************

// Define I2C_IO Input Ports Driver 1 (from External 24V Inputs)
#define IN_1A 8  //(Button 1 - DIN Pin 4 - Black)
#define IN_1B 9  //(Button 2 - DIN Pin 2 - Yellow)

// Define I2C_IO Input Ports Driver 2 (from External 24V Inputs)
#define IN_2A 10 //(Button 3 - DIN Pin 5 - Brown)
#define IN_2B 11 //(Button 4 - DIN Pin 3 - Red)

// Define I2C_IO Input Port Temperature Alarm
#define TALM 12

// Define I2C_IO USER Input Port (Solder Link)
#define GPB5 13 //User Configuration (Use Internal PU)

// Define I2C_IO ENABLE Input Port Driver 1
#define EN1 14

// Define I2C_IO ENABLE Input Port Driver 2
#define EN2 15

// Define SFLAG (Safety Flag) Input Port
#define SFLAG 16

// Define ADC (12 DC - 30V DC)
#define ADC A0 

// **************** OUTPUTS **************** 

// Define I2C_IO Driver 1 Output Ports (L6205 Inputs)
#define EN_1 0 // Driver 1
#define OUT_1A 1 //(DIN Pin 4 - Black)
#define OUT_1B 2 //(DIN Pin 2 - Yellow)

// Define I2C_IO Driver 2 Output Ports (L6205 Inputs)
#define EN_2 3 // Driver 2
#define OUT_2A 4 //(DIN Pin 5 - Brown)
#define OUT_2B 5 //(DIN Pin 3 - Red)

// Define I2C_IO Output Port LED (RGB LED)
#define LED_Green 6
#define LED_Red 7

// Define ESP Output Port LED (RGB LED)
#define LED_Blue 2

// **************** IO ****************

// Define I2C (Expansion Port)
#define I2C_SDA 4
#define I2C_SCL 5 

// Define SPI (Expansion Port)
#define SPI_MISO 12
#define SPI_MOSI 13
#define SPI_SCK 14
#define SPI_SS 15

// **************** VARIABLES ****************

// Define DIN Input Ports (from External 24V/29V Inputs)
  int IN_1A_State = 0; // DIN 4
  int IN_1B_State = 0; // DIN 3
  int IN_2A_State = 0; // DIN 5
  int IN_2B_State = 0; // DIN 3
  
  int ButtonVal_1A = 0; // 1
  int ButtonVal_1B = 0; // 3
  int ButtonVal_2A = 0; // 2
  int ButtonVal_2B = 0; // 4 
  
  float PCB_Temp;
  float ADC_Voltage;
  int ADC_RAW;
  
// **************** Default Addresses ****************
  int MCP9808 = 0x18;
  int MCP_IO = 0x20;
  
// Setup 
  void setup() {
  
  delay(250); //Settle

// I2C IO Expander
  if (!I2C_IO.begin_I2C()) {
    Serial.println("Error.");
    while (1);
  }
  
  //I2C_IO.begin();
  Serial.println("Connected to I2C IO Expander MCP23017\t");

// **************** INPUTS ****************

// Initialize Input Ports Driver 1 (from External 24V Inputs)
  I2C_IO.pinMode (IN_1A, INPUT); // Driver 1
  I2C_IO.pinMode (IN_1B, INPUT); // Driver 1

// Initialize Input Ports Driver 2 (from External 24V Inputs)  
  I2C_IO.pinMode (IN_2A, INPUT); // Driver 2
  I2C_IO.pinMode (IN_2B, INPUT); // Driver 2

// Initialize Temp Alarm (unused in this test code)
  I2C_IO.pinMode (TALM, INPUT);

// Initialize Safety Flag
  pinMode (SFLAG, INPUT);  

// **************** OUTPUTS ****************

// Initialize Output Ports Driver 1 (L6205 Inputs)
  I2C_IO.pinMode (EN_1, OUTPUT);
  I2C_IO.pinMode (OUT_1A, OUTPUT);
  I2C_IO.pinMode (OUT_1B, OUTPUT);

// Initialize Output Ports Driver 2 (L6205 Inputs)
  I2C_IO.pinMode (EN_2, OUTPUT);
  I2C_IO.pinMode (OUT_2A, OUTPUT);
  I2C_IO.pinMode (OUT_2B, OUTPUT);

// Initialize LED (RGB LED)
  I2C_IO.pinMode (LED_Red, OUTPUT);
  I2C_IO.pinMode (LED_Green, OUTPUT);
  pinMode (LED_Blue, OUTPUT);

// **************** COMMS ****************

// Initialize UART:
  Serial.begin(115200, SERIAL_8N1);  //115200  
  while (!Serial);
  Serial.println(""); 
  Serial.println("DitroniX WREN 12S SDK 2.0 Initialized");
  Serial.println(""); 

// Initialize I2C 
  Wire.begin(I2C_SDA, I2C_SCL);

// Scan I2C Devices
  Serial.print("Scanning I2C\t");
  byte count = 0;
  Serial.print("Found Devices: ");
  for (byte i = 8; i < 120; i++)
  {
    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0)
      {
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.print(")\t");
      count++;
      delay(1);
      }
  }      
  Serial.print("Found ");
  Serial.print(count, HEX);
  Serial.println(" Device(s).");  

// PCB Temperature Sensor
  if (!TempSensor.begin()) 
  {
    // Sensor or I2C Issue
    Serial.println("Couldn't find MCP9808! Sensor or I2C Issue. Check the PCB !!");
    digitalWrite(LED_Blue, LOW);
    I2C_IO.digitalWrite(LED_Green, LOW);
    I2C_IO.digitalWrite(LED_Red, LOW);    
    delay(2000);
  }
  else
  {
    Serial.print("Connected to Digital Temperature Sensor MCP9808\t");

    // Set Resolution 0.0625Â°C 250 ms and Wake Up
    TempSensor.setResolution(3);
    
     // Read PCB_Temp Sensor and Output
    TempSensor.wake(); // Wake Up
    PCB_Temp = TempSensor.readTempC();
    Serial.print("Test PCB Temp: ");
    Serial.print(PCB_Temp, 4); Serial.println("Â°C"); 
    TempSensor.shutdown_wake(1); // Sleep
   }

// **************** I/O Check ****************

  // Test Cycle RGB LED
  Serial.println("Test RGB LEDs White Blue Green Red");   
  digitalWrite(LED_Blue, LOW);
  I2C_IO.digitalWrite(LED_Green, LOW);
  I2C_IO.digitalWrite(LED_Red, LOW);
  delay(1000);     
  digitalWrite(LED_Blue, HIGH);
  I2C_IO.digitalWrite(LED_Green, HIGH);
  I2C_IO.digitalWrite(LED_Red, HIGH);
  delay(250);   
  
  // Test Cycle Blue LED
  digitalWrite(LED_Blue, LOW);   
  delay(1000);              
  digitalWrite(LED_Blue, HIGH);
  delay(250); 
  
  // Test Cycle Green LED
  I2C_IO.digitalWrite(LED_Green, LOW);   
  delay(1000);              
  I2C_IO.digitalWrite(LED_Green, HIGH);
  delay(250); 

  // Test Cycle Red LED
  I2C_IO.digitalWrite(LED_Red, LOW);   
  delay(1000);              
  I2C_IO.digitalWrite(LED_Red, HIGH);
  delay(250); 

  // Enable both Motor Driver 1 Ports
  I2C_IO.digitalWrite(EN_1, HIGH);      
  
  // Enable both Motor Driver 2 Ports
  I2C_IO.digitalWrite(EN_2, HIGH); 

//   // Test Cycle Motor 1
//  Serial.print("Motor 1 Cycle Check");   
//  digitalWrite(LED_Blue, LOW);   
//  I2C_IO.digitalWrite(OUT_1A, HIGH);
//  I2C_IO.digitalWrite(OUT_1B, LOW);
//  delay(1000);
//  I2C_IO.digitalWrite(OUT_1B, HIGH); 
//  I2C_IO.digitalWrite(OUT_1A, LOW);  
//  delay(1000);
//  I2C_IO.digitalWrite(OUT_1A, LOW);  
//  I2C_IO.digitalWrite(OUT_1B, LOW);  
//  digitalWrite(LED_Blue, HIGH);    
//
//  // Test Cycle Motor 2
//  Serial.println(", Motor 2 Cycle Check");     
//  I2C_IO.digitalWrite(LED_Green, LOW);     
//  I2C_IO.digitalWrite(OUT_2A, HIGH);  
//  I2C_IO.digitalWrite(OUT_2B, LOW);  
//  delay(1000);
//  I2C_IO.digitalWrite(OUT_2B, HIGH); 
//  I2C_IO.digitalWrite(OUT_2A, LOW);  
//  delay(1000);
//  I2C_IO.digitalWrite(OUT_2A, LOW); 
//  I2C_IO.digitalWrite(OUT_2B, LOW);  
//  I2C_IO.digitalWrite(LED_Green, HIGH);  

  // Sets Motor Driver 1 Outputs Low and Disable
  I2C_IO.digitalWrite(EN_1, LOW);  
  I2C_IO.digitalWrite(OUT_1A, LOW);    
  I2C_IO.digitalWrite(OUT_1B, LOW); 
          
  // Sets Motor Driver 2 Outputs Low and Disable
  I2C_IO.digitalWrite(EN_2, LOW); 
  I2C_IO.digitalWrite(OUT_2A, LOW); 
  I2C_IO.digitalWrite(OUT_2B, LOW); 

  Serial.println("Waiting for Controller Input");  
  Serial.println("");  
}

// ######### FUNCTIONS #########

// Read Sensors
  void ReadSensors ()
  {
    // Read ADC DC Voltage Sensor and Output (0-1V 0-1023)
    ADC_RAW = analogRead(ADC);
    ADC_Voltage = (analogRead(ADC) * 46.55 ) / 1024; // Divider value may need tweaking
  
    // Read PCB_Temp Sensor
    PCB_Temp = TempSensor.readTempC();  
  }

// Display Sensors
  void DisplaySensors()
  {
    ReadSensors();
    
    // PCB_Temp Sensor
    TempSensor.wake(); // Wake Up    
    Serial.print("PCB Temp: ");
    Serial.print(PCB_Temp, 4); Serial.print("Â°C\t"); 
    TempSensor.shutdown_wake(1); // Sleep
  
    // ADC Sensor
    Serial.print("ADC Raw Value: ");
    Serial.print(ADC_RAW);
    Serial.print("\t DC Input Voltage: ");
    Serial.print(ADC_Voltage);
    Serial.println(" V");  
    Serial.println("");       
  
    // Heat Beat Blue LED
    digitalWrite(LED_Blue, LOW);    
    delay(10);    
    digitalWrite(LED_Blue, HIGH);    
  }

// Check for Fault Condition
  void FaultConditionCheck()
  {  
    ReadSensors();
    
    // Detection of Main DC Power Fail or Safety Fuse Blown.  Shutdown ESP.
    if (ADC_Voltage < 5) {  
      Serial.println("Main DC Power Fail or Safety Fuse Blown.  Shutting Down ESP. Reset device to Wake Up");  

      // Red LED Flash Alert
      for (int x = 0; x <= 10; x++) {
        delay(250);   
        I2C_IO.digitalWrite(LED_Red, LOW);    
        delay(250);    
        I2C_IO.digitalWrite(LED_Red, HIGH); 
      }
            
      ESP.deepSleep(0);     
    }
    
    // Simple SFLAG alert and Voltage range check
    if (digitalRead(SFLAG) == HIGH || ADC_Voltage < 12  || ADC_Voltage > 39)  { 
    
        Serial.print("WREN Motor Driver Safety Fault Condition");
        Serial.print(" - DC Input (Low or High) or Back EMF Surge Voltage: ");
        Serial.print(ADC_Voltage);
        Serial.println(" V");          
    
        // Disable both Motor Driver Ports
        I2C_IO.digitalWrite(EN_1, LOW);      
        I2C_IO.digitalWrite(EN_2, LOW); 
    
        // Sets All Motor Driver Outputs Low
        I2C_IO.digitalWrite(OUT_1A, LOW);    
        I2C_IO.digitalWrite(OUT_2A, LOW); 
        I2C_IO.digitalWrite(OUT_1B, LOW); 
        I2C_IO.digitalWrite(OUT_2B, LOW); 
        
        // Red LED Flash Alert
        for (int x = 0; x <= 10; x++) {
          delay(250);   
          I2C_IO.digitalWrite(LED_Red, LOW);    
          delay(250);    
          I2C_IO.digitalWrite(LED_Red, HIGH); 
        }
    
    // Reset ESP
    Serial.println("> RESTARTING ESP");   
    void(* resetFunc) (void) = 0;//declare reset function at address 0
    resetFunc(); //call reset 
  }}

// ######### LOOP #########
  void loop() {
  
    //Read Input / Buttons
    ButtonVal_1A = I2C_IO.digitalRead(IN_1A);   // Read the input pin 1A
    ButtonVal_1B = I2C_IO.digitalRead(IN_1B);   // Read the input pin 1B    
    ButtonVal_2A = I2C_IO.digitalRead(IN_2A);   // Read the input pin 2A
    ButtonVal_2B = I2C_IO.digitalRead(IN_2B);   // Read the input pin 2B
    
    // Check for Fault Condition
    FaultConditionCheck();     
  
      // Basic Detection.  If any Input High then set appropriate Motor Output High
      if (ButtonVal_1A == HIGH || ButtonVal_1B == HIGH || ButtonVal_2A == HIGH || ButtonVal_2B == HIGH)  {  
      
          // Enable both Motor Driver Ports
          I2C_IO.digitalWrite(EN_1, HIGH);      
          I2C_IO.digitalWrite(EN_2, HIGH);     
      
          if (ButtonVal_1A == HIGH) {
           I2C_IO.digitalWrite(OUT_1A, HIGH);  // Sets Motor Driver Output High if Input High
            Serial.println("WREN Motor Driver 1A High");
          }
          
          if (ButtonVal_1B == HIGH) {
            I2C_IO.digitalWrite(OUT_1B, HIGH);  // Sets Motor Driver Output High if Input High
            Serial.println("WREN Motor Driver 1B High");
          }
          
          if (ButtonVal_2A == HIGH) {
           I2C_IO.digitalWrite(OUT_2A, HIGH);  // Sets Motor Driver Output High if Input High
            Serial.println("WREN Motor Driver 2A High");
          }
                        
          if (ButtonVal_2B == HIGH) {
            I2C_IO.digitalWrite(OUT_2B, HIGH);  // Sets Motor Driver Output High if Input High
            Serial.println("WREN Motor Driver 2B High");
          }   
  
          DisplaySensors();
        
      } else {
          
          // Sets Motor Driver 1 Outputs Low and Disable
          if (I2C_IO.digitalRead(EN_1) == HIGH)  { 
              I2C_IO.digitalWrite(EN_1, LOW);  
              I2C_IO.digitalWrite(OUT_1A, LOW);    
              I2C_IO.digitalWrite(OUT_1B, LOW); 
          }
                  
          // Sets Motor Driver 2 Outputs Low and Disable
          if (I2C_IO.digitalRead(EN_2) == HIGH)  {
              I2C_IO.digitalWrite(EN_2, LOW);          
              I2C_IO.digitalWrite(OUT_2A, LOW); 
              I2C_IO.digitalWrite(OUT_2B, LOW);        
          }

          // Heat Beat Green LED
          delay(250);   
          I2C_IO.digitalWrite(LED_Green, LOW);    
          delay(10);    
          I2C_IO.digitalWrite(LED_Green, HIGH);             
      }
  }
