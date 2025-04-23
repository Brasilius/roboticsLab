void box_distance_red(){
    create_stop(); //ensure the machine is stopped
    create_drive(-100,100); //turn the machine
    msleep(500); //needed delay for a turn
    create_drive(-100,-100);
    msleep(2000); //needed delay for 5 ft of distance - this should be the largest of the 3
    } 
    void box_distance_blue(){
        create_stop();
        create_drive(-100,100);
        msleep(500);
        create_drive(-100,100);
        msleep(100); //needed delay for 5 ft of distance - this should be the smallest of the three
    }
    
    void box_distance_green(){
        create_stop();
        create_drive(-100,100);
        msleep(500);
        create_drive(-100,100);
        msleep(500); //needed delay for 5 ft of distance - this should be the inbetween the three
    }