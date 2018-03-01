#include "ofApp.h"
#include "ofxGrafica.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(255);

	// Initialize the global variables
	pointColor = ofColor(255, 0, 0, 155);
	drawLines = true;
	circleResolution = 22;

	// Set the circle resolution
	ofSetCircleResolution(circleResolution);

	// Set the plot position and dimentions
	plot.setPos(0, 0);
	plot.setOuterDim(ofGetWidth(), ofGetHeight());

	// Set the title text relative position and alignment
	plot.getTitle().setRelativePos(0.4);
	plot.getTitle().setTextAlignment(GRAFICA_LEFT_ALIGN);

	// Set the axis labels
	plot.getXAxis().setAxisLabelText("x axis");
	plot.getYAxis().setAxisLabelText("y axis");

	// Prepare the points for the plot
	int nPoints = 10000;
	vector<ofxGPoint> points;

	for (int i = 0; i < nPoints; ++i) {
		points.emplace_back(ofRandom(100), ofRandom(100));
	}

	// Add the points
	plot.setPoints(points);

	// Activate panning and zooming
	plot.activatePanning();
	plot.activateZooming(1.1, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_LEFT);
}

//--------------------------------------------------------------
void ofApp::update() {
	plot.getTitle().setText("Frame rate: " + to_string(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw() {
	plot.beginDraw();
	plot.drawBox();
	plot.drawXAxis();
	plot.drawYAxis();
	plot.drawTitle();

	if (drawLines) {
		plot.drawLines();
	}

	plot.drawPoints(pointColor); // this is 3 times faster than drawPoints()
	plot.drawLabels();
	plot.endDraw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'l') {
		drawLines = !drawLines;
	} else if (key == '+') {
		circleResolution = ofClamp(++circleResolution, 3, 50);
		ofSetCircleResolution(circleResolution);
	} else if (key == '-') {
		circleResolution = ofClamp(--circleResolution, 3, 50);
		ofSetCircleResolution(circleResolution);
	}
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
