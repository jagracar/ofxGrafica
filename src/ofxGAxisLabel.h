#pragma once

#include "ofMain.h"

class ofxGAxisLabel {
public:
	ofxGAxisLabel();

protected:

	// General properties
	int type;
	array<float,2> dim;
	float relativePos;
	float plotPos;
	float offset;
	bool rotate;

	// Text properties
	string text;
	int textAlignment;
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;
};
