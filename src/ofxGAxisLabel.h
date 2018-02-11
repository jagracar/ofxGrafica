#pragma once

#include "ofxGConstants.h"
#include "ofMain.h"

class ofxGAxisLabel {
public:
	// Constructor
	ofxGAxisLabel(ofxGAxisType _type = GRAFICA_X_AXIS, const array<float, 2>& _dim = { 0, 100 }, const string& _text =
			"");

	// Drawing methods
	void draw() const;

	// Setter methods
	void setDim(float xDim, float yDim);
	void setDim(const array<float, 2>& newDim);
	void setRelativePos(float newRelativePos);
	void setOffset(float newOffset);
	void setRotate(bool newRotate);
	void setText(const string& newText);
	void setTextAlignment(ofxGTextAlignment newTextAlignment);
	void setFontName(const string& newFontName);
	void setFontColor(const ofColor& newFontColor);
	void setFontSize(int newFontSize);
	void setFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize);

protected:
	// Drawing methods
	void drawAsXLabel() const;
	void drawAsYLabel() const;
	void drawAsTopLabel() const;
	void drawAsRightLabel() const;

	// General properties
	ofxGAxisType type;
	array<float, 2> dim;
	string text;
	float relativePos;
	float offset;
	float plotPos;
	bool rotate;

	// Font properties
	ofxGTextAlignment textAlignment;
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;
};
