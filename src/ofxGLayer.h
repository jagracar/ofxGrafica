#pragma once

#include "ofxGPoint.h"
#include "ofxGHistogram.h"
#include "ofMain.h"

class ofxGLayer {
public:
	// Constructor
	ofxGLayer(const string &_id = "defaultId", const array<float, 2> &_dim = {
			0, 1 }, const array<float, 2> &_xLim = { 0, 1 },
			const array<float, 2> &_yLim = { 0, 1 }, bool _xLog = false,
			bool _yLog = false);

	// Drawing methods

	// Setter methods

	bool isId(const string &someId) const;
	float valueToXPlot(float x) const;
	float valueToYPlot(float y) const;
	array<float, 2> valueToPlot(float x, float y) const;
	ofxGPoint valueToPlot(ofxGPoint point) const;
	array<float, 2> plotToValue(float xPlot, float yPlot) const;
	bool isInside(float xPlot, float yPlot) const;
	bool isInside(ofxGPoint plotPoint) const;
	ofxGPoint getPointAtPlotPos(float xPlot, float yPlot) const;

protected:
	float xPlotToValue(float xPlot) const;
	float yPlotToValue(float yPlot) const;

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
