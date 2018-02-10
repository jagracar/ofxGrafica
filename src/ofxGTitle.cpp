#include "ofxGTitle.h"
#include "ofxGConstants.h"
#include "ofMain.h"

ofxGTitle::ofxGTitle(const array<float, 2> &_dim) :
		dim(_dim) {
	// General properties
	relativePos = 0.5;
	plotPos = relativePos * dim[0];
	offset = 10;

	// Font properties
	text = "";
	textAlignment = GRAFICA_CENTER_ALIGN;
	fontName = "SansSerif.rtt";
	fontColor = ofColor(100);
	fontSize = 11;
	font.load(fontName, fontSize);
}

void ofxGTitle::draw() const {
	ofRectangle bounds = font.getStringBoundingBox(text, 0, 0);

	ofPushStyle();
	ofFill();
	ofSetColor(fontColor);
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
	}
	ofPopStyle();
}

void ofxGTitle::setDim(float xDim, float yDim) {
	if (xDim > 0 && yDim > 0) {
		dim = {xDim, yDim};
		plotPos = relativePos * dim[0];
	}
}

void ofxGTitle::setDim(const array<float, 2> &newDim) {
	setDim(newDim[0], newDim[1]);
}

void ofxGTitle::setRelativePos(float newRelativePos) {
	relativePos = newRelativePos;
	plotPos = relativePos * dim[0];
}

void ofxGTitle::setOffset(float newOffset) {
	offset = newOffset;
}

void ofxGTitle::setText(const string &newText) {
	text = newText;
}

void ofxGTitle::setTextAlignment(ofxGTextAlignment newTextAlignment) {
	textAlignment = newTextAlignment;
}

void ofxGTitle::setFontName(const string &newFontName) {
	fontName = newFontName;
	font.load(fontName, fontSize);
}

void ofxGTitle::setFontColor(const ofColor &newFontColor) {
	fontColor = newFontColor;
}

void ofxGTitle::setFontSize(int newFontSize) {
	if (newFontSize > 0) {
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}

void ofxGTitle::setFontProperties(const string &newFontName,
		const ofColor &newFontColor, int newFontSize) {
	if (newFontSize > 0) {
		fontName = newFontName;
		fontColor = newFontColor;
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}
