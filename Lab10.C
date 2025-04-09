#include <kipr/wombat.h>

// PID constants
float Kp = 0.2;
float Ki = 0.01;
float Kd = 0.05;

// Sensor ports
int sensor_left_port = 0;
int sensor_right_port = 1;
int sensor_center_port = 2;

// Motor speed base (mm/s)
int base_speed = 100;


// PID variables
float error, previous_error = 0, integral = 0, derivative;

void turn();

int main() {
    printf("Connecting to Create...\n");
    create_connect();
    printf("Connected!\n");
	int programstate = 0;
    
    while (programstate != 1) {
        // Read sensors
        int left_val = analog(sensor_left_port);
        int right_val = analog(sensor_right_port);
        int center_val = analog(sensor_center_port);
        
        //If Center sensor detects a wall close, robot is at a dead end.
        //Turn 180*
        float centerDist = 0.0001*center_val*center_val - 0.804*center_val + 1341;
                // Print for debugging
        printf("L: %d R: %d C: %.2f\n", left_val, right_val, centerDist);
        if(centerDist < 5){
            turn();
        }
        
        //Convert to distance in mm using polynomial trendline found in HW 1
        float leftDist = 0.0001*left_val*left_val - 0.804*left_val + 1341;
        float rightDist = 0.0001*right_val*right_val - 0.804*right_val + 1341;
            
        // Compute error
        error = leftDist - rightDist;

        // PID calculations
        
        integral += error;
        derivative = error - previous_error;
        float correction = Kp * error + Kd * derivative;

        // Apply to drive speeds
        int left_speed = base_speed - correction;
        int right_speed = base_speed + correction;

        // Drive
        create_drive_direct(left_speed, right_speed);

        // Update error
        previous_error = error;

        // Print for debugging
        //printf("L: %d R: %d C: %.2f | Error: %.2f | Correction: %.2f\n", left_val, right_val, centerDist, error, correction);

        msleep(20);
    }

    printf("Stopping and disconnecting...\n");
    create_stop();
    create_disconnect();
    return 0;
}

void turn(){
    printf("Dead End\n");
	set_create_total_angle(0);
    create_drive_direct(base_speed, -base_speed);
    while(get_create_total_angle() < 90 && get_create_total_angle() > -90){
        printf("Angle: %d\n", get_create_total_angle());
        msleep(20);
    }
    create_stop();
}
