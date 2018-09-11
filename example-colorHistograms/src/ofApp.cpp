#include "ofApp.h"
#include "ofxGrafica.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(255);

	// Load the image and set the image position
	img.load("picture.jpg");
	imgPos = glm::vec2(410, 100);

	// Set the rectangle position and size
	rect.set(imgPos, 20, 20);

	// Calculate the color histograms
	calculateHistograms();

	// Setup for the first plot
	plot1.setPos(0, 0);
	plot1.setDim(300, 200);
	plot1.setXLim(0, 255);
	plot1.setTitleText("Color histograms");
	plot1.getYAxis().getAxisLabel().setText("N");
	plot1.getYAxis().getAxisLabel().setRotate(false);
	plot1.setPoints(redHistPoints);
	plot1.startHistograms(GRAFICA_VERTICAL_HISTOGRAM);
	plot1.getHistogram().setBgColors( { ofColor(255, 0, 0, 100) });
	plot1.getHistogram().setLineColors( { ofColor(0, 0) });
	plot1.getHistogram().setSeparations( { 0 });
	plot1.activateZooming(1.2, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_LEFT);
	plot1.activateReset();

	// Setup for the second plot
	plot2.setPos(0, 245);
	plot2.setDim(300, 200);
	plot2.setXLim(0, 255);
	plot2.getYAxis().getAxisLabel().setText("N");
	plot2.getYAxis().getAxisLabel().setRotate(false);
	plot2.setPoints(greenHistPoints);
	plot2.startHistograms(GRAFICA_VERTICAL_HISTOGRAM);
	plot2.getHistogram().setBgColors( { ofColor(0, 255, 0, 100) });
	plot2.getHistogram().setLineColors( { ofColor(0, 0) });
	plot2.getHistogram().setSeparations( { 0 });
	plot2.activateZooming(1.2, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_LEFT);
	plot2.activateReset();

	// Setup for the third plot
	plot3.setPos(0, 490);
	plot3.setDim(300, 200);
	plot3.setXLim(0, 255);
	plot3.getXAxis().getAxisLabel().setText("color value");
	plot3.getYAxis().getAxisLabel().setText("N");
	plot3.getYAxis().getAxisLabel().setRotate(false);
	plot3.setPoints(blueHistPoints);
	plot3.startHistograms(GRAFICA_VERTICAL_HISTOGRAM);
	plot3.getHistogram().setBgColors( { ofColor(0, 0, 255, 100) });
	plot3.getHistogram().setLineColors( { ofColor(0, 0) });
	plot3.getHistogram().setSeparations( { 0 });
	plot3.activateZooming(1.2, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_LEFT);
	plot3.activateReset();
}

//--------------------------------------------------------------
void ofApp::calculateHistograms() {
	// Calculate the color histograms
	int delta = 2;
	int nBins = 255 / delta;
	vector<float> redHist(nBins, 0);
	vector<float> greenHist(nBins, 0);
	vector<float> blueHist(nBins, 0);
	int xStart = ofClamp(rect.getX() - imgPos.x, 0, img.getWidth());
	int yStart = ofClamp(rect.getY() - imgPos.y, 0, img.getHeight());
	int xEnd = ofClamp(rect.getX() + rect.getWidth() - imgPos.x, 0, img.getWidth());
	int yEnd = ofClamp(rect.getY() + rect.getHeight() - imgPos.y, 0, img.getHeight());
	int counter = 0;

	for (int x = xStart; x < xEnd; ++x) {
		for (int y = yStart; y < yEnd; ++y) {
			ofColor c = img.getColor(x, y);
			++redHist[c.r / delta];
			++greenHist[c.g / delta];
			++blueHist[c.b / delta];
			++counter;
		}
	}

	// Calculate the plot points
	redHistPoints.clear();
	greenHistPoints.clear();
	blueHistPoints.clear();

	for (int i = 0; i < nBins; ++i) {
		redHistPoints.emplace_back((i + 0.5) * delta, redHist[i] / counter);
		greenHistPoints.emplace_back((i + 0.5) * delta, greenHist[i] / counter);
		blueHistPoints.emplace_back((i + 0.5) * delta, blueHist[i] / counter);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	// Draw the first plot
	plot1.beginDraw();
	plot1.drawBox();
	plot1.drawTitle();
	plot1.drawXAxis();
	plot1.drawYAxis();
	plot1.drawGridLines(GRAFICA_VERTICAL_DIRECTION);
	plot1.drawHistograms();
	plot1.endDraw();

	// Draw the second plot
	plot2.beginDraw();
	plot2.drawBox();
	plot2.drawXAxis();
	plot2.drawYAxis();
	plot2.drawGridLines(GRAFICA_VERTICAL_DIRECTION);
	plot2.drawHistograms();
	plot2.endDraw();

	// Draw the third plot
	plot3.beginDraw();
	plot3.drawBox();
	plot3.drawXAxis();
	plot3.drawYAxis();
	plot3.drawGridLines(GRAFICA_VERTICAL_DIRECTION);
	plot3.drawHistograms();
	plot3.endDraw();

	// Draw the image
	img.draw(imgPos);

	// Draw the square
	ofPushStyle();
	ofNoFill();
	ofSetColor(0, 255, 0);
	ofDrawRectangle(rect);
	ofPopStyle();
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
	// Center the rectangle on the mouse position
	rect.setX(x - rect.getWidth() / 2);
	rect.setY(y - rect.getHeight() / 2);

	// Calculate the color histograms
	calculateHistograms();

	// Update the plots
	plot1.setPoints(redHistPoints);
	plot2.setPoints(greenHistPoints);
	plot3.setPoints(blueHistPoints);
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
