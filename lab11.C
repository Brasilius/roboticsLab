#include <kipr/wombat.h>

int i, buttonPort, IRPort;
float kP = 0.6;

for(i = 0; i < 3 i++)
{

//First Goal: Correct heading
//Strategy: 


//Second Goal: Line up with first box
//Strategy: Use known cardinal heading along with camera left and right position
//Choose a box, look in cardinal direction toward box, read object positon on screen, turn direction to drive to line up better,
//drive short time, repeat until lined up


//Third Goal: Space robot 5' away from box
//Strategy: Use IR to read distance, calibrate well to get accurate readings.
//Look at box, take IR readings, use p control on distance to 5' away to reach desired distance
    int read = analog(IRPort);
    float dist = 5 - (1341 - 0.804 * read) / 304.8; // Parentheses is callibration eq, 304.8 is mm to feet, subtract from 5 to get dist from desired
    

//Wait for pushbutton to repeat, run 3 times.
    printf("Press button to continue.\n")
    int button = 0;
    while(button = 0){
        button = digital(buttonPort);
    }
}