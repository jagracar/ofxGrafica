#pragma once

#include "ofMain.h"
#include "ofxGConstants.h"
#include "ofxGAxisLabel.h"

class ofxGAxis {
public:

	ofxGAxis() = default;
	ofxGAxis(int _type, array<float,2> _dim, array<float,2> _lim, bool _log);
	void draw();

protected:

	void updateTicks();
	void obtainLogarithmicTicks();
	void obtainLinearTicks();
	void updatePlotTicks();
	void updateTicksInside();
	void updateTickLabels();
	void drawAsXAxis();
	void drawAsYAxis();
	void drawAsTopAxis();
	void drawAsRightAxis();

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
