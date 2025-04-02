#include <kipr/wombat.h>

// PID constants
float Kp = 0.1;
float Ki = 0.01;
float Kd = 0.05;

// Sensor ports
int sensor_left_port = 0;
int sensor_right_port = 1;

// Motor speed base (mm/s)
int base_speed = 100;


// PID variables
float error, previous_error = 0, integral = 0, derivative;

int main() {
    printf("Connecting to Create...\n");
    create_connect();
    printf("Connected!\n");
	int programstate = 0;
    
    while (programstate != 1) {
        // Read sensors
        int left_val = analog(sensor_left_port);
        int right_val = analog(sensor_right_port);
        
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
        printf("L: %d R: %d | Error: %.2f | Correction: %.2f\n", left_val, right_val, error, correction);

        msleep(20);
    }

    printf("Stopping and disconnecting...\n");
    create_stop();
    create_disconnect();
    return 0;
}
