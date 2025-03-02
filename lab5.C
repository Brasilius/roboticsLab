#include <kipr/wombat.h>
#include <time.h>

int driveRightWheel = 140;
int driveLeftWheel = 140;
int lightSensorThreshold = 2700; // Threshold for line detection
float kP = 1.4;                  // Proportional Gain
float kI = 0.09;                 // Integral Gain
float kD = 0.3;                  // Derivative Gain
int MAX_INTEGRAL = 1000;         // Maximum integral term to prevent windup
int programState = 0;            // Controls primary loop
int maxState = 1;                // Final State of program
int analogFilter = 4800;         // Just an upper limit for sum checks if needed

// These flags let you disable each sensor after it’s triggered
int sensorActive0 = 1;
int sensorActive1 = 1;

// Threshold for bin detection
int binThreshold = 1600;

// Number of times to read and average analog
int samplesCount = 5;

void turn(int control);
int readFilteredAnalog(int channel);
void armControl(int armLeftorRight);

int main()
{
    enable_servos();
    set_servo_position(0, 0);
    set_servo_position(1, 1100);
    
    create_connect();
    int starttime = seconds();
    
    int previousError = 0;
    int integral = 0;

    while(programState < maxState)
    {
        int time = (seconds() - starttime);
        int lfCliff = get_create_lfcliff_amt();
        int rfCliff = get_create_rfcliff_amt();
        int lCliff = get_create_lcliff_amt();
        int rCliff = get_create_rcliff_amt();
        
        int error = lfCliff - rfCliff;
        integral += error;
        if (integral > MAX_INTEGRAL) { integral = MAX_INTEGRAL; }
        if (integral < -MAX_INTEGRAL){ integral = -MAX_INTEGRAL; }
        
        int derivative = error - previousError;
        previousError = error;
        
        int control = (int)(kP * error + kI * integral + kD * derivative);

        if (lfCliff > lightSensorThreshold && rfCliff > lightSensorThreshold 
            && (lCliff > lightSensorThreshold || rCliff > lightSensorThreshold) 
            && time > 55)
        {
            printf("End Found\n");
            printf("time: %d\n", time);
            create_stop();
            programState++;
        }
        else if (control > 20 || control < -20)
        {
            turn(control);
        }
        else
        {
            create_drive_direct(driveLeftWheel, driveRightWheel);
        }
        
        // Check if sensor 0 is active and triggered
        if(sensorActive0)
        {
            int sensorVal0 = readFilteredAnalog(0);
            if(sensorVal0 >= binThreshold)
            {
                create_stop();
                armControl(1);  // left arm
                printf("Left bin found, disabling sensor 0.\n");
                sensorActive0 = 0;
            }
        }
        
        // Check if sensor 1 is active and triggered
        if(sensorActive1)
        {
            int sensorVal1 = readFilteredAnalog(1);
            if(sensorVal1 >= binThreshold)
            {
                create_stop();
                armControl(0);  // right arm
                printf("Right bin found, disabling sensor 1.\n");
                sensorActive1 = 0;
            }
        }
    }
    
    disable_servos();
    create_disconnect();
    return 0;
}

// Simple function to read a channel multiple times and return the average
int readFilteredAnalog(int channel)
{
    long sum = 0;
    for(int i = 0; i < samplesCount; i++)
    {
        sum += analog(channel);
        msleep(10); // short delay to space out readings
    }
    return (int)(sum / samplesCount);
}

void turn(int control)
{
    create_drive_direct(driveLeftWheel - control, driveRightWheel + control);
}

// Same servo moves you had before, just uncommented for full usage
void armControl(int armLeftorRight)
{
    if (armLeftorRight == 0) // Right
    {
        set_servo_position(0, 300); 
        msleep(1000);
        set_servo_position(0, 0);
        create_drive_direct(140, 100);
        msleep(500);
        create_stop();
    }
    else if (armLeftorRight == 1) // Left
    {
        set_servo_position(1, 845);
        msleep(1000);
        set_servo_position(1, 1100);
        create_drive_direct(100, 140);
        msleep(500);
        create_stop();
    }
    else
    {
        printf("Invalid arm selection.\n");
    }
}
