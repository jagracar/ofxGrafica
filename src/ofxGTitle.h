#pragma once

#include "ofMain.h"

class ofxGTitle {
public:
	ofxGTitle();
	//ofxGTitle(array<float, 2> _dim = {0, 0});

protected:

	// General properties
	array<float, 2> dim;
	float relativePos;
	float plotPos;
	float offset;

	// Text properties
	string text;
	int textAlignment;
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;
};
