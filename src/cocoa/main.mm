/*
   main.mm
*/

#import "cocoa/gabba.h"
#import "frame/frame.h"
#import "frame/operations.h"
#import <memory>
#import <algorithm>

static void
toString( const NSString     *nsString,
          std::string        &str )
{
	NSData    *asData;

	size_t maxLen = [nsString lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
	str.resize(maxLen);

	if ( [nsString getCString: &(*(str.begin())) maxLength:maxLen encoding: NSUTF8StringEncoding] )
		return;

	asData = [nsString dataUsingEncoding: NSUTF8StringEncoding];
	str.assign(static_cast<const char*>([asData bytes]), [asData length]/sizeof(char));
}

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
	CGContextRef cgContext = static_cast<CGContextRef>( [ [NSGraphicsContext currentContext] graphicsPort ] );
	if ( imageRef )
		CGContextDrawImage(cgContext,  NSRectToCGRect(rect), imageRef);

	using namespace rikiGlue;

	Context::Rect bounds(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	Context ctx(cgContext, imageRef, bounds);
	Application::instance().process(ctx);
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
	std::auto_ptr<Frame> frame( new Frame(imageRect.size.width, imageRect.size.height, Application::instance().decoderThread()) );

#define SPLIT_OP

#if defined(SPLIT_OP)
	const Frame::operation_t  rgbOps[1] = { splitterRGBA };
#else
	const Frame::operation_t  rgbOps[4] = { argbToRGB, lutDecrypt, gChannel, rgbToARGB };
#endif

//	NSDate *date0 = [ NSDate date ];

#if defined(SPLIT_OP)
	frame->operate(&rgbOps[0], 1, &argb[0], rowBytes, frame->bytes(), frame->rowBytes() );
#else
	frame->operate(&rgbOps[0], 1, &argb[0], rowBytes);
	frame->operate(&rgbOps[1], 2, frame->bytes(), frame->rowBytes());
	frame->operate(&rgbOps[3], 1, frame->bytes(), frame->rowBytes(), &argb[0], rowBytes);
#endif

//	double interval0 = - [ date0 timeIntervalSinceNow ];
//	printf("time: %f, %f fps\n", interval0, 1.0/interval0);

	Application::instance().dmtxFrame(frame.release());

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
	rikiGlue::Application::instance().start();

	colorSpace = CGColorSpaceCreateDeviceRGB();
	[ window setDelegate: self ];
	[ self setPaused: nil ];

	interval = 0;
	nIntervals = 0;
}

- (void) applicationWillTerminate: (NSNotification*) notification
{
	[ timer invalidate ];
	[ timer release ];
	[ window setDelegate: nil ];
	CGColorSpaceRelease(colorSpace);

	rikiGlue::Application::instance().stop();

	interval /= -nIntervals;
	printf("t: %f, %f fps\n", interval, 1.0/interval);

#if defined(TEST_FRAME_LEAKS)

	int aFrames = Frame::allocedFrames();
	int dFrames = Frame::deAllocedFrames();
	
	if ( aFrames != dFrames )
		printf("******* LEAKING ********\n");
	printf("number of allocated frames  : %d\n", aFrames);
	printf("number of deallocated frames: %d\n", dFrames);
	if ( aFrames != dFrames )
		printf("******* LEAKING ********\n");

#endif

}

- (void) openPanelDidEnd: (NSOpenPanel*) panel returnCode: (int) returnCode contextInfo: (void*) contextInfo
{
	if ( returnCode == NSOKButton )
	{
		NSArray *files = [ panel filenames ];
		if ( [ files count ] )
		{
			std::string filePath;
			toString([ files objectAtIndex: 0 ], filePath);
			if ( rikiGlue::Application::instance().setRSAKey(filePath) == false )
			{
				NSAlert *alert = [ [ NSAlert alloc ] init ];
				if ( alert )
				{
					[ alert setMessageText: @"Invalid Key" ];
					[ alert runModal ];
					[ alert release ];
				}
			}
		}
	}
	[panel release];
}

- (IBAction) openDocument: (id) sender
{
	NSOpenPanel *openPanel = [[ NSOpenPanel openPanel ] retain ];
	[ openPanel beginForDirectory:nil file:nil types:nil modelessDelegate:self didEndSelector:@selector(openPanelDidEnd:returnCode:contextInfo:) contextInfo:NULL];
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
