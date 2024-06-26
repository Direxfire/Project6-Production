#include<msp430.h>
/*
 *   Control the temperature of a "plant" consisting of a Peltier device and a temperature sensor
 *
 */

#define KP 1.0 // Proportional gain
#define KI 0.05 // Integral gain 0.1
#define KD 0.3 // Derivative gain 0.5
#define DT 0.2 // Sampling time interval in seconds 0.1


// Need some global variables
extern int I2C_Message_Counter;
extern char I2C_Message_Global[32];
extern int Data_Valid;
extern float Derivative;
extern float Integral;
extern float Previous_Error;
extern float Previous_Temperature;
extern float MIN_OUTPUT;


void Setup_PID_Ports(void){

    P5DIR |= BIT2;              // Set led dir out Cool
    P5OUT &= ~BIT2;             // Clear output

    P5DIR |= BIT3;              // Set led dir out Heat
    P5OUT &= ~BIT3;             // Clear output

}
void Peltier_PID(float Current_Temperature, float Target_Temperature)
{

                 int i;
                 // Calculate Error
                 float Error = Target_Temperature - Current_Temperature;
                 // Calculate Integral
                 Integral = Integral + Error * DT;

                 // Calculate Derivative
                 Derivative = (Error - Previous_Error) / DT;

                 // Calculate Output using PID formula
                 float Output = KP * Error + KI * Integral + KD * Derivative;

                 // We are lower than the target
                 if (Output > (2.0*MIN_OUTPUT) ) {
                     P5OUT &= ~BIT2;             // Clear Output
                     for(i = 0; i<1000; i++){}
                     P5OUT |= BIT3;
                 }

                 // We are higher than the target
                 else if (Output < ( 0 - MIN_OUTPUT) ) {
                     P5OUT &= ~BIT3;             // Clear Output
                     for(i = 0; i<1000; i++){}
                     P5OUT |= BIT2;
                 }
                 else{

                     P5OUT &= ~BIT2;
                     P5OUT &= ~BIT3;             // Clear Output
                     for(i = 0; i<1000; i++){}
                 }

                 // Update previous values
                 Previous_Temperature = Current_Temperature;
                 Previous_Error = Error;

}
