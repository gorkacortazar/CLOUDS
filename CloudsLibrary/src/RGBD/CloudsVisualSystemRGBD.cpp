
#include "CloudsVisualSystemRGBD.h"

//--------------------------------------------------------------
string CloudsVisualSystemRGBD::getSystemName(){
	return "RGBD";
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::selfSetup(){
	currentFlowPosition = 0;
	
	drawMesh = false;
	
	generatePointGrid();
	generateScanlines();
	connectionGenerator.numParticles = 200;
	connectionGenerator.setMinDistance(40);
	connectionGenerator.setup();
	
	particulateController.setParticleCount(1e5);
	particulateController.setShaderDirectory(getDataPath() + "shaders/GPUParticles/");
	particulateController.setup();
	
	cloudsCamera.setup();
	cloudsCamera.lookTarget = ofVec3f(0,25,0);
	setCurrentCamera(cloudsCamera);
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::selfSetupGuis(){
	
	meshGui = new ofxUISuperCanvas("MESH", gui);
	meshGui->copyCanvasStyle(gui);
    meshGui->copyCanvasProperties(gui);
    meshGui->setName("Mesh");
    meshGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	meshGui->addSlider("CLOUD SCALE", .001,  1.0, &pointcloudScale);
	meshGui->addSlider("CLOUD OFFSET",   0, -800, &pointcloudOffsetZ);
	
	meshGui->addLabel("MESH");
	meshGui->addToggle("DRAW MESH", &drawMesh);
	meshGui->addSlider("MESH ALPHA", 0, 1.0f, &meshAlpha);
	meshGui->addSlider("SKIN MULTIPLIER", 0, 1.0f, &skinMultiplier);
	meshGui->addSlider("EYE MULTIPLIER", 0, 1.0f, &eyeMultiplier);
	
	meshGui->addLabel("POINTS");
	meshGui->addToggle("DRAW POINTS", &drawPoints);
	meshGui->addSlider("SIZE MAX", 0, 3, &pointSizeMax);
	meshGui->addSlider("SIZE MIN", 0, 3, &pointSizeMin);
	meshGui->addSpacer();
	meshGui->addSlider("P GRID ALPHA", 0, 1.0, &pointGridAlpha);
	meshGui->addSlider("P GRID VERT SPACE", 1, 10, &pointVerticalSpace);
	meshGui->addSlider("P GRID HORI SPACE", 1, 10, &pointHorizontalSpace);
	meshGui->addSpacer();
	meshGui->addSlider("NUM RANDOM POINTS", 0, 500000.0f, &numRandomPoints);
	meshGui->addSlider("RANDOM ALPHA", 0, 1.0f, &randomPointAlpha);
	
	meshGui->addLabel("LINES");
	meshGui->addToggle("DRAW LINES", &drawScanlines);
	meshGui->addSlider("VERT LINE SPACE", .5, 12, &scanlineSimplify.x);
	meshGui->addSlider("VERT LINE ALPHA", 0, 1.0, &verticalScanlineAlpha);
	meshGui->addSlider("VERT LINE THICKNESS", 0, 2.0, &verticalScanlineThickness);
	meshGui->addSpacer();
	meshGui->addSlider("HORIZ LINE SPACE", .5, 12, &scanlineSimplify.y);
	meshGui->addSlider("HORIZ LINE ALPHA", 0, 1.0, &horizontalScanlineAlpha);
	meshGui->addSlider("HORIZ LINE THICKNESS", 0, 2.0, &horizontalScanlineThickness);
	
	meshGui->addLabel("FLOW");
	meshGui->addSlider("CLOUD FLOW", -5, 5, &cloudFlow);
	ofAddListener(meshGui->newGUIEvent, this, &CloudsVisualSystemRGBD::selfGuiEvent);
	
	guis.push_back(meshGui);
	guimap[meshGui->getName()] = meshGui;

	
	cameraGui = new ofxUISuperCanvas("CAMERA", gui);
	cameraGui->copyCanvasStyle(gui);
    cameraGui->copyCanvasProperties(gui);
    cameraGui->setName("Camera");
    cameraGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	cameraGui->addLabel("OFFSETS");
	cameraGui->addSlider("FRONT DISTANCE", 50, 200, &cloudsCamera.frontDistance);
	cameraGui->addSlider("SIDE DISTANCE", 20, 200, &cloudsCamera.sideDistance);
	cameraGui->addSlider("SIDE PULLBACK", -200, 200, &cloudsCamera.sidePullback);
	cameraGui->addSlider("LIFT RANGE", 0, 100, &cloudsCamera.liftRange);
	cameraGui->addSlider("LIFT AMOUNT", 10, 200, &cloudsCamera.liftAmount);
	cameraGui->addSlider("DROP AMOUNT", 0, 200, &cloudsCamera.dropAmount);
	
	guis.push_back(cameraGui);
	guimap[meshGui->getName()] = cameraGui;
	
	particleGui = new ofxUISuperCanvas("PARTICLE", gui);
	particleGui->copyCanvasStyle(gui);
    particleGui->copyCanvasProperties(gui);
    particleGui->setName("Particle");
    particleGui->setWidgetFontSize(OFX_UI_FONT_SMALL);
	
	particleGui->addSlider("BIRTH RATE", 0, .01, &particulateController.birthRate);
	particleGui->addSlider("BIRTH SPREAD", 10, 10000, &particulateController.birthSpread);
	
	guis.push_back(particleGui);
	guimap[meshGui->getName()] = particleGui;
	
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::selfUpdate(){
	if(refreshScanlineMesh){
		generateScanlines();
	}
	if(refreshPointcloud){
		generatePointGrid();
	}
	if(numRandomPoints != randomPoints.getNumVertices()){
		generateRandomPoints();
	}
	

	currentFlowPosition += cloudFlow;
	sharedRenderer->flowPosition = currentFlowPosition;
	translatedHeadPosition = sharedRenderer->headPosition*pointcloudScale + ofVec3f(0,0,pointcloudOffsetZ);
	cloudsCamera.lookTarget = translatedHeadPosition;
	
	particulateController.birthPlace = translatedHeadPosition;
	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	particulateController.update();
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::generatePointGrid(){
	
	pointGrid.clear();
	
	pointVerticalSpace = MAX(pointVerticalSpace, 1);
	pointHorizontalSpace = MAX(pointHorizontalSpace, 1);
	for(float y = 0; y <= 480; y+= pointVerticalSpace){
		for(float x = 0; x <= 640; x+= pointHorizontalSpace){
			pointGrid.addVertex(ofVec3f(x,y,0));
		}
	}
	
	pointGrid.clearIndices();
	int x = 0;
	int y = 0;
	
	int gw = ceil(640 / pointHorizontalSpace);
	int w = gw*pointHorizontalSpace;
	int h = 480;
	
	for (float ystep = 0; ystep < h-pointVerticalSpace; ystep += pointVerticalSpace){
		for (float xstep = 0; xstep < w-pointHorizontalSpace; xstep += pointHorizontalSpace){
			ofIndexType a,b,c;
			
			a = x+y*gw;
			b = (x+1)+y*gw;
			c = x+(y+1)*gw;
			pointGrid.addIndex(a);
			pointGrid.addIndex(b);
			pointGrid.addIndex(c);
            
			a = (x+1)+(y+1)*gw;
			b = x+(y+1)*gw;
			c = (x+1)+(y)*gw;
			pointGrid.addIndex(a);
			pointGrid.addIndex(b);
			pointGrid.addIndex(c);
			
			x++;
		}
		
		y++;
		x = 0;
	}

//	pointGrid.setMode(OF_PRIMITIVE_POINTS);
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::generateRandomPoints(){
	if(numRandomPoints == 0){
		randomPoints.clear();
	}
	else if(numRandomPoints < randomPoints.getNumVertices() ){
		randomPoints.getVertices().erase(randomPoints.getVertices().begin(), randomPoints.getVertices().begin() + (randomPoints.getNumVertices() - numRandomPoints) );
	}
	while(numRandomPoints > randomPoints.getNumVertices()){
		randomPoints.addVertex( ofVec3f(ofRandom(640),ofRandom(480),0) );
	}
	
	randomPoints.setMode(OF_PRIMITIVE_POINTS);
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::generateScanlines(){

	verticalScanLines.clear();
	horizontalScanLines.clear();
	
	if(scanlineSimplify.x <= 0) scanlineSimplify.x = 1;
	if(scanlineSimplify.y <= 0) scanlineSimplify.y = 1;
	
	int height = 480;
	int width = 640;
	
	//VERTICAL
	for (float xstep = 0; xstep <= width-scanlineSimplify.x; xstep += scanlineSimplify.x){
		for (float ystep = 0; ystep <= height-scanlineSimplify.y; ystep += scanlineSimplify.y){
			verticalScanLines.addVertex( ofVec3f(xstep, ystep, 0) );
			verticalScanLines.addVertex( ofVec3f(xstep, ystep+scanlineSimplify.y, 0) );
		}
	}
	
	//HORIZONTAL
	for (float ystep = 0; ystep <= height-scanlineSimplify.y; ystep += scanlineSimplify.y){
		for (float xstep = 0; xstep <= width-scanlineSimplify.x; xstep += scanlineSimplify.x){
			
			float ystepOffset = ofRandom(-scanlineSimplify.y/4,scanlineSimplify.y/4);
			
			horizontalScanLines.addColor(ofFloatColor(ofRandom(1.)));
			horizontalScanLines.addVertex( ofVec3f(xstep, ystep+ystepOffset, 0) );
			
			horizontalScanLines.addColor(ofFloatColor(ofRandom(1.)));
			horizontalScanLines.addVertex( ofVec3f(xstep+scanlineSimplify.x, ystep+ystepOffset, 0) );
		}
	}

	verticalScanLines.setMode( OF_PRIMITIVE_LINES );
	horizontalScanLines.setMode( OF_PRIMITIVE_LINES );
	
	refreshScanlineMesh = false;
}

void CloudsVisualSystemRGBD::selfDrawBackground(){
	
}

void CloudsVisualSystemRGBD::selfDrawDebug(){
	ofDrawSphere(translatedHeadPosition, 10);
}

void CloudsVisualSystemRGBD::selfSceneTransformation(){

}

void CloudsVisualSystemRGBD::selfDraw(){
	
	if(sharedRenderer != NULL && hasSpeaker){

//		cout << "RGBD DRAW" << endl;
		
		ofPushStyle();
		ofPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_LIGHTING);
		
		//Enable smooth lines and screen blending
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);	// allows per-point size
		glEnable(GL_POINT_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glEnable(GL_LINE_SMOOTH);
		

		ofEnableAlphaBlending();
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		
		//move the pointcloud
		ofTranslate(0,0,pointcloudOffsetZ);
		ofScale(pointcloudScale,pointcloudScale,pointcloudScale);
		
		sharedRenderer->bindRenderer();
		sharedRenderer->getShader().setUniform1f("baseMultiplier", 1.0);
		
		//set up the renderer so that any geometry within 640x480 space
		//can be prjected onto the pointcloud
		if(drawMesh){

			sharedRenderer->getShader().setUniform1f("flowPosition", 0);
			sharedRenderer->getShader().setUniform1f("eyeMultiplier", eyeMultiplier);
			sharedRenderer->getShader().setUniform1f("skinMultiplier", skinMultiplier);
			sharedRenderer->getShader().setUniform1f("baseMultiplier", meshAlpha);
			
			sharedRenderer->setSimplification(ofVec2f(pointHorizontalSpace, pointVerticalSpace));
			
			ofPushStyle();
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);

			pointGrid.draw();
			ofTranslate(0,0,-3);
			
			ofPopStyle();
			
			sharedRenderer->getShader().setUniform1f("eyeMultiplier", 0);
			sharedRenderer->getShader().setUniform1f("skinMultiplier", 0);
			sharedRenderer->getShader().setUniform1f("baseMultiplier", 1.0);
			
		}
		
		glDisable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		
		sharedRenderer->getShader().setUniform1f("flowPosition", currentFlowPosition);

		if(drawPoints){
			//draw the points
			glPointSize(pointSizeMin);
			ofSetColor(255*randomPointAlpha);
			randomPoints.draw();
			ofSetColor(255*pointGridAlpha);
			pointGrid.drawVertices();
		}
		
		sharedRenderer->setSimplification(scanlineSimplify);

		//draw the lines
		if(drawScanlines){
			
			ofSetColor(255*horizontalScanlineAlpha);
			ofSetLineWidth(horizontalScanlineThickness);
			horizontalScanLines.draw();

			sharedRenderer->getShader().setUniform1f("flowPosition", 0);
			ofSetLineWidth(verticalScanlineThickness);
			ofSetColor(255*verticalScanlineAlpha);
			verticalScanLines.draw();
		
		}
		
		sharedRenderer->getShader().setUniform1f("flowPosition", 0);
//		connectionGenerator.draw();

		sharedRenderer->unbindRenderer();
				
		
		glPopAttrib();
		ofPopMatrix();
		ofPopStyle();
		
		glEnable(GL_DEPTH_TEST);
		particulateController.draw();

	}
}

void CloudsVisualSystemRGBD::selfExit(){
	
}

void CloudsVisualSystemRGBD::selfBegin(){

}

void CloudsVisualSystemRGBD::selfEnd(){
	
}

void CloudsVisualSystemRGBD::selfKeyPressed(ofKeyEventArgs & args){
	if(args.key == 'R'){
		particulateController.reloadShaders();
	}
}

void CloudsVisualSystemRGBD::selfKeyReleased(ofKeyEventArgs & args){
	
}

void CloudsVisualSystemRGBD::selfMouseDragged(ofMouseEventArgs& data){
	
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::selfMouseMoved(ofMouseEventArgs& data){
	
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::selfMousePressed(ofMouseEventArgs& data){
	
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::selfMouseReleased(ofMouseEventArgs& data){

}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::selfSetupGui(){
	
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::selfGuiEvent(ofxUIEventArgs &e){
	if(e.widget->getName() == "VERT LINE SPACE"){
		refreshScanlineMesh = true;
	}
	else if(e.widget->getName() == "HORIZ LINE SPACE"){
		refreshScanlineMesh = true;
	}
	else if(e.widget->getName() == "P GRID VERT SPACE"){
		refreshPointcloud = true;
	}
	else if(e.widget->getName() == "P GRID HORI SPACE"){
		refreshPointcloud = true;
	}

}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::selfSetupSystemGui(){
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::guiSystemEvent(ofxUIEventArgs &e){
}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::selfSetupRenderGui(){
	

}

//--------------------------------------------------------------
void CloudsVisualSystemRGBD::guiRenderEvent(ofxUIEventArgs &e){
	
//	cout << "GUI EVENT WITH WIDGET " << e.widget->getName();
	

}
