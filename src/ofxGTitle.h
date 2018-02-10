#pragma once

#include "ofxGConstants.h"
#include "ofMain.h"

class ofxGTitle {
public:
	// Constructor
	ofxGTitle(const array<float, 2> &_dim = { 0, 1 });

	// Drawing methods
	void draw() const;

	// Setter methods
	void setDim(float xDim, float yDim);
	void setDim(const array<float, 2> &newDim);
	void setRelativePos(float newRelativePos);
	void setOffset(float newOffset);
	void setText(const string &newText);
	void setTextAlignment(ofxGTextAlignment newTextAlignment);
	void setFontName(const string &newFontName);
	void setFontColor(const ofColor &newFontColor);
	void setFontSize(int newFontSize);
	void setFontProperties(const string &newFontName, const ofColor &newFontColor, int newFontSize);

protected:

	// General properties
	array<float, 2> dim;
	float relativePos;
	float plotPos;
	float offset;

	// Font properties
	string text;
	ofxGTextAlignment textAlignment;
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;
};
