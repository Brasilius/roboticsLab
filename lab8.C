#include <kipr/wombat.h>
#include <time.h>

int distance, error, control;
int driveLeftWheel = 120;
int driveRightWheel = 120;
int programstate = 0;
int maxstate = 4; // search for spong, drive to spong, search for box, drive to box
float Kp = 0.5;
float Ki = 0.02;
float Kd = 0.1;
float dt = 0.05;
float MAX_INTEGRAL = 1000;
int spong = 0;            // Channel for sponge
int spongThreshold = 120; // Threshold bounding box width for red box.
int greenBucket = 1;      // Channel for green bucket
int greenThreshold = 100; // Threshold Bounding Box for green bucket.

void armControlGrab();
void armControlDrop();
void drive(int control);
void turn();

int main()
{
    create_connect();
    camera_open();
    enable_servos();
    set_servo_position(0, 1005);
    set_servo_position(1, 1500);

    // state 1 : find red sponge 1 (broken into multiple substates (such as find, drive too, stop) 7.5 seconds max
    while (programstate < 1) // Search for spong
    {
        create_stop();
        camera_update();
        int count = get_object_count(spong);
        if (count > 0)
        {
            int i = 0;
            for (i = 0; i < count; i++)
            {
                if (get_object_bbox(spong, i).width > 10)
                {
                    printf("Sponge Found\n");
                    programstate++;
                    break;
                }
            }
        }
        if (programstate < 1)
        {
            turn();
            msleep(50);
        }
    }

    int integral = 0;
    int derivative = 0;
    int previous_error = 0;
    while (programstate < 2) // go to spong and grab
    {
        camera_update();
        if (get_object_bbox(spong, 0).width > 70)
        {
            create_stop();
            printf("Sponge Reached\n");
            armControlGrab();
            msleep(2000);
            programstate++;
            continue;
        }

        distance = get_object_center_x(spong, 0);
        error = 80 - distance;
        integral += error * dt;
        derivative = (error - previous_error) / dt;
        if (integral > MAX_INTEGRAL)
        {
            integral = MAX_INTEGRAL;
        }
        if (integral < -MAX_INTEGRAL)
        {
            integral = -MAX_INTEGRAL;
        }
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
    // state 2 : drop into bucket 7.5 seconds max
    while (programstate < 3) // Search for bucket
    {
        create_stop();
        camera_update();
        int count = get_object_count(greenBucket);
        if (count > 0)
        {
            int i = 0;
            for (i = 0; i < count; i++)
            {
                if (get_object_bbox(greenBucket, i).width > 10)
                {
                    printf(" Box Found\n");
                    programstate++;
                    break;
                }
            }
        }
        if (programstate < 1)
        {
            turn();
            msleep(50);
        }
    }

    integral = 0;
    derivative = 0;
    previous_error = 0;
    while (programstate < maxstate) // go to bucket
    {
        camera_update();
        if (get_object_bbox(greenBucket, 0).width > 120)
        {
            create_stop();
            printf("Bucket Reached\n");
            armControlDrop();
            msleep(2000);
            programstate++;
            continue;
        }

        distance = get_object_center_x(greenBucket, 0);
        error = 80 - distance;
        integral += error * dt;
        derivative = (error - previous_error) / dt;
        if (integral > MAX_INTEGRAL)
        {
            integral = MAX_INTEGRAL;
        }
        if (integral < -MAX_INTEGRAL)
        {
            integral = -MAX_INTEGRAL;
        }
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
    // state 3 : find red sponge 2 (broken into multiple substates (such as find, drive too, stop) 7.5 seconds max
    // state 4 : drop into bucket 7.5 seconds max
    return (0);
}
void armControlGrab()
{ // calls the arm to perform the grabbing action
    // ensure arm is in proper starting position
    msleep(500);
    set_servo_position(0, 1005);
    set_servo_position(1, 1500); // claw is open for retrieval
    msleep(500);
    set_servo_position(0, 1400); // lowers claw to barely above ground level - might even be touching
    msleep(500);
    create_drive_direct(50, 50);
    msleep(1000);
    set_servo_position(1, 1950); // claw closes to grab sponge
    msleep(500);
    set_servo_position(0, 1005); // raises arm back up for travel
    msleep(500);
}
void armControlDrop()
{
    msleep(500);
    set_servo_position(1, 1400);
    msleep(500);
}
void drive(int control)
{
    create_drive_direct(driveLeftWheel - control, driveRightWheel + control); // for right turn, input negative number
}

void turn()
{
    create_drive_direct(driveLeftWheel, -driveRightWheel); // Turns right
}
/*void searchForCube()
{
    while (programstate < 1)
    { // the program state statement needs to be reworked
        create_stop();
        camera_update();
        int count = get_object_count(redBox);
        if (count > 0)
        {
            int i = 0;
            for (i = 0; i < count; i++)
            {
                if (get_object_bbox(redBox, i).width > 10)
                {
                    printf("Red Box Found\n"); // tune the fucking color channels leo (leo wrote this)
                    programstate++;
                    break;
                }
            }
        }
        if (programstate < 1)
        {
            turn();
            msleep(50);
        }
    }
}*/
/*void driveToCube()
{
    int integral = 0;
    int derivative = 0;
    int previous_error = 0;
    while (programstate < 2) // figure out method to rework program state statements
    {
        camera_update();
        if (get_object_bbox(redBox, 0).width > 90)
        {
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
        if (integral > MAX_INTEGRAL)
        {
            integral = MAX_INTEGRAL;
        }
        if (integral < -MAX_INTEGRAL)
        {
            integral = -MAX_INTEGRAL;
        }
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
}*/