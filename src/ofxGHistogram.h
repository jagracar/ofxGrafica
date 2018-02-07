#pragma once

#include "ofMain.h"
#include "ofxGPoint.h"

class ofxGHistogram {
public:
	ofxGHistogram();

protected:

	// General properties
	int type;
	array<float, 2> dim;
	vector<ofxGPoint> plotPoints;
	bool visible;
	vector<float> separations;
	vector<ofColor> bgColors;
	vector<ofColor> lineColors;
	vector<float> lineWidths;
	vector<float> differences;
	vector<float> leftSides;
	vector<float> rightSides;

	// Labels properties
	float labelsOffset;
	bool drawLabels;
	bool rotateLabels;
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;

};
