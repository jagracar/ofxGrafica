#include "ofApp.h"
#include "ofxGrafica.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(150);

	// Initialize the sketch variables
	stepsPerCycle = 100;
	clockwise = true;
	scale = 5;
	step = 0;

	// Prepare the first set of points
	vector<ofxGPoint> points1;

	for (int i = 0; i < 10; ++i) {
		points1.push_back(calculatePoint(step, stepsPerCycle, scale));
		step = clockwise ? step + 1 : step - 1;
	}

	// Prepare the second set of points
	vector<ofxGPoint> points2;

	for (int i = 0; i < stepsPerCycle + 1; ++i) {
		points2.push_back(calculatePoint(i, stepsPerCycle, 0.9 * scale));
	}

	// Set the plot position and the dimension
	plot.setPos(25, 25);
	plot.setDim(300, 300);

	// Set the plot limits (this will fix them)
	plot.setXLim(-1.2 * scale, 1.2 * scale);
	plot.setYLim(-1.2 * scale, 1.2 * scale);

	// Set the plot title and the axis labels
	plot.setTitleText("Clockwise movement");
	plot.getXAxis().setAxisLabelText("x axis");
	plot.getYAxis().setAxisLabelText("y axis");

	// Activate the panning effect
	plot.activatePanning();

	// Add the two sets of points to the plot
	plot.setPoints(points1);
	plot.addLayer("surface", points2);

	// Change the second layer line color
	plot.getLayer("surface").setLineColor(ofColor(100, 255, 100));
}

//--------------------------------------------------------------
ofxGPoint ofApp::calculatePoint(float i, float n, float rad) {
	float delta = 0.1 * cos(TWO_PI * 10 * i / n);
	float ang = TWO_PI * i / n;
	return ofxGPoint(rad * (1 + delta) * sin(ang), rad * (1 + delta) * cos(ang));
}

//--------------------------------------------------------------
void ofApp::update() {
	// Add and remove new points every 10th of a second
	if (ofGetElapsedTimeMillis() > 100) {
		if (clockwise) {
			// Add the point at the end of the array
			plot.addPoint(calculatePoint(step, stepsPerCycle, scale));
			++step;

			// Remove the first point
			plot.removePoint(0);
		} else {
			// Add the point at the beginning of the array
			plot.addPoint(0, calculatePoint(step, stepsPerCycle, scale));
			--step;

			// Remove the last point
			plot.removePoint(plot.getPointsRef().size() - 1);
		}

		// Reset the time
		ofResetElapsedTimeCounter();
	}
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
	plot.getMainLayer().drawPoints();
	plot.getLayer("surface").drawFilledContour(GRAFICA_HORIZONTAL_CONTOUR, 0);
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
	// Change the movement sense
	clockwise = !clockwise;

	if (clockwise) {
		step += plot.getPointsRef().size() + 1;
		plot.setTitleText("Clockwise movement");
	} else {
		step -= plot.getPointsRef().size() + 1;
		plot.setTitleText("Anti-clockwise movement");
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
