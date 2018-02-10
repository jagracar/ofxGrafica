#include "ofxGLayer.h"
#include "ofMain.h"

ofxGLayer::ofxGLayer(const string &_id, const array<float, 2> &_dim,
		const array<float, 2> &_xLim, const array<float, 2> &_yLim, bool _xLog,
		bool _yLog) :
		id(_id), dim(_dim), xLim(_xLim), yLim(_yLim), xLog(_xLog), yLog(_yLog) {
	// Do some sanity checks
	if ((xLog && (xLim[0] <= 0 || xLim[1] <= 0))
			|| (yLog && (yLim[0] <= 0 || yLim[1] <= 0))) {
		throw invalid_argument(
				"The axis limits are negative and this is not allowed in logarithmic scale.");
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
	return ofxGPoint(valueToXPlot(point.getX()), valueToYPlot(point.getY()),
			point.getLabel());
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
	return (xPlot >= 0) && (xPlot <= dim[0]) && (-yPlot >= 0)
			&& (-yPlot <= dim[1]);
}

bool ofxGLayer::isInside(const ofxGPoint &plotPoint) const {
	return plotPoint.isValid() ?
			isInside(plotPoint.getX(), plotPoint.getY()) : false;
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

ofxGPoint ofxGLayer::getPointAtPlotPos(float xPlot, float yPlot) const {
	//int pointIndex = getPointIndexAtPlotPos(xPlot, yPlot);

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
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(),
						pointSizes[0]);
			}
		}
	} else if (nColors == 1) {
		ofSetColor(pointColors[0]);

		for (int i = 0; i < nPoints; i++) {
			if (inside[i]) {
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(),
						pointSizes[i % nSizes]);
			}
		}
	} else if (nSizes == 1) {
		for (int i = 0; i < nPoints; i++) {
			if (inside[i]) {
				ofSetColor(pointColors[i % nColors]);
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(),
						pointSizes[0]);
			}
		}
	} else {
		for (int i = 0; i < nPoints; i++) {
			if (inside[i]) {
				ofSetColor(pointColors[i % nColors]);
				ofDrawCircle(plotPoints[i].getX(), plotPoints[i].getY(),
						pointSizes[i % nSizes]);
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
			ofDrawLine(plotPoints[i].getX(), plotPoints[i].getY(),
					plotPoints[i + 1].getX(), plotPoints[i + 1].getY());
		} else if (plotPoints[i].isValid() && plotPoints[i + 1].isValid()) {
			// At least one of the points is outside the inner region.
			// Obtain the valid line box intersections
			int nCuts = obtainBoxIntersections(plotPoints[i],
					plotPoints[i + 1]);

			if (inside[i]) {
				ofDrawLine(plotPoints[i].getX(), plotPoints[i].getY(),
						cuts[0][0], cuts[0][1]);
			} else if (inside[i + 1]) {
				ofDrawLine(cuts[0][0], cuts[0][1], plotPoints[i + 1].getX(),
						plotPoints[i + 1].getY());
			} else if (nCuts >= 2) {
				ofDrawLine(cuts[0][0], cuts[0][1], cuts[1][0], cuts[1][1]);
			}
		}
	}

	ofPopStyle();
}

int ofxGLayer::obtainBoxIntersections(const ofxGPoint &plotPoint1,
		const ofxGPoint &plotPoint2) {
	return 0;
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
			throw invalid_argument(
					"The axis limits are negative and this is not allowed in logarithmic scale.");
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
			throw invalid_argument(
					"The axis limits are negative and this is not allowed in logarithmic scale.");
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
	if (xMin != xMax && yMin != yMax && isfinite(xMin) && isfinite(xMax)
			&& isfinite(yMin) && isfinite(yMax)) {
		// Make sure the new limits make sense
		if ((xLog && (xMin <= 0 || xMax <= 0))
				|| (yLog && (yMin <= 0 || yMax <= 0))) {
			throw invalid_argument(
					"The axis limits are negative and this is not allowed in logarithmic scale.");
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
void ofxGLayer::setXYLim(const array<float, 2> &newXLim,
		const array<float, 2> &newYLim) {
	setXYLim(newXLim[0], newXLim[1], newYLim[0], newYLim[1]);
}

void ofxGLayer::setLimAndLog(float xMin, float xMax, float yMin, float yMax,
		bool newXLog, bool newYLog) {
	if (xMin != xMax && yMin != yMax && isfinite(xMin) && isfinite(xMax)
			&& isfinite(yMin) && isfinite(yMax)) {
		// Make sure the new limits make sense

		if ((newXLog && (xMin <= 0 || xMax <= 0))
				|| (newYLog && (yMin <= 0 || yMax <= 0))) {
			throw invalid_argument(
					"The axis limits are negative and this is not allowed in logarithmic scale.");
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

void ofxGLayer::setLimAndLog(const array<float, 2> &newXLim,
		const array<float, 2> &newYLim, bool newXLog, bool newYLog) {
	setLimAndLog(newXLim[0], newXLim[1], newYLim[0], newYLim[1], newXLog,
			newYLog);
}

void ofxGLayer::setXLog(bool newXLog) {
	if (newXLog != xLog) {
		if (newXLog && (xLim[0] <= 0 || xLim[1] <= 0)) {
			throw invalid_argument(
					"The axis limits are negative and this is not allowed in logarithmic scale.");
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
			throw invalid_argument(
					"The axis limits are negative and this is not allowed in logarithmic scale.");
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
		plotPoints.push_back(
				ofxGPoint(valueToXPlot(p.getX()), valueToYPlot(p.getY()),
						p.getLabel()));
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

void ofxGLayer::setFontProperties(const string &newFontName,
		const ofColor &newFontColor, int newFontSize) {
	if (newFontSize > 0) {
		fontName = newFontName;
		fontColor = newFontColor;
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}

void ofxGLayer::setAllFontProperties(const string &newFontName,
		const ofColor &newFontColor, int newFontSize) {
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

