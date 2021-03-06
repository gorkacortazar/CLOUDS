//
//  CloudsSecondaryDisplayOSCSender.h
//  CloudsStoryEngine
//
//  Created by James George on 12/23/13.
//
//

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "CloudsEvents.h"
#include "ofxXmlSettings.h"
#include "CloudsGlobal.h"

class CloudsSecondaryDisplayOSCSender
{
  public:
	CloudsSecondaryDisplayOSCSender();
	
	void setup();
	
    void reset();
    
	void actCreated(CloudsActEventArgs& args);
	void actBegan(CloudsActEventArgs& args);
	void actEnded(CloudsActEventArgs& args);
	void clipBegan(CloudsClipEventArgs& args);
	void visualSystemBegan(CloudsVisualSystemEventArgs& args);
	void visualSystemEnded(CloudsVisualSystemEventArgs& args);
    void questionProposed(CloudsQuestionEventArgs& args);
    void questionSelected(CloudsQuestionEventArgs& args);
	void topicChanged(CloudsTopicEventArgs& newTopic);
    void preRollRequested(CloudsPreRollEventArgs& clip);
	
	void sendClip(CloudsClip* clip);
	string lastQuestionAsked;
	string currentTopic;

	bool bSetup;
	ofxOscSender sender;

};