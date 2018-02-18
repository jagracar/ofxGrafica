#include "ofApp.h"
#include "ofxGrafica.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(255);

	// Create the first plot
	plot1.setPos(0, 0);
	plot1.setMar(60, 70, 40, 70);
	plot1.setDim(300, 300);
	plot1.setAxesOffset(4);
	plot1.setTicksLength(4);

	// Create the second plot with the same dimensions
	plot2.setPos(plot1.getPos());
	plot2.setMar(plot1.getMar());
	plot2.setDim(plot1.getDim());
	plot2.setAxesOffset(4);
	plot2.setTicksLength(4);

	// Prepare the points
	int nPoints = 50;
	vector<ofxGPoint> points;

	for (int i = 0; i < nPoints; i++) {
		points.emplace_back(i, 30 + 10 * ofNoise(0.1 * i));
	}

	// Set the points, the title and the axis labels
	plot1.setPoints(points);
	plot1.setTitleText("Temperature");
	plot1.getYAxis().setAxisLabelText("T (Celsius)");
	plot1.getXAxis().setAxisLabelText("Time (minutes)");

	plot2.getRightAxis().setAxisLabelText("T (Kelvin)");

	// Make the right axis of the second plot visible
	plot2.getRightAxis().setDrawTickLabels(true);

	// Activate the panning (only for the first plot)
	plot1.activatePanning();
}

//--------------------------------------------------------------
float ofApp::celsiusToKelvin(float celsius) {
	return 273.15 + celsius;
}

//--------------------------------------------------------------
void ofApp::update() {
	// Change the second plot vertical scale from Celsius to Kelvin
	array<float, 2> yLim = plot1.getYLim();
	plot2.setYLim(celsiusToKelvin(yLim[0]), celsiusToKelvin(yLim[1]));
}

//--------------------------------------------------------------
void ofApp::draw() {
	// Draw the first plot
	plot1.beginDraw();
	plot1.drawBox();
	plot1.drawXAxis();
	plot1.drawYAxis();
	plot1.drawTitle();
	plot1.drawPoints();
	plot1.drawLines();
	plot1.endDraw();

	// Draw only the right axis
	plot2.beginDraw();
	plot2.drawRightAxis();
	plot2.endDraw();
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
