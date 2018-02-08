#pragma once

#include "ofMain.h"
#include "ofxGPoint.h"
#include "ofxGHistogram.h"

class ofxGLayer {
public:
	ofxGLayer() = default;
	ofxGLayer(const string &_id, const array<float, 2> &_dim,
			const array<float, 2> &_xLim, const array<float, 2> &_yLim,
			bool _xLog, bool _yLog);

	bool isId(const string &someId);
	float valueToXPlot(float x);
	float valueToYPlot(float y);
	array<float, 2> valueToPlot(float x, float y);
	ofxGPoint valueToPlot(ofxGPoint point);
	array<float, 2> plotToValue(float xPlot, float yPlot);
	bool isInside(float xPlot, float yPlot);
	bool isInside(ofxGPoint plotPoint);
	ofxGPoint getPointAtPlotPos(float xPlot, float yPlot);

protected:
	float xPlotToValue(float xPlot);
	float yPlotToValue(float yPlot);

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
	array<float, 2> labelSeparation;
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;

	// Helper variable
	array<array<float, 2>, 4> cuts;
};
