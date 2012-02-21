#import "AVAudio.h"


@implementation AudioInterruptDelegate


 
 //audioPlayerEndInterruption: is called when the audio session interruption has ended and this player had been interrupted while playing. (phone rings, user ignores it... music stops, now its back on yay)
 //The player can be restarted at this point. 
 
- (void)audioPlayerEndInterruption:(AVAudioPlayer *)player
{
  NSError* activationError = nil;
  [[AVAudioSession sharedInstance] setActive:YES error:&activationError];
  [player play];  
}

@end


@implementation AVAudio


-(AVAudio*) initWithURL:(NSURL*)url
{
  if((self = [super init]) != nil)
  {
    NSError* error;
    m_player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];

    if( m_player == nil )
    {
      NSLog( @"ERROR Initializing AVAudioPlayer in AVAudio::initWithURL \n\t%@\n\tURL [%@]\n", error, url );
    }
    else 
    {
      NSLog( @"SUCCESS Initializing AVAudioPlayer [%x] in AVAudio::initWithURL \n\tURL [%@]\n", m_player,  url );
    }

    
    [self loop:NO];
    [m_player prepareToPlay];
  }
  
  return self;
}

-(void) dealloc
{
  [self stop];
  
  [m_player release];
  [super dealloc];
}

-(void) play
{
  if(m_player.playing) {
    [m_player stop];
  }
  [m_player play];
}

-(void) stop
{
  if(m_player.playing == YES) {
    [m_player stop];
  }
}

-(void) rewind
{
  m_player.currentTime = 0.0f;
}

-(bool)isPlaying
{
  return m_player.playing;
}

-(void)loop:(bool)flag
{
  m_player.numberOfLoops = (flag == YES ? -1 : 0);
}


-(void)setVolume:(float)volume
{
//  if     (volume < 0.0f) { volume = 0.0f; }
//  else if(volume > 1.0f) { volume = 1.0f; }
  m_player.volume = volume;
}

-(void) pause
{
  if(m_player.playing == YES) {
    [m_player pause];
  }
}

-(void) seek:(NSTimeInterval)idx
{
  if     (idx <              0.0f) { idx =              0.0f; }
  else if(idx > m_player.duration) { idx = m_player.duration; }
  m_player.currentTime = idx;
}

// Set the audio-session-interrupt handler
-(void) setInterruptDelegate:(AudioInterruptDelegate*)delegate;
{
  m_player.delegate = delegate;
}

@end
