//
//  CloudsVisualSystemRandomDigits.cpp
//

#include "CloudsVisualSystemRandomDigits.h"

//#include "CloudsRGBDVideoPlayer.h"
//#ifdef AVF_PLAYER
//#include "ofxAVFVideoPlayer.h"
//#endif

//These methods let us add custom GUI parameters and respond to their events
void CloudsVisualSystemRandomDigits::selfSetupGui(){

    
    ofGetRectMode() == OF_RECTMODE_CENTER;
    /*
	customGui = new ofxUISuperCanvas("CUSTOM", gui);
	customGui->copyCanvasStyle(gui);
	customGui->copyCanvasProperties(gui);
	customGui->setName("Custom");
	customGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	
	ofAddListener(customGui->newGUIEvent, this, &CloudsVisualSystemRandomDigits::selfGuiEvent);
	guis.push_back(customGui);
	guimap[customGui->getName()] = customGui;
     */
}

void CloudsVisualSystemRandomDigits::selfGuiEvent(ofxUIEventArgs &e){
//	if(e.widget->getName() == "Custom Button"){
//		cout << "Button pressed!" << endl;
//	}
}

//Use system gui for global or logical settings, for exmpl
void CloudsVisualSystemRandomDigits::selfSetupSystemGui(){
	
}

void CloudsVisualSystemRandomDigits::guiSystemEvent(ofxUIEventArgs &e){
	
}
//use render gui for display settings, like changing colors
void CloudsVisualSystemRandomDigits::selfSetupRenderGui(){

}

void CloudsVisualSystemRandomDigits::guiRenderEvent(ofxUIEventArgs &e){
	
}

// selfSetup is called when the visual system is first instantiated
// This will be called during a "loading" screen, so any big images or
// geometry should be loaded here
void CloudsVisualSystemRandomDigits::selfSetup(){
	
    glEnable(GL_DEPTH_TEST);
    ofBackground(0);
    //ofSetFrameRate(30);
    Rand::Font.loadFont(getVisualSystemDataPath() + "Courier.ttf", 14, true, true, true);

    //int IDnumber = 0;
    
    for (int i = 0; i < num-1; i++){
        for (int j = 0; j < num-1; j ++){ //rows
    grid.push_back(Rand(j*xoffset, i*yoffset, 0));
          //  grid.back().ID = IDnumber + 1;
        }
    }

}


// selfPresetLoaded is called whenever a new preset is triggered
// it'll be called right before selfBegin() and you may wish to
// refresh anything that a preset may offset, such as stored colors or particles
void CloudsVisualSystemRandomDigits::selfPresetLoaded(string presetPath){

}

// selfBegin is called when the system is ready to be shown
// this is a good time to prepare for transitions
// but try to keep it light weight as to not cause stuttering
void CloudsVisualSystemRandomDigits::selfBegin(){
	
}

//do things like ofRotate/ofTranslate here
//any type of transformation that doesn't have to do with the camera
void CloudsVisualSystemRandomDigits::selfSceneTransformation(){
	
}

//normal update call
void CloudsVisualSystemRandomDigits::selfUpdate(){
    

}

// selfDraw draws in 3D using the default ofEasyCamera
// you can change the camera by returning getCameraRef()
void CloudsVisualSystemRandomDigits::selfDraw(){
    
    ofBackground(0);
    
    
    cam.begin();

            //ofDrawBox(10);
            //ofDrawAxis(20)

    for (int i = 0; i < grid.size(); i++){

            ofPushStyle();
            ofPushMatrix();
        
            //noiseRotation
            sleep(50);
            grid[i].drawNumbers();
            sleep(50);
       
            ofPopMatrix();
            ofPopStyle();
    }
    cam.end();

}

 
// draw any debug stuff here
void CloudsVisualSystemRandomDigits::selfDrawDebug(){

}

// or you can use selfDrawBackground to do 2D drawings that don't use the 3D camera
void CloudsVisualSystemRandomDigits::selfDrawBackground(){

	//turn the background refresh off
	//bClearBackground = false;
	
}
// this is called when your system is no longer drawing.
// Right after this selfUpdate() and selfDraw() won't be called any more
void CloudsVisualSystemRandomDigits::selfEnd(){

	
}
// this is called when you should clear all the memory and delet anything you made in setup
void CloudsVisualSystemRandomDigits::selfExit(){
	
}

//events are called when the system is active
//Feel free to make things interactive for you, and for the user!
void CloudsVisualSystemRandomDigits::selfKeyPressed(ofKeyEventArgs & args){
	
}
void CloudsVisualSystemRandomDigits::selfKeyReleased(ofKeyEventArgs & args){
	
}

void CloudsVisualSystemRandomDigits::selfMouseDragged(ofMouseEventArgs& data){
    
    
	
}


void CloudsVisualSystemRandomDigits::selfMouseMoved(ofMouseEventArgs& data){
    
    float x = ofMap(data.x, 0, ofGetWidth(), -(ofGetWidth()/2.0), (ofGetWidth()/2.0));
    float y = ofMap(data.y, 0, ofGetHeight(), -(ofGetHeight()/2.0), (ofGetHeight()/2.0));
    
    ofPushStyle();
    //ofSetColor(255,255, 255);
    //ofFill();
    ofRect(x, y, 20, 20);
    ofPopStyle();
	
}

void CloudsVisualSystemRandomDigits::selfMousePressed(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemRandomDigits::selfMouseReleased(ofMouseEventArgs& data){
	
}

void CloudsVisualSystemRandomDigits::sleep(long d){
 
    
        clock_t start=clock();
        while(clock() - start < d); ///loop until time's up
    

}
