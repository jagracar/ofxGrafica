#pragma once

#include "ofMain.h"
#include "ofxGLayer.h"
#include "ofxGAxis.h"
#include "ofxGTitle.h"

class ofxGPlot {
public:

	ofxGPlot(float xPos=0.0, float yPos=0.0, float plotWidth=450.0, float plotHeight=300.0);

	static const string MAINLAYERID;
	static const int VERTICAL = 0;
	static const int HORIZONTAL = 1;
	static const int BOTH = 2;
	static const int NONE = 0;
	static constexpr float LOG10 = log(10);

protected:

	// General properties
	array<float, 2> pos;
	array<float, 2> outerDim;
	array<float, 4> mar;
	array<float, 4> dim;
	array<float, 2> xLim;
	array<float, 2> yLim;
	bool fixedXLim;
	bool fixedYLim;
	bool xLog;
	bool yLog;
	bool invertedXScale;
	bool invertedYScale;
	bool includeAllLayersInLim;
	float expandLimFactor;

	// Format properties
	ofColor bgColor;
	ofColor boxBgColor;
	ofColor boxLineColor;
	float boxLineWidth;
	ofColor gridLineColor;
	float gridLineWidth;

	// Layers
	ofxGLayer mainLayer;
	vector<ofxGLayer> layerList;

	// Axes and title
	ofxGAxis xAxis;
	ofxGAxis topAxis;
	ofxGAxis yAxis;
	ofxGAxis rightAxis;
	ofxGTitle title;
};
