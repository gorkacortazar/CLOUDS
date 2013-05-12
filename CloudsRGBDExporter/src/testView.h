#pragma once

#include "ofMain.h"
#include "ofxCocoaGLView.h"
#include "CloudsFCPParser.h"
#include "ofxRGBDGPURenderer.h"
#include "ofxRGBDPlayer.h"

@interface testView : ofxCocoaGLView {
	
	IBOutlet NSTableView* clipTable;
	
	CloudsFCPParser parser;
	ofxRGBDPlayer player;
	ofxRGBDGPURenderer renderer;
	
}

- (void)setup;
- (void)update;
- (void)draw;
- (void)exit;

- (void)keyPressed:(int)key;
- (void)keyReleased:(int)key;
- (void)mouseMoved:(NSPoint)p;
- (void)mouseDragged:(NSPoint)p button:(int)button;
- (void)mousePressed:(NSPoint)p button:(int)button;
- (void)mouseReleased:(NSPoint)p button:(int)button;
- (void)windowResized:(NSSize)size;


@end
