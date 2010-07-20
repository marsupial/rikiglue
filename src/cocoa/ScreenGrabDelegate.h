/*
	ScreenGrabDelegate.h
*/

#import <Cocoa/Cocoa.h>

@interface ScreenGrabDelegate : NSObject
{
	IBOutlet NSWindow *window;
	IBOutlet NSView   *imageView;

	NSTimer           *timer;
}

@end
