//
//  CloudsInput.cpp
//  CLOUDS
//
//  Created by James George on 12/8/13.
//
//

#include "CloudsInput.h"
#include "CloudsInputEvents.h"
#include "CloudsInputMouse.h"

CloudsInput::CloudsInput(){
	enabled = false;
	events = new CloudsInputEvents();
	currentPosition = ofVec3f();
	lastPosition = ofVec3f();
	dragging = false;
}

CloudsInputEvents& CloudsInput::getEvents(){
	return *events;
}

map<int, CloudsInteractionEventArgs>& CloudsInput::getInputPoints(){
    return inputPoints;
}

void CloudsInput::interactionMoved(ofVec3f pos, bool primary, int actionType, int playerId){
	CloudsInteractionEventArgs args(pos, primary, actionType, playerId);
    inputPoints[playerId] = args;
	ofNotifyEvent(getEvents().interactionMoved, args, this);	
}

void CloudsInput::interactionStarted(ofVec3f pos, bool primary, int actionType, int playerId){
	CloudsInteractionEventArgs args(pos, primary, actionType, playerId);
    inputPoints[playerId] = args;
	ofNotifyEvent(getEvents().interactionStarted, args, this);
}

void CloudsInput::interactionDragged(ofVec3f pos, bool primary, int actionType, int playerId){
	CloudsInteractionEventArgs args(pos, primary, actionType, playerId);
    inputPoints[playerId] = args;
	ofNotifyEvent(getEvents().interactionDragged, args, this);
}

void CloudsInput::interactionEnded(ofVec3f pos, bool primary, int actionType, int playerId){	
	CloudsInteractionEventArgs args(pos, primary, actionType, playerId);
    inputPoints[playerId] = args;
	ofNotifyEvent(getEvents().interactionEnded, args, this);
}

bool CloudsInput::isDragging(){
	return dragging;
}
float CloudsInput::getPositionX(){
	return currentPosition.x;
}
float CloudsInput::getPositionY(){
	return currentPosition.y;
}
float CloudsInput::getPositionZ(){
	return currentPosition.z;
}
ofVec3f CloudsInput::getPosition(){
	return currentPosition;
}
ofVec3f CloudsInput::getPreviousPosition(){
	return currentPosition;
}

///////////// //STATICS
static ofPtr<CloudsInput> cloudsInput;
void SetCloudsInput(ofPtr<CloudsInput> input){
	
	if(cloudsInput != NULL)
		cloudsInput->disable();
	
	cloudsInput = input;
	
	if(cloudsInput != NULL)
		cloudsInput->enable();
}

ofPtr<CloudsInput> GetCloudsInput(){
	if(cloudsInput == NULL){
		SetCloudsInput( ofPtr<CloudsInput>( new CloudsInputMouse() ));
	}
	return cloudsInput;
}

//global accessors
bool GetCloudsInputPressed(){
	return GetCloudsInput()->isDragging();
}
float GetCloudsInputX(){
	return GetCloudsInput()->getPosition().x;
}
float GetCloudsInputY(){
	return GetCloudsInput()->getPosition().y;
}
float GetCloudsInputZ(){
	return GetCloudsInput()->getPosition().z;
}
ofVec3f GetCloudsInputPosition(){
	return GetCloudsInput()->getPosition();
}
map<int, CloudsInteractionEventArgs>& GetCloudsInputPoints(){
    return GetCloudsInput()->getInputPoints();
}

ofVec3f GetCloudsPreviousInputPosition(){
	return GetCloudsInput()->getPreviousPosition();
}
float GetCloudsPreviousInputX(){
	return GetCloudsPreviousInputPosition().x;
}
float GetCloudsPreviousInputY(){
	return GetCloudsPreviousInputPosition().y;
}
float GetCloudsPreviousInputZ(){
	return GetCloudsPreviousInputPosition().z;
}
