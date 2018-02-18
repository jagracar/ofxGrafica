#pragma once

#include "ofMain.h"
#include "ofxGrafica.h"
#include <random>

class ofApp: public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxGPlot plot1, plot2, plot3, plot4;
	vector<ofxGPoint> polygonPoints;
	ofImage mug;
	bool trackingMouse;
	ofPath star;
	int gaussianCounter;
	array<float, 10> gaussianStack;
	default_random_engine generator;
	normal_distribution<float> distribution;
	int uniformCounter;
	array<float, 30> uniformStack;
};
