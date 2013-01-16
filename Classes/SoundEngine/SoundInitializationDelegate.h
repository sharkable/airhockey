//
//  SoundInitializationDelegate.h
//  PetVet
//
//  Created by Jonathan Sharkey on 09-10-26.
//  Copyright 2012 Sharkable. All rights reserved.
//

class SoundPlayer;

class SoundInitializationDelegate {
 public:
  virtual void SoundInitialized(SoundPlayer *sound_player) = 0;
};
