#pragma once

#include "ofxGConstants.h"
#include "ofxGAxisLabel.h"
#include "ofMain.h"

class ofxGAxis {
public:
	// Constructor
	ofxGAxis(ofxGAxisType _type = GRAFICA_X_AXIS, const array<float, 2>& _dim = { 0, 100 },
			const array<float, 2>& _lim = { 0, 1 }, bool _log = false);

	// Special methods
	void moveLim(array<float, 2>& newLim);

	// Drawing methods
	void draw() const;

	// Setter methods
	void setDim(float xDim, float yDim);
	void setDim(const array<float, 2>& newDim);
	void setLim(const array<float, 2>& newLim);
	void setLimAndLog(const array<float, 2>& newLim, bool newLog);
	void setLog(bool newLog);
	void setOffset(float newOffset);
	void setLineColor(const ofColor& newLineColor);
	void setLineWidth(float newLineWidth);
	void setNTicks(int newNTicks);
	void setTicksSeparation(float newTicksSeparation);
	void setTicks(const vector<float>& newTicks);
	void setTickLabels(const vector<string>& newTickLabels);
	void setFixedTicks(bool newFixedTicks);
	void setTickLength(float newTickLength);
	void setSmallTickLength(float newSmallTickLength);
	void setExpTickLabels(bool newExpTickLabels);
	void setRotateTickLabels(bool newRotateTickLabels);
	void setDrawTickLabels(bool newDrawTicksLabels);
	void setTickLabelOffset(float newTickLabelOffset);
	void setDrawAxisLabel(bool newDrawAxisLabel);
	void setAxisLabelText(const string& text);
	void setFontName(const string& newFontName);
	void setFontColor(const ofColor& newFontColor);
	void setFontSize(int newFontSize);
	void setFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize);
	void setAllFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize);

	// Getter methods
	vector<float> getTicks() const;
	vector<float>& getTicksRef();
	vector<float> getPlotTicks() const;
	vector<float>& getPlotTicksRef();
	ofxGAxisLabel& getAxisLabel();

protected:
	// Static methods
	static int obtainSigDigits(float number);
	static float roundPlus(float number, int sigDigits);

	// Update methods
	void updateTicks();
	void obtainLogarithmicTicks();
	void obtainLinearTicks();
	void updatePlotTicks();
	void updateTicksInside();
	void updateTickLabels();

	// Drawing methods
	void drawAsXAxis() const;
	void drawAsYAxis() const;
	void drawAsTopAxis() const;
	void drawAsRightAxis() const;

	// General properties
	ofxGAxisType type;
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

	// Font properties
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;
};
