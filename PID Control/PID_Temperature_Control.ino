/* 
PID Regulate the temperature according to the setpoint, the desired value => Setpoint. 
Set the desired temperature value with the trimmer potentiometer. 
Author: Milan Ivič 
Date: March 2016 
*/
 
#include <PID_v1.h> 

//Declarations of variables and settings of PID parameters:
double Kp=1, Ki=51, Kd=0;
double Setpoint, Input, Output;
double bitIn, vIn, tempIn;
double bitSetpoint, vSetpoint, tempSetpoint;

//Start PID:
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

int Aktuator = 3;                                       //Connect the bulb as a heater over the transistor BC337 to pin 3 (PWM output Timer2)
void setup()
{
 Serial.begin(9600);
 /*
 bitIn = analogRead(A0);
 vIn = (bitIn*5.0)/1023;
 tempIn = vIn * 100;
 Input = map(tempIn, 0, 1023, 255, 0);          // Set the reading area to A0 (temp sensor LM35).
 bitSetpoint = analogRead(A1);
 vSetpoint = (bitIn*5.0)/1023;
 tempSetpoint = vIn * 100;
 Setpoint = map(tempSetpoint, 0, 1023, 255, 0);       // Setpoint is set with the potentiometer on the A2 pin.
 */
 myPID.SetOutputLimits(0, 255);                         // Set the PWM signal area at the output (pin 3).

 myPID.SetMode(AUTOMATIC);                              // Turn on the PID control.
 myPID.SetTunings(Kp, Ki, Kd);                          // PID settings with our selected parameters. 
}

void loop()
{ 
 //Setpoint with the potentiometer. The input A2 contains a 10 bit AD converter (0-1023)
 bitSetpoint = analogRead(A1);
 vSetpoint = (bitSetpoint*5.0)/1023;
 tempSetpoint = vSetpoint * 100;
 Setpoint = tempSetpoint;                               // Move the 0-1023 range to the range 255-0.  
 bitIn = analogRead(A0);
 vIn = (bitIn*5.0)/1023;
 tempIn = vIn * 100;
 Input = tempIn;                                        // Read values ​​at A0 (temperature).  
 myPID.SetTunings(Kp, Ki, Kd);                          // PID settings with our selected parameters. 
 myPID.Compute();                                       // PID calculation:
 analogWrite(3,Output);                                 // Calculated value of PID function is written to pin 3
 Serial.print(Setpoint);                                // Print on the serial monitor or plot in the graph.
 Serial.print(" ");
 Serial.println(Input);   
}
