#include <kipr/wombat.h>
#include <time.h>

int driveRightWheel = 120;
int driveLeftWheel = 120;
int lightSensorThreshold = 2700; // Threshold for when a sensor is directly over line to detect end
float kP = 0.5;                  // Proportional Gain
float kI = 0.13;                 // Integral Gain
float kD = 0.3;                  // Derivative Gain
int MAX_INTEGRAL = 1000;         // Maximum integral term to prevent windup
int programState = 0;            // controls primary loop
int maxState = 1;                // Final State of program
int rIR_SensorThreshold = 1900;  // Sensor threshold for right side IR sensor
int lIR_SensorThreshold = 1900;  // Sensor threshold for left side IR sensor

void turn(int control);
void armControl(int armLeftorRight);
int main()
{
    enable_servos();
    set_servo_position(0, 0);
    set_servo_position(1, 1100);
    int previousError = 0;
    int integral = 0;
    create_connect();
    int starttime = seconds();
    while (programState < maxState)
    {
        int time = (seconds() - starttime);
        int lfCliff = get_create_lfcliff_amt();
        int rfCliff = get_create_rfcliff_amt();
        int lCliff = get_create_lcliff_amt();
        int rCliff = get_create_rcliff_amt();
        int error = lfCliff - rfCliff;
        integral += error;
        if (integral > MAX_INTEGRAL)
            integral = MAX_INTEGRAL;
        if (integral < -MAX_INTEGRAL)
            integral = -MAX_INTEGRAL;
        int derivative = error - previousError;
        previousError = error;
        int control = (int)(kP * error + kI * integral + kD * derivative);

        if (lfCliff > lightSensorThreshold && rfCliff > lightSensorThreshold && (lCliff > lightSensorThreshold || rCliff > lightSensorThreshold) && time > 80)
        {
            printf("End Found\n");
            printf("time: %d\n", time);
            create_stop();
            programState += 1;
        }
        if (control > 120 || control < -120)
        {
            turn(control);
        }
        else
        {
            create_drive_direct(driveLeftWheel, driveRightWheel);
        }

        if (analog(0) >= lIR_SensorThreshold)
        {
            printf("left bin found\n");
            create_stop();
            armControl(1); // armcontrol 1 == left arm
        }
        else if (analog(1) >= rIR_SensorThreshold)
        {
            printf("right bin found\n");
            create_stop();
            armControl(0); // armcontrol 0 == right arm
        }
        else
        {
        }
    }
    disable_servos();
}

void turn(int control)
{
    create_drive_direct(driveLeftWheel - control, driveRightWheel + control);
}

void armControl(int armLeftorRight)
{

    if (armLeftorRight == 0) // right
    {
        rIR_SensorThreshold = 100000000; // set the threshold to an unibtainable number

        set_servo_position(0, 400); // calibrate for lower positon

        msleep(1500);

        set_servo_position(0, 0); // calibrate for upper position
    }

    else if (armLeftorRight == 1) // left
    {
        lIR_SensorThreshold = 100000000; // set the threshold to an unibtainable number

        set_servo_position(1, 745); // calibrate for lower positon

        msleep(1500);

        set_servo_position(1, 1100); // calibrate for upper position
    }

    else
    {

        printf("Invalid arm selection\n");
    }
}
// ethan