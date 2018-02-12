#include "ofxGHistogram.h"
#include "ofxGConstants.h"
#include "ofxGPoint.h"
#include "ofMain.h"

ofxGHistogram::ofxGHistogram(ofxGHistogramType _type, const array<float, 2>& _dim, const vector<ofxGPoint>& _plotPoints) :
		type(_type), dim(_dim), plotPoints(_plotPoints) {
	// General properties
	visible = true;
	separations = {2};
	bgColors = {ofColor(150, 150, 255)};
	lineColors = {ofColor(100, 100, 255)};
	lineWidths = {1};

	// Labels properties
	labelsOffset = 8;
	drawLabels = false;
	rotateLabels = false;

	// Font properties
	fontName = "SansSerif.plain";
	fontColor = ofColor(0);
	fontSize = 11;
	font.load(fontName, fontSize);

	// Update the histogram containers
	updateArrays();
}

void ofxGHistogram::updateArrays() {
	int nPoints = plotPoints.size();
	leftSides.clear();
	rightSides.clear();

	if (nPoints == 1) {
		leftSides.push_back((type == GRAFICA_VERTICAL_HISTOGRAM) ? 0.2 * dim[0] : 0.2 * dim[1]);
		rightSides.push_back(leftSides[0]);
	} else if (nPoints > 1) {
		// Calculate the differences between consecutive points
		vector<float> differences;

		for (int i = 0; i < nPoints - 1; ++i) {
			if (plotPoints[i].isValid() && plotPoints[i + 1].isValid()) {
				float separation = separations[i % separations.size()];
				float pointsSeparation;

				if (type == GRAFICA_VERTICAL_HISTOGRAM) {
					pointsSeparation = plotPoints[i + 1].getX() - plotPoints[i].getX();
				} else {
					pointsSeparation = plotPoints[i + 1].getY() - plotPoints[i].getY();
				}

				if (pointsSeparation > 0) {
					differences.push_back((pointsSeparation - separation) / 2);
				} else {
					differences.push_back((pointsSeparation + separation) / 2);
				}
			} else {
				differences.push_back(0);
			}
		}

		// Fill the leftSides and rightSides vectors
		leftSides.push_back(differences[0]);
		rightSides.push_back(differences[0]);

		for (int i = 1; i < nPoints - 1; i++) {
			leftSides.push_back(differences[i - 1]);
			rightSides.push_back(differences[i]);
		}

		leftSides.push_back(differences[nPoints - 2]);
		rightSides.push_back(differences[nPoints - 2]);
	}
}

void ofxGHistogram::draw(const ofxGPoint& plotBasePoint) const {
	if (visible) {
		// Calculate the baseline for the histogram
		float baseline = 0;

		if (plotBasePoint.isValid()) {
			baseline = (type == GRAFICA_VERTICAL_HISTOGRAM) ? plotBasePoint.getY() : plotBasePoint.getX();
		}

		// Draw the rectangles
		ofPushStyle();
		ofSetRectMode(OF_RECTMODE_CORNER);

		for (vector<ofxGPoint>::size_type i = 0; i < plotPoints.size(); ++i) {
			if (plotPoints[i].isValid()) {
				// Obtain the corners
				float x1, x2, y1, y2;

				if (type == GRAFICA_VERTICAL_HISTOGRAM) {
					x1 = plotPoints[i].getX() - leftSides[i];
					x2 = plotPoints[i].getX() + rightSides[i];
					y1 = plotPoints[i].getY();
					y2 = baseline;
				} else {
					x1 = baseline;
					x2 = plotPoints[i].getX();
					y1 = plotPoints[i].getY() - leftSides[i];
					y2 = plotPoints[i].getY() + rightSides[i];
				}

				if (x1 < 0) {
					x1 = 0;
				} else if (x1 > dim[0]) {
					x1 = dim[0];
				}

				if (-y1 < 0) {
					y1 = 0;
				} else if (-y1 > dim[1]) {
					y1 = -dim[1];
				}

				if (x2 < 0) {
					x2 = 0;
				} else if (x2 > dim[0]) {
					x2 = dim[0];
				}

				if (-y2 < 0) {
					y2 = 0;
				} else if (-y2 > dim[1]) {
					y2 = -dim[1];
				}

				// Draw the rectangle
				float lineWidth = lineWidths[i % lineWidths.size()];
				ofSetLineWidth(lineWidth);

				if (abs(x2 - x1) > 2 * lineWidth && abs(y2 - y1) > 2 * lineWidth) {
					ofFill();
					ofSetColor(bgColors[i % bgColors.size()]);
					ofDrawRectangle(x1, y1, x2, y2);
					ofNoFill();
					ofSetColor(lineColors[i % lineColors.size()]);
					ofDrawRectangle(x1, y1, x2, y2);
				} else if ((type == GRAFICA_VERTICAL_HISTOGRAM && x2 != x1 && !(y1 == y2 && (y1 == 0 || y1 == -dim[1])))
						|| (type == GRAFICA_HORIZONTAL_HISTOGRAM && y2 != y1 && !(x1 == x2 && (x1 == 0 || x1 == dim[0])))) {
					ofFill();
					ofSetColor(bgColors[i % bgColors.size()]);
					ofDrawRectangle(x1, y1, x2, y2);
					ofSetColor(lineColors[i % lineColors.size()]);
					ofDrawLine(x1, y1, x1, y2);
					ofDrawLine(x2, y1, x2, y2);
					ofDrawLine(x1, y1, x2, y1);
					ofDrawLine(x1, y2, x2, y2);
				}
			}
		}

		ofPopStyle();

		// Draw the labels
		if (drawLabels) {
			drawHistLabels();
		}
	}
}

void ofxGHistogram::drawHistLabels() const {
	ofPushStyle();
	ofSetColor(fontColor);

	if (type == GRAFICA_VERTICAL_HISTOGRAM) {
		if (rotateLabels) {
			for (ofxGPoint p : plotPoints) {
				if (p.isValid() && p.getX() >= 0 && p.getX() <= dim[0]) {
					ofRectangle bounds = font.getStringBoundingBox(p.getLabel(), 0, 0);
					ofPushMatrix();
					ofTranslate(p.getX() + bounds.height / 2, labelsOffset + bounds.width);
					ofRotateZ(-90);
					font.drawString(p.getLabel(), 0, 0);
					ofPopMatrix();
				}
			}
		} else {
			for (ofxGPoint p : plotPoints) {
				if (p.isValid() && p.getX() >= 0 && p.getX() <= dim[0]) {
					ofRectangle bounds = font.getStringBoundingBox(p.getLabel(), 0, 0);
					font.drawString(p.getLabel(), p.getX() - bounds.width / 2, labelsOffset + bounds.height);
				}
			}
		}
	} else {
		if (rotateLabels) {
			for (ofxGPoint p : plotPoints) {
				if (p.isValid() && -p.getY() >= 0 && -p.getY() <= dim[1]) {
					ofRectangle bounds = font.getStringBoundingBox(p.getLabel(), 0, 0);
					ofPushMatrix();
					ofTranslate(-labelsOffset, p.getY() + bounds.width / 2);
					ofRotateZ(-90);
					font.drawString(p.getLabel(), 0, 0);
					ofPopMatrix();
				}
			}
		} else {
			for (ofxGPoint p : plotPoints) {
				if (p.isValid() && -p.getY() >= 0 && -p.getY() <= dim[1]) {
					ofRectangle bounds = font.getStringBoundingBox(p.getLabel(), 0, 0);
					font.drawString(p.getLabel(), -labelsOffset - bounds.width, p.getY() + bounds.height / 2);
				}
			}
		}
	}

	ofPopStyle();
}

void ofxGHistogram::setType(ofxGHistogramType newType) {
	if (newType != type) {
		type = newType;
		updateArrays();
	}
}

void ofxGHistogram::setDim(float xDim, float yDim) {
	if (xDim <= 0 || yDim <= 0) {
		throw invalid_argument("The dimensions should be larger than zero.");
	}

	dim = {xDim, yDim};
	updateArrays();
}

void ofxGHistogram::setDim(const array<float, 2>& newDim) {
	setDim(newDim[0], newDim[1]);
}

void ofxGHistogram::setPlotPoints(const vector<ofxGPoint>& newPlotPoints) {
	plotPoints = newPlotPoints;
	updateArrays();
}

void ofxGHistogram::setPlotPoint(int index, const ofxGPoint& newPlotPoint) {
	plotPoints[index] = newPlotPoint;
	updateArrays();
}

void ofxGHistogram::addPlotPoint(const ofxGPoint& newPlotPoint) {
	plotPoints.push_back(newPlotPoint);
	updateArrays();
}

void ofxGHistogram::addPlotPoint(int index, ofxGPoint& newPlotPoint) {
	plotPoints.insert(plotPoints.begin() + index, newPlotPoint);
	updateArrays();
}

void ofxGHistogram::addPlotPoints(const vector<ofxGPoint>& newPlotPoints) {
	plotPoints.insert(plotPoints.end(), newPlotPoints.begin(), newPlotPoints.end());
	updateArrays();
}

void ofxGHistogram::removePlotPoint(int index) {
	plotPoints.erase(plotPoints.begin() + index);
	updateArrays();
}

void ofxGHistogram::setSeparations(const vector<float>& newSeparations) {
	separations = newSeparations;
	updateArrays();
}

void ofxGHistogram::setBgColors(const vector<ofColor>& newBgColors) {
	bgColors = newBgColors;
}

void ofxGHistogram::setLineColors(const vector<ofColor>& newLineColors) {
	lineColors = newLineColors;
}

void ofxGHistogram::setLineWidths(const vector<float>& newLineWidths) {
	lineWidths = newLineWidths;
}

void ofxGHistogram::setVisible(bool newVisible) {
	visible = newVisible;
}

void ofxGHistogram::setLabelsOffset(float newLabelsOffset) {
	labelsOffset = newLabelsOffset;
}

void ofxGHistogram::setDrawLabels(bool newDrawLabels) {
	drawLabels = newDrawLabels;
}

void ofxGHistogram::setRotateLabels(bool newRotateLabels) {
	rotateLabels = newRotateLabels;
}

void ofxGHistogram::setFontName(const string& newFontName) {
	fontName = newFontName;
	font.load(fontName, fontSize);
}

void ofxGHistogram::setFontColor(const ofColor& newFontColor) {
	fontColor = newFontColor;
}

void ofxGHistogram::setFontSize(int newFontSize) {
	if (newFontSize <= 0) {
		throw invalid_argument("The font size should be larger than zero.");
	}

	fontSize = newFontSize;
	font.load(fontName, fontSize);
}

void ofxGHistogram::setFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize) {
	if (newFontSize <= 0) {
		throw invalid_argument("The font size should be larger than zero.");
	}

	fontName = newFontName;
	fontColor = newFontColor;
	fontSize = newFontSize;
	font.load(fontName, fontSize);
}
