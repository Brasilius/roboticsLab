#include <kipr/wombat.h>
#include <time.h>

int distance, error;
Kp = 0.5;
int main()
{

    while (1)
    {
        distance = analog(0);
        error = 18 - distance;
        create_drive_direct(Kp * error, Kp * error);
    }
}