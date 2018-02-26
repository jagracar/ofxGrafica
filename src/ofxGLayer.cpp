#include "ofxGLayer.h"
#include "ofxGConstants.h"
#include "ofxGPoint.h"
#include "ofxGHistogram.h"
#include "ofMain.h"

ofxGLayer::ofxGLayer(const string& _id, const array<float, 2>& _dim, const array<float, 2>& _xLim,
		const array<float, 2>& _yLim, bool _xLogScale, bool _yLogScale) :
		id(_id), dim(_dim), xLim(_xLim), yLim(_yLim), xLogScale(_xLogScale), yLogScale(_yLogScale) {
	// Do some sanity checks
	if ((xLogScale && (xLim[0] <= 0 || xLim[1] <= 0)) || (yLogScale && (yLim[0] <= 0 || yLim[1] <= 0))) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	// Points properties
	pointColors = {ofColor(255, 0, 0, 150)};
	pointSizes = {3.5};

	// Line properties
	lineColor = ofColor(0, 150);
	lineWidth = 1;

	// Histogram properties
	histBasePoint = ofxGPoint(0, 0);
	histIsActive = false;

	// Labels properties
	labelBgColor = ofColor(255, 200);
	labelSeparation = {7, 7};

	// Font properties
	fontName = OF_TTF_SANS;
	fontColor = ofColor(0);
	fontSize = 8;
	fontMakeContours = false;
	font.load(fontName, fontSize, true, true, fontMakeContours);
}

bool ofxGLayer::isId(const string& someId) const {
	return id == someId;
}

float ofxGLayer::valueToXPlot(float x) const {
	if (xLogScale) {
		return dim[0] * log10(x / xLim[0]) / log10(xLim[1] / xLim[0]);
	} else {
		return dim[0] * (x - xLim[0]) / (xLim[1] - xLim[0]);
	}
}

float ofxGLayer::valueToYPlot(float y) const {
	if (yLogScale) {
		return -dim[1] * log(y / yLim[0]) / log(yLim[1] / yLim[0]);
	} else {
		return -dim[1] * (y - yLim[0]) / (yLim[1] - yLim[0]);
	}
}

array<float, 2> ofxGLayer::valueToPlot(float x, float y) const {
	return {valueToXPlot(x), valueToYPlot(y)};
}

ofxGPoint ofxGLayer::valueToPlot(const ofxGPoint& point) const {
	return ofxGPoint(valueToXPlot(point.getX()), valueToYPlot(point.getY()), point.getLabel());
}

vector<ofxGPoint> ofxGLayer::valueToPlot(const vector<ofxGPoint>& pts) const {
	vector<ofxGPoint> plotPts;

	if (xLogScale && yLogScale) {
		float xScalingFactor = dim[0] / log10(xLim[1] / xLim[0]);
		float yScalingFactor = -dim[1] / log10(yLim[1] / yLim[0]);

		for (const ofxGPoint& p : pts) {
			float xPlot = log10(p.getX() / xLim[0]) * xScalingFactor;
			float yPlot = log10(p.getY() / yLim[0]) * yScalingFactor;
			plotPts.emplace_back(xPlot, yPlot, p.getLabel());
		}
	} else if (xLogScale) {
		float xScalingFactor = dim[0] / log10(xLim[1] / xLim[0]);
		float yScalingFactor = -dim[1] / (yLim[1] - yLim[0]);

		for (const ofxGPoint& p : pts) {
			float xPlot = log10(p.getX() / xLim[0]) * xScalingFactor;
			float yPlot = (p.getY() - yLim[0]) * yScalingFactor;
			plotPts.emplace_back(xPlot, yPlot, p.getLabel());
		}
	} else if (yLogScale) {
		float xScalingFactor = dim[0] / (xLim[1] - xLim[0]);
		float yScalingFactor = -dim[1] / log10(yLim[1] / yLim[0]);

		for (const ofxGPoint& p : pts) {
			float xPlot = (p.getX() - xLim[0]) * xScalingFactor;
			float yPlot = log10(p.getY() / yLim[0]) * yScalingFactor;
			plotPts.emplace_back(xPlot, yPlot, p.getLabel());
		}
	} else {
		float xScalingFactor = dim[0] / (xLim[1] - xLim[0]);
		float yScalingFactor = -dim[1] / (yLim[1] - yLim[0]);

		for (const ofxGPoint& p : pts) {
			float xPlot = (p.getX() - xLim[0]) * xScalingFactor;
			float yPlot = (p.getY() - yLim[0]) * yScalingFactor;
			plotPts.emplace_back(xPlot, yPlot, p.getLabel());
		}
	}

	return plotPts;
}

void ofxGLayer::updatePlotPoints() {
	plotPoints.clear();

	if (xLogScale && yLogScale) {
		float xScalingFactor = dim[0] / log10(xLim[1] / xLim[0]);
		float yScalingFactor = -dim[1] / log10(yLim[1] / yLim[0]);

		for (const ofxGPoint& p : points) {
			float xPlot = log10(p.getX() / xLim[0]) * xScalingFactor;
			float yPlot = log10(p.getY() / yLim[0]) * yScalingFactor;
			plotPoints.emplace_back(xPlot, yPlot, p.getLabel());
		}
	} else if (xLogScale) {
		float xScalingFactor = dim[0] / log10(xLim[1] / xLim[0]);
		float yScalingFactor = -dim[1] / (yLim[1] - yLim[0]);

		for (const ofxGPoint& p : points) {
			float xPlot = log10(p.getX() / xLim[0]) * xScalingFactor;
			float yPlot = (p.getY() - yLim[0]) * yScalingFactor;
			plotPoints.emplace_back(xPlot, yPlot, p.getLabel());
		}
	} else if (yLogScale) {
		float xScalingFactor = dim[0] / (xLim[1] - xLim[0]);
		float yScalingFactor = -dim[1] / log10(yLim[1] / yLim[0]);

		for (const ofxGPoint& p : points) {
			float xPlot = (p.getX() - xLim[0]) * xScalingFactor;
			float yPlot = log10(p.getY() / yLim[0]) * yScalingFactor;
			plotPoints.emplace_back(xPlot, yPlot, p.getLabel());
		}
	} else {
		float xScalingFactor = dim[0] / (xLim[1] - xLim[0]);
		float yScalingFactor = -dim[1] / (yLim[1] - yLim[0]);

		for (const ofxGPoint& p : points) {
			float xPlot = (p.getX() - xLim[0]) * xScalingFactor;
			float yPlot = (p.getY() - yLim[0]) * yScalingFactor;
			plotPoints.emplace_back(xPlot, yPlot, p.getLabel());
		}
	}
}

float ofxGLayer::xPlotToValue(float xPlot) const {
	if (xLogScale) {
		return pow(10, log10(xLim[0]) + log10(xLim[1] / xLim[0]) * xPlot / dim[0]);
	} else {
		return xLim[0] + (xLim[1] - xLim[0]) * xPlot / dim[0];
	}
}

float ofxGLayer::yPlotToValue(float yPlot) const {
	if (yLogScale) {
		return pow(10, log10(yLim[0]) - log10(yLim[1] / yLim[0]) * yPlot / dim[1]);
	} else {
		return yLim[0] - (yLim[1] - yLim[0]) * yPlot / dim[1];
	}
}

array<float, 2> ofxGLayer::plotToValue(float xPlot, float yPlot) const {
	return {xPlotToValue(xPlot), yPlotToValue(yPlot)};
}

bool ofxGLayer::isInside(float xPlot, float yPlot) const {
	return (xPlot >= 0) && (xPlot <= dim[0]) && (-yPlot >= 0) && (-yPlot <= dim[1]);
}

bool ofxGLayer::isInside(const ofxGPoint& plotPoint) const {
	return plotPoint.isValid() ? isInside(plotPoint.getX(), plotPoint.getY()) : false;
}

vector<bool> ofxGLayer::isInside(const vector<ofxGPoint>& plotPts) const {
	vector<bool> pointsInside;

	for (const ofxGPoint& p : plotPts) {
		pointsInside.push_back(isInside(p));
	}

	return pointsInside;
}

void ofxGLayer::updateInsideList() {
	inside.clear();

	for (const ofxGPoint& p : plotPoints) {
		inside.push_back(isInside(p));
	}
}

vector<ofxGPoint>::size_type ofxGLayer::getPointIndexAtPlotPos(float xPlot, float yPlot) const {
	vector<ofxGPoint>::size_type pointIndex = plotPoints.size();

	if (isInside(xPlot, yPlot)) {
		float minDistSq = 1000000;

		for (vector<ofxGPoint>::size_type i = 0; i < plotPoints.size(); ++i) {
			if (inside[i]) {
				float distSq = pow(plotPoints[i].getX() - xPlot, 2) + pow(plotPoints[i].getY() - yPlot, 2);

				if (distSq < max(pow(pointSizes[i % pointSizes.size()], 2), 25.0)) {
					if (distSq < minDistSq) {
						minDistSq = distSq;
						pointIndex = i;
					}
				}
			}
		}
	}

	return pointIndex;
}

const ofxGPoint* ofxGLayer::getPointAtPlotPos(float xPlot, float yPlot) const {
	vector<ofxGPoint>::size_type pointIndex = getPointIndexAtPlotPos(xPlot, yPlot);

	return (pointIndex < points.size()) ? &points[pointIndex] : nullptr;
}

int ofxGLayer::obtainBoxIntersections(const ofxGPoint& plotPoint1, const ofxGPoint& plotPoint2) {
	int nCuts = 0;

	if (plotPoint1.isValid() && plotPoint2.isValid()) {
		float x1 = plotPoint1.getX();
		float y1 = plotPoint1.getY();
		float x2 = plotPoint2.getX();
		float y2 = plotPoint2.getY();
		bool inside1 = isInside(x1, y1);
		bool inside2 = isInside(x2, y2);

		// Check if the line between the two points could cut the box borders
		bool dontCut = (inside1 && inside2) || (x1 < 0 && x2 < 0) || (x1 > dim[0] && x2 > dim[0])
				|| (-y1 < 0 && -y2 < 0) || (-y1 > dim[1] && -y2 > dim[1]);

		if (!dontCut) {
			// Obtain the axis cuts of the line that cross the two points
			float deltaX = x2 - x1;
			float deltaY = y2 - y1;

			if (deltaX == 0) {
				nCuts = 2;
				cuts[0][0] = x1;
				cuts[0][1] = 0;
				cuts[1][0] = x1;
				cuts[1][1] = -dim[1];
			} else if (deltaY == 0) {
				nCuts = 2;
				cuts[0][0] = 0;
				cuts[0][1] = y1;
				cuts[1][0] = dim[0];
				cuts[1][1] = y1;
			} else {
				// Obtain the straight line (y = yCut + slope*x) that crosses the two points
				float slope = deltaY / deltaX;
				float yCut = y1 - slope * x1;

				// Calculate the axis cuts of that line
				nCuts = 4;
				cuts[0][0] = -yCut / slope;
				cuts[0][1] = 0;
				cuts[1][0] = (-dim[1] - yCut) / slope;
				cuts[1][1] = -dim[1];
				cuts[2][0] = 0;
				cuts[2][1] = yCut;
				cuts[3][0] = dim[0];
				cuts[3][1] = yCut + slope * dim[0];
			}

			// Select only the cuts that fall inside the box and are located between the two points
			nCuts = getValidCuts(cuts, nCuts, plotPoint1, plotPoint2);

			// Make sure we have the correct number of cuts
			if (inside1 || inside2) {
				// One of the points is inside. We should have one cut only
				if (nCuts != 1) {
					const ofxGPoint& pointInside = (inside1) ? plotPoint1 : plotPoint2;

					// If too many cuts
					if (nCuts > 1) {
						nCuts = removeDuplicatedCuts(cuts, nCuts, 0);

						if (nCuts > 1) {
							nCuts = removePointFromCuts(cuts, nCuts, pointInside, 0);

							// In case of rounding number errors
							if (nCuts > 1) {
								nCuts = removeDuplicatedCuts(cuts, nCuts, 0.001f);

								if (nCuts > 1) {
									nCuts = removePointFromCuts(cuts, nCuts, pointInside, 0.001f);
								}
							}
						}
					}

					// If the cut is missing, then it must be equal to the point inside
					if (nCuts == 0) {
						nCuts = 1;
						cuts[0][0] = pointInside.getX();
						cuts[0][1] = pointInside.getY();
					}
				}
			} else {
				// Both points are outside. We should have either two cuts or none
				if (nCuts > 2) {
					nCuts = removeDuplicatedCuts(cuts, nCuts, 0);

					// In case of rounding number errors
					if (nCuts > 2) {
						nCuts = removeDuplicatedCuts(cuts, nCuts, 0.001f);
					}
				}

				// If we have two cuts, order them (the closest to the first point goes first)
				if (nCuts == 2) {
					if ((pow(cuts[0][0] - x1, 2) + pow(cuts[0][1] - y1, 2))
							> (pow(cuts[1][0] - x1, 2) + pow(cuts[1][1] - y1, 2))) {
						cuts[2][0] = cuts[0][0];
						cuts[2][1] = cuts[0][1];
						cuts[0][0] = cuts[1][0];
						cuts[0][1] = cuts[1][1];
						cuts[1][0] = cuts[2][0];
						cuts[1][1] = cuts[2][1];
					}
				}

				// If one cut is missing, add the same one twice
				if (nCuts == 1) {
					nCuts = 2;
					cuts[1][0] = cuts[0][0];
					cuts[1][1] = cuts[0][1];
				}
			}

			// Some sanity checks
			if ((inside1 || inside2) && nCuts != 1) {
				ofLogError("There should be one cut!!!");
			} else if (!inside1 && !inside2 && nCuts != 0 && nCuts != 2) {
				ofLogError("There should be either 0 or 2 cuts!!! " + to_string(nCuts) + " were found");
			}
		}
	}

	return nCuts;
}

int ofxGLayer::getValidCuts(array<array<float, 2>, 4>& cuts, int nCuts, const ofxGPoint& plotPoint1,
		const ofxGPoint& plotPoint2) {
	float x1 = plotPoint1.getX();
	float y1 = plotPoint1.getY();
	float x2 = plotPoint2.getX();
	float y2 = plotPoint2.getY();
	float deltaX = abs(x2 - x1);
	float deltaY = abs(y2 - y1);
	int counter = 0;

	for (int i = 0; i < nCuts; ++i) {
		// Check that the cut is inside the inner plotting area
		if (isInside(cuts[i][0], cuts[i][1])) {
			// Check that the cut falls between the two points
			if (abs(cuts[i][0] - x1) <= deltaX && abs(cuts[i][1] - y1) <= deltaY && abs(cuts[i][0] - x2) <= deltaX
					&& abs(cuts[i][1] - y2) <= deltaY) {
				cuts[counter][0] = cuts[i][0];
				cuts[counter][1] = cuts[i][1];
				++counter;
			}
		}
	}

	return counter;
}

int ofxGLayer::removeDuplicatedCuts(array<array<float, 2>, 4>& cuts, int nCuts, float tolerance) {
	int counter = 0;

	for (int i = 0; i < nCuts; ++i) {
		bool repeated = false;

		for (int j = 0; j < counter; ++j) {
			if (abs(cuts[j][0] - cuts[i][0]) <= tolerance && abs(cuts[j][1] - cuts[i][1]) <= tolerance) {
				repeated = true;
				break;
			}
		}

		if (!repeated) {
			cuts[counter][0] = cuts[i][0];
			cuts[counter][1] = cuts[i][1];
			++counter;
		}
	}

	return counter;
}

int ofxGLayer::removePointFromCuts(array<array<float, 2>, 4>& cuts, int nCuts, const ofxGPoint& plotPoint,
		float tolerance) {
	float x = plotPoint.getX();
	float y = plotPoint.getY();
	int counter = 0;

	for (int i = 0; i < nCuts; ++i) {
		if (abs(cuts[i][0] - x) > tolerance || abs(cuts[i][1] - y) > tolerance) {
			cuts[counter][0] = cuts[i][0];
			cuts[counter][1] = cuts[i][1];
			++counter;
		}
	}

	return counter;
}

void ofxGLayer::startHistogram(ofxGHistogramType histType) {
	hist = ofxGHistogram(histType, dim, plotPoints);
	histIsActive = true;
}

void ofxGLayer::stopHistogram() {
	histIsActive = false;
}

void ofxGLayer::drawPoints() const {
	int nPoints = plotPoints.size();
	int nColors = pointColors.size();
	int nSizes = pointSizes.size();

	ofPushStyle();
	ofFill();

	if (nColors == 1 && nSizes == 1) {
		ofSetColor(pointColors[0]);

		for (int i = 0; i < nPoints; ++i) {
			if (inside[i]) {
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(), pointSizes[0]);
			}
		}
	} else if (nColors == 1) {
		ofSetColor(pointColors[0]);

		for (int i = 0; i < nPoints; ++i) {
			if (inside[i]) {
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(), pointSizes[i % nSizes]);
			}
		}
	} else if (nSizes == 1) {
		for (int i = 0; i < nPoints; ++i) {
			if (inside[i]) {
				ofSetColor(pointColors[i % nColors]);
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(), pointSizes[0]);
			}
		}
	} else {
		for (int i = 0; i < nPoints; ++i) {
			if (inside[i]) {
				ofSetColor(pointColors[i % nColors]);
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(), pointSizes[i % nSizes]);
			}
		}
	}

	ofPopStyle();
}

void ofxGLayer::drawPoints(ofPath& pointShape) const {
	int nPoints = plotPoints.size();
	int nColors = pointColors.size();

	if (nColors == 1) {
		pointShape.setColor(pointColors[0]);

		for (int i = 0; i < nPoints; ++i) {
			if (inside[i]) {
				pointShape.draw(plotPoints[i].getX(), plotPoints[i].getY());
			}
		}
	} else {
		for (int i = 0; i < nPoints; ++i) {
			if (inside[i]) {
				pointShape.setColor(pointColors[i % nColors]);
				pointShape.draw(plotPoints[i].getX(), plotPoints[i].getY());
			}
		}
	}
}

void ofxGLayer::drawPoints(const ofImage& pointImg) const {
	int nPoints = plotPoints.size();
	int imgWidth = pointImg.getWidth();
	int imgHeight = pointImg.getHeight();

	for (int i = 0; i < nPoints; ++i) {
		if (inside[i]) {
			pointImg.draw(plotPoints[i].getX() - imgWidth / 2, plotPoints[i].getY() - imgHeight / 2);
		}
	}
}

void ofxGLayer::drawPoint(const ofxGPoint& point, const ofColor& pointColor, float pointSize) const {
	float xPlot = valueToXPlot(point.getX());
	float yPlot = valueToYPlot(point.getY());

	if (isInside(xPlot, yPlot)) {
		ofPushStyle();
		ofFill();
		ofSetColor(pointColor);
		ofDrawCircle(xPlot, yPlot, pointSize);
		ofPopStyle();
	}
}

void ofxGLayer::drawPoint(const ofxGPoint& point) const {
	drawPoint(point, pointColors[0], pointSizes[0]);
}

void ofxGLayer::drawPoint(const ofxGPoint& point, const ofPath& pointShape) const {
	float xPlot = valueToXPlot(point.getX());
	float yPlot = valueToYPlot(point.getY());

	if (isInside(xPlot, yPlot)) {
		pointShape.draw(xPlot, yPlot);
	}
}

void ofxGLayer::drawPoint(const ofxGPoint& point, ofPath& pointShape, const ofColor& pointColor) const {
	float xPlot = valueToXPlot(point.getX());
	float yPlot = valueToYPlot(point.getY());

	if (isInside(xPlot, yPlot)) {
		pointShape.setColor(pointColor);
		pointShape.draw(xPlot, yPlot);
	}
}

void ofxGLayer::drawPoint(const ofxGPoint& point, const ofImage& pointImg) const {
	float xPlot = valueToXPlot(point.getX());
	float yPlot = valueToYPlot(point.getY());

	if (isInside(xPlot, yPlot)) {
		pointImg.draw(xPlot - pointImg.getWidth() / 2, yPlot - pointImg.getHeight() / 2);
	}
}

void ofxGLayer::drawLines() {
	if (plotPoints.size() > 1) {
		ofPushStyle();
		ofSetColor(lineColor);
		ofSetLineWidth(lineWidth);

		for (vector<ofxGPoint>::size_type i = 0; i < plotPoints.size() - 1; ++i) {
			if (inside[i] && inside[i + 1]) {
				ofDrawLine(plotPoints[i].getX(), plotPoints[i].getY(), plotPoints[i + 1].getX(),
						plotPoints[i + 1].getY());
			} else if (plotPoints[i].isValid() && plotPoints[i + 1].isValid()) {
				// At least one of the points is outside the inner region.
				// Obtain the valid line box intersections
				int nCuts = obtainBoxIntersections(plotPoints[i], plotPoints[i + 1]);

				if (inside[i]) {
					ofDrawLine(plotPoints[i].getX(), plotPoints[i].getY(), cuts[0][0], cuts[0][1]);
				} else if (inside[i + 1]) {
					ofDrawLine(cuts[0][0], cuts[0][1], plotPoints[i + 1].getX(), plotPoints[i + 1].getY());
				} else if (nCuts >= 2) {
					ofDrawLine(cuts[0][0], cuts[0][1], cuts[1][0], cuts[1][1]);
				}
			}
		}

		ofPopStyle();
	}
}

void ofxGLayer::drawLine(const ofxGPoint& point1, const ofxGPoint& point2, const ofColor& lc, float lw) {
	ofxGPoint plotPoint1 = valueToPlot(point1);
	ofxGPoint plotPoint2 = valueToPlot(point2);

	if (plotPoint1.isValid() && plotPoint2.isValid()) {
		bool inside1 = isInside(plotPoint1);
		bool inside2 = isInside(plotPoint2);

		ofPushStyle();
		ofSetColor(lc);
		ofSetLineWidth(lw);

		if (inside1 && inside2) {
			ofDrawLine(plotPoint1.getX(), plotPoint1.getY(), plotPoint2.getX(), plotPoint2.getY());
		} else {
			// At least one of the points is outside the inner region. Obtain the valid line box intersections
			int nCuts = obtainBoxIntersections(plotPoint1, plotPoint2);

			if (inside1) {
				ofDrawLine(plotPoint1.getX(), plotPoint1.getY(), cuts[0][0], cuts[0][1]);
			} else if (inside2) {
				ofDrawLine(cuts[0][0], cuts[0][1], plotPoint2.getX(), plotPoint2.getY());
			} else if (nCuts >= 2) {
				ofDrawLine(cuts[0][0], cuts[0][1], cuts[1][0], cuts[1][1]);
			}
		}

		ofPopStyle();
	}
}

void ofxGLayer::drawLine(const ofxGPoint& point1, const ofxGPoint& point2) {
	drawLine(point1, point2, lineColor, lineWidth);
}

void ofxGLayer::drawLine(float slope, float yCut, const ofColor& lc, float lw) {
	if (xLogScale && yLogScale) {
		ofxGPoint point1 = ofxGPoint(xLim[0], pow(10, slope * log10(xLim[0]) + yCut));
		ofxGPoint point2 = ofxGPoint(xLim[1], pow(10, slope * log10(xLim[1]) + yCut));
		drawLine(point1, point2, lc, lw);
	} else if (xLogScale) {
		ofxGPoint point1 = ofxGPoint(xLim[0], slope * log10(xLim[0]) + yCut);
		ofxGPoint point2 = ofxGPoint(xLim[1], slope * log10(xLim[1]) + yCut);
		drawLine(point1, point2, lc, lw);
	} else if (yLogScale) {
		ofxGPoint point1 = ofxGPoint(xLim[0], pow(10, slope * xLim[0] + yCut));
		ofxGPoint point2 = ofxGPoint(xLim[1], pow(10, slope * xLim[1] + yCut));
		drawLine(point1, point2, lc, lw);
	} else {
		ofxGPoint point1 = ofxGPoint(xLim[0], slope * xLim[0] + yCut);
		ofxGPoint point2 = ofxGPoint(xLim[1], slope * xLim[1] + yCut);
		drawLine(point1, point2, lc, lw);
	}
}

void ofxGLayer::drawLine(float slope, float yCut) {
	drawLine(slope, yCut, lineColor, lineWidth);
}

void ofxGLayer::drawHorizontalLine(float value, const ofColor& lc, float lw) const {
	float yPlot = valueToYPlot(value);

	if (isfinite(yPlot) && -yPlot >= 0 && -yPlot <= dim[1]) {
		ofPushStyle();
		ofSetColor(lc);
		ofSetLineWidth(lw);
		ofDrawLine(0, yPlot, dim[0], yPlot);
		ofPopStyle();
	}
}

void ofxGLayer::drawHorizontalLine(float value) const {
	drawHorizontalLine(value, lineColor, lineWidth);
}

void ofxGLayer::drawVerticalLine(float value, const ofColor& lc, float lw) const {
	float xPlot = valueToXPlot(value);

	if (isfinite(xPlot) && xPlot >= 0 && xPlot <= dim[0]) {
		ofPushStyle();
		ofSetColor(lc);
		ofSetLineWidth(lw);
		ofDrawLine(xPlot, 0, xPlot, -dim[1]);
		ofPopStyle();
	}
}

void ofxGLayer::drawVerticalLine(float value) const {
	drawVerticalLine(value, lineColor, lineWidth);
}

void ofxGLayer::drawFilledContour(ofxGContourType contourType, float referenceValue) {
	// Get the points that compose the shape
	vector<ofxGPoint> shapePoints;

	if (contourType == GRAFICA_HORIZONTAL_CONTOUR) {
		shapePoints = getHorizontalShape(referenceValue);
	} else {
		shapePoints = getVerticalShape(referenceValue);
	}

	// Draw the shape
	if (shapePoints.size() > 0) {
		ofPushStyle();
		ofFill();
		ofSetColor(lineColor);

		ofBeginShape();

		for (const ofxGPoint& p : shapePoints) {
			if (p.isValid()) {
				ofVertex(p.getX(), p.getY());
			}
		}

		ofEndShape(true);

		ofPopStyle();
	}
}

vector<ofxGPoint> ofxGLayer::getHorizontalShape(float referenceValue) {
	// Collect the points and cuts inside the box
	int nPoints = plotPoints.size();
	vector<ofxGPoint> shapePoints;
	int indexFirstPoint = -1;
	int indexLastPoint = -1;

	for (int i = 0; i < nPoints; ++i) {
		if (plotPoints[i].isValid()) {
			bool addedPoints = false;

			// Add the point if it's inside the box
			if (inside[i]) {
				shapePoints.emplace_back(plotPoints[i].getX(), plotPoints[i].getY(), "normal point");
				addedPoints = true;
			} else if (plotPoints[i].getX() >= 0 && plotPoints[i].getX() <= dim[0]) {
				// If it's outside, add the projection of the point on the
				// horizontal axes
				if (-plotPoints[i].getY() < 0) {
					shapePoints.emplace_back(plotPoints[i].getX(), 0, "projection");
					addedPoints = true;
				} else {
					shapePoints.emplace_back(plotPoints[i].getX(), -dim[1], "projection");
					addedPoints = true;
				}
			}

			// Add the box cuts if there is any
			int nextIndex = i + 1;

			while (nextIndex < nPoints - 1 && !plotPoints[nextIndex].isValid()) {
				++nextIndex;
			}

			if (nextIndex < nPoints && plotPoints[nextIndex].isValid()) {
				int nCuts = obtainBoxIntersections(plotPoints[i], plotPoints[nextIndex]);

				for (int j = 0; j < nCuts; ++j) {
					shapePoints.emplace_back(cuts[j][0], cuts[j][1], "cut");
					addedPoints = true;
				}
			}

			if (addedPoints) {
				if (indexFirstPoint < 0) {
					indexFirstPoint = i;
				}

				indexLastPoint = i;
			}
		}
	}

	// Continue if there are points in the shape
	if (shapePoints.size() > 0) {
		// Calculate the starting point
		ofxGPoint startPoint = shapePoints[0];

		if (startPoint.getX() != 0 && startPoint.getX() != dim[0]) {
			if (startPoint.getLabel() == "cut") {
				if (plotPoints[indexFirstPoint].getX() < 0) {
					startPoint.setX(0);
					startPoint.setLabel("extreme");
				} else {
					startPoint.setX(dim[0]);
					startPoint.setLabel("extreme");
				}
			} else if (indexFirstPoint != 0) {
				// Get the previous valid point
				int prevIndex = indexFirstPoint - 1;

				while (prevIndex > 0 && !plotPoints[prevIndex].isValid()) {
					--prevIndex;
				}

				if (plotPoints[prevIndex].isValid()) {
					if (plotPoints[prevIndex].getX() < 0) {
						startPoint.setX(0);
						startPoint.setLabel("extreme");
					} else {
						startPoint.setX(dim[0]);
						startPoint.setLabel("extreme");
					}
				}
			}
		}

		// Calculate the end point
		ofxGPoint endPoint = shapePoints.back();

		if (endPoint.getX() != 0 && endPoint.getX() != dim[0] && indexLastPoint != nPoints - 1) {
			int nextIndex = indexLastPoint + 1;

			while (nextIndex < nPoints - 1 && !plotPoints[nextIndex].isValid()) {
				++nextIndex;
			}

			if (plotPoints[nextIndex].isValid()) {
				if (plotPoints[nextIndex].getX() < 0) {
					endPoint.setX(0);
					endPoint.setLabel("extreme");
				} else {
					endPoint.setX(dim[0]);
					endPoint.setLabel("extreme");
				}
			}
		}

		// Add the end point if it's a new extreme
		if (endPoint.getLabel() == "extreme") {
			shapePoints.push_back(endPoint);
		}

		// Add the reference connections
		if (yLogScale && referenceValue <= 0) {
			referenceValue = min(yLim[0], yLim[1]);
		}

		array<float, 2> plotReference = valueToPlot(1, referenceValue);

		if (-plotReference[1] < 0) {
			shapePoints.emplace_back(endPoint.getX(), 0);
			shapePoints.emplace_back(startPoint.getX(), 0);
		} else if (-plotReference[1] > dim[1]) {
			shapePoints.emplace_back(endPoint.getX(), -dim[1]);
			shapePoints.emplace_back(startPoint.getX(), -dim[1]);
		} else {
			shapePoints.emplace_back(endPoint.getX(), plotReference[1]);
			shapePoints.emplace_back(startPoint.getX(), plotReference[1]);
		}

		// Add the starting point if it's a new extreme
		if (startPoint.getLabel() == "extreme") {
			shapePoints.push_back(startPoint);
		}
	}

	return shapePoints;
}

vector<ofxGPoint> ofxGLayer::getVerticalShape(float referenceValue) {
	// Collect the points and cuts inside the box
	int nPoints = plotPoints.size();
	vector<ofxGPoint> shapePoints;
	int indexFirstPoint = -1;
	int indexLastPoint = -1;

	for (int i = 0; i < nPoints; ++i) {
		if (plotPoints[i].isValid()) {
			bool addedPoints = false;

			// Add the point if it's inside the box
			if (inside[i]) {
				shapePoints.emplace_back(plotPoints[i].getX(), plotPoints[i].getY(), "normal point");
				addedPoints = true;
			} else if (-plotPoints[i].getY() >= 0 && -plotPoints[i].getY() <= dim[1]) {
				// If it's outside, add the projection of the point on the
				// vertical axes
				if (plotPoints[i].getX() < 0) {
					shapePoints.emplace_back(0, plotPoints[i].getY(), "projection");
					addedPoints = true;
				} else {
					shapePoints.emplace_back(dim[0], plotPoints[i].getY(), "projection");
					addedPoints = true;
				}
			}

			// Add the box cuts if there is any
			int nextIndex = i + 1;

			while (nextIndex < nPoints - 1 && !plotPoints[nextIndex].isValid()) {
				++nextIndex;
			}

			if (nextIndex < nPoints && plotPoints[nextIndex].isValid()) {
				int nCuts = obtainBoxIntersections(plotPoints[i], plotPoints[nextIndex]);

				for (int j = 0; j < nCuts; ++j) {
					shapePoints.emplace_back(cuts[j][0], cuts[j][1], "cut");
					addedPoints = true;
				}
			}

			if (addedPoints) {
				if (indexFirstPoint < 0) {
					indexFirstPoint = i;
				}

				indexLastPoint = i;
			}
		}
	}

	// Continue if there are points in the shape
	if (shapePoints.size() > 0) {
		// Calculate the starting point
		ofxGPoint startPoint = shapePoints[0];

		if (startPoint.getY() != 0 && startPoint.getY() != -dim[1]) {
			if (startPoint.getLabel() == "cut") {
				if (-plotPoints[indexFirstPoint].getY() < 0) {
					startPoint.setY(0);
					startPoint.setLabel("extreme");
				} else {
					startPoint.setY(-dim[1]);
					startPoint.setLabel("extreme");
				}
			} else if (indexFirstPoint != 0) {
				// Get the previous valid point
				int prevIndex = indexFirstPoint - 1;

				while (prevIndex > 0 && !plotPoints[prevIndex].isValid()) {
					--prevIndex;
				}

				if (plotPoints[prevIndex].isValid()) {
					if (-plotPoints[prevIndex].getY() < 0) {
						startPoint.setY(0);
						startPoint.setLabel("extreme");
					} else {
						startPoint.setY(-dim[1]);
						startPoint.setLabel("extreme");
					}
				}
			}
		}

		// Calculate the end point
		ofxGPoint endPoint = shapePoints.back();

		if (endPoint.getY() != 0 && endPoint.getY() != -dim[1] && indexLastPoint != nPoints - 1) {
			int nextIndex = indexLastPoint + 1;

			while (nextIndex < nPoints - 1 && !plotPoints[nextIndex].isValid()) {
				++nextIndex;
			}

			if (plotPoints[nextIndex].isValid()) {
				if (-plotPoints[nextIndex].getY() < 0) {
					endPoint.setY(0);
					endPoint.setLabel("extreme");
				} else {
					endPoint.setY(-dim[1]);
					endPoint.setLabel("extreme");
				}
			}
		}

		// Add the end point if it's a new extreme
		if (endPoint.getLabel() == "extreme") {
			shapePoints.push_back(endPoint);
		}

		// Add the reference connections
		if (xLogScale && referenceValue <= 0) {
			referenceValue = min(xLim[0], xLim[1]);
		}

		array<float, 2> plotReference = valueToPlot(referenceValue, 1);

		if (plotReference[0] < 0) {
			shapePoints.emplace_back(0, endPoint.getY());
			shapePoints.emplace_back(0, startPoint.getY());
		} else if (plotReference[0] > dim[0]) {
			shapePoints.emplace_back(dim[0], endPoint.getY());
			shapePoints.emplace_back(dim[0], startPoint.getY());
		} else {
			shapePoints.emplace_back(plotReference[0], endPoint.getY());
			shapePoints.emplace_back(plotReference[0], startPoint.getY());
		}

		// Add the starting point if it's a new extreme
		if (startPoint.getLabel() == "extreme") {
			shapePoints.emplace_back(startPoint);
		}
	}

	return shapePoints;
}

void ofxGLayer::drawLabel(const ofxGPoint& point) const {
	float xPlot = valueToXPlot(point.getX());
	float yPlot = valueToYPlot(point.getY());

	if (isfinite(xPlot) && isfinite(yPlot)) {
		float xLabelPos = xPlot + labelSeparation[0];
		float yLabelPos = yPlot - labelSeparation[1];
		float delta = fontSize / 2.0;

		ofRectangle bounds = font.getStringBoundingBox(point.getLabel(), 0, 0);

		ofPushStyle();
		ofFill();
		ofSetColor(labelBgColor);
		ofSetRectMode(OF_RECTMODE_CORNER);
		ofDrawRectangle(xLabelPos - delta, yLabelPos - fontSize - delta, bounds.width + 2 * delta,
				fontSize + 2 * delta);
		ofSetColor(fontColor);
		font.drawString(point.getLabel(), xLabelPos, yLabelPos);
		ofPopStyle();
	}
}

void ofxGLayer::drawLabelAtPlotPos(float xPlot, float yPlot) const {
	const ofxGPoint* pointPtr = getPointAtPlotPos(xPlot, yPlot);

	if (pointPtr != nullptr) {
		drawLabel(*pointPtr);
	}
}

void ofxGLayer::drawHistogram() const {
	if (histIsActive) {
		hist.draw(valueToPlot(histBasePoint));
	}
}

void ofxGLayer::drawPolygon(const vector<ofxGPoint>& polygonPoints, const ofColor& polygonColor) {
	if (polygonPoints.size() > 2) {
		vector<ofxGPoint> plotPolygonPoints = valueToPlot(polygonPoints);

		// Remove the polygon invalid points
		vector<ofxGPoint> cleanPoints;

		for (const ofxGPoint& p : plotPolygonPoints) {
			if (p.isValid()) {
				cleanPoints.push_back(p);
			}
		}

		// Create a temporal array with the points inside the plotting area and the valid box cuts
		vector<ofxGPoint> tmp;
		int nPoints = cleanPoints.size();

		for (int i = 0; i < nPoints; ++i) {
			if (isInside(cleanPoints[i])) {
				tmp.emplace_back(cleanPoints[i].getX(), cleanPoints[i].getY(), "normal point");
			}

			// Obtain the cuts with the next point
			int nextIndex = (i + 1 < nPoints) ? i + 1 : 0;
			int nCuts = obtainBoxIntersections(cleanPoints[i], cleanPoints[nextIndex]);

			if (nCuts == 1) {
				tmp.emplace_back(cuts[0][0], cuts[0][1], "single cut");
			} else if (nCuts > 1) {
				tmp.emplace_back(cuts[0][0], cuts[0][1], "double cut");
				tmp.emplace_back(cuts[1][0], cuts[1][1], "double cut");
			}
		}

		// Final version of the polygon
		nPoints = tmp.size();
		vector<ofxGPoint> croppedPoly;

		for (int i = 0; i < nPoints; ++i) {
			// Add the point
			croppedPoly.push_back(tmp[i]);

			// Add new points in case we have two consecutive cuts, one of them is single, and they are in
			// consecutive axes
			int nextIndex = (i + 1 < nPoints) ? i + 1 : 0;
			string label = tmp[i].getLabel();
			string nextLabel = tmp[nextIndex].getLabel();

			bool cond = (label == "single cut" && nextLabel == "single cut")
					|| (label == "single cut" && nextLabel == "double cut")
					|| (label == "double cut" && nextLabel == "single cut");

			if (cond) {
				float x1 = tmp[i].getX();
				float y1 = tmp[i].getY();
				float x2 = tmp[nextIndex].getX();
				float y2 = tmp[nextIndex].getY();
				float deltaX = abs(x2 - x1);
				float deltaY = abs(y2 - y1);

				// Check that they come from consecutive axes
				if (deltaX > 0 && deltaY > 0 && deltaX != dim[0] && deltaY != dim[1]) {
					float x = (x1 == 0 || x1 == dim[0]) ? x1 : x2;
					float y = (y1 == 0 || y1 == -dim[1]) ? y1 : y2;
					croppedPoly.emplace_back(x, y, "special cut");
				}
			}
		}

		// Draw the cropped polygon
		if (croppedPoly.size() > 2) {
			ofPushStyle();
			ofFill();
			ofSetColor(polygonColor);

			ofBeginShape();

			for (const ofxGPoint& p : croppedPoly) {
				ofVertex(p.getX(), p.getY());
			}

			ofEndShape(true);

			ofPopStyle();
		}
	}
}

void ofxGLayer::drawAnnotation(const string& text, float x, float y, ofxGTextAlignment verAlign) const {
	float xPlot = valueToXPlot(x);
	float yPlot = valueToYPlot(y);

	if (isfinite(xPlot) && isfinite(yPlot) && isInside(xPlot, yPlot)) {
		ofPushStyle();
		ofSetColor(fontColor);

		switch (verAlign) {
		case GRAFICA_CENTER_ALIGN:
			font.drawString(text, xPlot, yPlot + fontSize / 2.0);
			break;
		case GRAFICA_TOP_ALIGN:
			font.drawString(text, xPlot, yPlot + fontSize);
			break;
		case GRAFICA_BOTTOM_ALIGN:
			font.drawString(text, xPlot, yPlot);
			break;
		default:
			font.drawString(text, xPlot, yPlot);
			break;
		}

		ofPopStyle();
	}
}

void ofxGLayer::setDim(float xDim, float yDim) {
	if (xDim <= 0 || yDim <= 0) {
		throw invalid_argument("The dimensions should be larger than zero.");
	}

	dim = {xDim, yDim};
	updatePlotPoints();

	if (histIsActive) {
		hist.setDim(xDim, yDim);
		hist.setPlotPoints(plotPoints);
	}
}

void ofxGLayer::setDim(const array<float, 2>& newDim) {
	setDim(newDim[0], newDim[1]);
}

void ofxGLayer::setXLim(float xMin, float xMax) {
	// Check that the new limit makes sense
	if (xMin == xMax) {
		throw invalid_argument("The limit range cannot be zero.");
	} else if (!isfinite(xMin) || !isfinite(xMax)) {
		throw invalid_argument("The limits need to be finite numbers.");
	} else if (xLogScale && (xMin <= 0 || xMax <= 0)) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	xLim = {xMin, xMax};
	updatePlotPoints();
	updateInsideList();

	if (histIsActive) {
		hist.setPlotPoints(plotPoints);
	}
}

void ofxGLayer::setXLim(const array<float, 2>& newXLim) {
	setXLim(newXLim[0], newXLim[1]);
}

void ofxGLayer::setYLim(float yMin, float yMax) {
	// Check that the new limit makes sense
	if (yMin == yMax) {
		throw invalid_argument("The limit range cannot be zero.");
	} else if (!isfinite(yMin) || !isfinite(yMax)) {
		throw invalid_argument("The limits need to be finite numbers.");
	} else if (yLogScale && (yMin <= 0 || yMax <= 0)) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	yLim = {yMin, yMax};
	updatePlotPoints();
	updateInsideList();

	if (histIsActive) {
		hist.setPlotPoints(plotPoints);
	}
}

void ofxGLayer::setYLim(const array<float, 2>& newYLim) {
	setYLim(newYLim[0], newYLim[1]);
}

void ofxGLayer::setXYLim(float xMin, float xMax, float yMin, float yMax) {
	// Check that the new limit makes sense
	if (xMin == xMax || yMin == yMax) {
		throw invalid_argument("The limit range cannot be zero.");
	} else if (!isfinite(xMin) || !isfinite(xMax) || !isfinite(yMin) || !isfinite(yMax)) {
		throw invalid_argument("The limits need to be finite numbers.");
	} else if ((xLogScale && (xMin <= 0 || xMax <= 0)) || (yLogScale && (yMin <= 0 || yMax <= 0))) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	xLim = {xMin, xMax};
	yLim = {yMin, yMax};

	updatePlotPoints();
	updateInsideList();

	if (histIsActive) {
		hist.setPlotPoints(plotPoints);
	}
}

void ofxGLayer::setXYLim(const array<float, 2>& newXLim, const array<float, 2>& newYLim) {
	setXYLim(newXLim[0], newXLim[1], newYLim[0], newYLim[1]);
}

void ofxGLayer::setLimAndLogScale(float xMin, float xMax, float yMin, float yMax, bool newXLogScale,
		bool newYLogScale) {
	// Check that the new limit makes sense
	if (xMin == xMax || yMin == yMax) {
		throw invalid_argument("The limit range cannot be zero.");
	} else if (!isfinite(xMin) || !isfinite(xMax) || !isfinite(yMin) || !isfinite(yMax)) {
		throw invalid_argument("The limits need to be finite numbers.");
	} else if ((newXLogScale && (xMin <= 0 || xMax <= 0)) || (newYLogScale && (yMin <= 0 || yMax <= 0))) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	xLogScale = newXLogScale;
	yLogScale = newYLogScale;
	xLim = {xMin, xMax};
	yLim = {yMin, yMax};

	updatePlotPoints();
	updateInsideList();

	if (histIsActive) {
		hist.setPlotPoints(plotPoints);
	}
}

void ofxGLayer::setLimAndLogScale(const array<float, 2>& newXLim, const array<float, 2>& newYLim, bool newXLogScale,
		bool newYLogScale) {
	setLimAndLogScale(newXLim[0], newXLim[1], newYLim[0], newYLim[1], newXLogScale, newYLogScale);
}

void ofxGLayer::setXLogScale(bool newXLogScale) {
	if (newXLogScale != xLogScale) {
		if (newXLogScale && (xLim[0] <= 0 || xLim[1] <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		xLogScale = newXLogScale;
		updatePlotPoints();
		updateInsideList();

		if (histIsActive) {
			hist.setPlotPoints(plotPoints);
		}
	}
}

void ofxGLayer::setYLogScale(bool newYLogScale) {
	if (newYLogScale != yLogScale) {
		if (newYLogScale && (yLim[0] <= 0 || yLim[1] <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		yLogScale = newYLogScale;
		updatePlotPoints();
		updateInsideList();

		if (histIsActive) {
			hist.setPlotPoints(plotPoints);
		}
	}
}

void ofxGLayer::setPoints(const vector<ofxGPoint>& newPoints) {
	points = newPoints;
	updatePlotPoints();
	updateInsideList();

	if (histIsActive) {
		hist.setPlotPoints(plotPoints);
	}
}

void ofxGLayer::setPoint(vector<ofxGPoint>::size_type index, float x, float y, const string& label) {
	points[index].set(x, y, label);
	plotPoints[index].set(valueToXPlot(x), valueToYPlot(y), label);
	inside[index] = isInside(plotPoints[index]);

	if (histIsActive) {
		hist.setPlotPoints(plotPoints);
	}
}

void ofxGLayer::setPoint(vector<ofxGPoint>::size_type index, float x, float y) {
	setPoint(index, x, y, points[index].getLabel());
}

void ofxGLayer::setPoint(vector<ofxGPoint>::size_type index, const ofxGPoint& newPoint) {
	setPoint(index, newPoint.getX(), newPoint.getY(), newPoint.getLabel());
}

void ofxGLayer::addPoint(float x, float y, const string& label) {
	points.emplace_back(x, y, label);
	plotPoints.emplace_back(valueToXPlot(x), valueToYPlot(y), label);
	inside.push_back(isInside(plotPoints.back()));

	if (histIsActive) {
		hist.addPlotPoint(plotPoints.back());
	}
}

void ofxGLayer::addPoint(float x, float y) {
	addPoint(x, y, "");
}

void ofxGLayer::addPoint(const ofxGPoint& newPoint) {
	addPoint(newPoint.getX(), newPoint.getY(), newPoint.getLabel());
}

void ofxGLayer::addPoint(vector<ofxGPoint>::size_type index, float x, float y, const string& label) {
	if (index > points.size()) {
		throw invalid_argument("The index should be inside the vector range.");
	}

	points.insert(points.begin() + index, ofxGPoint(x, y, label));
	plotPoints.insert(plotPoints.begin() + index, ofxGPoint(valueToXPlot(x), valueToYPlot(y), label));
	inside.insert(inside.begin() + index, isInside(plotPoints[index]));

	if (histIsActive) {
		hist.addPlotPoint(index, plotPoints[index]);
	}
}

void ofxGLayer::addPoint(vector<ofxGPoint>::size_type index, float x, float y) {
	addPoint(index, x, y, "");
}

void ofxGLayer::addPoint(vector<ofxGPoint>::size_type index, const ofxGPoint& newPoint) {
	addPoint(index, newPoint.getX(), newPoint.getY(), newPoint.getLabel());
}

void ofxGLayer::addPoints(const vector<ofxGPoint>& newPoints) {
	for (const ofxGPoint& p : newPoints) {
		points.emplace_back(p.getX(), p.getY(), p.getLabel());
		plotPoints.emplace_back(valueToXPlot(p.getX()), valueToYPlot(p.getY()), p.getLabel());
		inside.push_back(isInside(plotPoints.back()));
	}

	if (histIsActive) {
		hist.setPlotPoints(plotPoints);
	}
}

void ofxGLayer::removePoint(vector<ofxGPoint>::size_type index) {
	if (index >= points.size()) {
		throw invalid_argument("The index should be inside the vector range.");
	}

	points.erase(points.begin() + index);
	plotPoints.erase(plotPoints.begin() + index);
	inside.erase(inside.begin() + index);

	if (histIsActive) {
		hist.removePlotPoint(index);
	}
}

void ofxGLayer::setInside(const vector<bool>& newInside) {
	if (newInside.size() == inside.size()) {
		throw invalid_argument("The vector size should match the number of points.");
	}

	inside = newInside;
}

void ofxGLayer::setPointColors(const vector<ofColor>& newPointColors) {
	if (newPointColors.size() == 0) {
		throw invalid_argument("The vector is empty.");
	}

	pointColors = newPointColors;
}

void ofxGLayer::setPointColor(const ofColor& newPointColor) {
	pointColors = {newPointColor};
}

void ofxGLayer::setPointSizes(const vector<float>& newPointSizes) {
	if (newPointSizes.size() == 0) {
		throw invalid_argument("The vector is empty.");
	}

	pointSizes = newPointSizes;
}

void ofxGLayer::setPointSize(float newPointSize) {
	pointSizes = {newPointSize};
}

void ofxGLayer::setLineColor(const ofColor& newLineColor) {
	lineColor = newLineColor;
}

void ofxGLayer::setLineWidth(float newLineWidth) {
	if (newLineWidth <= 0) {
		throw invalid_argument("The line width should be larger than zero.");
	}

	lineWidth = newLineWidth;
}

void ofxGLayer::setHistBasePoint(const ofxGPoint& newHistBasePoint) {
	histBasePoint = newHistBasePoint;
}

void ofxGLayer::setHistType(ofxGHistogramType histType) {
	if (histIsActive) {
		hist.setType(histType);
	}
}

void ofxGLayer::setHistVisible(bool visible) {
	if (histIsActive) {
		hist.setVisible(visible);
	}
}

void ofxGLayer::setDrawHistLabels(bool drawHistLabels) {
	if (histIsActive) {
		hist.setDrawLabels(drawHistLabels);
	}
}

void ofxGLayer::setLabelBgColor(const ofColor& newLabelBgColor) {
	labelBgColor = newLabelBgColor;
}

void ofxGLayer::setLabelSeparation(const array<float, 2>& newLabelSeparation) {
	labelSeparation = newLabelSeparation;
}

void ofxGLayer::setFontName(const string& newFontName) {
	fontName = newFontName;
	font.load(fontName, fontSize, true, true, fontMakeContours);
}

void ofxGLayer::setFontColor(const ofColor& newFontColor) {
	fontColor = newFontColor;
}

void ofxGLayer::setFontSize(int newFontSize) {
	if (newFontSize <= 0) {
		throw invalid_argument("The font size should be larger than zero.");
	}

	fontSize = newFontSize;
	font.load(fontName, fontSize, true, true, fontMakeContours);
}

void ofxGLayer::setFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize) {
	if (newFontSize <= 0) {
		throw invalid_argument("The font size should be larger than zero.");
	}

	fontName = newFontName;
	fontColor = newFontColor;
	fontSize = newFontSize;
	font.load(fontName, fontSize, true, true, fontMakeContours);
}

void ofxGLayer::setAllFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize) {
	if (newFontSize <= 0) {
		throw invalid_argument("The font size should be larger than zero.");
	}

	setFontProperties(newFontName, newFontColor, newFontSize);

	if (histIsActive) {
		hist.setFontProperties(newFontName, newFontColor, newFontSize);
	}
}

void ofxGLayer::setFontsMakeContours(bool newFontMakeContours) {
	fontMakeContours = newFontMakeContours;
	font.load(fontName, fontSize, true, true, fontMakeContours);

	if (histIsActive) {
		hist.setFontMakeContours(fontMakeContours);
	}
}

string ofxGLayer::getId() const {
	return id;
}

array<float, 2> ofxGLayer::getDim() const {
	return dim;
}

array<float, 2> ofxGLayer::getXLim() const {
	return xLim;
}

array<float, 2> ofxGLayer::getYLim() const {
	return yLim;
}

bool ofxGLayer::getXLogScale() const {
	return xLogScale;
}

bool ofxGLayer::getYLogScale() const {
	return yLogScale;
}

vector<ofxGPoint> ofxGLayer::getPoints() const {
	return points;
}

const vector<ofxGPoint>& ofxGLayer::getPointsRef() const {
	return points;
}

vector<ofColor> ofxGLayer::getPointColors() const {
	return pointColors;
}

vector<float> ofxGLayer::getPointSizes() const {
	return pointSizes;
}

ofColor ofxGLayer::getLineColor() const {
	return lineColor;
}

float ofxGLayer::getLineWidth() const {
	return lineWidth;
}

ofxGHistogram& ofxGLayer::getHistogram() {
	return hist;
}
