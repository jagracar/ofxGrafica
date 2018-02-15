#include "ofApp.h"
#include "ofxGrafica.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(255);

	monthNames = {"January", "February", "March", "April", "May", "June", "July",
		"August", "September", "October", "November", "December"};
	daysPerMonth = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	daysPerMonthLeapYear = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	// Load the Oktoberfest vs. Bundestagswahl (German elections day) Google
	// search history file (obtained from the Google trends page).
	// The csv file has the following format:
	// year,month,day,oktoberfest,bundestagswahl
	// 2004,0,1,5,1
	// ...
	ofBuffer buffer = ofBufferFromFile("OktoberfestVSGermanElections.csv");

	// Save the data in two ofxGPoint vectors
	vector<ofxGPoint> pointsOktoberfest;
	vector<ofxGPoint> pointsElections;
	bool firstLine = true;

	for (auto line : buffer.getLines()) {
		// Skip the header
		if (firstLine) {
			firstLine = false;
		} else if (line.size() > 0) {
			vector<string> columns = ofSplitString(line, ",");

			int year = stoi(columns[0]);
			int month = stoi(columns[1]);
			int day = stoi(columns[2]);
			int oktoberfestCount = stoi(columns[3]);
			int electionsCount = stoi(columns[4]);
			float date = getExactDate(year, month, day);

			pointsOktoberfest.emplace_back(date, oktoberfestCount, monthNames[month]);
			pointsElections.emplace_back(date, electionsCount, monthNames[month]);
		}
	}

	// Create the plot
	plot.setDim(700, 300);
	plot.setTitleText("Oktoberfest vs. Bundestagwahl Google search history");
	plot.getXAxis().setAxisLabelText("Year");
	plot.getYAxis().setAxisLabelText("Google normalized searches");
	plot.getXAxis().setNTicks(10);
	plot.setPoints(pointsOktoberfest);
	plot.setLineColor(ofColor(100, 100, 100));
	plot.addLayer("German elections day", pointsElections);
	plot.getLayer("German elections day").setLineColor(ofColor(255, 100, 255));
	//plot.activatePointLabels();
}

float ofApp::getExactDate(int year, int month, int day) {
	bool leapYear = false;

	if (year % 400 == 0) {
		leapYear = true;
	} else if (year % 100 == 0) {
		leapYear = false;
	} else if (year % 4 == 0) {
		leapYear = true;
	}

	if (leapYear) {
		return year + (month + (day - 1.0) / daysPerMonthLeapYear[month]) / 12.0;
	} else {
		return year + (month + (day - 1.0) / daysPerMonth[month]) / 12.0;
	}
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
	plot.drawGridLines(GRAFICA_VERTICAL_DIRECTION);
	plot.drawFilledContours(GRAFICA_HORIZONTAL_CONTOUR, 0);
	plot.drawLegend( { "Oktoberfest", "Bundestagswahl" }, { 0.07, 0.22 }, { 0.92, 0.92 });
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
