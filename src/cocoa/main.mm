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

	using namespace rikiGlue;
	Frame  frame(imageRect.size.width, imageRect.size.height, &decoder);	

	const Frame::operation_t  rgbOps[4] = { argbToRGB, lutDecrypt, gChannel, rgbToARGB };
	frame.operate(&rgbOps[0], 1, &argb[0], rowBytes);
	frame.operate(&rgbOps[1], 2, frame.bytes(), frame.rowBytes());
	frame.operate(&rgbOps[3], 1, frame.bytes(), frame.rowBytes(), &argb[0], rowBytes);

	CGImageRef imageRef = CGBitmapContextCreateImage(contextRef);
	CGContextRelease(contextRef);

	interval += [ date timeIntervalSinceNow ];
	nIntervals++;

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
		CGImageRelease(image);
	}
}

- (void) setItem: (id) sender check: (BOOL) b
{
	if ( sender && [ sender respondsToSelector: @selector(setState:) ] )
		[ sender setState: b ];
}

- (IBAction) setPaused: (id) sender
{
	if ( timer != nil || [ window isMiniaturized ] )
	{
		if ( timer )
		{
			[ timer invalidate ];
			[ timer release ];
			timer = nil;
		}
		[ self setItem: sender check: TRUE ];
	}
	else
	{
		timer = [ [ NSTimer scheduledTimerWithTimeInterval: (1.0 / 30.0)
															target: self
															selector: @selector(timerFired:)
															userInfo: nil
															repeats: YES ] retain ];
		[ self setItem: sender check: FALSE ];
	}
}

- (void) windowDidMiniaturize: (NSNotification*) notification
{
	if ( timer )
		[ self setPaused: pauseItem ];
}

- (void) windowDidDeminiaturize: (NSNotification*) notification
{
	if ( [ pauseItem state ] )
		[ self setPaused: pauseItem ];

}

- (IBAction) setFloating: (id) sender
{
	if ( [ window level ] == NSNormalWindowLevel )
	{
		[ window setLevel: NSFloatingWindowLevel ];
		[ self setItem: sender check: TRUE ];
	}
	else
	{
		[ window setLevel: NSNormalWindowLevel ];
		[ self setItem: sender check: FALSE ];
	}
}

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
	colorSpace = CGColorSpaceCreateDeviceRGB();
	[ window setDelegate: self ];
	[ self setPaused: nil ];
	decoder.create();
	interval = 0;
	nIntervals = 0;
}

- (void) applicationWillTerminate: (NSNotification*) notification
{
	decoder.lock();
	decoder.exit();
	decoder.unlock();
	decoder.signal();
	
	[ window setDelegate: nil ];
	[ timer invalidate ];
	[ timer release ];
	CGColorSpaceRelease(colorSpace);

	interval /= -nIntervals;
	printf("t: %f, %f fps\n", interval, 1.0/interval);
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
