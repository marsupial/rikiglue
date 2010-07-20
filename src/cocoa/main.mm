//
//  main.m
//  Untitled
//
//  Created by pimpleface on 7/18/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <ApplicationServices/ApplicationServices.h>
#import "cocoa/gabba.h"
#import "frame/frame.h"

@implementation Gabba

- (NSImage*) imageBelowWindow: (NSWindow *) inWindow withRect: (NSRect) inRect
{
    // Get the CGWindowID of supplied window
    CGWindowID windowID = [ inWindow windowNumber ];

    // Get window's rect in flipped screen coordinates
    NSRect windowRect = [inWindow frame];
	windowRect.origin.x += inRect.origin.x;
	windowRect.origin.y += inRect.origin.y;
	windowRect.size = inRect.size;

    CGRect captureRect = NSRectToCGRect(windowRect);
    captureRect.origin.y = NSMaxY([[inWindow screen] frame]) - NSMaxY(windowRect);

    // Get a composite image of all the windows beneath your window
    CGImageRef capturedImage = CGWindowListCreateImage(captureRect, kCGWindowListOptionOnScreenBelowWindow, windowID, kCGWindowImageDefault);

    CGRect imageRect = { {0,0}, { CGImageGetWidth(capturedImage), CGImageGetHeight(capturedImage)} };
	
    if( imageRect.size.width <= 1 || imageRect.size.height <= 1  )
	{
        CGImageRelease(capturedImage);
        return nil;
    }

	size_t rowBytes = imageRect.size.width * 4;
	void *argb = malloc( rowBytes * imageRect.size.height );
	CGColorSpaceRef csRef = CGColorSpaceCreateDeviceRGB();
	CGContextRef contextRef = CGBitmapContextCreate(argb, imageRect.size.width, imageRect.size.height, 8, rowBytes, csRef, kCGImageAlphaPremultipliedFirst);
	CGImageRef bmapImage = CGBitmapContextCreateImage(contextRef);
	CGContextDrawImage(contextRef, imageRect, capturedImage); 
	CGImageRelease(capturedImage);

	rikiGlue::Frame  frame(argb, rowBytes, imageRect.size.width, imageRect.size.height);	
	//frame.rsaDecrypt();
	frame.lutDecrypt();

	uint8_t *bSplice = static_cast<uint8_t*>(argb);
	for ( register_t y = 0; y < imageRect.size.height; ++y )
		for ( register_t x = 0; x < imageRect.size.width; ++x )
		{
			bSplice[1] = bSplice[3];
			bSplice[2] = bSplice[3];
			bSplice += 4;
		}

	// Create a bitmap rep from the window and convert to NSImage...
	NSBitmapImageRep *bitmapRep = [ [[NSBitmapImageRep alloc] initWithCGImage: bmapImage] autorelease ];
//	NSBitmapImageRep *bitmapRep = [ [[NSBitmapImageRep alloc] initWithBitmapDataPlanes: argb pixelsWide: imageRect.size.width pixelsHigh: imageRect.size.height bitsPerSample: 8 samplesPerPixel: 4 hasAlpha: YES isPlanar: NO colorSpaceName: NSDeviceRGBColorSpace bytesPerRow: rowBytes bitsPerPixel: 0] autorelease ];

	NSImage *image = [ [[NSImage alloc] initWithSize: NSSizeFromCGSize(imageRect.size) ] autorelease ];
	[ image addRepresentation: bitmapRep ];

	CGImageRelease(bmapImage);
	CGContextRelease(contextRef);
	CGColorSpaceRelease(csRef);
	free(argb);

    return image;
}

- (void) timerFired :(NSTimer*) timer
{
	NSImage *img = [ self imageBelowWindow: window withRect: [imageView frame] ];
	if ( img )
	{
		[ imageView setImage: img ];
		[ imageView setNeedsDisplay: YES];
	}
}

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
	timer = [ [ NSTimer scheduledTimerWithTimeInterval: (1.0 / 60.0)
	                                                    target: self
	                                                    selector: @selector(timerFired:)
	                                                    userInfo: nil
	                                                    repeats: YES ] retain];
}

- (void) applicationWillTerminate: (NSNotification*) notification
{
	[ timer release ];
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
