#pragma once

#include "ofxGConstants.h"
#include "ofxGPoint.h"
#include "ofMain.h"

class ofxGHistogram {
public:
	// Constructor
	ofxGHistogram(ofxGHistogramType _type = GRAFICA_HORIZONTAL_HISTOGRAM, const array<float, 2>& _dim = { 0, 100 },
			const vector<ofxGPoint>& _plotPoints = { });

	// Drawing methods
	void draw(const ofxGPoint& plotBasePoint) const;

	// Setter methods
	void setType(ofxGHistogramType newType);
	void setDim(float xDim, float yDim);
	void setDim(const array<float, 2>& newDim);
	void setPlotPoints(const vector<ofxGPoint>& newPlotPoints);
	void setPlotPoint(int index, const ofxGPoint& newPlotPoint);
	void addPlotPoint(const ofxGPoint& newPlotPoint);
	void addPlotPoint(int index, ofxGPoint& newPlotPoint);
	void addPlotPoints(const vector<ofxGPoint>& newPlotPoints);
	void removePlotPoint(int index);
	void setSeparations(const vector<float>& newSeparations);
	void setBgColors(const vector<ofColor>& newBgColors);
	void setLineColors(const vector<ofColor>& newLineColors);
	void setLineWidths(const vector<float>& newLineWidths);
	void setVisible(bool newVisible);
	void setLabelsOffset(float newLabelsOffset);
	void setDrawLabels(bool newDrawLabels);
	void setRotateLabels(bool newRotateLabels);
	void setFontName(const string& newFontName);
	void setFontColor(const ofColor& newFontColor);
	void setFontSize(int newFontSize);
	void setFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize);

protected:
	// Update methods
	void updateArrays();

	// Drawing methods
	void drawHistLabels() const;

	// General properties
	ofxGHistogramType type;
	array<float, 2> dim;
	vector<ofxGPoint> plotPoints;
	bool visible;
	vector<float> separations;
	vector<ofColor> bgColors;
	vector<ofColor> lineColors;
	vector<float> lineWidths;
	vector<float> leftSides;
	vector<float> rightSides;

	// Labels properties
	float labelsOffset;
	bool drawLabels;
	bool rotateLabels;

	// Font properties
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;
};
