#pragma once

#include "ofMain.h"
#include "ofxGAxisLabel.h"

class ofxGAxis {
public:
	ofxGAxis();

	static const int X_AXIS = 0;
	static const int Y_AXIS = 1;
	static const int TOP_AXIS = 2;
	static const int RIGHT_AXIS = 3;

protected:

	// General properties
	int type;
	array<float, 2> dim;
	array<float, 2> lim;
	bool log;

	// Format properties
	float offset;
	ofColor lineColor;
	float lineWidth;

	// Ticks properties
	int nTicks;
	float ticksSeparation;
	vector<float> ticks;
	vector<float> plotTicks;
	vector<bool> ticksInside;
	vector<string> tickLabels;
	bool fixedTicks;
	float tickLength;
	float smallTickLength;
	bool expTickLabels;
	bool rotateTickLabels;
	bool drawTickLabels;
	float tickLabelOffset;

	// Label properties
	ofxGAxisLabel lab;
	bool drawAxisLabel;

	// Text properties
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;
};
