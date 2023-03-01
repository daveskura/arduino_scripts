/*
Playlists will be in folders 01,02,03...

Voice files will be in folder 99



*/

int nbr_playlists = 3;

// 01,02,03
int *PlaylistFolder_nbr[] = {1,2,3};

char *PlaylistNames[] = {"Rock", "80s","Dance"};

int playlist(char* playlistname) {
  int ikey = -1;
  for (int i = 0; i < nbr_playlists; i++) {
    if (PlaylistNames[i] == playlistname) {
      ikey = PlaylistFolder_nbr[i];      
    }
  }
	return ikey;
}

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


int nbr_audiofiles = 2;

// 001, 002
int *AudioFile_nbr[] = {1,2};

char *AudioFilenames[] = {"001.mp3", "002.mp3"};
                          
char *AudioFileContent[] = {
		"good morning sir", 
		"Thunderstruck"		
			};

int audiofile(char* filename) {
  int ikey = -1;
  for (int i = 0; i < nbr_audiofiles; i++) {
    if (AudioFileContent[i] == filename) {
      ikey = AudioFile_nbr[i];      
    }
  }
	return ikey;
}

 /*
   * To play a single MP3.  Rename the MP3 to be unique in the 99 folder using the pattern NNN.mp3
   * as follows.... 001.mp3, 002.mp3 and so on.  File Name(1~255)
	 * update arrays to map text to filenumber
   * tested this in folder 15 & 99
  
  myDFPlayer.playFolder(99,audiofile("good morning sir"));   //play specific mp3 in SD:/99/001.mp3; 
  myDFPlayer.playFolder(99,audiofile("Thunderstruck"));   //play specific mp3 in SD:/99/002.mp3; 

  */
