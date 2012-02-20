#import "Audio.h"
#import "MyOpenALSupport.h"

@interface ALAudio : NSObject <Audio>
{
	ALuint m_buffer_id;
	ALuint m_source_id;
}

-(ALAudio*)initWithFilename:(NSString*)filename andExt:(NSString*)ext;

@end
