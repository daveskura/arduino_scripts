/*
Playlists will be in folders 01,02,03...

Voice files will be in folder 99

To generate MP3 files for chicken car
go here: https://freetts.com/#ads
select: English (UK) Brian

*/
#define DFPLAYER_VOICE_VOLUME 20 //Set volume value. From 0 to 30

#include "DFRobotDFPlayerMini.h"

int gbl_nbr_playlists = 4;

int gbl_nbr_volume_ranges = 12;
int *volume_range_array[] = {5,10,15,20,25,30,25,20,15,10,5,0};


/*
   -1 Right then, no music for now
    0 Boom 97.3 on the FM dial
    1 80s Retro
    2 Rock, well, Technically Chicken Rock
    3 Dance music, for chickens.  So I guess chicken dance music
    4 Just drop the needle
*/

int *PlaylistFolder_nbr[] = {1,2,3,4};
// 01,02,03

char *PlaylistNames[] = {
		"80s Retro"
		,"Rock, well, Technically Chicken Rock"
		,"Dance music, for chickens.  So I guess chicken dance music"
		,"Just drop the needle"};

  /*
	 * Playlists 
	 *
   * it seems we can create playlists for the player to loop through and play 1 song after another.
   * playlist folders are named...01,02,03 ...to 99
   * 
   * Filenames which work so far ... 0001.mp3, 0002.mp3 and alphanumeric and spaces in file names .. all with suffix .mp3
   * Have not tested special characters in filenames yet.

		 myDFPlayer.loopFolder(playlist("Rock"));  //loop all mp3 files in folder SD:/01
		 myDFPlayer.loopFolder(playlist("80s"));  //loop all mp3 files in folder SD:/02
		 myDFPlayer.loopFolder(playlist("Dance"));  //loop all mp3 files in folder SD:/03
  
  */

int playlist(char* playlistname) {
  int ikey = -1;
  for (int i = 0; i < gbl_nbr_playlists; i++) {
    if (PlaylistNames[i] == playlistname) {
      ikey = PlaylistFolder_nbr[i];      
    }
  }
	return ikey;
}



int nbr_audiofiles = 18;

// 001, 002
int *AudioFile_nbr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};

// char *AudioFilenames[] = {"001.mp3", "002.mp3", ....
char *AudioFileContent[] = {
	"Good morning sir"
	,"Good morning sir, the power is on"
	,"The chicken car is ready to go"
	,"Auto Chicken is on, starting forward"
	,"Auto Chicken is on, starting in reverse"
	,"Auto Chicken is off.  The car will remain stationary until you move it"
	,"careful chickens, moving backwards this time"
	,"heads up chickens, going to move a bit forward"
	,"look alive chickens, we're heading backward a little"
	,"mind your beaks and feet, moving forwards this time"
	,"The control mechanism has been set to Radio frequency.  You'll need the remote control to move the Chicken car."
	,"The control mechanism has been set to local.  You can lift the lid on the joystick compartment and control the chicken car with the oncar joystick."
	,"Boom 97.3 on the FM dial"
	,"80s Retro"
	,"Rock, well, Technically Chicken Rock"
	,"Dance music, for chickens.  So I guess chicken dance music"
	,"Just drop the needle"
	,"Right then, no music for now"
		};

/*
   * To play a single MP3.  Rename the MP3 to be unique in the 99 folder using the pattern NNN.mp3
   * as follows.... 001.mp3, 002.mp3 and so on.  File Name(1~255)
	 * update arrays to map text to filenumber
   * tested this in folder 15 & 99
  
  myDFPlayer.playFolder(99,audiofile("good morning sir"));   //play specific mp3 in SD:/99/001.mp3; 
  myDFPlayer.playFolder(99,audiofile("Thunderstruck"));   //play specific mp3 in SD:/99/002.mp3; 
*/

int audiofile(char* filename) {
  int ikey = 404;
  for (int i = 0; i < nbr_audiofiles; i++) {
    if (AudioFileContent[i] == filename) {
      ikey = AudioFile_nbr[i];      
    }
  }
	return ikey;
}
/*
6) When Radio is turned on, or MP3 playlist cycled.  Button should cycle Radio or playlists.  Announcing each playlist or radio station, then proceeding in a few seconds to play the songs.
	a) Right then, no music for now
	b) Boom 97.3 on the FM dial
	c) 80s Retro
	d) Rock, well, Technically Chicken Rock
	e) Dance music, for chickens.  So I guess chicken dance music
	f) Just drop the needle
*/
void music_voice_notify(DFRobotDFPlayerMini &var_myDFPlayer) {
  var_myDFPlayer.volume(DFPLAYER_VOICE_VOLUME);  
	//int gbl_music_mode = 0; // 0 = Radio, 1=playlist #1, 2=playlist #2, 3=playlist #3.... gbl_nbr_playlists
  if (gbl_music_mode == -1) {
    var_myDFPlayer.playFolder(99,audiofile("Right then, no music for now"));
  } else if (gbl_music_mode == 0) {
    var_myDFPlayer.playFolder(99,audiofile("Boom 97.3 on the FM dial"));
  } else if (gbl_music_mode == 1)  {
    var_myDFPlayer.playFolder(99,audiofile("80s Retro"));
  } else if (gbl_music_mode == 2)  {
    var_myDFPlayer.playFolder(99,audiofile("Rock, well, Technically Chicken Rock"));
  } else if (gbl_music_mode == 3)  {
    var_myDFPlayer.playFolder(99,audiofile("Dance music, for chickens.  So I guess chicken dance music"));
  } else if (gbl_music_mode == 4)  {
    var_myDFPlayer.playFolder(99,audiofile("Just drop the needle"));
  }
	delay(2000);
}

/*
 1) power on
    a) Good morning sir.
    b) Good morning sir, the power is on.
    c) The chicken car is ready to go.
    d) Might I suggest checking the battery voltage soon
*/
void Power_on_voice_notify(DFRobotDFPlayerMini &var_myDFPlayer) {
  int idx = random(1,3);
  var_myDFPlayer.volume(DFPLAYER_VOICE_VOLUME);  
  if (idx == 1) {
    var_myDFPlayer.playFolder(99,audiofile("Good morning sir"));
  } else if (idx == 2)  {
    var_myDFPlayer.playFolder(99,audiofile("Good morning sir, the power is on"));
  } else {
    var_myDFPlayer.playFolder(99,audiofile("The chicken car is ready to go"));
  }
  // MP3 files are played asyncronously, so adding a wait period here.
  delay(3000);
}


/*
  2) Autochicken toggle change  
  3) Autochicken, forward, backward toggle change
    a) Auto Chicken is on, starting forward
    b) Auto Chicken is on, starting in reverse 
    c) Auto Chicken is off.  The car will remain stationary until you move it
*/
void Autochicken_toggle_voice_notify(DFRobotDFPlayerMini &var_myDFPlayer) {
  var_myDFPlayer.volume(DFPLAYER_VOICE_VOLUME);  
 
  if (last_checked_switch_auto_chicken == LOW) {
    if (last_checked_switch_auto_chicken_fwd_rvrse == LOW)  {
      var_myDFPlayer.playFolder(99,audiofile("Auto Chicken is on, starting forward"));
    } else {
      var_myDFPlayer.playFolder(99,audiofile("Auto Chicken is on, starting in reverse"));
    }
  } else {
    var_myDFPlayer.playFolder(99,audiofile("Auto Chicken is off.  The car will remain stationary until you move it"));
  }

}


/*
4) RF/Local toggle change
  a) The control mechanism has been set to Radio frequency.  You'll need the remote control to move the Chicken car.
  a) The control mechanism has been set to local.  You can lift the lid on the joystick compartment and control the chicken car with the oncar joystick.

*/
void RF_vs_Local_Toggle_voice_notify(DFRobotDFPlayerMini &var_myDFPlayer) {
  var_myDFPlayer.volume(DFPLAYER_VOICE_VOLUME);  
  // need to add logic to determine when switch is flipped
  if (gbl_rf_vs_local==LOW )  {
      var_myDFPlayer.playFolder(99,audiofile("The control mechanism has been set to Radio frequency.  You'll need the remote control to move the Chicken car."));

  } else {
      var_myDFPlayer.playFolder(99,audiofile("The control mechanism has been set to local.  You can lift the lid on the joystick compartment and control the chicken car with the oncar joystick."));
  }
}
