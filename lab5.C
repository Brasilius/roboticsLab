#include <kipr/wombat.h>
#include <time.h>

int distance, error, control;
int driveLeftWheel = 120;
int driveRightWheel = 120;
int programstate = 0;
int maxstate = 4; //Search, Go to Red, search, Go to green, end
float Kp = 0.5;
int redBox = 0; //Channel for red box
int redThreshold = 120; //Threshold bounding box width for red box.
int greenBucket = 1; // Channel for green bucket
int greenThreshold = 100; // Threshold Bounding Box for green bucket.

void drive(int control);
void turn();

int main()
{
    create_connect();	
    camera_open();
    while(programstate < 1) //Search
    {
        create_stop();
        camera_update();
        
        if(get_object_count(0) > 0){
         	printf("Red Box Found\n");
            programstate++;
            continue;
        }
        else{
        	turn();
            msleep(50);
        }
    }
    while (programstate < 2)
    {
        camera_update();
        if(get_object_bbox(redBox, 0).width > 120){
            create_stop();
			printf("Red Box Reached\n");
            programstate++;
            continue;
        }

        
        distance = get_object_center_x(redBox, 0);
        error = 80 - distance;
        control = Kp * error;

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
    
    while(programstate < 3)
    {
    	create_stop();
        camera_update();
        
        if(get_object_count(greenBucket) > 0){
         	printf("Green Bucket Found\n");
            programstate++;
            continue;
        }
        else{
        	turn();
            msleep(50);
        } 
    }
    
    while(programstate < maxstate){
        camera_update();
        if(get_object_bbox(greenBucket, 0).width > 120){
            create_stop();
			printf("Green Bucket Reached\n");
            programstate++;
            continue;
        }
        
        distance = get_object_center_x(greenBucket, 0);
        error = 80 - distance;
        control = Kp * error;

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

void drive(int control)
{
    create_drive_direct(driveLeftWheel - control, driveRightWheel + control); // for right turn, input negative number
}
                
void turn(){
    create_drive_direct(driveLeftWheel, -driveRightWheel); // Turns right
}