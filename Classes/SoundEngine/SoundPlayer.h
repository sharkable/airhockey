//
//  SoundPlayer.h
//  PetVet
//
//  Created by Jonathan Sharkey on 09-10-21.
//  Copyright 2009 Sharkable. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ALAudio.h"
#import "AVAudio.h"
#import "SoundInitializationDelegate.h"

typedef enum {
  kSoundScore = 0,
  kSoundScoreFinal,
  kSoundPaddleHit,
  kSoundPuckRinkBounce,
  kSoundTwoPuckHit,
  kSoundButton,
  kSoundMultiSelect,
  kSoundGetReady,
  kSoundStart,
  kNumSounds
} Sound;

@interface SoundPlayer : NSObject {
  NSMutableArray* sounds_;
  AVAudio* song_;
  BOOL musicOn_;
  BOOL soundEffectsOn_;
  AudioInterruptDelegate* delegate_;
}

+(AVAudioSession*) session;
+(BOOL) isMusicPlayingInITunes;
+(void) syncAudioSessionForITunes;
+(void) duckAudioFromITunes:(BOOL)duck;

+ (void) initializeWithDelegate:(NSObject <SoundInitializationDelegate> *)delegate;
+ (BOOL) setGlobalVolume:(float)volume;
+ (BOOL) setVolume:(Sound)sound volume:(float)volume;
+ (BOOL) playSound:(Sound)sound;
+ (BOOL) stopSound:(Sound)sound;
+ (void) playSong:(NSString*)filename;
+ (void) stopSong;
+ (void) setMusicOn:(BOOL)on;
+ (void) setSoundEffectsOn:(BOOL)on;
- (void) loadSoundsWithDelegate:(NSObject <SoundInitializationDelegate> *)delegate;

@property (assign) NSMutableArray* sounds;
@property (assign) AVAudio* song;
@property BOOL musicOn;
@property BOOL soundEffectsOn;

@end
