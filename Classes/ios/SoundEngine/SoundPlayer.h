//
//  SoundPlayer.h
//  SocialDeck
//
//  Created by Jonathan Sharkey on 09-10-21.
//  Copyright 2009 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ALAudio.h"
#import "AVAudio.h"
#import "SoundInitializationDelegate.h"
#import "sound_player.h"

class SoundPlayerImpl : public SoundPlayer {
 public:
  SoundPlayerImpl();
  
  AVAudioSession* session();
  bool isMusicPlayingInITunes();
  void syncAudioSessionForITunes();
  void duckAudioFromITunes(bool duck);
  
  void initializeWithDelegate(SoundInitializationDelegate *delegate);
  bool setGlobalVolume(float volume);
  bool setVolume(Sound sound, float volume);
  bool playSound(Sound sound);
  bool stopSound(Sound sound);
  void playSong(string filename);
  void stopSong();
  void setMusicOn(bool on);
  void setSoundEffectsOn(bool on);

 private:
  NSMutableArray* sounds_;
  AVAudio* song_;
  bool musicOn_;
  bool soundEffectsOn_;
  AudioInterruptDelegate* delegate_;
  
  void loadSoundsWithDelegate(SoundInitializationDelegate *delegate);
  NSURL *filenameToUrl(NSString *name);
};

//@property (assign) NSMutableArray* sounds;
//@property (assign) AVAudio* song;
//@property BOOL musicOn;
//@property BOOL soundEffectsOn;

//@end
