#include "ofApp.h"
#include "ofxGrafica.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(255);

	// Load the cvs dataset.
	// The file has the following format:
	// country,income,health,population
	// Central African Republic,599,53.8,4900274
	// ...
	ofBuffer buffer = ofBufferFromFile("data.csv");

	// Save the data in one ofxGPoint vector and calculate the point sizes
	vector<ofxGPoint> points;
	vector<float> pointSizes;
	bool firstLine = true;

	for (auto line : buffer.getLines()) {
		// Skip the header
		if (firstLine) {
			firstLine = false;
		} else if (line.size() > 0) {
			vector<string> columns = ofSplitString(line, ",");

			string country = columns[0];
			float income = stof(columns[1]);
			float health = stof(columns[2]);
			int population = stoi(columns[3]);
			points.emplace_back(income, health, country);

			// The point area should be proportional to the country population
			// population = pi * sq(diameter/2)
			pointSizes.emplace_back(sqrt(population / (200000 * PI)));
		}
	}

	// Create the plot
	plot.setDim(650, 300);
	plot.setTitleText("Life expectancy connection to average income");
	plot.getXAxis().setAxisLabelText("Personal income ($/year)");
	plot.getYAxis().setAxisLabelText("Life expectancy (years)");
	plot.setLogScale("x");
	plot.setPoints(points);
	plot.setPointSizes(pointSizes);
	plot.activatePointLabels();
	plot.activatePanning();
	plot.activateZooming(1.1, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_LEFT);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	plot.beginDraw();
	plot.drawBox();
	plot.drawXAxis();
	plot.drawYAxis();
	plot.drawTitle();
	plot.drawGridLines(GRAFICA_BOTH_DIRECTIONS);
	plot.drawPoints();
	plot.drawLabels();
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
