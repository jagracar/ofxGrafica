#include "ofApp.h"
#include "ofxGrafica.h"
#include <random>

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(150);

	// Prepare the points for the plot
	int nPoints = 1000;
	vector<ofxGPoint> points;
	default_random_engine generator;
	normal_distribution<float> distribution(0.0, 2.0);

	for (int i = 0; i < nPoints; ++i) {
		float x = 10 + ofRandom(200);
		float y = 10 * exp(0.015 * x);
		float xErr = distribution(generator);
		float yErr = distribution(generator);
		points.emplace_back(x + xErr, y + yErr);
	}

	// Set the plot position and dimension
	plot.setPos(25, 25);
	plot.setDim(300, 300);

	// Set the plot title and the axis labels
	plot.setTitleText("Exponential law");
	plot.getXAxis().setAxisLabelText("x");

	if (logScale) {
		plot.setLogScale("y");
		plot.getYAxis().setAxisLabelText("log y");
	} else {
		plot.setLogScale("");
		plot.getYAxis().setAxisLabelText("y");
	}

	// Add the points to the plot
	plot.setPoints(points);
	plot.setPointColor(ofColor(100, 100, 255, 50));
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	plot.beginDraw();
	plot.drawBackground();
	plot.drawBox();
	plot.drawXAxis();
	plot.drawYAxis();
	plot.drawTopAxis();
	plot.drawRightAxis();
	plot.drawTitle();
	plot.drawPoints();
	plot.endDraw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	// Change the log scale
	logScale = !logScale;

	if (logScale) {
		plot.setLogScale("y");
		plot.getYAxis().setAxisLabelText("log y");
	} else {
		plot.setLogScale("");
		plot.getYAxis().setAxisLabelText("y");
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
