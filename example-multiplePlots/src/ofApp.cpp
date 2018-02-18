#include "ofApp.h"
#include "ofxGrafica.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundColor(255);

	// Prepare the points for the first plot
	vector<ofxGPoint> points1a;
	vector<ofxGPoint> points1b;
	vector<ofxGPoint> points1c;

	for (int i = 0; i < 500; ++i) {
		points1a.emplace_back(i, ofNoise(0.1 * i) + 1, "point " + to_string(i));
		points1b.emplace_back(i, ofNoise(0.1 * i + 500) + 0.5, "point " + to_string(i));
		points1c.emplace_back(i, ofNoise(0.1 * i + 1000), "point " + to_string(i));
	}

	// Create a polygon to display inside the plot
	polygonPoints.emplace_back(2, 0.15);
	polygonPoints.emplace_back(6, 0.12);
	polygonPoints.emplace_back(15, 0.3);
	polygonPoints.emplace_back(8, 0.6);
	polygonPoints.emplace_back(1.5, 0.5);

	// Load and resize the beer mug image
	mug.load("beermug.png");
	mug.resize(0.5 * mug.getWidth(), 0.5 * mug.getHeight());

	// Setup for the first plot
	plot1.setPos(0, 0);
	plot1.setXLim(1, 100);
	plot1.setYLim(0.1, 3);
	plot1.getTitle().setText("Multiple layers plot");
	plot1.getXAxis().getAxisLabel().setText("time");
	plot1.getYAxis().getAxisLabel().setText("ofNoise(0.1 time)");
	plot1.setLogScale("xy");
	plot1.setPoints(points1a);
	plot1.setLineColor(ofColor(200, 200, 255));
	plot1.addLayer("layer 1", points1b);
	plot1.getLayer("layer 1").setLineColor(ofColor(150, 150, 255));
	plot1.addLayer("layer 2", points1c);
	plot1.getLayer("layer 2").setLineColor(ofColor(100, 100, 255));

	// Leave empty the points for the second plot. We will fill them in the update method
	trackingMouse = false;

	// Create the star path that will be used as points symbol
	star.moveTo(7, 0);

	for (int i = 1; i < 5; ++i) {
		star.lineTo(3 * cos(TWO_PI * i / 5 - TWO_PI / 10), 3 * sin(TWO_PI * i / 5 - TWO_PI / 10));
		star.lineTo(7 * cos(TWO_PI * i / 5), 7 * sin(TWO_PI * i / 5));
	}

	star.lineTo(3 * cos(TWO_PI - TWO_PI / 10), 3 * sin(TWO_PI - TWO_PI / 10));

	// Setup for the second plot
	plot2.setPos(460, 0);
	plot2.setDim(250, 250);
	plot2.getTitle().setText("Mouse position");
	plot2.getXAxis().getAxisLabel().setText("mouseX");
	plot2.getYAxis().getAxisLabel().setText("-mouseY");

	// Prepare the points for the third plot
	gaussianCounter = 0;
	generator = default_random_engine();
	distribution = normal_distribution<float>(0.0, 1.0);
	int gaussianStackSize = gaussianStack.size();

	for (int i = 0; i < 20; ++i) {
		int index = gaussianStackSize / 2.0 + distribution(generator);

		if (index >= 0 && index < gaussianStackSize) {
			++gaussianStack[index];
			++gaussianCounter;
		}
	}

	vector<ofxGPoint> points3;

	for (int i = 0; i < gaussianStackSize; ++i) {
		points3.emplace_back(i + 0.5 - gaussianStackSize / 2.0, gaussianStack[i] / gaussianCounter, "H" + to_string(i));
	}

	// Setup for the third plot
	plot3.setPos(0, 300);
	plot3.setDim(250, 250);
	plot3.setYLim(-0.02, 0.45);
	plot3.setXLim(-5, 5);
	plot3.getTitle().setText("Gaussian distribution (" + to_string(gaussianCounter) + " points)");
	plot3.getTitle().setTextAlignment(GRAFICA_LEFT_ALIGN);
	plot3.getTitle().setRelativePos(0);
	plot3.getYAxis().getAxisLabel().setText("Relative probability");
	plot3.getYAxis().getAxisLabel().setTextAlignment(GRAFICA_RIGHT_ALIGN);
	plot3.getYAxis().getAxisLabel().setRelativePos(1);
	plot3.setPoints(points3);
	plot3.startHistograms(GRAFICA_VERTICAL_HISTOGRAM);
	plot3.getHistogram().setDrawLabels(true);
	plot3.getHistogram().setRotateLabels(true);
	plot3.getHistogram().setBgColors( { ofColor(0, 0, 255, 50), ofColor(0, 0, 255, 100), ofColor(0, 0, 255, 150),
			ofColor(0, 0, 255, 200) });

	// Prepare the points for the fourth plot
	uniformCounter = 0;
	int uniformStackSize = uniformStack.size();

	for (int i = 0; i < 20; ++i) {
		int index = ofRandom(uniformStackSize);

		if (index >= 0 && index < uniformStackSize) {
			++uniformStack[index];
			++uniformCounter;
		}
	}

	vector<ofxGPoint> points4;

	for (int i = 0; i < uniformStackSize; ++i) {
		points4.emplace_back(i + 0.5 - uniformStackSize / 2.0, uniformStack[i] / uniformCounter,
				"point " + to_string(i));
	}

	// Setup for the fourth plot
	plot4.setPos(370, 350);
	plot4.setYLim(-0.005, 0.1);
	plot4.getTitle().setText("Uniform distribution (" + to_string(uniformCounter) + " points)");
	plot4.getTitle().setTextAlignment(GRAFICA_LEFT_ALIGN);
	plot4.getTitle().setRelativePos(0.1);
	plot4.getXAxis().getAxisLabel().setText("x variable");
	plot4.getYAxis().getAxisLabel().setText("Relative probability");
	plot4.setPoints(points4);
	plot4.startHistograms(GRAFICA_VERTICAL_HISTOGRAM);

	// Setup the mouse actions
	plot1.activatePanning();
	plot1.activateZooming(1.2, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_LEFT);
	plot1.activatePointLabels();
	plot2.activateZooming(1.5);
	plot3.activateCentering(OF_MOUSE_BUTTON_LEFT, OF_KEY_LEFT_CONTROL);
	plot4.activateZooming();
}

//--------------------------------------------------------------
void ofApp::update() {
	// Add a news points to the second plot
	if (trackingMouse) {
		if (plot2.getPointsRef().size() == 0) {
			plot2.addPoint(mouseX, -mouseY, "(" + to_string(mouseX) + " , " + to_string(-mouseY) + ")");
		} else {
			// Add a new point only if the mouse moved significantly
			const ofxGPoint lastPoint = plot2.getPointsRef().back();

			if (!lastPoint.isValid() || pow(lastPoint.getX() - mouseX, 2) + pow(lastPoint.getY() + mouseY, 2) > 2500) {
				plot2.addPoint(mouseX, -mouseY, "(" + to_string(mouseX) + " , " + to_string(-mouseY) + ")");
			}
		}
	}

	// Add one more point to the gaussian stack
	int gaussianStackSize = gaussianStack.size();
	int index = gaussianStackSize / 2.0 + distribution(generator);

	if (index >= 0 && index < gaussianStackSize) {
		gaussianStack[index]++;
		gaussianCounter++;

		vector<ofxGPoint> points3;

		for (int i = 0; i < gaussianStackSize; ++i) {
			points3.emplace_back(i + 0.5 - gaussianStackSize / 2.0, gaussianStack[i] / gaussianCounter,
					"H" + to_string(i));
		}

		plot3.setPoints(points3);
		plot3.getTitle().setText("Gaussian distribution (" + to_string(gaussianCounter) + " points)");
	}

	// Add one more point to the uniform stack
	int uniformStackSize = uniformStack.size();
	index = ofRandom(uniformStackSize);

	if (index >= 0 && index < uniformStackSize) {
		uniformStack[index]++;
		uniformCounter++;

		vector<ofxGPoint> points4;

		for (int i = 0; i < uniformStackSize; ++i) {
			points4.emplace_back(i + 0.5 - uniformStackSize / 2.0, uniformStack[i] / uniformCounter,
					"point " + to_string(i));
		}

		plot4.setPoints(points4);
		plot4.getTitle().setText("Uniform distribution (" + to_string(uniformCounter) + " points)");
	}

	// Actions over the fourth plot (scrolling)
	if (plot4.isOverBox(mouseX, mouseY)) {
		// Get the cursor relative position inside the inner plot area
		array<float, 2> relativePos = plot4.getRelativePlotPosAt(mouseX, mouseY);

		// Move the x axis
		if (relativePos[0] < 0.2) {
			plot4.moveHorizontalAxesLim(2);
		} else if (relativePos[0] > 0.8) {
			plot4.moveHorizontalAxesLim(-2);
		}

		// Move the y axis
		if (relativePos[1] < 0.2) {
			plot4.moveVerticalAxesLim(2);
		} else if (relativePos[1] > 0.8) {
			plot4.moveVerticalAxesLim(-2);
		}

		// Change the inner area bg color
		plot4.setBoxBgColor(ofColor(255, 255, 100, 50));
	} else {
		plot4.setBoxBgColor(ofColor(200, 50));
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	// Draw the first plot
	plot1.beginDraw();
	plot1.drawBackground();
	plot1.drawBox();
	plot1.drawXAxis();
	plot1.drawYAxis();
	plot1.drawTopAxis();
	plot1.drawRightAxis();
	plot1.drawTitle();
	plot1.drawFilledContours(GRAFICA_HORIZONTAL_CONTOUR, 0.05);
	plot1.drawPoint(ofxGPoint(65, 1.5), mug);
	plot1.drawPolygon(polygonPoints, ofColor(255, 200));
	plot1.drawLabels();
	plot1.endDraw();

	// Draw the second plot
	plot2.beginDraw();
	plot2.drawBackground();
	plot2.drawBox();
	plot2.drawXAxis();
	plot2.drawYAxis();
	plot2.drawTitle();
	plot2.drawGridLines(GRAFICA_BOTH_DIRECTIONS);
	plot2.drawLines();
	plot2.drawPoints(star);
	plot2.endDraw();

	// Draw the third plot
	plot3.beginDraw();
	plot3.drawBackground();
	plot3.drawBox();
	plot3.drawYAxis();
	plot3.drawTitle();
	plot3.drawHistograms();
	plot3.endDraw();

	// Draw the forth plot
	plot4.beginDraw();
	plot4.drawBackground();
	plot4.drawBox();
	plot4.drawXAxis();
	plot4.drawYAxis();
	plot4.drawTitle();
	plot4.drawHistograms();
	plot4.endDraw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	// Clear the points from the second plot if the user pressed the space bar
	if (key == ' ') {
		vector<ofxGPoint> emptyPoints;
		plot2.setPoints(emptyPoints);
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
	if (!trackingMouse) {
		trackingMouse = true;
	}
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
