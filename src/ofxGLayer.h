#pragma once

#include "ofMain.h"
#include "ofxGPoint.h"
#include "ofxGHistogram.h"

class ofxGLayer {
public:
	ofxGLayer();

protected:

	// General properties
	string id;
	array<float, 2> dim;
	array<float, 2> xLim;
	array<float, 2> yLim;
	bool xLog;
	bool yLog;

	// Points properties
	vector<ofxGPoint> points;
	vector<ofxGPoint> plotPoints;
	vector<bool> inside;
	vector<ofColor> pointColors;
	vector<float> pointSizes;

	// Line properties
	ofColor lineColor;
	float lineWidth;

	// Histogram properties
	ofxGHistogram hist;
	ofxGPoint histBasePoint;

	// Labels properties
	ofColor labelBgColor;
	vector<float> labelSeparation;
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;

	// Helper variable
	array<array<float, 2>, 4> cuts;
};
