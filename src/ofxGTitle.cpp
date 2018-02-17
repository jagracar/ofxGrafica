#include "ofxGTitle.h"
#include "ofxGConstants.h"
#include "ofMain.h"

ofxGTitle::ofxGTitle(const array<float, 2>& _dim, const string& _text) :
		dim(_dim), text(_text) {
	// General properties
	relativePos = 0.5;
	plotPos = relativePos * dim[0];
	offset = 13;

	// Font properties
	textAlignment = GRAFICA_CENTER_ALIGN;
	fontName = "SansSerif.rtt";
	fontColor = ofColor(100);
	fontSize = 10;
	font.load(fontName, fontSize);
}

void ofxGTitle::draw() const {
	ofPushStyle();
	ofSetColor(fontColor);
	ofRectangle bounds = font.getStringBoundingBox(text, 0, 0);

	switch (textAlignment) {
	case GRAFICA_CENTER_ALIGN:
		font.drawString(text, plotPos - bounds.width / 2, -offset - dim[1]);
		break;
	case GRAFICA_LEFT_ALIGN:
		font.drawString(text, plotPos, -offset - dim[1]);
		break;
	case GRAFICA_RIGHT_ALIGN:
		font.drawString(text, plotPos - bounds.width, -offset - dim[1]);
		break;
	default:
		font.drawString(text, plotPos - bounds.width / 2, -offset - dim[1]);
		break;
	}

	ofPopStyle();
}

void ofxGTitle::setDim(float xDim, float yDim) {
	if (xDim <= 0 || yDim <= 0) {
		throw invalid_argument("The dimensions should be larger than zero.");
	}

	dim = {xDim, yDim};
	plotPos = relativePos * dim[0];
}

void ofxGTitle::setDim(const array<float, 2>& newDim) {
	setDim(newDim[0], newDim[1]);
}

void ofxGTitle::setRelativePos(float newRelativePos) {
	relativePos = newRelativePos;
	plotPos = relativePos * dim[0];
}

void ofxGTitle::setOffset(float newOffset) {
	offset = newOffset;
}

void ofxGTitle::setText(const string& newText) {
	text = newText;
}

void ofxGTitle::setTextAlignment(ofxGTextAlignment newTextAlignment) {
	textAlignment = newTextAlignment;
}

void ofxGTitle::setFontName(const string& newFontName) {
	fontName = newFontName;
	font.load(fontName, fontSize);
}

void ofxGTitle::setFontColor(const ofColor& newFontColor) {
	fontColor = newFontColor;
}

void ofxGTitle::setFontSize(int newFontSize) {
	if (newFontSize <= 0) {
		throw invalid_argument("The font size should be larger than zero.");
	}

	fontSize = newFontSize;
	font.load(fontName, fontSize);
}

void ofxGTitle::setFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize) {
	if (newFontSize <= 0) {
		throw invalid_argument("The font size should be larger than zero.");
	}

	fontName = newFontName;
	fontColor = newFontColor;
	fontSize = newFontSize;
	font.load(fontName, fontSize);
}

void ofxGTitle::setFontMakeContours(bool makeContours) {
	font.load(fontName, fontSize, true, true, makeContours);
}
