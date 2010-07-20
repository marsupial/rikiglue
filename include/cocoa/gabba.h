/*
	gabba.h
*/

#import <Cocoa/Cocoa.h>

#import "threads/threads.h"

@interface Dabba : NSView
{
	CGImageRef   imageRef;
}
@end

@interface Gabba : NSObject
{
	IBOutlet NSWindow       *window;
	IBOutlet Dabba          *imageView;

	CGColorSpaceRef         colorSpace;
	NSTimer                 *timer;
	
	rikiGlue::DecodeThread  decoder;

	double interval, nIntervals;
}
@end
