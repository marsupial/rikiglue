/*
	gabba.h
*/

#import <Cocoa/Cocoa.h>

@interface Gabba : NSObject
{
	IBOutlet NSWindow *window;
	IBOutlet NSView   *imageView;

	NSTimer           *timer;
}

@end
