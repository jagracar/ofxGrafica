#include "ofxGAxisLabel.h"
#include "ofxGConstants.h"
#include "ofMain.h"

ofxGAxisLabel::ofxGAxisLabel(ofxGAxisType _type, const array<float, 2> &_dim) :
		type(_type), dim(_dim) {
	// General properties
	relativePos = 0.5;
	offset = 35;
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
	fontSize = 11;
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

	ofRectangle bounds = font.getStringBoundingBox(text, 0, 0);

	if (rotate) {
		ofPushMatrix();
		ofTranslate(plotPos + bounds.height / 2, offset + bounds.width);
		ofRotateZ(-90);
		font.drawString(text, 0, 0);
		ofPopMatrix();
	} else {
		switch (textAlignment) {
		case GRAFICA_CENTER_ALIGN:
			font.drawString(text, plotPos - bounds.width / 2,
					offset + bounds.height);
			break;
		case GRAFICA_LEFT_ALIGN:
			font.drawString(text, plotPos, offset + bounds.height);
			break;
		case GRAFICA_RIGHT_ALIGN:
			font.drawString(text, plotPos - bounds.width,
					offset + bounds.height);
			break;
		}
	}

	ofPopStyle();
}

void ofxGAxisLabel::drawAsYLabel() const {
	ofPushStyle();
	ofFill();
	ofSetColor(fontColor);

	ofRectangle bounds = font.getStringBoundingBox(text, 0, 0);

	if (rotate) {
		ofPushMatrix();
		switch (textAlignment) {
		case GRAFICA_CENTER_ALIGN:
			ofTranslate(-offset, plotPos + bounds.width / 2);
			break;
		case GRAFICA_LEFT_ALIGN:
			ofTranslate(-offset, plotPos);
			break;
		case GRAFICA_RIGHT_ALIGN:
			ofTranslate(-offset, plotPos + bounds.width);
			break;
		}
		ofRotateZ(-90);
		font.drawString(text, 0, 0);
		ofPopMatrix();
	} else {
		font.drawString(text, -offset - bounds.width,
				plotPos + bounds.height / 2);
	}

	ofPopStyle();
}

void ofxGAxisLabel::drawAsTopLabel() const {
	ofPushStyle();
	ofFill();
	ofSetColor(fontColor);

	ofRectangle bounds = font.getStringBoundingBox(text, 0, 0);

	if (rotate) {
		ofPushMatrix();
		ofTranslate(plotPos + bounds.height / 2, -offset - dim[1]);
		ofRotateZ(-90);
		font.drawString(text, 0, 0);
		ofPopMatrix();
	} else {
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
	}

	ofPopStyle();
}

void ofxGAxisLabel::drawAsRightLabel() const {
	ofPushStyle();
	ofFill();
	ofSetColor(fontColor);

	ofRectangle bounds = font.getStringBoundingBox(text, 0, 0);

	if (rotate) {
		ofPushMatrix();
		switch (textAlignment) {
		case GRAFICA_CENTER_ALIGN:
			ofTranslate(offset + dim[0] + bounds.height,
					plotPos + bounds.width / 2);
			break;
		case GRAFICA_LEFT_ALIGN:
			ofTranslate(offset + dim[0] + bounds.height, plotPos);
			break;
		case GRAFICA_RIGHT_ALIGN:
			ofTranslate(offset + dim[0] + bounds.height,
					plotPos + bounds.width);
			break;
		}
		ofRotateZ(-90);
		font.drawString(text, 0, 0);
		ofPopMatrix();
	} else {
		font.drawString(text, offset + dim[0], plotPos + bounds.height / 2);
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

void ofxGAxisLabel::setFontName(const string &newFontName) {
	fontName = newFontName;
	font.load(fontName, fontSize);
}

void ofxGAxisLabel::setFontColor(const ofColor &newFontColor) {
	fontColor = newFontColor;
}

void ofxGAxisLabel::setFontSize(int newFontSize) {
	if (newFontSize > 0) {
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}

void ofxGAxisLabel::setFontProperties(const string &newFontName,
		const ofColor &newFontColor, int newFontSize) {
	if (newFontSize > 0) {
		fontName = newFontName;
		fontColor = newFontColor;
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}
