//
//  CloudsVisualSystemLSys.cpp
//  VSLSys
//
//  Created by Patricio Gonzalez Vivo on 5/25/13.
//
//

#include "CloudsVisualSystemLSystem.h"

CloudsVisualSystemLSystem::CloudsVisualSystemLSystem(){
	selfSetDefaults();
}

string CloudsVisualSystemLSystem::getSystemName(){
	return "LSystem";
}

void CloudsVisualSystemLSystem::selfSetup(){
    lsysAngle = 5;
    lsysAxiom = "B";
    lsysRule1 = "B=F[5+B][7-B]-F[4+B][6-B]-[3+B][5+B]-FB";
    lsysRule2 = "F=FF";
    
    lsysGrowingSpeed = 1.0;
    lsysGrowingBorn = 2.0;
    
    lsysOriginal.setMode(OF_PRIMITIVE_LINES);
    lsysGrowing.setMode(OF_PRIMITIVE_LINES);
    
    postShader.load("",getVisualSystemDataPath()+"shaders/postprocess.fs");
    ofLoadImage(postTexture, getVisualSystemDataPath()+"images/3.jpg");
}

void CloudsVisualSystemLSystem::selfSetupGuis(){
    
}

void CloudsVisualSystemLSystem::selfAutoMode(){
    
}

void CloudsVisualSystemLSystem::selfBegin(){
    buildLSystem();
    buildGrid();
}

void CloudsVisualSystemLSystem::selfEnd(){
    
}

void CloudsVisualSystemLSystem::selfSetDefaults(){

	grid.clear();
    gridSize = 0;
    gridResolution = 0;
    gridCrossSize = 0;
    gridCrossWidth = 0;
    gridCrossAlpha = 0;
    
    lsysLines.clear();
    lsysNodes.clear();
    
    lsysOriginal.clear();
    lsysAngle = 0;
	lsysScale = 0;
	lsysDepth = 0;
    lsysOriginalAlpha = 0;
    
    lsysGrowing.clear();
    lsysGrowingBorn = 0;
	lsysGrowingSpeed = 0;
    lsysGrowingAlpha = 0;
    dots.clear();
    dotSize = 0;
	dotAlpha = 0;
    
    lsysFlowSpeed = 0;
    lsysFlowLenght = 0;
    lsysFlowAlpha = 0;
    
    postChromaDist = 0;
    postGrainDist = 0;
    
    time = 0;

    primaryCursorMode = CURSOR_MODE_CAMERA;
    secondaryCursorMode = CURSOR_MODE_INACTIVE;
}

void CloudsVisualSystemLSystem::selfSetupSystemGui(){
    sysGui->addLabel("Structure");
    ofxUITextInput *uiAxiom = sysGui->addTextInput("Axiom", "B", OFX_UI_FONT_SMALL);
    uiAxiom->setAutoClear(false);
    ofxUITextInput *uiRule1 = sysGui->addTextInput("Rule1", "B=F[5+B][7-B]-F[4+B][6-B]-[3+B][5+B]-FB", OFX_UI_FONT_SMALL);
    uiRule1->setAutoClear(false);
    ofxUITextInput *uiRule2 = sysGui->addTextInput("Rule2", "F=FF", OFX_UI_FONT_SMALL);
    uiRule2->setAutoClear(false);
    sysGui->addSlider("Angle", 0, 90, &lsysAngle);
    sysGui->addSlider("Scale", 0.5, 2, &lsysScale);
    sysGui->addSlider("Depth", 1, 10, &lsysDepth);
    
    sysGui->addLabel("Growing");
    sysGui->addSlider("Growing_speed", 0.0, 10, &lsysGrowingSpeed);
    sysGui->addSlider("Growing_Born", 0.0, 10.0, &lsysGrowingBorn);
    
    sysGui->addLabel("Flow");
    sysGui->addSlider("Flow_speed", 0.0, 10, &lsysFlowSpeed);
    
    
    lsysAxiom = uiAxiom->getTextString();
    lsysRule1 = uiRule1->getTextString();
    lsysRule2 = uiRule2->getTextString();
    
    buildLSystem();
}

void CloudsVisualSystemLSystem::selfSetupRenderGui(){
    rdrGui->addLabel("Grid");
    rdrGui->addSlider("Grid_Size", 0, 100, &gridSize);
    rdrGui->addSlider("Grid_Resolution", 0, 5, &gridResolution);
    rdrGui->addSlider("Grid_Cross_Size", 0.01, 0.1, &gridCrossSize);
    rdrGui->addSlider("Grid_Cross_Width", 0.0, 5.0, &gridCrossWidth);
    rdrGui->addSlider("Grid_Cross_Alpha", 0.0, 1.0, &gridCrossAlpha);
    
    rdrGui->addLabel("LSystem");
    rdrGui->addSlider("Oringinal_alpha", 0.0, 1.0, &lsysOriginalAlpha );
    rdrGui->addSlider("Growing_alpha", 0.0, 1.0, &lsysGrowingAlpha );
    rdrGui->addSlider("Dots_alpha", 0.0, 1.0, &dotAlpha);
    rdrGui->addSlider("Dots_size", 0.0, 10.0, &dotSize);
    rdrGui->addSlider("Flow_lenght", 0.0, 30.0, &lsysFlowLenght);
    rdrGui->addSlider("Flow_alpha", 0.0, 1.0, &lsysFlowAlpha);
    
    rdrGui->addLabel("Post-Process");
    rdrGui->addSlider("Chroma_Distortion", -0.25, 0.25, &postChromaDist);
    rdrGui->addSlider("Grain_Distortion", 0.0, 0.25, &postGrainDist);
    
    buildGrid();
}

void CloudsVisualSystemLSystem::buildGrid(){
    
    ofFloatColor crossColor = ofFloatColor(gridCrossAlpha);
    ofPoint normal = ofPoint(0,0.0,1.0);
    
    grid.clear();
    grid.setMode(OF_PRIMITIVE_LINES);
    for(int y = -gridSize; y < gridSize; y++){
        for(int x = -gridSize; x < gridSize; x++){
            grid.addColor(crossColor);
            grid.addNormal(normal);
            grid.addVertex(ofPoint(x*gridResolution-gridCrossSize,y*gridResolution));
            
            grid.addColor(crossColor);
            grid.addNormal(normal);
            grid.addVertex(ofPoint(x*gridResolution+gridCrossSize,y*gridResolution));
            
            grid.addColor(crossColor);
            grid.addNormal(normal);
            grid.addVertex(ofPoint(x*gridResolution,y*gridResolution-gridCrossSize));
            
            grid.addColor(crossColor);
            grid.addNormal(normal);
            grid.addVertex(ofPoint(x*gridResolution,y*gridResolution+gridCrossSize));
        }
    }
}

void CloudsVisualSystemLSystem::buildLSystem(){
    //  Clear
    //
    lsysLines.clear();
    lsysLines.push_back(ofPolyline());
    lsysNodes.clear();
    lsysOriginal.clear();
    
    LSystem sys;
    sys.initialPos.set(0,0);
    sys.unoise = 0.0;
    sys.utime = 0.0;
    
    sys.ds = lsysScale;
    sys.setAngle( lsysAngle );
    sys.addAxion( lsysAxiom );
    sys.addRule( lsysRule1 );
    if (lsysRule2.size() > 0){
        sys.addRule( lsysRule2);
    }

    sys.make( lsysDepth );
    
    lsysOriginal = sys.mesh;
    
    if ( lsysOriginal.getVertices().size() > 2){
        lineTo( lsysOriginal.getVertices()[0] );
        addNode( lsysOriginal.getVertices()[0] );
        
        for (int i = 1; i < lsysOriginal.getVertices().size();i++){
            if ( i%2 == 1){
                lineTo( lsysOriginal.getVertices()[i]);
            } else {
                if ( lsysOriginal.getVertices()[i] != lsysOriginal.getVertices()[i-1]){
                    lsysLines.push_back(ofPolyline());
                    lineTo( lsysOriginal.getVertices()[i] );
                    addNode( lsysOriginal.getVertices()[i] );
                }
            }
        }
    }
    
    if ( lsysNodes.size() > 0){
        lsysNodes[0].startTime = 0;
    }
}


void CloudsVisualSystemLSystem::lineTo(ofPoint &_pnt){
    lsysLines[ lsysLines.size()-1 ].addVertex(_pnt);
}

void CloudsVisualSystemLSystem::addNode(ofPoint &_pnt){
    
    vector<ofPoint> blank;
    
    for (int i = lsysNodes.size()-1; i >=0; i--) {
        if ( lsysNodes[i] == _pnt ){
            lsysNodes[i].branchesIndex.push_back( lsysLines.size()-1 );
            lsysNodes[i].trails.push_back( blank );
            return;
        }
    }
    
    LNode node;
    node.set(_pnt);
    node.startTime = -1.0;
    node.branchesIndex.push_back( lsysLines.size()-1 );
    node.trailsPct = -1.0;
    node.trails.push_back( blank );
    
    lsysNodes.push_back( node );
}

void CloudsVisualSystemLSystem::addBranch(ofMesh &_mesh, int _index, float _relativeTime, float _speed){
    
    int totalPoints = lsysLines[_index].size();
    int drawPoints = 0;
    
    for (int k = 0 ; k < totalPoints-1; k++){
        float thisTime = _speed*(float)k;
        float nextTime = _speed*((float)k+1.0f);
        
        if ( k == totalPoints-1){
            nextTime = thisTime;
        }
        
        if (_relativeTime > thisTime && _relativeTime < nextTime ){
            float part = _relativeTime - thisTime;
            float whole = nextTime - thisTime;
            float pct = part / whole;
            
            ofPoint A = lsysLines[ _index ][k];
            ofPoint B = lsysLines[ _index ][k+1];
            
            // figure out where we are between a and b
            //
            ofPoint pos = (1.0-pct)*A + (pct)*B;
            
            dots.push_back(pos);
            
            _mesh.addVertex(lsysLines[ _index ][k]);
            _mesh.addVertex(pos);
            
        } else if ( _relativeTime > thisTime ){
            ofPoint pos = lsysLines[ _index ][k];
            
            _mesh.addVertex(pos);
            _mesh.addVertex(lsysLines[_index][k+1]);
            
            //  check if pass over a node
            //
            int index = isNode(pos);
            if (index != -1){
                if ( lsysNodes[index].startTime == -1.0 ){
                    lsysNodes[index].startTime = time +lsysGrowingBorn*_speed;
                }
            }
            
        } else {
            break;
        }
    }
}

int CloudsVisualSystemLSystem::isNode(ofPoint &_pnt){
    for (int i = lsysNodes.size()-1; i >= 0; i--) {
        if ( lsysNodes[i] == _pnt ){
            return i;
        }
    }
    
    return -1;
}

void CloudsVisualSystemLSystem::selfUpdate(){

    time = timeline->getCurrentTime()*lsysGrowingSpeed;
    
    dots.clear();
    
    lsysGrowing.clear();
    for(int i = 0; i < lsysNodes.size(); i++){
                
        if (lsysNodes[i].startTime >= 0.0){
            float relativeTime = time - lsysNodes[i].startTime;
            
            for (int j = 0; j < lsysNodes[i].branchesIndex.size(); j++){
                addBranch( lsysGrowing, lsysNodes[i].branchesIndex[j], relativeTime, ofNoise(lsysNodes[i].x+time*0.01,lsysNodes[i].y));
            }
        }
    }
    
    if ( lsysFlowSpeed > 0.0 ){
        for(int i = 0; i < lsysNodes.size(); i++){
            
            if (lsysNodes[i].trailsPct >= 0.0){
                
                if (lsysNodes[i].startTime < 0.0){
                    break;
                }
                
                for (int j = 0; j < lsysNodes[i].branchesIndex.size(); j++){
                    
                    int index = lsysNodes[i].branchesIndex[j];
                    int totalPoints = lsysLines[ index ].size();
                    
                    float line = (totalPoints-1)*lsysNodes[i].trailsPct;
                    int k = line;
                    
                    float pct = line-(int)(line);
                    
                    ofPoint A = lsysLines[ index ][k];
                    ofPoint B = lsysLines[ index ][k+1];
                    ofPoint pos = (1.0-pct)*A + (pct)*B;
                    
                    lsysNodes[i].trails[j].push_back(pos);
                    while (lsysNodes[i].trails[j].size() > lsysFlowLenght) {
                        lsysNodes[i].trails[j].erase(lsysNodes[i].trails[j].begin());
                    }
                }
                
                lsysNodes[i].trailsPct += lsysFlowSpeed*ofNoise(ofGetElapsedTimef()*0.01)*0.01;
                
                if(lsysNodes[i].trailsPct > 1.0){
                    lsysNodes[i].trailsPct = -1.0;
                    for (int j = 0; j < lsysNodes[i].trails.size(); j++){
                        lsysNodes[i].trails[j].clear();
                    }
                }
            } else {
                for (int j = 0; j < lsysNodes[i].trails.size(); j++){
                    lsysNodes[i].trails[j].clear();
                }
                lsysNodes[i].trailsPct = ofRandom(-10.0,0.1);
            }
        }
    }
}

void CloudsVisualSystemLSystem::selfDrawBackground()
{
}

void CloudsVisualSystemLSystem::selfDraw(){
//    mat->begin();
    ofDisableDepthTest();
    
    ofPushMatrix();
    ofPushStyle();
    
    //  Grid
    //
	if (gridCrossWidth > 0) {
		ofPushStyle();
		//ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofSetLineWidth(gridCrossWidth);
		grid.draw();
		ofPopStyle();
	}
    
	//  Original L-System
    //
    ofEnableAlphaBlending();
    ofSetColor(255, lsysOriginalAlpha*255);
    lsysOriginal.draw();
    
    //  Dots
    //
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255, dotAlpha*255);
    for(int i = 0; i < dots.size(); i++){
        glPointSize(dotSize);
        glBegin(GL_POINTS);
        glVertex3f(dots[i].x,dots[i].y,dots[i].z);
        glEnd();
    }
    
    ofDisableBlendMode();
    ofEnableAlphaBlending();
    for (int i = 0; i < lsysNodes.size(); i++) {
        if ( lsysNodes[i].trailsPct > 0.1 && lsysNodes[i].trailsPct < 0.9){
            for(int j = 0; j < lsysNodes[i].trails.size(); j++){
                ofMesh mesh;
                mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
                for(int k = 0; k < lsysNodes[i].trails[j].size(); k++){
                    float alpha = ofMap(k, 0,lsysNodes[i].trails[j].size(), 0.0, lsysFlowAlpha);
                    mesh.addColor( ofFloatColor( 1.0, alpha) );
                    mesh.addVertex(lsysNodes[i].trails[j][k]);
                }
                ofFill();
                ofSetColor( 255 );
                mesh.draw();
            }
        }
    }

    //  Growing Trace
    //
    ofSetColor(255, lsysGrowingAlpha*255);
    lsysGrowing.draw();

    ofPopStyle();
    ofPopMatrix();
    
//    mat->end();
}


void CloudsVisualSystemLSystem::selfPostDraw(){
	
    postShader.begin();
    postShader.setUniformTexture("tex1", postTexture, 1);
    //MA: changed ofGetWidth() to getCanvasWidth() and ofGetHeight() to getCanvasHeight()
    postShader.setUniform2f("resolution", getCanvasWidth(), getCanvasHeight());
    postShader.setUniform2f("textureResolution", postTexture.getWidth(), postTexture.getHeight());
    postShader.setUniform1f("chroma", postChromaDist);
    postShader.setUniform1f("grain", postGrainDist);
    CloudsVisualSystem::selfPostDraw();
    postShader.end();
}

void CloudsVisualSystemLSystem::guiSystemEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if ( name == "Axiom"){
        ofxUITextInput *uiAxiom = (ofxUITextInput *) e.widget;
        lsysAxiom = uiAxiom->getTextString();
        buildLSystem();
    } else if ( name == "Rule1"){
        ofxUITextInput *uiRule1 = (ofxUITextInput *) e.widget;
        lsysRule1 = uiRule1->getTextString();
        buildLSystem();
    } else if ( name == "Rule2"){
        ofxUITextInput *uiRule2 = (ofxUITextInput *) e.widget;
        lsysRule2 = uiRule2->getTextString();
        buildLSystem();
    } else if ( name == "Angle" || name == "Depth" || name == "Scale" ){
        buildLSystem();
    } 
}

void CloudsVisualSystemLSystem::guiRenderEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    
    if ( name.find("Grid") == 0){
        buildGrid();
    }

}

void CloudsVisualSystemLSystem::selfExit()
{
    
}

void CloudsVisualSystemLSystem::selfKeyPressed(ofKeyEventArgs & args)
{
    
}

void CloudsVisualSystemLSystem::selfKeyReleased(ofKeyEventArgs & args)
{
    
}

void CloudsVisualSystemLSystem::selfMouseDragged(ofMouseEventArgs& data)
{
    
}

void CloudsVisualSystemLSystem::selfMouseMoved(ofMouseEventArgs& data)
{
    
}

void CloudsVisualSystemLSystem::selfMousePressed(ofMouseEventArgs& data)
{
    
}

void CloudsVisualSystemLSystem::selfMouseReleased(ofMouseEventArgs& data)
{
    
}

void CloudsVisualSystemLSystem::selfSetupGui()
{
      
}

void CloudsVisualSystemLSystem::selfGuiEvent(ofxUIEventArgs &e)
{
    
}

void CloudsVisualSystemLSystem::selfDrawDebug()
{
    
}

void CloudsVisualSystemLSystem::selfSceneTransformation()
{
    
}
