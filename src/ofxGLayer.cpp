#include "ofxGLayer.h"
#include "ofMain.h"

ofxGLayer::ofxGLayer(const string &_id, const array<float, 2> &_dim, const array<float, 2> &_xLim,
		const array<float, 2> &_yLim, bool _xLog, bool _yLog) :
		id(_id), dim(_dim), xLim(_xLim), yLim(_yLim), xLog(_xLog), yLog(_yLog) {
	// Do some sanity checks
	if ((xLog && (xLim[0] <= 0 || xLim[1] <= 0)) || (yLog && (yLim[0] <= 0 || yLim[1] <= 0))) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	// Points properties
	pointColors.push_back(ofColor(255, 0, 0, 150));
	pointSizes.push_back(3.5);

	// Line properties
	lineColor = ofColor(0, 150);
	lineWidth = 1;

	// Histogram properties
	//hist = nullptr;
	histBasePoint = ofxGPoint(0, 0);

	// Labels properties
	labelBgColor = ofColor(255, 200);
	labelSeparation = {7, 7};
	fontName = "SansSerif.ttf";
	fontColor = ofColor(0);
	fontSize = 11;
	font.load(fontName, fontSize);
}

bool ofxGLayer::isId(const string &someId) const {
	return id == someId;
}

float ofxGLayer::valueToXPlot(float x) const {
	if (xLog) {
		return dim[0] * log10(x / xLim[0]) / log10(xLim[1] / xLim[0]);
	} else {
		return dim[0] * (x - xLim[0]) / (xLim[1] - xLim[0]);
	}
}

float ofxGLayer::valueToYPlot(float y) const {
	if (yLog) {
		return -dim[1] * log(y / yLim[0]) / log(yLim[1] / yLim[0]);
	} else {
		return -dim[1] * (y - yLim[0]) / (yLim[1] - yLim[0]);
	}
}

array<float, 2> ofxGLayer::valueToPlot(float x, float y) const {
	return {valueToXPlot(x), valueToYPlot(y)};
}

ofxGPoint ofxGLayer::valueToPlot(const ofxGPoint &point) const {
	return ofxGPoint(valueToXPlot(point.getX()), valueToYPlot(point.getY()), point.getLabel());
}

vector<ofxGPoint> &ofxGLayer::valueToPlot(const vector<ofxGPoint> &pts) const {
	vector<ofxGPoint> plotPts;

	// Go case by case. More code, but it's faster
	if (xLog && yLog) {
		float xScalingFactor = dim[0] / log10(xLim[1] / xLim[0]);
		float yScalingFactor = -dim[1] / log10(yLim[1] / yLim[0]);

		for (ofxGPoint p : pts) {
			float xPlot = log10(p.getX() / xLim[0]) * xScalingFactor;
			float yPlot = log10(p.getY() / yLim[0]) * yScalingFactor;
			plotPts.push_back(ofxGPoint(xPlot, yPlot, p.getLabel()));
		}
	} else if (xLog) {
		float xScalingFactor = dim[0] / log10(xLim[1] / xLim[0]);
		float yScalingFactor = -dim[1] / (yLim[1] - yLim[0]);

		for (ofxGPoint p : pts) {
			float xPlot = log10(p.getX() / xLim[0]) * xScalingFactor;
			float yPlot = (p.getY() - yLim[0]) * yScalingFactor;
			plotPts.push_back(ofxGPoint(xPlot, yPlot, p.getLabel()));
		}
	} else if (yLog) {
		float xScalingFactor = dim[0] / (xLim[1] - xLim[0]);
		float yScalingFactor = -dim[1] / log10(yLim[1] / yLim[0]);

		for (ofxGPoint p : pts) {
			float xPlot = (p.getX() - xLim[0]) * xScalingFactor;
			float yPlot = log10(p.getY() / yLim[0]) * yScalingFactor;
			plotPts.push_back(ofxGPoint(xPlot, yPlot, p.getLabel()));
		}
	} else {
		float xScalingFactor = dim[0] / (xLim[1] - xLim[0]);
		float yScalingFactor = -dim[1] / (yLim[1] - yLim[0]);

		for (ofxGPoint p : pts) {
			float xPlot = (p.getX() - xLim[0]) * xScalingFactor;
			float yPlot = (p.getY() - yLim[0]) * yScalingFactor;
			plotPts.push_back(ofxGPoint(xPlot, yPlot, p.getLabel()));
		}
	}

	return plotPts;
}

void ofxGLayer::updatePlotPoints() {
	plotPoints.clear();

	// Go case by case. More code, but it should be faster
	if (xLog && yLog) {
		float xScalingFactor = dim[0] / log10(xLim[1] / xLim[0]);
		float yScalingFactor = -dim[1] / log10(yLim[1] / yLim[0]);

		for (ofxGPoint p : points) {
			float xPlot = log10(p.getX() / xLim[0]) * xScalingFactor;
			float yPlot = log10(p.getY() / yLim[0]) * yScalingFactor;
			plotPoints.push_back(ofxGPoint(xPlot, yPlot, p.getLabel()));
		}
	} else if (xLog) {
		float xScalingFactor = dim[0] / log10(xLim[1] / xLim[0]);
		float yScalingFactor = -dim[1] / (yLim[1] - yLim[0]);

		for (ofxGPoint p : points) {
			float xPlot = log10(p.getX() / xLim[0]) * xScalingFactor;
			float yPlot = (p.getY() - yLim[0]) * yScalingFactor;
			plotPoints.push_back(ofxGPoint(xPlot, yPlot, p.getLabel()));
		}
	} else if (yLog) {
		float xScalingFactor = dim[0] / (xLim[1] - xLim[0]);
		float yScalingFactor = -dim[1] / log10(yLim[1] / yLim[0]);

		for (ofxGPoint p : points) {
			float xPlot = (p.getX() - xLim[0]) * xScalingFactor;
			float yPlot = log10(p.getY() / yLim[0]) * yScalingFactor;
			plotPoints.push_back(ofxGPoint(xPlot, yPlot, p.getLabel()));
		}
	} else {
		float xScalingFactor = dim[0] / (xLim[1] - xLim[0]);
		float yScalingFactor = -dim[1] / (yLim[1] - yLim[0]);

		for (ofxGPoint p : points) {
			float xPlot = (p.getX() - xLim[0]) * xScalingFactor;
			float yPlot = (p.getY() - yLim[0]) * yScalingFactor;
			plotPoints.push_back(ofxGPoint(xPlot, yPlot, p.getLabel()));
		}
	}
}

float ofxGLayer::xPlotToValue(float xPlot) const {
	if (xLog) {
		return exp(log(xLim[0]) + log(xLim[1] / xLim[0]) * xPlot / dim[0]);
	} else {
		return xLim[0] + (xLim[1] - xLim[0]) * xPlot / dim[0];
	}
}

float ofxGLayer::yPlotToValue(float yPlot) const {
	if (yLog) {
		return exp(log(yLim[0]) - log(yLim[1] / yLim[0]) * yPlot / dim[1]);
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

bool ofxGLayer::isInside(const ofxGPoint &plotPoint) const {
	return plotPoint.isValid() ? isInside(plotPoint.getX(), plotPoint.getY()) : false;
}

vector<bool> &ofxGLayer::isInside(const vector<ofxGPoint> &plotPts) const {
	vector<bool> pointsInside;

	for (ofxGPoint p : plotPts) {
		pointsInside.push_back(isInside(p));
	}

	return pointsInside;
}

void ofxGLayer::updateInsideList() {
	// Clear the list first, because the size could have changed
	inside.clear();

	// Refill the list
	for (ofxGPoint p : plotPoints) {
		inside.push_back(isInside(p));
	}
}

int ofxGLayer::getPointIndexAtPlotPos(float xPlot, float yPlot) const {
	int pointIndex = -1;

	if (isInside(xPlot, yPlot)) {
		int nPoints = plotPoints.size();
		float minDistSq = 1000;
		int nSizes = pointSizes.size();

		for (int i = 0; i < nPoints; ++i) {
			if (inside[i]) {
				float distSq = pow(plotPoints[i].getX() - xPlot, 2) + pow(plotPoints[i].getY() - yPlot, 2);

				if (distSq < max(pow(pointSizes[i % nSizes] / 2, 2), 25.)) {
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

ofxGPoint ofxGLayer::getPointAtPlotPos(float xPlot, float yPlot) const {
	int pointIndex = getPointIndexAtPlotPos(xPlot, yPlot);

	//return (pointIndex >= 0) ? points[pointIndex] : nullptr;

	return ofxGPoint();
}

void ofxGLayer::drawPoints() const {
	int nPoints = plotPoints.size();
	int nColors = pointColors.size();
	int nSizes = pointSizes.size();

	ofPushStyle();
	ofFill();

	if (nColors == 1 && nSizes == 1) {
		ofSetColor(pointColors[0]);

		for (int i = 0; i < nPoints; i++) {
			if (inside[i]) {
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(), pointSizes[0]);
			}
		}
	} else if (nColors == 1) {
		ofSetColor(pointColors[0]);

		for (int i = 0; i < nPoints; i++) {
			if (inside[i]) {
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(), pointSizes[i % nSizes]);
			}
		}
	} else if (nSizes == 1) {
		for (int i = 0; i < nPoints; i++) {
			if (inside[i]) {
				ofSetColor(pointColors[i % nColors]);
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(), pointSizes[0]);
			}
		}
	} else {
		for (int i = 0; i < nPoints; i++) {
			if (inside[i]) {
				ofSetColor(pointColors[i % nColors]);
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(), pointSizes[i % nSizes]);
			}
		}
	}

	ofPopStyle();
}

void ofxGLayer::drawLines() {
	ofPushStyle();
	ofSetColor(lineColor);
	ofSetLineWidth(lineWidth);

	for (int i = 0; i < plotPoints.size() - 1; ++i) {
		if (inside[i] && inside[i + 1]) {
			ofDrawLine(plotPoints[i].getX(), plotPoints[i].getY(), plotPoints[i + 1].getX(), plotPoints[i + 1].getY());
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

int ofxGLayer::obtainBoxIntersections(const ofxGPoint &plotPoint1, const ofxGPoint &plotPoint2) {
	int nCuts = 0;

	if (plotPoint1.isValid() && plotPoint2.isValid()) {
		float x1 = plotPoint1.getX();
		float y1 = plotPoint1.getY();
		float x2 = plotPoint2.getX();
		float y2 = plotPoint2.getY();
		bool inside1 = isInside(x1, y1);
		bool inside2 = isInside(x2, y2);

		// Check if the line between the two points could cut the box
		// borders
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
				// Obtain the straight line (y = yCut + slope*x) that
				// crosses the two points
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

			// Select only the cuts that fall inside the box and are located
			// between the two points
			nCuts = getValidCuts(cuts, nCuts, plotPoint1, plotPoint2);

			// Make sure we have the correct number of cuts
			if (inside1 || inside2) {
				// One of the points is inside. We should have one cut only
				if (nCuts != 1) {
					ofxGPoint pointInside = (inside1) ? plotPoint1 : plotPoint2;

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

					// If the cut is missing, then it must be equal to the
					// point inside
					if (nCuts == 0) {
						nCuts = 1;
						cuts[0][0] = pointInside.getX();
						cuts[0][1] = pointInside.getY();
					}
				}
			} else {
				// Both points are outside. We should have either two cuts
				// or none
				if (nCuts > 2) {
					nCuts = removeDuplicatedCuts(cuts, nCuts, 0);

					// In case of rounding number errors
					if (nCuts > 2) {
						nCuts = removeDuplicatedCuts(cuts, nCuts, 0.001f);
					}
				}

				// If we have two cuts, order them (the closest to the first
				// point goes first)
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

int ofxGLayer::getValidCuts(array<array<float, 2>, 4> &cuts, int nCuts, const ofxGPoint &plotPoint1,
		const ofxGPoint &plotPoint2) {
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

int ofxGLayer::removeDuplicatedCuts(array<array<float, 2>, 4> &cuts, int nCuts, float tolerance) {
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

int ofxGLayer::removePointFromCuts(array<array<float, 2>, 4> &cuts, int nCuts, const ofxGPoint &plotPoint,
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

void ofxGLayer::setDim(float xDim, float yDim) {
	if (xDim > 0 && yDim > 0) {
		dim = {xDim, yDim};
		updatePlotPoints();

		//if (hist != null) {
		//	hist.setDim(xDim, yDim);
		//	hist.setPlotPoints(plotPoints);
		//}
	}
}

void ofxGLayer::setDim(const array<float, 2> &newDim) {
	setDim(newDim[0], newDim[1]);
}

void ofxGLayer::setXLim(float xMin, float xMax) {
	if (xMin != xMax && isfinite(xMin) && isfinite(xMax)) {
		// Make sure the new limits makes sense
		if (xLog && (xMin <= 0 || xMax <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		xLim = {xMin, xMax};
		updatePlotPoints();
		updateInsideList();

		//if (hist != null) {
		//	hist.setPlotPoints(plotPoints);
		//}
	}
}

void ofxGLayer::setXLim(const array<float, 2> &newXLim) {
	setXLim(newXLim[0], newXLim[1]);
}

void ofxGLayer::setYLim(float yMin, float yMax) {
	if (yMin != yMax && isfinite(yMin) && isfinite(yMax)) {
		// Make sure the new limits makes sense
		if (yLog && (yMin <= 0 || yMax <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		yLim = {yMin, yMax};
		updatePlotPoints();
		updateInsideList();

		//if (hist != null) {
		//	hist.setPlotPoints(plotPoints);
		//}

	}
}

void ofxGLayer::setYLim(const array<float, 2> &newYLim) {
	setYLim(newYLim[0], newYLim[1]);
}

void ofxGLayer::setXYLim(float xMin, float xMax, float yMin, float yMax) {
	if (xMin != xMax && yMin != yMax && isfinite(xMin) && isfinite(xMax) && isfinite(yMin) && isfinite(yMax)) {
		// Make sure the new limits make sense
		if ((xLog && (xMin <= 0 || xMax <= 0)) || (yLog && (yMin <= 0 || yMax <= 0))) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		xLim = {xMin, xMax};
		yLim = {yMin, yMax};

		updatePlotPoints();
		updateInsideList();

		//if (hist != null) {
		//	hist.setPlotPoints(plotPoints);
		//}
	}
}
void ofxGLayer::setXYLim(const array<float, 2> &newXLim, const array<float, 2> &newYLim) {
	setXYLim(newXLim[0], newXLim[1], newYLim[0], newYLim[1]);
}

void ofxGLayer::setLimAndLog(float xMin, float xMax, float yMin, float yMax, bool newXLog, bool newYLog) {
	if (xMin != xMax && yMin != yMax && isfinite(xMin) && isfinite(xMax) && isfinite(yMin) && isfinite(yMax)) {
		// Make sure the new limits make sense

		if ((newXLog && (xMin <= 0 || xMax <= 0)) || (newYLog && (yMin <= 0 || yMax <= 0))) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		xLog = newXLog;
		yLog = newYLog;
		xLim = {xMin, xMax};
		yLim = {yMin, yMax};

		updatePlotPoints();
		updateInsideList();

		//if (hist != null) {
		//	hist.setPlotPoints(plotPoints);
		//}
	}
}

void ofxGLayer::setLimAndLog(const array<float, 2> &newXLim, const array<float, 2> &newYLim, bool newXLog,
		bool newYLog) {
	setLimAndLog(newXLim[0], newXLim[1], newYLim[0], newYLim[1], newXLog, newYLog);
}

void ofxGLayer::setXLog(bool newXLog) {
	if (newXLog != xLog) {
		if (newXLog && (xLim[0] <= 0 || xLim[1] <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		xLog = newXLog;
		updatePlotPoints();
		updateInsideList();

		//if (hist != null) {
		//	hist.setPlotPoints(plotPoints);
		//}
	}
}

void ofxGLayer::setYLog(bool newYLog) {
	if (newYLog != yLog) {
		if (newYLog && (yLim[0] <= 0 || yLim[1] <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		yLog = newYLog;
		updatePlotPoints();
		updateInsideList();

		//if (hist != null) {
		//	hist.setPlotPoints(plotPoints);
		//}
	}
}

void ofxGLayer::setPoints(const vector<ofxGPoint> &newPoints) {
	points = newPoints;
	updatePlotPoints();
	updateInsideList();

	//if (hist != null) {
	//	hist.setPlotPoints(plotPoints);
	//}
}

void ofxGLayer::setPoint(int index, float x, float y, const string &label) {

}

void ofxGLayer::setPoint(int index, float x, float y) {

}

void ofxGLayer::setPoint(int index, const ofxGPoint &newPoint) {

}

void ofxGLayer::addPoint(float x, float y, const string &label) {
	points.push_back(ofxGPoint(x, y, label));
	plotPoints.push_back(ofxGPoint(valueToXPlot(x), valueToYPlot(y), label));
	inside.push_back(isInside(plotPoints.back()));

	//if (hist != null) {
	//	hist.addPlotPoint(plotPoints.getLastPoint());
	//}
}

void ofxGLayer::addPoint(float x, float y) {
	addPoint(x, y, "");
}

void ofxGLayer::addPoint(const ofxGPoint &newPoint) {
	addPoint(newPoint.getX(), newPoint.getY(), newPoint.getLabel());
}

void ofxGLayer::addPoint(int index, float x, float y, const string &label) {

}

void ofxGLayer::addPoint(int index, float x, float y) {

}

void ofxGLayer::addPoint(int index, const ofxGPoint &newPoint) {

}

void ofxGLayer::addPoints(const vector<ofxGPoint> &newPoints) {
	for (ofxGPoint p : newPoints) {
		points.push_back(p);
		plotPoints.push_back(ofxGPoint(valueToXPlot(p.getX()), valueToYPlot(p.getY()), p.getLabel()));
		inside.push_back(isInside(plotPoints.back()));
	}

	//if (hist != null) {
	//	hist.setPlotPoints(plotPoints);
	//}
}

void ofxGLayer::removePoint(int index) {

}

void ofxGLayer::setInside(const vector<bool> &newInside) {
	if (newInside.size() == inside.size()) {
		inside = newInside;
	}
}

void ofxGLayer::setPointColors(const vector<ofColor> &newPointColors) {
	if (newPointColors.size() > 0) {
		pointColors = newPointColors;
	}
}

void ofxGLayer::setPointColor(const ofColor &newPointColor) {
	pointColors = {newPointColor};
}

void ofxGLayer::setPointSizes(const vector<float> &newPointSizes) {
	if (newPointSizes.size() > 0) {
		pointSizes = newPointSizes;
	}
}

void ofxGLayer::setPointSize(float newPointSize) {
	pointSizes = {newPointSize};
}

void ofxGLayer::setLineColor(const ofColor &newLineColor) {
	lineColor = newLineColor;
}

void ofxGLayer::setLineWidth(float newLineWidth) {
	if (newLineWidth > 0) {
		lineWidth = newLineWidth;
	}
}

void ofxGLayer::setHistBasePoint(const ofxGPoint &newHistBasePoint) {
	histBasePoint = newHistBasePoint;
}

void ofxGLayer::setHistType(int histType) {
	//if (hist != null) {
	//	hist.setType(histType);
	//}
}

void ofxGLayer::setHistVisible(bool visible) {
	//if (hist != null) {
	//	hist.setVisible(visible);
	//}
}

void ofxGLayer::setDrawHistLabels(bool drawHistLabels) {
	//if (hist != null) {
	//	hist.setDrawLabels(drawHistLabels);
	//}
}

void ofxGLayer::setLabelBgColor(const ofColor &newLabelBgColor) {
	labelBgColor = newLabelBgColor;
}

void ofxGLayer::setLabelSeparation(const array<float, 2> &newLabelSeparation) {
	labelSeparation = newLabelSeparation;
}

void ofxGLayer::setFontName(const string &newFontName) {
	fontName = newFontName;
	font.load(fontName, fontSize);
}

void ofxGLayer::setFontColor(const ofColor &newFontColor) {
	fontColor = newFontColor;
}

void ofxGLayer::setFontSize(int newFontSize) {
	if (newFontSize > 0) {
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}

void ofxGLayer::setFontProperties(const string &newFontName, const ofColor &newFontColor, int newFontSize) {
	if (newFontSize > 0) {
		fontName = newFontName;
		fontColor = newFontColor;
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}

void ofxGLayer::setAllFontProperties(const string &newFontName, const ofColor &newFontColor, int newFontSize) {
	setFontProperties(newFontName, newFontColor, newFontSize);

	//if (hist != null) {
	//	hist.setFontProperties(newFontName, newFontColor, newFontSize);
	//}
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

bool ofxGLayer::getXLog() const {
	return xLog;
}

bool ofxGLayer::getYLog() const {
	return yLog;
}

vector<ofxGPoint> ofxGLayer::getPoints() const {
	return points;
}

vector<ofxGPoint> &ofxGLayer::getPointsRef() {
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

ofxGHistogram &ofxGLayer::getHistogram() {
	return hist;
}

