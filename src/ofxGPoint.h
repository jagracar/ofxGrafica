#pragma once

#include "ofMain.h"

class ofxGPoint {
public:
	// Constructors
	ofxGPoint();
	ofxGPoint(float _x, float _y, const string &_label = "");
	ofxGPoint(const ofVec2f &v, const string &_label = "");
	ofxGPoint(const ofxGPoint &p);

	// Setter methods
	void set(float newX, float newY, const string &newLabel);
	void set(const ofVec2f &v, const string &newLabel);
	void set(const ofxGPoint &p);
	void setX(float newX);
	void setY(float newY);
	void setXY(float newX, float newY);
	void setXY(const ofVec2f &v);
	void setLabel(const string &newLabel);

	// Getter methods
	float getX() const;
	float getY() const;
	string getLabel() const;
	bool getValid() const;
	bool isValid() const;

protected:
	float x;
	float y;
	string label;
	bool valid;
};
