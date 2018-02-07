#include "ofxGPoint.h"

ofxGPoint::ofxGPoint() :
		x(0.0), y(0.0), label(""), valid(true) {
}

ofxGPoint::ofxGPoint(float _x, float _y, const string &_label) :
		x(_x), y(_y), label(_label), valid(isfinite(x) && isfinite(y)) {
}

ofxGPoint::ofxGPoint(const ofVec2f &v, const string &_label) :
		x(v.x), y(v.y), label(_label), valid(isfinite(x) && isfinite(y)) {
}

ofxGPoint::ofxGPoint(const ofxGPoint &p) :
		x(p.x), y(p.y), label(p.label), valid(p.valid) {
}

void ofxGPoint::set(float newX, float newY, const string &newLabel) {
	x = newX;
	y = newY;
	label = newLabel;
	valid = isfinite(x) && isfinite(y);
}

void ofxGPoint::set(const ofVec2f &v, const string &newLabel) {
	set(v.x, v.y, newLabel);
}
void ofxGPoint::set(const ofxGPoint &p) {
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

void ofxGPoint::setXY(const ofVec2f &v) {
	x = v.x;
	y = v.y;
	valid = isfinite(x) && isfinite(y);
}
void ofxGPoint::setLabel(const string &newLabel) {
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
