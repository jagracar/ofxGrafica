#pragma once

#include "ofMain.h"
#include "ofxGConstants.h"

class ofxGAxisLabel {
public:

	ofxGAxisLabel() = default;
	ofxGAxisLabel(int _type, const array<float, 2> &_dim);

protected:

	// General properties
	int type;
	array<float, 2> dim;
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
