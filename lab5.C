#include <kipr/wombat.h>
#include <time.h>

int distance, error, control;
driveLeftWheel = 120;
driveRightWheel = 120;
programstate = 0;
maxstate = 0;
Kp = 0.5;

int main()
{
    camera_open();
    while (program_state > maxstate)
    {
        camera_update();
        distance = get_object_center_x(0, 0);
        error = 80 - distance;
        control = Kp * error;

        if (control > 1 || control < -1)
        {
            drive(control);
        }
        else
        {
            create_drive_direct(driveLeftWheel, driveRightWheel);
        }
    }
}

void drive(int control)
{
    create_drive_direct(driveLeftWheel - control, driveRightWheel + control); // for right turn, input negative number
}