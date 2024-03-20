#include <TinyGPsS++.h> //TinyGPSPlus: A library for parsing NMEA data from GPS modules.
#include <SoftwareSerial.h> //SoftwareSerial.h: For creating software serial ports on Arduino boards.

#include <TimeLib.h> //TimeLib.h: For time-related functions and operations.

#include <Wire.h> //#include <Wire.h>  you can use the Wire library functions to communicate with I2C devices. This includes functions like Wire.beginTransmission(), Wire.write(), Wire.endTransmission(), Wire.requestFrom(), etc.

#include <Adafruit_Sensor.h> //Adafruit_Sensor.h and Adafruit_ADXL345_U.h: These are likely used for interfacing with the Adafruit ADXL345 accelerometer sensor.

#include <Adafruit_ADXL345_U.h> //Adafruit_ADXL345_U.h library is used for interfacing with the Adafruit ADXL345 accelerometer sensor.



Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(); 
#define time_offset 19800 // Local time is UTC+3, so 10800 is 3hrs in sec, change based on your location 
Const int buz = 12; 
SoftwareSerial SIM900(6, 7); 
Char incoming_char = 0; 
String gMap = http://www.google.com/maps/place/; 
Double Latitude, Longitude, Altitude, speed; 
Boolean GPSstatus = false; 
Static const uint32_t GPSBaud = 9600; 
TinyGPSPlus gps; // The TinyGPS++ object 
 
Char Time[]  = “00:00:00”; 
Char Date[]  = “00-00-2000”; 
String nameofday; 
Int Year; 
Byte last_second , Month, Day, Hour, Minute, Second; 
String DateString = “”; 
Int buttonState = 0; 
Int button = 5; 
 
Void setup() 
    { 
        pinMode(buz, OUTPUT); pinMode(button, INPUT); 
        pinMode(9, OUTPUT); 
        digitalWrite(9, LOW); 
        SIM900.begin(9600); // for GSM shield 
        Serial.begin(9600); // For serial monitor<br>  delay(20000);  // Give time to log on to network.<br> 
        SIM900.print(“AT+CLIP=1\r”); // turn on caller ID notification 
        If(!accel.begin()) 
        { 
            Serial.println(“No valid sensor found”); 
            While(1); 
        } 
        Delay(100); 
    } 
Void loop() 
{ 
    Sensors_event_t event; 
    Accel.getEvent(&event); 
    While (Serial.available() > 0) 
    If (gps.encode(Serial.read())) getGPSdata(); 
 
    // check if the pushbutton is pressed. 
    buttonState = digitalRead(button); 
    if(event.acceleration.y > 5 || event.acceleration.y < -5 || event.acceleration.x > 5 || event.acceleration.x < -5 || buttonState == 1) 
    //If it is, the buttonState is HIGH: and if anny change in the sensor or accident occured then buzzer will sounds for next 10 sec
    { 
        digitalWrite(buz, HIGH); 
        delay(1000); 
        
        digitalWrite(buz, LOW); 
        delay(1000); 
        
        digitalWrite(buz, HIGH); 
        delay(1000); 
        
        digitalWrite(buz, LOW); 
        delay(1000); 
        
        digitalWrite(buz, HIGH); 
        delay(1000); 
        
        digitalWrite(buz, LOW); 
        delay(1000); 
        
        digitalWrite(buz, HIGH); 
        delay(1000); 
        
        digitalWrite(buz, LOW); 
        delay(1000); 
        
        digitalWrite(buz, HIGH); 
        delay(1000); 
        
        digitalWrite(buz, LOW);
         delay(1000); 
        
        Valid_GPS_Message(); 
    } 
Else 
{ 
} 
} 
 
Void Valid_GPS_Message() 
//if any changes in the system then system send  emargency msg to given mobile no
{ 
        dateString(); //include time stamp 
    SIM900.println(“AT+CMGF=1”);   //Sets the GSM Module in Text Mode 
    Delay(1000); 
    SIM900.println(“AT+CMGS=\”+919591071490\”\r”); 	// 	Replace 	x 	with 	 	mobile number+Country Code 
    Delay(1000); 
    SIM900.println(dateString()); 
    Delay(100); 
    SIM900.print(gMap); 
    Delay(100); 
    SIM900.print(Latitude, 6);// 6 digit has to be defined, otherwaise only 2 digit is displayed 
    Delay(100); 
    SIM900.print(“,”); 
    Delay(100); 
    SIM900.println(Longitude, 6);// 6 digit has to be defined, otherwaise only 2 digit is displayed Delay(100); 
    SIM900.print(“ACCIDENT DETECTED!!”); 
    SIM900.println((char)26);// ASCII code of CTRL+Z digitalWrite(buz, HIGH); delay(1000); digitalWrite(buz, LOW); delay(1000); 
} 
 //sending sms to another no
Void No_GPS_Message() 
{ 
        SIM900.println(“AT+CMGF=1”);    //Sets the GSM Module in Text Mode 
        Delay(1000); 
        SIM900.println(“AT+CMGS=\”+919591071490\”\r”); // Replace x with mobile number 
        Delay(1000); 
        SIM900.println(“GPS DATA NOT AVAILABLE”); 
        Delay(100); 
        SIM900.println(“PLEASE TRY LATER”); 
        Delay(100); 
        SIM900.println((char)26);// ASCII code of CTRL+Z 
        Delay(1000); 
} 
Void getGPSdata() 
{ 
    If (gps.location.isValid()) 
    { 
        Latitude = gps.location.lat(), 6; // Latitude in degrees (double) 
        Longitude = gps.location.lng(), 6; // Longitude in degrees (double) 
        GPSstatus =  true; 
    } 
    Else 
    GPSstatus =  false; 
    If (gps.date.isValid()) 
    { 
        Year = gps.date.year(); // Year (2000+) (u16) 
        Month = gps.date.month(); // Month (1-12) (u8) 
        Day = gps.date.day(); // Day (1-31) (u8) 
    } 
    Else 
    GPSstatus =  false; 
    
    If (gps.time.isValid()) 
    //getthing current time 
    { 
        Hour = gps.time.hour(); // Hour (0-23) (u8) 
        Minute = gps.time.minute(); // Minute (0-59) (u8) 
        Second = gps.time.second(); // Second (0-59) (u8) 
    } 
    Else 
    GPSstatus =  false; 
    //getthing current time 
    If (last_second != gps.time.second()) 
    {

        Last_second = gps.time.second(); // set current UTC time setTime(Hour, Minute, Second, Day, Month, Year); 
        // add the offset to get local time adjustTime(time_offset); // update time array 
        Time[6] = second() / 10 + ‘0’; 
        Time[7] = second() % 10 + ‘0’; 
        Time[3]  = minute() / 10 + ‘0’; 
        Time[4] = minute() % 10 + ‘0’; 
        Time[0]  = hour()   / 10 + ‘0’; 
        Time[1]  = hour()   % 10 + ‘0’; 
        // update date array 
        Date[8] = (year()  / 10) % 10 + ‘0’; 
        Date[9] =  year()  % 10 + ‘0’; 
        Date[3]  =  month() / 10 + ‘0’; 
        Date[4] =  month() % 10 + ‘0’; 
        Date[0]  =  day()   / 10 + ‘0’; 
        Date[1]  =  day()   % 10 + ‘0’; 
    } 
    //getting current day of week
    Switch (weekday())  
    { 
        Case 1:  nameofday = “SUNDAY, “;   break; 
        Case 2:  nameofday = “MONDAY, “;   break; 
        Case 3:  nameofday = “TUESDAY, “;   break; 
        
        Case 4:  nameofday = “WEDNESDAY, “;   break; 
        Case 5:  nameofday = “THURSDAY, “;   break; 
        Case 6:  nameofday = “FRIDAY, “;   break; 
        Default: nameofday = “SATURDAY, “; 
    } 
    If (gps.speed.isValid()) 
    { 
        Speed = gps.speed.mps(); // Speed in meters per second (double) 
    } 
    Else 
    GPSstatus =  false; 
    
    If (gps.altitude.isValid()) 
    { 
         Altitude = gps.altitude.meters(); // Altitude in meters (double) 
    } 
    Else 
    GPSstatus =  false; 
    
    Delay(1000); 
} 
 
String dateString() 
{ 
    DateString = “”; 
    DateString += nameofday; 
    DateString += “ “; 
    
    DateString += Date; 
    DateString += “ “; 
    DateString += Time; 
    Return DateString; 
} 