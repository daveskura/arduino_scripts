
int nbr_audiofiles = 9;

char *AudioFilenames[] = {"0000.ad4", "0001.ad4", "0002.ad4","0003.ad4", "0004.ad4", "0005.ad4",
													"0006.ad4", "0007.ad4", "0008.ad4"};
char *AudioFileContent[] = {
		"auto_chicken_is_off_the_car_will_remain_here_until_you_move_it", 
		"auto_chicken_is_on_starting_forward", 
		"auto_chicken_is_on_starting_in_reverse",
		"careful_chickens_moving_backwards_this_time", 
		"good_morning_sir", 
		"heads_up_chickens_going_to_move_a_bit_forward",
		"look_alive_chickens_were_heading_backward_a_little", 
		"mind_your_beaks_and_feet_moving_forwards_this_time", 
		"the_chicken_car_is_ready_to_go"};

void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600); 
	Serial.println("Starting...");

	Serial.print("audiofile(`good_morning_sir`) -> ");
	Serial.println(audiofile("good_morning_sir"));
	;

	Serial.println("Done");

}

int audiofile(char* filename) {
  int ikey = -1;
  for (int i = 0; i < nbr_audiofiles; i++) {
    if (AudioFileContent[i] == filename) {
      ikey = i;      
    }
  }
	return ikey;
}



void loop() {
  // put your main code here, to run repeatedly:
}
