/*
Author: Victor M
PURPOSE: Control PWM of NOCTUA NF-A8 PWM fan via user input to achieve low fan speeds

FAN PROPERTIES:
PWM Fan PIN Specifications:
                  PIN ID  PIN Color Pin Purpose
                   1       Black         GND
                   2       Yellow        5V
                   3       Green        Tachometer 
                   4       Blue          PWM
General PWM Fan Specs:
Operating Voltage: 12 V
Current 0.08 A
RPM Range: 2200 RPM (Max)
           1750 RPM (Max when using Low Noise Adpater)
           450 RPM  (Stall Speed) 
PWM Frequency Range: 21-28 kHz
Target PWM Frequency: 25 kHz

Arduino PWM Notes:
Timer output  Arduino output  Chip pin  Pin name  Base Frequency(Hz)  Divisors Available
OC0A               6            12        PD6           62500           1,8,64,256,1024
OC0B               5            11        PD5           62500           1,8,64,256,1024
OC1A               9            15        PB1           31250           1,8,64,256,1024
OC1B              10            16        PB2           31250           1,8,64,256,1024
OC2A              11            17        PB3           31250         1,8,32,64,128,256,1024
OC2B               3             5        PD3           31250         1,8,32,64,128,256,1024
                   
INSTRUCTIONS:    
                 1.Power Arduino via USB connection
                 2.Connect Fan GND to PSU and Arduino GND to PSU(i.e. common ground)
                 3.Connect Fan 5V directly to Arduino 5V 
                 4.Connect Fan TACH directly to Arduino DigitalPin TX->1
                 5.Connect Fan PWM directly to Arduino DigitalPin 3
                 6.Upload code onto Arduino UNO Board
                 7.Open up Serial Monitor
                 8.Type in "PWM" with quotations onto Serial Monitor
                 9.Enter an integer value between 0-255 to regulate the value of the PWM [0 is OFF and 255 is MAX)
                 **Important Note: At low Voltages(V < 1.00V) the fan can actually be turned OFF by entering 0 into the serial monitor**
                 **At Voltages > 1.00 V if 0 is entered into serial monitor, the fan will run at some minimum speed w/o shutting OFF**
                 10. Repeat Step 9 as needed
*/      

// variable declarations
String command;
char werd;
int currentFan = 0;
int PwmPin = 3;
int PwmPinSpeed = 0;

int commandCheck;

void setup() {

  // fans
  pinMode(3, OUTPUT);//PWM pin
  TCCR2A = 0x23;
  TCCR2B = 0x09;  // select clock
  OCR2A = 79;  // aiming for 25kHz
  OCR2B = 62;
  analogWrite(3,0);
  Serial.begin(57600);
  Serial.println();
}

void loop(){

  OCR2B = 62;
  delay(1000);
  OCR2B = 120;
  delay(1000);
  // this is what reads your serial commands
  while ( Serial.available() > 0 ) {
    // small delay because it can only deliver the bytes so quickly
    delay(5);
    // you have to read one byte (one character) at a time
    werd = Serial.read();
    // append the character to "command" string
    command += werd;
  }
  
  // clear serial so it's ready for your next command
  Serial.flush();
  if ( command.length() > 0 ) {
    commandCheck = 0;
    if ( command == "PWM" ) {
        currentFan = 0;
    } 
    else {
        commandCheck = 1;
    }
    if ( commandCheck == 1 ) {
      // the ".toInt()" appended to the command variable converts the "command" string onto an integer used to regulate PWM
      if ( currentFan == 0 ) {
        //front
       analogWrite(PwmPin, command.toInt());
       PwmPinSpeed = command.toInt();
      }
      else {
        //unknown command
      }
    }
    command = "";
    Serial.print(PwmPin);
    Serial.print("Fan Speed: ");
    Serial.println(PwmPinSpeed);
    clear();
  }
}

void clear() {
  Serial.println();
}
