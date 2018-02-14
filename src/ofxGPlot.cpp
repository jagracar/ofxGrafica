#include "ofxGPlot.h"
#include "ofMain.h"

const string ofxGPlot::MAINLAYERID = "main layer";

ofxGPlot::ofxGPlot(float xPos, float yPos, float plotWidth, float plotHeight) {
	pos = {xPos, yPos};
	outerDim = {plotWidth, plotHeight};
	mar = {60, 70, 40, 30};
	dim = {outerDim[0] - mar[1] - mar[3], outerDim[1] - mar[0] - mar[2]};
	xLim = {0, 100};
	yLim = {0, 100};
	fixedXLim = false;
	fixedYLim = false;
	xLog = false;
	yLog = false;
	invertedXScale = false;
	invertedYScale = false;
	includeAllLayersInLim = true;
	expandLimFactor = 0.1;

	bgColor = ofColor(255);
	boxBgColor = ofColor(245);
	boxLineColor = ofColor(210);
	boxLineWidth = 1;
	gridLineColor = ofColor(210);
	gridLineWidth = 1;

	mainLayer = ofxGLayer(MAINLAYERID, dim, xLim, yLim, xLog, yLog);

	xAxis = ofxGAxis(GRAFICA_X_AXIS, dim, xLim, xLog);
	topAxis = ofxGAxis(GRAFICA_TOP_AXIS, dim, xLim, xLog);
	yAxis = ofxGAxis(GRAFICA_Y_AXIS, dim, yLim, yLog);
	rightAxis = ofxGAxis(GRAFICA_RIGHT_AXIS, dim, yLim, yLog);
	title = ofxGTitle(dim);
}

array<float, 2> ofxGPlot::getPlotPosAt(float xScreen, float yScreen) const {
	float xPlot = xScreen - (pos[0] + mar[1]);
	float yPlot = yScreen - (pos[1] + mar[2] + dim[1]);

	return {xPlot, yPlot};
}

array<float, 2> ofxGPlot::getScreenPosAtValue(float xValue, float yValue) const {
	float xScreen = mainLayer.valueToXPlot(xValue) + (pos[0] + mar[1]);
	float yScreen = mainLayer.valueToYPlot(yValue) + (pos[1] + mar[2] + dim[1]);

	return {xScreen, yScreen};
}

const ofxGPoint* ofxGPlot::getPointAt(float xScreen, float yScreen) const {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);

	return mainLayer.getPointAtPlotPos(plotPos[0], plotPos[1]);
}

const ofxGPoint* ofxGPlot::getPointAt(float xScreen, float yScreen, const string &layerId) const {
	const ofxGPoint* p = nullptr;

	if (mainLayer.isId(layerId)) {
		p = getPointAt(xScreen, yScreen);
	} else {
		for (ofxGLayer layer : layerList) {
			if (layer.isId(layerId)) {
				array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);
				p = layer.getPointAtPlotPos(plotPos[0], plotPos[1]);
				break;
			}
		}
	}

	return p;
}

array<float, 2> ofxGPlot::getValueAt(float xScreen, float yScreen) const {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);

	return mainLayer.plotToValue(plotPos[0], plotPos[1]);
}

array<float, 2> ofxGPlot::getRelativePlotPosAt(float xScreen, float yScreen) const {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);

	return {plotPos[0] / dim[0], -plotPos[1] / dim[1]};
}

bool ofxGPlot::isOverPlot(float xScreen, float yScreen) const {
	return (xScreen >= pos[0]) && (xScreen <= pos[0] + outerDim[0]) && (yScreen >= pos[1])
			&& (yScreen <= pos[1] + outerDim[1]);
}

bool ofxGPlot::isOverBox(float xScreen, float yScreen) const {
	return (xScreen >= pos[0] + mar[1]) && (xScreen <= pos[0] + outerDim[0] - mar[3]) && (yScreen >= pos[1] + mar[2])
			&& (yScreen <= pos[1] + outerDim[1] - mar[0]);
}

void ofxGPlot::updateLimits() {
	// Calculate the new limits and update the axes if needed
	if (!fixedXLim) {
		xLim = calculatePlotXLim();
		xAxis.setLim(xLim);
		topAxis.setLim(xLim);
	}

	if (!fixedYLim) {
		yLim = calculatePlotYLim();
		yAxis.setLim(yLim);
		rightAxis.setLim(yLim);
	}

	// Update the layers
	mainLayer.setXYLim(xLim, yLim);

	for (ofxGLayer layer : layerList) {
		layer.setXYLim(xLim, yLim);
	}
}

array<float, 2> ofxGPlot::calculatePlotXLim() {
	// Find the limits for the main layer
	array<float, 2> lim = calculatePointsXLim(mainLayer.getPointsRef());

	// Include the other layers in the limit calculation if necessary
	if (includeAllLayersInLim) {
		for (ofxGLayer layer : layerList) {
			array<float, 2> newLim = calculatePointsXLim(layer.getPointsRef());

			if (isfinite(newLim[0])) {
				if (isfinite(lim[0])) {
					lim[0] = min(lim[0], newLim[0]);
					lim[1] = max(lim[1], newLim[1]);
				} else {
					lim = newLim;
				}
			}
		}
	}

	if (isfinite(lim[0])) {
		// Expand the axis limits a bit
		float delta = (lim[0] == 0) ? 0.1 : 0.1 * lim[0];

		if (xLog) {
			if (lim[0] != lim[1]) {
				delta = pow(10, expandLimFactor * log10(lim[1] / lim[0]));
			}

			lim[0] = lim[0] / delta;
			lim[1] = lim[1] * delta;
		} else {
			if (lim[0] != lim[1]) {
				delta = expandLimFactor * (lim[1] - lim[0]);
			}

			lim[0] = lim[0] - delta;
			lim[1] = lim[1] + delta;
		}
	} else {
		if (xLog && (xLim[0] <= 0 || xLim[1] <= 0)) {
			lim = {0.1, 10};
		} else {
			lim = xLim;
		}
	}

	// Invert the limits if necessary
	if (invertedXScale && lim[0] < lim[1]) {
		lim = {lim[1], lim[0]};
	}

	return lim;
}

array<float, 2> ofxGPlot::calculatePlotYLim() {
	// Find the limits for the main layer
	array<float, 2> lim = calculatePointsYLim(mainLayer.getPointsRef());

	// Include the other layers in the limit calculation if necessary
	if (includeAllLayersInLim) {
		for (ofxGLayer layer : layerList) {
			array<float, 2> newLim = calculatePointsYLim(layer.getPointsRef());

			if (isfinite(newLim[0])) {
				if (isfinite(lim[0])) {
					lim[0] = min(lim[0], newLim[0]);
					lim[1] = max(lim[1], newLim[1]);
				} else {
					lim = newLim;
				}
			}
		}
	}

	if (isfinite(lim[0])) {
		// Expand the axis limits a bit
		float delta = (lim[0] == 0) ? 0.1 : 0.1 * lim[0];

		if (yLog) {
			if (lim[0] != lim[1]) {
				delta = pow(10, expandLimFactor * log10(lim[1] / lim[0]));
			}

			lim[0] = lim[0] / delta;
			lim[1] = lim[1] * delta;
		} else {
			if (lim[0] != lim[1]) {
				delta = expandLimFactor * (lim[1] - lim[0]);
			}

			lim[0] = lim[0] - delta;
			lim[1] = lim[1] + delta;
		}
	} else {
		if (yLog && (yLim[0] <= 0 || yLim[1] <= 0)) {
			lim = {0.1, 10};
		} else {
			lim = yLim;
		}
	}

	// Invert the limits if necessary
	if (invertedYScale && lim[0] < lim[1]) {
		lim = {lim[1], lim[0]};
	}

	return lim;
}

array<float, 2> ofxGPlot::calculatePointsXLim(const vector<ofxGPoint> &points) {
	// Find the points limits
	array<float, 2> lim = { numeric_limits<float>::infinity(), -numeric_limits<float>::infinity() };

	for (ofxGPoint p : points) {
		if (p.isValid()) {
			// Use the point if it's inside, and it's not negative if
			// the scale is logarithmic
			float x = p.getX();
			float y = p.getY();
			bool isInside = true;

			if (fixedYLim) {
				isInside = ((yLim[1] >= yLim[0]) && (y >= yLim[0]) && (y <= yLim[1]))
						|| ((yLim[1] < yLim[0]) && (y <= yLim[0]) && (y >= yLim[1]));
			}

			if (isInside && !(xLog && x <= 0)) {
				if (x < lim[0]) {
					lim[0] = x;
				}
				if (x > lim[1]) {
					lim[1] = x;
				}
			}
		}
	}

	// Check that the new limits make sense
	if (lim[1] < lim[0]) {
		lim = {numeric_limits<float>::infinity(), -numeric_limits<
			float>::infinity()};
	}

	return lim;
}

array<float, 2> ofxGPlot::calculatePointsYLim(const vector<ofxGPoint> &points) {
	// Find the points limits
	array<float, 2> lim = { numeric_limits<float>::infinity(), -numeric_limits<float>::infinity() };

	for (ofxGPoint p : points) {
		if (p.isValid()) {
			// Use the point if it's inside, and it's not negative if
			// the scale is logarithmic
			float x = p.getX();
			float y = p.getY();
			bool isInside = true;

			if (fixedXLim) {
				isInside = ((xLim[1] >= xLim[0]) && (x >= xLim[0]) && (x <= xLim[1]))
						|| ((xLim[1] < xLim[0]) && (x <= xLim[0]) && (x >= xLim[1]));
			}

			if (isInside && !(yLog && y <= 0)) {
				if (y < lim[0]) {
					lim[0] = y;
				}
				if (y > lim[1]) {
					lim[1] = y;
				}
			}
		}
	}

	// Check that the new limits make sense
	if (lim[1] < lim[0]) {
		lim = {numeric_limits<float>::infinity(), -numeric_limits<
			float>::infinity()};
	}

	return lim;
}

void ofxGPlot::defaultDraw() {
	beginDraw();
	drawBackground();
	drawBox();
	drawXAxis();
	drawYAxis();
	drawTitle();
	drawLines();
	drawPoints();
	endDraw();
}

void ofxGPlot::beginDraw() const {
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(pos[0] + mar[1], pos[1] + mar[2] + dim[1], 0);
}

void ofxGPlot::endDraw() const {
	ofPopMatrix();
	ofPopStyle();
}

void ofxGPlot::drawBackground() const {
	ofPushStyle();
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofFill();
	ofSetColor(bgColor);
	ofDrawRectangle(-mar[1], -mar[2] - dim[1], outerDim[0], outerDim[1]);
	ofPopStyle();
}

void ofxGPlot::drawBox() const {
	ofPushStyle();
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofFill();
	ofSetColor(boxBgColor);
	ofDrawRectangle(0, -dim[1], dim[0], dim[1]);
	ofNoFill();
	ofSetColor(boxLineColor);
	ofSetLineWidth(boxLineWidth);
	ofDrawRectangle(0, -dim[1], dim[0], dim[1]);
	ofPopStyle();
}

void ofxGPlot::drawXAxis() const {
	xAxis.draw();
}

void ofxGPlot::drawYAxis() const {
	yAxis.draw();
}

void ofxGPlot::drawTopAxis() const {
	topAxis.draw();
}

void ofxGPlot::drawRightAxis() const {
	rightAxis.draw();
}

void ofxGPlot::drawTitle() const {
	title.draw();
}

void ofxGPlot::drawPoints() const {
	mainLayer.drawPoints();

	for (ofxGLayer layer : layerList) {
		layer.drawPoints();
	}
}

void ofxGPlot::drawLines() {
	mainLayer.drawLines();

	for (ofxGLayer layer : layerList) {
		layer.drawLines();
	}
}

void ofxGPlot::setPos(float x, float y) {
	pos = {x, y};
}

void ofxGPlot::setPos(const array<float, 2> &newPos) {
	pos = newPos;
}

void ofxGPlot::setOuterDim(float xOuterDim, float yOuterDim) {
	if (xOuterDim > 0 && yOuterDim > 0) {
		// Make sure that the new plot dimensions are positive
		float xDim = xOuterDim - mar[1] - mar[3];
		float yDim = yOuterDim - mar[0] - mar[2];

		if (xDim > 0 && yDim > 0) {
			outerDim = {xOuterDim, yOuterDim};
			dim = {xDim, yDim};
			xAxis.setDim(dim);
			topAxis.setDim(dim);
			yAxis.setDim(dim);
			rightAxis.setDim(dim);
			title.setDim(dim);

			// Update the layers
			mainLayer.setDim(dim);

			for (ofxGLayer layer : layerList) {
				layer.setDim(dim);
			}
		}
	}
}

void ofxGPlot::setOuterDim(const array<float, 2> &newOuterDim) {
	setOuterDim(newOuterDim[0], newOuterDim[1]);
}

void ofxGPlot::setMar(float bottomMargin, float leftMargin, float topMargin, float rightMargin) {
	// Make sure that the new outer dimensions are positive
	float xOuterDim = dim[0] + leftMargin + rightMargin;
	float yOuterDim = dim[1] + bottomMargin + topMargin;

	if (xOuterDim > 0 && yOuterDim > 0) {
		mar = {bottomMargin, leftMargin, topMargin, rightMargin};
		outerDim = {xOuterDim, yOuterDim};
	}
}

void ofxGPlot::setMar(const array<float, 4> &newMar) {
	setMar(newMar[0], newMar[1], newMar[2], newMar[3]);
}

void ofxGPlot::setDim(float xDim, float yDim) {
	if (xDim > 0 && yDim > 0) {
		// Make sure that the new outer dimensions are positive
		float xOuterDim = xDim + mar[1] + mar[3];
		float yOuterDim = yDim + mar[0] + mar[2];

		if (xOuterDim > 0 && yOuterDim > 0) {
			outerDim = {xOuterDim, yOuterDim};
			dim = {xDim, yDim};
			xAxis.setDim(dim);
			topAxis.setDim(dim);
			yAxis.setDim(dim);
			rightAxis.setDim(dim);
			title.setDim(dim);

			// Update the layers
			mainLayer.setDim(dim);

			for (ofxGLayer layer : layerList) {
				layer.setDim(dim);
			}
		}
	}
}

void ofxGPlot::setDim(const array<float, 2> &newDim) {
	setDim(newDim[0], newDim[1]);
}

void ofxGPlot::setXLim(float lowerLim, float upperLim) {
	if (lowerLim != upperLim) {
		// Make sure the new limits makes sense
		if (xLog && (lowerLim <= 0 || upperLim <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		xLim = {lowerLim, upperLim};
		invertedXScale = xLim[0] > xLim[1];

		// Fix the limits
		fixedXLim = true;

		// Update the axes
		xAxis.setLim(xLim);
		topAxis.setLim(xLim);

		// Update the plot limits
		updateLimits();
	}
}

void ofxGPlot::setXLim(const array<float, 2> &newXLim) {
	setXLim(newXLim[0], newXLim[1]);
}

void ofxGPlot::setYLim(float lowerLim, float upperLim) {
	if (lowerLim != upperLim) {
		// Make sure the new limits makes sense
		if (yLog && (lowerLim <= 0 || upperLim <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		yLim = {lowerLim, upperLim};
		invertedYScale = yLim[0] > yLim[1];

		// Fix the limits
		fixedYLim = true;

		// Update the axes
		yAxis.setLim(yLim);
		rightAxis.setLim(yLim);

		// Update the plot limits
		updateLimits();
	}
}

void ofxGPlot::setYLim(const array<float, 2> &newYLim) {
	setYLim(newYLim[0], newYLim[1]);
}

void ofxGPlot::setFixedXLim(bool newFixedXLim) {
	fixedXLim = newFixedXLim;

	// Update the plot limits
	updateLimits();
}

void ofxGPlot::setFixedYLim(bool newFixedYLim) {
	fixedYLim = newFixedYLim;

	// Update the plot limits
	updateLimits();
}

void ofxGPlot::setLogScale(const string &logType) {
	bool newXLog = xLog;
	bool newYLog = yLog;

	if (logType == "xy" || logType == "yx") {
		newXLog = true;
		newYLog = true;
	} else if (logType == "x") {
		newXLog = true;
		newYLog = false;
	} else if (logType == "y") {
		newXLog = false;
		newYLog = true;
	} else if (logType == "") {
		newXLog = false;
		newYLog = false;
	}

	// Do something only if the scale changed
	if (newXLog != xLog || newYLog != yLog) {
		// Set the new log scales
		xLog = newXLog;
		yLog = newYLog;

		// Unfix the limits if the old ones don't make sense
		if (xLog && fixedXLim && (xLim[0] <= 0 || xLim[1] <= 0)) {
			fixedXLim = false;
		}

		if (yLog && fixedYLim && (yLim[0] <= 0 || yLim[1] <= 0)) {
			fixedYLim = false;
		}

		// Calculate the new limits if needed
		if (!fixedXLim) {
			xLim = calculatePlotXLim();
		}

		if (!fixedYLim) {
			yLim = calculatePlotYLim();
		}

		// Update the axes
		xAxis.setLimAndLog(xLim, xLog);
		topAxis.setLimAndLog(xLim, xLog);
		yAxis.setLimAndLog(yLim, yLog);
		rightAxis.setLimAndLog(yLim, yLog);

		// Update the layers
		mainLayer.setLimAndLog(xLim, yLim, xLog, yLog);

		for (ofxGLayer layer : layerList) {
			layer.setLimAndLog(xLim, yLim, xLog, yLog);
		}
	}
}

void ofxGPlot::setInvertedXScale(bool newInvertedXScale) {
	if (newInvertedXScale != invertedXScale) {
		invertedXScale = newInvertedXScale;
		xLim = {xLim[1], xLim[0]};

		// Update the axes
		xAxis.setLim(xLim);
		topAxis.setLim(xLim);

		// Update the layers
		mainLayer.setXLim(xLim);

		for (ofxGLayer layer : layerList) {
			layer.setXLim(xLim);
		}
	}
}

void ofxGPlot::invertXScale() {
	setInvertedXScale(!invertedXScale);
}

void ofxGPlot::setInvertedYScale(bool newInvertedYScale) {
	if (newInvertedYScale != invertedYScale) {
		invertedYScale = newInvertedYScale;
		yLim = {yLim[1], yLim[0]};

		// Update the axes
		yAxis.setLim(yLim);
		rightAxis.setLim(yLim);

		// Update the layers
		mainLayer.setYLim(yLim);

		for (ofxGLayer layer : layerList) {
			layer.setYLim(yLim);
		}
	}
}

void ofxGPlot::invertYScale() {
	setInvertedYScale(!invertedYScale);
}

void ofxGPlot::setIncludeAllLayersInLim(bool includeAllLayers) {
	if (includeAllLayers != includeAllLayersInLim) {
		includeAllLayersInLim = includeAllLayers;

		// Update the plot limits
		updateLimits();
	}
}

void ofxGPlot::setExpandLimFactor(float expandFactor) {
	if (expandFactor >= 0 && expandFactor != expandLimFactor) {
		expandLimFactor = expandFactor;

		// Update the plot limits
		updateLimits();
	}
}

void ofxGPlot::setBgColor(const ofColor &newBgColor) {
	bgColor = newBgColor;
}

void ofxGPlot::setBoxBgColor(const ofColor &newBoxBgColor) {
	boxBgColor = newBoxBgColor;
}

void ofxGPlot::setBoxLineColor(const ofColor &newBoxLineColor) {
	boxLineColor = newBoxLineColor;
}

void ofxGPlot::setBoxLineWidth(float newBoxLineWidth) {
	if (newBoxLineWidth > 0) {
		boxLineWidth = newBoxLineWidth;
	}
}

void ofxGPlot::setGridLineColor(const ofColor &newGridLineColor) {
	gridLineColor = newGridLineColor;
}

void ofxGPlot::setGridLineWidth(float newGridLineWidth) {
	if (newGridLineWidth > 0) {
		gridLineWidth = newGridLineWidth;
	}
}

void ofxGPlot::setPoints(const vector<ofxGPoint> &points) {
	mainLayer.setPoints(points);
	updateLimits();
}

void ofxGPlot::setPoints(const vector<ofxGPoint> &points, const string &layerId) {
	getLayer(layerId).setPoints(points);
	updateLimits();
}

void ofxGPlot::setPoint(int index, float x, float y, const string &label) {
	mainLayer.setPoint(index, x, y, label);
	updateLimits();
}

void ofxGPlot::setPoint(int index, float x, float y, const string &label, const string &layerId) {
	getLayer(layerId).setPoint(index, x, y, label);
	updateLimits();
}

void ofxGPlot::setPoint(int index, float x, float y) {
	mainLayer.setPoint(index, x, y);
	updateLimits();
}

void ofxGPlot::setPoint(int index, const ofxGPoint &newPoint) {
	mainLayer.setPoint(index, newPoint);
	updateLimits();
}

void ofxGPlot::setPoint(int index, const ofxGPoint &newPoint, const string &layerId) {
	getLayer(layerId).setPoint(index, newPoint);
	updateLimits();
}

void ofxGPlot::addPoint(float x, float y, const string &label) {
	mainLayer.addPoint(x, y, label);
	updateLimits();
}

void ofxGPlot::addPoint(float x, float y, const string &label, const string &layerId) {
	getLayer(layerId).addPoint(x, y, label);
	updateLimits();
}

void ofxGPlot::addPoint(float x, float y) {
	mainLayer.addPoint(x, y);
	updateLimits();
}

void ofxGPlot::addPoint(const ofxGPoint &newPoint) {
	mainLayer.addPoint(newPoint);
	updateLimits();
}

void ofxGPlot::addPoint(const ofxGPoint &newPoint, const string &layerId) {
	getLayer(layerId).addPoint(newPoint);
	updateLimits();
}

void ofxGPlot::addPoint(int index, float x, float y, const string &label) {
	mainLayer.addPoint(index, x, y, label);
	updateLimits();
}

void ofxGPlot::addPoint(int index, float x, float y, const string &label, const string &layerId) {
	getLayer(layerId).addPoint(index, x, y, label);
	updateLimits();
}

void ofxGPlot::addPoint(int index, float x, float y) {
	mainLayer.addPoint(index, x, y);
	updateLimits();
}

void ofxGPlot::addPoint(int index, const ofxGPoint &newPoint) {
	mainLayer.addPoint(index, newPoint);
	updateLimits();
}

void ofxGPlot::addPoint(int index, const ofxGPoint &newPoint, const string &layerId) {
	getLayer(layerId).addPoint(index, newPoint);
	updateLimits();
}

void ofxGPlot::addPoints(const vector<ofxGPoint> &newPoints) {
	mainLayer.addPoints(newPoints);
	updateLimits();
}

void ofxGPlot::addPoints(const vector<ofxGPoint> &newPoints, const string &layerId) {
	getLayer(layerId).addPoints(newPoints);
	updateLimits();
}

void ofxGPlot::removePoint(int index) {
	mainLayer.removePoint(index);
	updateLimits();
}

void ofxGPlot::removePoint(int index, const string &layerId) {
	getLayer(layerId).removePoint(index);
	updateLimits();
}

void ofxGPlot::setPointColors(const vector<ofColor> &pointColors) {
	mainLayer.setPointColors(pointColors);
}

void ofxGPlot::setPointColor(const ofColor &pointColor) {
	mainLayer.setPointColor(pointColor);
}

void ofxGPlot::setPointSizes(const vector<float> &pointSizes) {
	mainLayer.setPointSizes(pointSizes);
}

void ofxGPlot::setPointSize(float pointSize) {
	mainLayer.setPointSize(pointSize);
}

void ofxGPlot::setLineColor(const ofColor &lineColor) {
	mainLayer.setLineColor(lineColor);
}

void ofxGPlot::setLineWidth(float lineWidth) {
	mainLayer.setLineWidth(lineWidth);
}

void ofxGPlot::setHistBasePoint(const ofxGPoint &basePoint) {
	mainLayer.setHistBasePoint(basePoint);
}

void ofxGPlot::setHistType(ofxGHistogramType histType) {
	mainLayer.setHistType(histType);
}

void ofxGPlot::setHistVisible(bool visible) {
	mainLayer.setHistVisible(visible);
}

void ofxGPlot::setDrawHistLabels(bool drawHistLabels) {
	mainLayer.setDrawHistLabels(drawHistLabels);
}

void ofxGPlot::setLabelBgColor(const ofColor &labelBgColor) {
	mainLayer.setLabelBgColor(labelBgColor);
}

void ofxGPlot::setLabelSeparation(const array<float, 2> &labelSeparation) {
	mainLayer.setLabelSeparation(labelSeparation);
}

void ofxGPlot::setTitleText(const string &text) {
	title.setText(text);
}

void ofxGPlot::setAxesOffset(float offset) {
	xAxis.setOffset(offset);
	topAxis.setOffset(offset);
	yAxis.setOffset(offset);
	rightAxis.setOffset(offset);
}

void ofxGPlot::setTicksLength(float tickLength) {
	xAxis.setTickLength(tickLength);
	topAxis.setTickLength(tickLength);
	yAxis.setTickLength(tickLength);
	rightAxis.setTickLength(tickLength);
}

void ofxGPlot::setHorizontalAxesNTicks(int nTicks) {
	xAxis.setNTicks(nTicks);
	topAxis.setNTicks(nTicks);
}

void ofxGPlot::setHorizontalAxesTicksSeparation(float ticksSeparation) {
	xAxis.setTicksSeparation(ticksSeparation);
	topAxis.setTicksSeparation(ticksSeparation);
}

void ofxGPlot::setHorizontalAxesTicks(const vector<float> &ticks) {
	xAxis.setTicks(ticks);
	topAxis.setTicks(ticks);
}

void ofxGPlot::setVerticalAxesNTicks(int nTicks) {
	yAxis.setNTicks(nTicks);
	rightAxis.setNTicks(nTicks);
}

void ofxGPlot::setVerticalAxesTicksSeparation(float ticksSeparation) {
	yAxis.setTicksSeparation(ticksSeparation);
	rightAxis.setTicksSeparation(ticksSeparation);
}

void ofxGPlot::setVerticalAxesTicks(const vector<float> &ticks) {
	yAxis.setTicks(ticks);
	rightAxis.setTicks(ticks);
}

void ofxGPlot::setFontName(const string &fontName) {
	mainLayer.setFontName(fontName);
}

void ofxGPlot::setFontColor(const ofColor &fontColor) {
	mainLayer.setFontColor(fontColor);
}

void ofxGPlot::setFontSize(int fontSize) {
	mainLayer.setFontSize(fontSize);
}

void ofxGPlot::setFontProperties(const string &fontName, const ofColor &fontColor, int fontSize) {
	mainLayer.setFontProperties(fontName, fontColor, fontSize);
}

void ofxGPlot::setAllFontProperties(const string &fontName, const ofColor &fontColor, int fontSize) {
	xAxis.setAllFontProperties(fontName, fontColor, fontSize);
	topAxis.setAllFontProperties(fontName, fontColor, fontSize);
	yAxis.setAllFontProperties(fontName, fontColor, fontSize);
	rightAxis.setAllFontProperties(fontName, fontColor, fontSize);
	title.setFontProperties(fontName, fontColor, fontSize);

	mainLayer.setAllFontProperties(fontName, fontColor, fontSize);

	for (ofxGLayer layer : layerList) {
		layer.setAllFontProperties(fontName, fontColor, fontSize);
	}
}

array<float, 2> ofxGPlot::getPos() const {
	return pos;
}

array<float, 2> ofxGPlot::getOuterDim() const {
	return outerDim;
}

array<float, 4> ofxGPlot::getMar() const {
	return mar;
}

array<float, 2> ofxGPlot::getDim() const {
	return dim;
}

array<float, 2> ofxGPlot::getXLim() const {
	return xLim;
}

array<float, 2> ofxGPlot::getYLim() const {
	return yLim;
}

bool ofxGPlot::getFixedXLim() const {
	return fixedXLim;
}

bool ofxGPlot::getFixedYLim() const {
	return fixedYLim;
}

bool ofxGPlot::getXLog() const {
	return xLog;
}

bool ofxGPlot::getYLog() const {
	return yLog;
}

bool ofxGPlot::getInvertedXScale() {
	return invertedXScale;
}

bool ofxGPlot::getInvertedYScale() {
	return invertedYScale;
}

ofxGLayer& ofxGPlot::getMainLayer() {
	return mainLayer;
}

ofxGLayer& ofxGPlot::getLayer(const string &id) {
	if (mainLayer.isId(id)) {
		return mainLayer;
	}

	for (ofxGLayer& layer : layerList) {
		if (layer.isId(id)) {
			return layer;
		}
	}
}

ofxGAxis &ofxGPlot::getXAxis() {
	return xAxis;
}

ofxGAxis &ofxGPlot::getTopAxis() {
	return topAxis;
}

ofxGAxis &ofxGPlot::getYAxis() {
	return yAxis;
}

ofxGAxis &ofxGPlot::getRightAxis() {
	return rightAxis;
}

ofxGTitle &ofxGPlot::getTitle() {
	return title;
}

vector<ofxGPoint> ofxGPlot::getPoints() const {
	return mainLayer.getPoints();
}

vector<ofxGPoint> ofxGPlot::getPoints(const string &layerId) const {
	if (mainLayer.isId(layerId)) {
		return mainLayer.getPoints();
	}

	for (ofxGLayer layer : layerList) {
		if (layer.isId(layerId)) {
			return layer.getPoints();
		}
	}
}

const vector<ofxGPoint> &ofxGPlot::getPointsRef() {
	return mainLayer.getPointsRef();
}

const vector<ofxGPoint> &ofxGPlot::getPointsRef(const string &layerId) {
	return getLayer(layerId).getPointsRef();
}

ofxGHistogram &ofxGPlot::getHistogram() {
	return mainLayer.getHistogram();
}

ofxGHistogram &ofxGPlot::getHistogram(const string &layerId) {
	return getLayer(layerId).getHistogram();
}

