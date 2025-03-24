#include <kipr/wombat.h>
#include <time.h>

int main()
{
    printf("Hello World\n");
    
int distance, error, control;
int driveLeftWheel = 120;
int driveRightWheel = 120;
int programstate = 0;
int maxstate = 4; //Search, Go to Red, search, Go to green, end
float Kp = 0.5;
float Ki = 0.02;
float Kd = 0.1;
float dt = 0.05;
float MAX_INTEGRAL = 1000;
int redBox = 0; //Channel for red box
int redThreshold = 120; //Threshold bounding box width for red box.
int greenBucket = 1; // Channel for green bucket
int greenThreshold = 100; // Threshold Bounding Box for green bucket.
    
void armControl(int positionValForServo);    
void drive(int control);
void turn();
    
	create_connect();	
    camera_open();
    
    //state 1 : find red sponge 1 (broken into multiple substates (such as find, drive too, stop) 7.5 seconds max
    while(state not finished){
    
    //state 2 : drop into bucket 7.5 seconds max
    while(state not finished){
    
    }
    //state 3 : find red sponge 2 (broken into multiple substates (such as find, drive too, stop) 7.5 seconds max 
    while(state not finished){
    
    }
    //state 4 : drop into bucket 7.5 seconds max
    while(state not finished){
    
    }
    
    
}


void armControl(){ //calls the arm to perform the grabbing action



}
void drive(int control)
{
    create_drive_direct(driveLeftWheel - control, driveRightWheel + control); // for right turn, input negative number
}
                
void turn()
{
    create_drive_direct(driveLeftWheel, -driveRightWheel); // Turns right
}
void searchForCube(){
 		while(programstate < 1){//the program state statement needs to be reworked
    	create_stop();
        camera_update();
        int count = get_object_count(redBox);
        if(count > 0){
        int i = 0;
        for(i = 0; i < count; i++){
		if(get_object_bbox(redBox, i).width > 10){
			printf("Red Box Found\n"); //tune the fucking color channels leo (leo wrote this)
        	programstate++;
            break;
                }
            }

        }
        if(programstate < 1){
        	turn();
            msleep(50);
        }
    }
}
void driveToCube(){
 	int integral = 0;
    int derivative = 0;
    int previous_error = 0;
    while (programstate < 2) //figure out method to rework program state statements
    {
        camera_update();
        if(get_object_bbox(redBox, 0).width > 90){
            create_stop();
			printf("Red Box Reached\n");
            msleep(2000);
            programstate++;
            continue;
        }

        
        distance = get_object_center_x(redBox, 0);
        error = 80 - distance;
        integral += error * dt;
        derivative = (error - previous_error) / dt;
        if(integral > MAX_INTEGRAL){integral = MAX_INTEGRAL;}
        if(integral < -MAX_INTEGRAL){integral = -MAX_INTEGRAL;}
        control = Kp * error + Ki * integral + Kd * derivative;

        if (control > 1 || control < -1)
        {
            drive(control);
        }
        else
        {
            drive(0);
        }
        msleep(50);
    }
    
    return 0;
}
}
