/*
	gabba.h
*/

#import <Cocoa/Cocoa.h>
#import "common/application.h"

@interface Dabba : NSView
{
	CGImageRef   imageRef;
}
@end

@interface Gabba : NSObject
#ifdef MAC_OS_X_VERSION_10_6
<NSWindowDelegate>
#endif
{
	IBOutlet NSWindow       *window;
	IBOutlet Dabba          *imageView;
	IBOutlet NSMenuItem     *pauseItem;

	CGColorSpaceRef         colorSpace;
	NSTimer                 *timer;
	
	rikiGlue::Application   application;

	double interval, nIntervals;
}

- (IBAction) setPaused: (id) sender;
- (IBAction) setFloating: (id) sender;
- (void) windowDidMiniaturize: (NSNotification*) notification;
- (void) windowDidDeminiaturize: (NSNotification*) notification;

@end
