/*
   main.mm
*/

#import "cocoa/gabba.h"
#import "frame/frame.h"

@implementation Dabba

- (id) initWithFrame: (NSRect) frameRect
{
	self = [ super initWithFrame: frameRect ];
	if ( self )
		imageRef = NULL;
	
	return ( self );
}

- (void) dealloc
{
	if ( imageRef )
		CGImageRelease(imageRef);

	[ super dealloc ];
}

- (void) setImage: (CGImageRef) image
{
	if ( image == NULL )
		return;

	if ( imageRef )
		CGImageRelease(imageRef);

	imageRef = CGImageRetain(image);
}

- (void) drawRect: (NSRect) rect
{
	if ( imageRef )
	{
		CGContextRef cgContext = static_cast<CGContextRef>( [ [NSGraphicsContext currentContext] graphicsPort ] );
		//CGRect imageRect = { {0,0}, { CGImageGetWidth(imageRef), CGImageGetHeight(imageRef)} };
		CGContextDrawImage(cgContext,  NSRectToCGRect(rect), imageRef);
	}
}

@end

@implementation Gabba

- (CGImageRef) imageBelowWindow: (NSWindow *) inWindow withRect: (NSRect) inRect
{
	NSDate *date = [ NSDate date ];

    // Get window's rect in flipped screen coordinates
    NSRect windowRect = [inWindow frame];
	windowRect.origin.x += inRect.origin.x;
	windowRect.origin.y += inRect.origin.y;
	windowRect.size = inRect.size;

    CGRect captureRect = NSRectToCGRect(windowRect);
    captureRect.origin.y = NSMaxY([[inWindow screen] frame]) - NSMaxY(windowRect);

    // Get a composite image of all the windows beneath your window
    CGImageRef capturedImage = CGWindowListCreateImage(captureRect, kCGWindowListOptionOnScreenBelowWindow, [ inWindow windowNumber ], kCGWindowImageDefault);
	if ( capturedImage == NULL )
		return ( NULL );

    CGRect imageRect = { {0,0}, { CGImageGetWidth(capturedImage), CGImageGetHeight(capturedImage)} };
    if( imageRect.size.width <= 1 || imageRect.size.height <= 1  )
	{
		CGImageRelease(capturedImage);
		return ( NULL );
	}

	const size_t rowBytes = imageRect.size.width * 4;
	std::vector<uint8_t>  argb( rowBytes * imageRect.size.height );
	CGContextRef contextRef = CGBitmapContextCreate(&argb[0], imageRect.size.width, imageRect.size.height, 8, rowBytes, colorSpace, kCGImageAlphaPremultipliedFirst);
	if ( contextRef == NULL )
	{
		CGImageRelease(capturedImage);
		return ( NULL );
	}

	CGContextDrawImage(contextRef, imageRect, capturedImage); 
	CGImageRelease(capturedImage);

	rikiGlue::Frame  frame(&argb[0], rowBytes, imageRect.size.width, imageRect.size.height);	
	//frame.rsaDecrypt();
	frame.lutDecrypt();

	uint8_t *bSplice = &argb[0];
	for ( register_t y = 0; y < imageRect.size.height; ++y )
		for ( register_t x = 0; x < imageRect.size.width; ++x )
		{
			bSplice[1] = bSplice[3];
			bSplice[2] = bSplice[3];
			bSplice += 4;
		}

	CGImageRef imageRef = CGBitmapContextCreateImage(contextRef);
	CGContextRelease(contextRef);

	NSTimeInterval interval = [ date timeIntervalSinceNow ];
	printf("t: %f\n", interval);
	//[ date release ];
    return ( imageRef );
}

- (void) timerFired :(NSTimer*) timer
{
	CGImageRef image = [ self imageBelowWindow: window withRect: [imageView frame] ];
	if ( image )
	{
		[ imageView setImage: image ];
		[ imageView setNeedsDisplay: YES];
	}
}

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
	colorSpace = CGColorSpaceCreateDeviceRGB();
	timer = [ [ NSTimer scheduledTimerWithTimeInterval: (1.0 / 60.0)
	                                                    target: self
	                                                    selector: @selector(timerFired:)
	                                                    userInfo: nil
	                                                    repeats: YES ] retain];
}

- (void) applicationWillTerminate: (NSNotification*) notification
{
	[ timer release ];
	CGColorSpaceRelease(colorSpace);
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed: (NSApplication*) application
{
	return ( TRUE );
}

@end

int main(int argc, char *argv[])
{												   
    return NSApplicationMain(argc,  (const char **) argv);
}
