//
//  CloudsGlobal.h
//  CloudsStoryEngine
//
//  Created by James George on 7/11/13.
//
//

#include "ofMain.h"
#include "CloudsLocalization.h"
#include "CloudsGlobal.h"
#include "ofxLocalization.h"


//--------------------------------------
static ofxLocalization localization;
static bool localizationLoaded = false;
bool InitLocalization(){
	localizationLoaded = true;
	return localization.load(GetCloudsDataPath() + "language/languagefile.csv");
}

//--------------------------------------
string GetTranslationForString(string toTranslate){
	if(!localizationLoaded){
		InitLocalization();
	}
	if(GetLanguage() == "ENGLISH"){
		return toTranslate;
	}
	else{
		return localization.translateKeyToLanguage(ofToUpper(toTranslate), GetLanguage())  + "\n" + localization.translateKeyToLanguage(ofToUpper(toTranslate), "ENGLISH");
	}
}

string GetThinFontPath(){
	if(GetLanguage() == "JAPANESE"){
		return GetCloudsDataPath() + "font/AxisStd-Regular.otf";
	}
	else{
		return GetCloudsDataPath() + "font/Blender-THIN.ttf";
	}
}
//--------------------------------------
string GetMediumFontPath(){
	if(GetLanguage() == "JAPANESE"){
		return GetCloudsDataPath() + "font/AxisStd-Regular.otf";
	}
	else{
		return 	GetCloudsDataPath() + "font/Blender-MEDIUM.ttf";
	}
}

string GetFontPath(){
	if(GetLanguage() == "JAPANESE"){
		return GetCloudsDataPath() + "font/AxisStd-Regular.otf";
	}
	else{
		return 	GetCloudsDataPath() + "font/Blender-BOOK.ttf";
	}
}

string utf8_substr(string originalString, int maxLength)
{
    std::string resultString = originalString;

    int len = 0;
    int byteCount = 0;

    const char* aStr = originalString.c_str();

    while(*aStr)
    {
        if( (*aStr & 0xc0) != 0x80 )
            len += 1;

        if(len>maxLength)
        {
            resultString = resultString.substr(0, byteCount);
            break;
        }
        byteCount++;
        aStr++;
    }

    return resultString;
}
