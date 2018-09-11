#include "ofxGPoint.h"
#include "ofMain.h"

ofxGPoint::ofxGPoint(float _x, float _y, const string& _label) :
		x(_x), y(_y), label(_label), valid(isfinite(x) && isfinite(y)) {
}

ofxGPoint::ofxGPoint() :
		ofxGPoint(0.0, 0.0) {
}

ofxGPoint::ofxGPoint(const glm::vec2& v, const string& _label) :
		ofxGPoint(v.x, v.y, _label) {
}

void ofxGPoint::set(float newX, float newY, const string& newLabel) {
	x = newX;
	y = newY;
	label = newLabel;
	valid = isfinite(x) && isfinite(y);
}

void ofxGPoint::set(const glm::vec2& v, const string& newLabel) {
	set(v.x, v.y, newLabel);
}

void ofxGPoint::set(const ofxGPoint& p) {
	set(p.x, p.y, p.label);
}

void ofxGPoint::setX(float newX) {
	x = newX;
	valid = isfinite(x) && isfinite(y);
}

void ofxGPoint::setY(float newY) {
	y = newY;
	valid = isfinite(x) && isfinite(y);
}

void ofxGPoint::setXY(float newX, float newY) {
	x = newX;
	y = newY;
	valid = isfinite(x) && isfinite(y);
}

void ofxGPoint::setXY(const glm::vec2& v) {
	setXY(v.x, v.y);
}

void ofxGPoint::setXY(const ofxGPoint& p) {
	setXY(p.x, p.y);
}

void ofxGPoint::setLabel(const string& newLabel) {
	label = newLabel;
}

float ofxGPoint::getX() const {
	return x;
}

float ofxGPoint::getY() const {
	return y;
}

string ofxGPoint::getLabel() const {
	return label;
}

bool ofxGPoint::getValid() const {
	return valid;
}

bool ofxGPoint::isValid() const {
	return valid;
}
