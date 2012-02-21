#import "ALAudio.h"

ALCcontext* m_context = nil;
ALCdevice*  m_device  = nil; 


@implementation ALAudio

-(ALAudio*)initWithFilename:(NSString*)filename andExt:(NSString*)ext
{
  if((self = [self init]) != nil)
  {
    if(m_device == nil)
    {
      m_device = alcOpenDevice(NULL);
      if (m_device) 
      {
        m_context = alcCreateContext(m_device,NULL);
        alcMakeContextCurrent(m_context);
      }
    }
    
    ALvoid* audioData;
    ALenum  error = AL_NO_ERROR;
    ALenum  format;
    ALsizei size;
    ALsizei freq;
    
    NSBundle * bundle = [NSBundle mainBundle];
    
    // get some audio data from a wave file
    CFURLRef fileURL = (CFURLRef)[[NSURL fileURLWithPath:[bundle pathForResource:filename ofType:ext]] retain];
    if(!fileURL)
    {
      return nil;
    }
    
    audioData = MyGetOpenALAudioData(fileURL, &size, &format, &freq);
    
    CFRelease(fileURL);
    
    if((error = alGetError()) != AL_NO_ERROR) {
      printf("error loading sound: %x\n", error);
      exit(1);
    }
    
    // grab a buffer ID from openAL
    alGenBuffers(1, &m_buffer_id);
    
    // load the awaiting data blob into the openAL buffer.
    alBufferData(m_buffer_id,format,audioData,size,freq); 
    
    // grab a source ID from openAL
    alGenSources(1, &m_source_id); 
    
    // attach the buffer to the source
    alSourcei(m_source_id, AL_BUFFER, m_buffer_id);

    alSourcef(m_source_id, AL_PITCH, 1.0f);
    alSourcef(m_source_id, AL_GAIN,  1.0f);
    alSourcef(m_source_id, AL_MIN_GAIN,  0.0f);
    alSourcef(m_source_id, AL_MAX_GAIN,  1.0f);
    //    if (loops) alSourcei(m_source_id, AL_LOOPING, AL_TRUE);
    
    // clean up the buffer
    if (audioData)
    {
      free(audioData);
      audioData = NULL;
    }
    
  }
  
  return self;
}

-(void)dealloc
{
  alDeleteSources(1, &m_source_id);
  alDeleteBuffers(1, &m_buffer_id);
  
  alcDestroyContext(m_context);
  alcCloseDevice(m_device);
  
  [super dealloc];
}

-(void)play
{
  alSourcePlay(m_source_id);
}

-(void)stop
{
  alSourceStop(m_source_id);
}

-(void)rewind
{
  alSourceRewind(m_source_id);
}

-(bool)isPlaying
{
  ALenum state;
  alGetSourcei(m_source_id, AL_SOURCE_STATE, &state);
  return (state == AL_PLAYING);
}

-(void)loop:(bool)flag
{
  alSourcei(m_source_id, AL_LOOPING, (flag == YES) ? AL_TRUE : AL_FALSE);
}

-(void)setVolume:(float)volume
{
  float gain = volume / 1.0f * 0.5f;
  alSourcef(m_source_id, AL_GAIN, gain);
}

@end
