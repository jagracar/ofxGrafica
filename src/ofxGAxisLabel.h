#pragma once

#include "ofxGConstants.h"
#include "ofMain.h"

class ofxGAxisLabel {
public:
	// Constructor
	ofxGAxisLabel(ofxGAxisType _type = GRAFICA_X_AXIS,
			const array<float, 2> &_dim = { 0, 1 });

	// Drawing methods
	void draw() const;

	// Setter methods
	void setDim(float xDim, float yDim);
	void setDim(const array<float, 2> &newDim);
	void setRelativePos(float newRelativePos);
	void setOffset(float newOffset);
	void setRotate(bool newRotate);
	void setText(const string &newText);
	void setTextAlignment(ofxGTextAlignment newTextAlignment);
	void setFontName(const string &newFontName);
	void setFontColor(ofColor newFontColor);
	void setFontSize(int newFontSize);
	void setFontProperties(const string &newFontName, ofColor newFontColor,
			int newFontSize);

protected:
	// Drawing methods
	void drawAsXLabel() const;
	void drawAsYLabel() const;
	void drawAsTopLabel() const;
	void drawAsRightLabel() const;

	// General properties
	ofxGAxisType type;
	array<float, 2> dim;
	float relativePos;
	float offset;
	bool rotate;
	float plotPos;

	// Font properties
	string text;
	ofxGTextAlignment textAlignment;
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;
};
