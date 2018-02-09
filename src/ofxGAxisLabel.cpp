#include "ofxGAxisLabel.h"
#include "ofxGConstants.h"
#include "ofMain.h"

ofxGAxisLabel::ofxGAxisLabel(ofxGAxisType _type, const array<float, 2> &_dim) :
		type(_type), dim(_dim), relativePos(0.5), offset(35) {
	rotate =
			(type == GRAFICA_X_AXIS || type == GRAFICA_TOP_AXIS) ? false : true;

	plotPos =
			(type == GRAFICA_X_AXIS || type == GRAFICA_TOP_AXIS) ?
					relativePos * dim[0] : -relativePos * dim[1];

	// Font properties
	text = "";
	textAlignment = GRAFICA_CENTER_ALIGN;
	fontName = "SansSerif.ttf";
	fontColor = ofColor(0);
	fontSize = 13;
	font.load(fontName, fontSize);
}

void ofxGAxisLabel::draw() const {
	switch (type) {
	case GRAFICA_X_AXIS:
		drawAsXLabel();
		break;
	case GRAFICA_Y_AXIS:
		drawAsYLabel();
		break;
	case GRAFICA_TOP_AXIS:
		drawAsTopLabel();
		break;
	case GRAFICA_RIGHT_AXIS:
		drawAsRightLabel();
		break;
	}
}

void ofxGAxisLabel::drawAsXLabel() const {
	ofPushStyle();
	ofFill();
	ofSetColor(fontColor);

	if (rotate) {
		ofPushMatrix();
		ofTranslate(plotPos, offset);
		ofRotate(-HALF_PI);
		font.drawString(text, 0, 0);
		ofPopMatrix();
	} else {
		font.drawString(text, plotPos, offset);
	}

	ofPopStyle();
}

void ofxGAxisLabel::drawAsYLabel() const {
	ofPushStyle();
	ofFill();
	ofSetColor(fontColor);

	if (rotate) {
		ofPushMatrix();
		ofTranslate(-offset, plotPos);
		ofRotate(-HALF_PI);
		font.drawString(text, 0, 0);
		ofPopMatrix();
	} else {
		font.drawString(text, -offset, plotPos);
	}

	ofPopStyle();
}

void ofxGAxisLabel::drawAsTopLabel() const {
	ofPushStyle();
	ofFill();
	ofSetColor(fontColor);

	if (rotate) {
		ofPushMatrix();
		ofTranslate(plotPos, -offset - dim[1]);
		ofRotate(-HALF_PI);
		font.drawString(text, 0, 0);
		ofPopMatrix();
	} else {
		font.drawString(text, plotPos, -offset - dim[1]);
	}

	ofPopStyle();
}

void ofxGAxisLabel::drawAsRightLabel() const {
	ofPushStyle();
	ofFill();
	ofSetColor(fontColor);

	if (rotate) {
		ofPushMatrix();
		ofTranslate(offset + dim[0], plotPos);
		ofRotate(-HALF_PI);
		font.drawString(text, 0, 0);
		ofPopMatrix();
	} else {
		font.drawString(text, offset + dim[0], plotPos);
	}

	ofPopStyle();
}

void ofxGAxisLabel::setDim(float xDim, float yDim) {
	if (xDim > 0 && yDim > 0) {
		dim = {xDim, yDim};
		plotPos = (type == GRAFICA_X_AXIS || type == GRAFICA_TOP_AXIS) ? relativePos * dim[0] : -relativePos * dim[1];
	}
}

void ofxGAxisLabel::setDim(const array<float, 2> &newDim) {
	setDim(newDim[0], newDim[1]);
}

void ofxGAxisLabel::setRelativePos(float newRelativePos) {
	relativePos = newRelativePos;
	plotPos =
			(type == GRAFICA_X_AXIS || type == GRAFICA_TOP_AXIS) ?
					relativePos * dim[0] : -relativePos * dim[1];
}

void ofxGAxisLabel::setOffset(float newOffset) {
	offset = newOffset;
}

void ofxGAxisLabel::setRotate(bool newRotate) {
	rotate = newRotate;
}

void ofxGAxisLabel::setText(const string &newText) {
	text = newText;
}

void ofxGAxisLabel::setTextAlignment(ofxGTextAlignment newTextAlignment) {
	textAlignment = newTextAlignment;
}

void ofxGAxisLabel::setFontName(const string &newFontName){
	fontName = newFontName;
	font.load(fontName, fontSize);
}

void ofxGAxisLabel::setFontColor(ofColor newFontColor){
	fontColor = newFontColor;
}

void ofxGAxisLabel::setFontSize(int newFontSize){
	if (newFontSize > 0) {
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}

void ofxGAxisLabel::setFontProperties(const string &newFontName, ofColor newFontColor,
			int newFontSize){
	if (newFontSize > 0) {
		fontName = newFontName;
		fontColor = newFontColor;
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}
