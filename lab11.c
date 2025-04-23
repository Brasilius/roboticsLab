#include <kipr/wombat.h>

// PID Constants (PLACEHOLDER ATM)
float Kp = 0.2;
float Ki = 0.1;
float Kd = 0.05;

//safety variables
float MaxIntegral = 1000;

//camera variables (PLACEHOLDER ATM)
int greenBucket = 0; //channel for green bucket
int redBucket = 0; //channel for red bucket
int blueBucket = 0; //channel for blue bucket
int greenThreshold = 0; //placeholder threshold
int redThreshold = 0; //placeholder threshold
int blueThreshold = 0; //placeholder threshold

//Sensor port  (PLACEHOLDER ATM) MAY NOT BE NEEDED
int sensor_left_port = 0;
int sensor_right_port = 1;
int sensor_center_port = 2;

//motor speed base
int driveLeftWheel = 120;
int driveRightWheel = 120;

//PID Variables
float error, previous_error = 0, integral = 0, derivative;

void turn();
void drive();
void returnToRectangle();
void findBorder();

int main(){
    printf("connecting to create wallaby...\n");
    create_connect();
    printf("connected!");
    printf("connecting camera!");
    camera_open();
    printf("connected!");
    int programState = 0; //used for program progression
    //state 1:: find rectangle border
    while (programState < 1){  
        //find the rectangle border and align 
       findBorder();
    }
    //begin the wallride
    while (programState < 2){

    }
    //find the first box
    while(programState < 3){

    }
    //return to rectangle
    while(programState<4){
        returnToRectangle();
        findBorder();
    }
    //find the second box
    while(programState<5){

    }
    //return to rectangle
    while(programState<6){
        returnToRectangle();
        findBorder();
    }
    //find the third box
    while(programState<7){

    }
    printf("disconnecting...\n");
    create_stop();
    create_disconnect();
    printf("disconnected!");
    return 0;
}

void drive{
    create_drive_direct(driveLeftWheel - control, driveRightWheel - control);
}

void turn{
    create_drive_direct(driveLeftWheel, -driveRightWheel); // Turns right
}

void findBorder{
      //while loop reading when we reach border [mind you example code here]
      while([analogSensor] < THRESHOLD_FOR_WHITETAPE){
        drive();
    }
    //stop when we reach the border
    create_stop();
}

void returnToRectangle{
    create_drive_direct(-driveLeftWheel*2, -driveRightWheel*2); // goes backwards back into circle
    msleep(1000); //sleep with drive command to return to rectangle
    create_drive_direct(0, 0); //turn off motors post return
}
