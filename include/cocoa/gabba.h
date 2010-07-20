/*
	gabba.h
*/

#import <Cocoa/Cocoa.h>

@interface Dabba : NSView
{
	CGImageRef   imageRef;
}
@end

@interface Gabba : NSObject
{
	IBOutlet NSWindow *window;
	IBOutlet Dabba    *imageView;

	CGColorSpaceRef   colorSpace;
	NSTimer           *timer;
	
	double interval, nIntervals;
}
@end
