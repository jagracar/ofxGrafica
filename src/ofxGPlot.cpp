#include "ofxGPlot.h"
#include "ofxGConstants.h"
#include "ofxGLayer.h"
#include "ofxGAxis.h"
#include "ofxGTitle.h"
#include "ofxGHistogram.h"
#include "ofMain.h"

ofxGPlot::ofxGPlot(float xPos, float yPos, float plotWidth, float plotHeight) :
		pos( { xPos, yPos }), outerDim( { plotWidth, plotHeight }) {
	// General properties
	mar = {60, 70, 40, 30};
	dim = {outerDim[0] - mar[1] - mar[3], outerDim[1] - mar[0] - mar[2]};
	xLim = {0, 1};
	yLim = {0, 1};
	fixedXLim = false;
	fixedYLim = false;
	xLog = false;
	yLog = false;
	invertedXScale = false;
	invertedYScale = false;
	includeAllLayersInLim = true;
	expandLimFactor = 0.1;

	// Format properties
	bgColor = ofColor(255);
	boxBgColor = ofColor(245);
	boxLineColor = ofColor(210);
	boxLineWidth = 1;
	gridLineColor = ofColor(210);
	gridLineWidth = 1;

	// Layers
	mainLayer = ofxGLayer("main layer", dim, xLim, yLim, xLog, yLog);

	// Axes and title
	xAxis = ofxGAxis(GRAFICA_X_AXIS, dim, xLim, xLog);
	topAxis = ofxGAxis(GRAFICA_TOP_AXIS, dim, xLim, xLog);
	yAxis = ofxGAxis(GRAFICA_Y_AXIS, dim, yLim, yLog);
	rightAxis = ofxGAxis(GRAFICA_RIGHT_AXIS, dim, yLim, yLog);
	title = ofxGTitle(dim);

	// Mouse events
	zoomingIsActive = false;
	zoomFactor = 1.3;
	increaseZoomButton = OF_MOUSE_BUTTON_LEFT;
	decreaseZoomButton = OF_MOUSE_BUTTON_RIGHT;
	increaseZoomKeyModifier = GRAFICA_NONE_MODIFIER;
	decreaseZoomKeyModifier = GRAFICA_NONE_MODIFIER;
	centeringIsActive = false;
	centeringButton = OF_MOUSE_BUTTON_LEFT;
	centeringKeyModifier = GRAFICA_NONE_MODIFIER;
	panningIsActive = false;
	panningButton = OF_MOUSE_BUTTON_LEFT;
	panningKeyModifier = GRAFICA_NONE_MODIFIER;
	panningReferencePointIsSet = false;
	labelingIsActive = false;
	labelingButton = OF_MOUSE_BUTTON_LEFT;
	labelingKeyModifier = GRAFICA_NONE_MODIFIER;
	mousePosIsSet = false;
	resetIsActive = false;
	resetButton = OF_MOUSE_BUTTON_RIGHT;
	resetKeyModifier = OF_KEY_LEFT_CONTROL;
	resetLimitsAreSet = false;
	pressedKey = 0;
	keyIsPressed = false;

	// Add the event listeners
	ofAddListener(ofEvents().mousePressed, this, &ofxGPlot::mouseEventHandler, OF_EVENT_ORDER_AFTER_APP);
	ofAddListener(ofEvents().mouseReleased, this, &ofxGPlot::mouseEventHandler, OF_EVENT_ORDER_AFTER_APP);
	ofAddListener(ofEvents().mouseDragged, this, &ofxGPlot::mouseEventHandler, OF_EVENT_ORDER_AFTER_APP);
	ofAddListener(ofEvents().mouseScrolled, this, &ofxGPlot::mouseEventHandler, OF_EVENT_ORDER_AFTER_APP);
	ofAddListener(ofEvents().keyPressed, this, &ofxGPlot::keyEventHandler, OF_EVENT_ORDER_AFTER_APP);
	ofAddListener(ofEvents().keyReleased, this, &ofxGPlot::keyEventHandler, OF_EVENT_ORDER_AFTER_APP);
}

void ofxGPlot::addLayer(const ofxGLayer& newLayer) {
	// Check that it is the only layer with that id
	string id = newLayer.getId();
	bool sameId = false;

	if (mainLayer.isId(id)) {
		sameId = true;
	} else {
		for (ofxGLayer& layer : layerList) {
			if (layer.isId(id)) {
				sameId = true;
				break;
			}
		}
	}

	// Add the layer to the list
	if (!sameId) {
		layerList.push_back(newLayer);
		layerList.back().setDim(dim);
		layerList.back().setLimAndLog(xLim, yLim, xLog, yLog);

		// Calculate and update the new plot limits if necessary
		if (includeAllLayersInLim) {
			updateLimits();
		}
	} else {
		throw invalid_argument("A layer with the same id exists. Please change the id and try to add it again.");
	}
}

void ofxGPlot::addLayer(const string& id, const vector<ofxGPoint>& points) {
	// Check that it is the only layer with that id
	bool sameId = false;

	if (mainLayer.isId(id)) {
		sameId = true;
	} else {
		for (const ofxGLayer& layer : layerList) {
			if (layer.isId(id)) {
				sameId = true;
				break;
			}
		}
	}

	// Add the layer to the list
	if (!sameId) {
		layerList.emplace_back(id, dim, xLim, yLim, xLog, yLog);
		layerList.back().setPoints(points);

		// Calculate and update the new plot limits if necessary
		if (includeAllLayersInLim) {
			updateLimits();
		}
	} else {
		throw invalid_argument("A layer with the same id exists. Please change the id and try to add it again.");
	}
}

void ofxGPlot::removeLayer(const string& id) {
	int index = -1;

	for (vector<ofxGLayer>::size_type i = 0; i < layerList.size(); ++i) {
		if (layerList[i].isId(id)) {
			index = i;
			break;
		}
	}

	if (index >= 0) {
		layerList.erase(layerList.begin() + index);

		// Calculate and update the new plot limits if necessary
		if (includeAllLayersInLim) {
			updateLimits();
		}
	} else {
		ofLogWarning("Couldn't find a layer in the plot with id = " + id);
	}
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

const ofxGPoint* ofxGPlot::getPointAt(float xScreen, float yScreen, const string& layerId) const {
	const ofxGPoint* p = nullptr;

	if (mainLayer.isId(layerId)) {
		p = getPointAt(xScreen, yScreen);
	} else {
		for (const ofxGLayer& layer : layerList) {
			if (layer.isId(layerId)) {
				array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);
				p = layer.getPointAtPlotPos(plotPos[0], plotPos[1]);
				break;
			}
		}
	}

	return p;
}

void ofxGPlot::addPointAt(float xScreen, float yScreen) {
	array<float, 2> value = getValueAt(xScreen, yScreen);
	addPoint(value[0], value[1]);
}

void ofxGPlot::addPointAt(float xScreen, float yScreen, const string& layerId) {
	array<float, 2> value = getValueAt(xScreen, yScreen);
	addPoint(value[0], value[1], layerId);
}

void ofxGPlot::removePointAt(float xScreen, float yScreen) {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);
	vector<ofxGPoint>::size_type pointIndex = mainLayer.getPointIndexAtPlotPos(plotPos[0], plotPos[1]);

	if (pointIndex < mainLayer.getPointsRef().size()) {
		removePoint(pointIndex);
	}
}

void ofxGPlot::removePointAt(float xScreen, float yScreen, const string& layerId) {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);
	vector<ofxGPoint>::size_type pointIndex = getLayer(layerId).getPointIndexAtPlotPos(plotPos[0], plotPos[1]);

	if (pointIndex >= 0) {
		removePoint(pointIndex, layerId);
	}
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

	for (ofxGLayer& layer : layerList) {
		layer.setXYLim(xLim, yLim);
	}
}

array<float, 2> ofxGPlot::calculatePlotXLim() {
	// Find the limits for the main layer
	array<float, 2> lim = calculatePointsXLim(mainLayer.getPointsRef());

	// Include the other layers in the limit calculation if necessary
	if (includeAllLayersInLim) {
		for (ofxGLayer& layer : layerList) {
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
		for (ofxGLayer& layer : layerList) {
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

array<float, 2> ofxGPlot::calculatePointsXLim(const vector<ofxGPoint>& points) {
	// Find the points limits
	array<float, 2> lim = { numeric_limits<float>::infinity(), -numeric_limits<float>::infinity() };

	for (const ofxGPoint& p : points) {
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

array<float, 2> ofxGPlot::calculatePointsYLim(const vector<ofxGPoint>& points) {
	// Find the points limits
	array<float, 2> lim = { numeric_limits<float>::infinity(), -numeric_limits<float>::infinity() };

	for (const ofxGPoint& p : points) {
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

void ofxGPlot::moveHorizontalAxesLim(float delta) {
	// Obtain the new x limits
	if (xLog) {
		float deltaLim = pow(10, log10(xLim[1] / xLim[0]) * delta / dim[0]);
		xLim[0] *= deltaLim;
		xLim[1] *= deltaLim;
	} else {
		float deltaLim = (xLim[1] - xLim[0]) * delta / dim[0];
		xLim[0] += deltaLim;
		xLim[1] += deltaLim;
	}

	// Fix the limits
	fixedXLim = true;
	fixedYLim = true;

	// Move the horizontal axes
	xAxis.moveLim(xLim);
	topAxis.moveLim(xLim);

	// Update the plot limits
	updateLimits();
}

void ofxGPlot::moveVerticalAxesLim(float delta) {
	// Obtain the new y limits
	if (yLog) {
		float deltaLim = pow(10, log10(yLim[1] / yLim[0]) * delta / dim[1]);
		yLim[0] *= deltaLim;
		yLim[1] *= deltaLim;
	} else {
		float deltaLim = (yLim[1] - yLim[0]) * delta / dim[1];
		yLim[0] += deltaLim;
		yLim[1] += deltaLim;
	}

	// Fix the limits
	fixedXLim = true;
	fixedYLim = true;

	// Move the vertical axes
	yAxis.moveLim(yLim);
	rightAxis.moveLim(yLim);

	// Update the plot limits
	updateLimits();
}

void ofxGPlot::centerAndZoom(float factor, float xValue, float yValue) {
	// Calculate the new limits
	if (xLog) {
		float deltaLim = pow(10, log10(xLim[1] / xLim[0]) / (2 * factor));
		xLim = {xValue / deltaLim, xValue * deltaLim};
	} else {
		float deltaLim = (xLim[1] - xLim[0]) / (2 * factor);
		xLim = {xValue - deltaLim, xValue + deltaLim};
	}

	if (yLog) {
		float deltaLim = pow(10, log10(yLim[1] / yLim[0]) / (2 * factor));
		yLim = {yValue / deltaLim, yValue * deltaLim};
	} else {
		float deltaLim = (yLim[1] - yLim[0]) / (2 * factor);
		yLim = {yValue - deltaLim, yValue + deltaLim};
	}

	// Fix the limits
	fixedXLim = true;
	fixedYLim = true;

	// Update the horizontal and vertical axes
	xAxis.setLim(xLim);
	topAxis.setLim(xLim);
	yAxis.setLim(yLim);
	rightAxis.setLim(yLim);

	// Update the plot limits (the layers, because the limits are fixed)
	updateLimits();
}

void ofxGPlot::zoom(float factor) {
	array<float, 2> centerValue = mainLayer.plotToValue(dim[0] / 2, -dim[1] / 2);

	centerAndZoom(factor, centerValue[0], centerValue[1]);
}

void ofxGPlot::zoom(float factor, float xScreen, float yScreen) {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);
	array<float, 2> value = mainLayer.plotToValue(plotPos[0], plotPos[1]);

	if (xLog) {
		float deltaLim = pow(10, log10(xLim[1] / xLim[0]) / (2 * factor));
		float offset = pow(10, (log10(xLim[1] / xLim[0]) / factor) * (0.5 - plotPos[0] / dim[0]));
		xLim = {value[0] * offset / deltaLim, value[0] * offset * deltaLim};
	} else {
		float deltaLim = (xLim[1] - xLim[0]) / (2 * factor);
		float offset = 2 * deltaLim * (0.5 - plotPos[0] / dim[0]);
		xLim = {value[0] + offset - deltaLim, value[0] + offset + deltaLim};
	}

	if (yLog) {
		float deltaLim = pow(10, log10(yLim[1] / yLim[0]) / (2 * factor));
		float offset = pow(10, (log10(yLim[1] / yLim[0]) / factor) * (0.5 + plotPos[1] / dim[1]));
		yLim = {value[1] * offset / deltaLim, value[1] * offset * deltaLim};
	} else {
		float deltaLim = (yLim[1] - yLim[0]) / (2 * factor);
		float offset = 2 * deltaLim * (0.5 + plotPos[1] / dim[1]);
		yLim = {value[1] + offset - deltaLim, value[1] + offset + deltaLim};
	}

	// Fix the limits
	fixedXLim = true;
	fixedYLim = true;

	// Update the horizontal and vertical axes
	xAxis.setLim(xLim);
	topAxis.setLim(xLim);
	yAxis.setLim(yLim);
	rightAxis.setLim(yLim);

	// Update the plot limits (the layers, because the limits are fixed)
	updateLimits();
}

void ofxGPlot::shiftPlotPos(const array<float, 2>& valuePlotPos, const array<float, 2>& newPlotPos) {
	// Calculate the new limits
	float deltaXPlot = valuePlotPos[0] - newPlotPos[0];
	float deltaYPlot = valuePlotPos[1] - newPlotPos[1];

	if (xLog) {
		float deltaLim = pow(10, log10(xLim[1] / xLim[0]) * deltaXPlot / dim[0]);
		xLim = {xLim[0] * deltaLim, xLim[1] * deltaLim};
	} else {
		float deltaLim = (xLim[1] - xLim[0]) * deltaXPlot / dim[0];
		xLim = {xLim[0] + deltaLim, xLim[1] + deltaLim};
	}

	if (yLog) {
		float deltaLim = pow(10, -log10(yLim[1] / yLim[0]) * deltaYPlot / dim[1]);
		yLim = {yLim[0] * deltaLim, yLim[1] * deltaLim};
	} else {
		float deltaLim = -(yLim[1] - yLim[0]) * deltaYPlot / dim[1];
		yLim = {yLim[0] + deltaLim, yLim[1] + deltaLim};
	}

	// Fix the limits
	fixedXLim = true;
	fixedYLim = true;

	// Move the horizontal and vertical axes
	xAxis.moveLim(xLim);
	topAxis.moveLim(xLim);
	yAxis.moveLim(yLim);
	rightAxis.moveLim(yLim);

	// Update the plot limits (the layers, because the limits are fixed)
	updateLimits();
}

void ofxGPlot::align(float xValue, float yValue, float xScreen, float yScreen) {
	array<float, 2> valuePlotPos = mainLayer.valueToPlot(xValue, yValue);
	array<float, 2> newPlotPos = getPlotPosAt(xScreen, yScreen);

	shiftPlotPos(valuePlotPos, newPlotPos);
}

void ofxGPlot::align(const array<float, 2>& value, float xScreen, float yScreen) {
	align(value[0], value[1], xScreen, yScreen);
}

void ofxGPlot::center(float xScreen, float yScreen) {
	array<float, 2> valuePlotPos = getPlotPosAt(xScreen, yScreen);
	array<float, 2> newPlotPos = { dim[0] / 2, -dim[1] / 2 };

	shiftPlotPos(valuePlotPos, newPlotPos);
}

void ofxGPlot::startHistograms(ofxGHistogramType histType) {
	mainLayer.startHistogram(histType);

	for (ofxGLayer& layer : layerList) {
		layer.startHistogram(histType);
	}
}

void ofxGPlot::stopHistograms() {
	mainLayer.stopHistogram();

	for (ofxGLayer& layer : layerList) {
		layer.stopHistogram();
	}
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

	for (const ofxGLayer& layer : layerList) {
		layer.drawPoints();
	}
}

void ofxGPlot::drawPoints(ofPath& pointShape) const {
	mainLayer.drawPoints(pointShape);

	for (const ofxGLayer& layer : layerList) {
		layer.drawPoints(pointShape);
	}
}

void ofxGPlot::drawPoints(const ofImage& pointImg) const {
	mainLayer.drawPoints(pointImg);

	for (const ofxGLayer& layer : layerList) {
		layer.drawPoints(pointImg);
	}
}

void ofxGPlot::drawPoint(const ofxGPoint& point, const ofColor& pointColor, float pointSize) const {
	mainLayer.drawPoint(point, pointColor, pointSize);
}

void ofxGPlot::drawPoint(const ofxGPoint& point) const {
	mainLayer.drawPoint(point);
}

void ofxGPlot::drawPoint(const ofxGPoint& point, const ofPath& pointShape) const {
	mainLayer.drawPoint(point, pointShape);
}

void ofxGPlot::drawPoint(const ofxGPoint& point, ofPath& pointShape, const ofColor& pointColor) const {
	mainLayer.drawPoint(point, pointShape, pointColor);
}

void ofxGPlot::drawPoint(const ofxGPoint& point, const ofImage& pointImg) const {
	mainLayer.drawPoint(point, pointImg);
}

void ofxGPlot::drawLines() {
	mainLayer.drawLines();

	for (ofxGLayer& layer : layerList) {
		layer.drawLines();
	}
}

void ofxGPlot::drawLine(const ofxGPoint& point1, const ofxGPoint& point2, const ofColor& lineColor, float lineWidth) {
	mainLayer.drawLine(point1, point2, lineColor, lineWidth);
}

void ofxGPlot::drawLine(const ofxGPoint& point1, const ofxGPoint& point2) {
	mainLayer.drawLine(point1, point2);
}

void ofxGPlot::drawLine(float slope, float yCut, const ofColor& lineColor, float lineWidth) {
	mainLayer.drawLine(slope, yCut, lineColor, lineWidth);
}

void ofxGPlot::drawLine(float slope, float yCut) {
	mainLayer.drawLine(slope, yCut);
}

void ofxGPlot::drawHorizontalLine(float value, const ofColor& lineColor, float lineWidth) const {
	mainLayer.drawHorizontalLine(value, lineColor, lineWidth);
}

void ofxGPlot::drawHorizontalLine(float value) const {
	mainLayer.drawHorizontalLine(value);
}

void ofxGPlot::drawVerticalLine(float value, const ofColor& lineColor, float lineWidth) const {
	mainLayer.drawVerticalLine(value, lineColor, lineWidth);
}

void ofxGPlot::drawVerticalLine(float value) const {
	mainLayer.drawVerticalLine(value);
}

void ofxGPlot::drawFilledContours(ofxGContourType contourType, float referenceValue) {
	mainLayer.drawFilledContour(contourType, referenceValue);

	for (ofxGLayer& layer : layerList) {
		layer.drawFilledContour(contourType, referenceValue);
	}
}

void ofxGPlot::drawLabel(const ofxGPoint& point) const {
	mainLayer.drawLabel(point);
}

void ofxGPlot::drawLabelsAt(float xScreen, float yScreen) const {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);
	mainLayer.drawLabelAtPlotPos(plotPos[0], plotPos[1]);

	for (const ofxGLayer& layer : layerList) {
		layer.drawLabelAtPlotPos(plotPos[0], plotPos[1]);
	}
}

void ofxGPlot::drawLabels() const {
	if (labelingIsActive && mousePosIsSet) {
		drawLabelsAt(mousePos[0], mousePos[1]);
	}
}

void ofxGPlot::drawGridLines(ofxGDirection gridType) const {
	ofPushStyle();
	ofSetColor(gridLineColor);
	ofSetLineWidth(gridLineWidth);

	if (gridType == GRAFICA_BOTH_DIRECTIONS || gridType == GRAFICA_VERTICAL_DIRECTION) {
		const vector<float> xPlotTicks = xAxis.getPlotTicksRef();

		for (float xPlot : xPlotTicks) {
			if (xPlot >= 0 && xPlot <= dim[0]) {
				ofDrawLine(xPlot, 0, xPlot, -dim[1]);
			}
		}
	}

	if (gridType == GRAFICA_BOTH_DIRECTIONS || gridType == GRAFICA_HORIZONTAL_DIRECTION) {
		const vector<float> yPlotTicks = yAxis.getPlotTicksRef();

		for (float yPlot : yPlotTicks) {
			if (-yPlot >= 0 && -yPlot <= dim[1]) {
				ofDrawLine(0, yPlot, dim[0], yPlot);
			}
		}
	}

	ofPopStyle();
}

void ofxGPlot::drawHistograms() {
	mainLayer.drawHistogram();

	for (ofxGLayer& layer : layerList) {
		layer.drawHistogram();
	}
}

void ofxGPlot::drawPolygon(const vector<ofxGPoint>& polygonPoints, const ofColor& polygonColor) {
	mainLayer.drawPolygon(polygonPoints, polygonColor);
}

void ofxGPlot::drawAnnotation(const string& text, float x, float y, ofxGTextAlignment verAlign) const {
	mainLayer.drawAnnotation(text, x, y, verAlign);
}

void ofxGPlot::drawLegend(const vector<string>& text, const vector<float>& xRelativePos,
		const vector<float>& yRelativePos) const {
	ofPushStyle();
	ofFill();
	ofSetRectMode(OF_RECTMODE_CENTER);

	for (vector<string>::size_type i = 0; i < text.size(); ++i) {
		array<float, 2> plotPosition = { xRelativePos[i] * dim[0], -yRelativePos[i] * dim[1] };
		array<float, 2> position = mainLayer.plotToValue(plotPosition[0], plotPosition[1]);

		if (i == 0) {
			ofSetColor(mainLayer.getLineColor());
			ofDrawRectangle(plotPosition[0] - 15, plotPosition[1], 14, 14);
			mainLayer.drawAnnotation(text[i], position[0], position[1], GRAFICA_CENTER_ALIGN);
		} else {
			ofSetColor(layerList[i - 1].getLineColor());
			ofDrawRectangle(plotPosition[0] - 15, plotPosition[1], 14, 14);
			layerList[i - 1].drawAnnotation(text[i], position[0], position[1], GRAFICA_CENTER_ALIGN);
		}
	}

	ofPopStyle();
}

void ofxGPlot::setPos(float x, float y) {
	pos = {x, y};
}

void ofxGPlot::setPos(const array<float, 2>& newPos) {
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

			for (ofxGLayer& layer : layerList) {
				layer.setDim(dim);
			}
		}
	}
}

void ofxGPlot::setOuterDim(const array<float, 2>& newOuterDim) {
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

void ofxGPlot::setMar(const array<float, 4>& newMar) {
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

			for (ofxGLayer& layer : layerList) {
				layer.setDim(dim);
			}
		}
	}
}

void ofxGPlot::setDim(const array<float, 2>& newDim) {
	setDim(newDim[0], newDim[1]);
}

void ofxGPlot::setXLim(float lowerLim, float upperLim) {
	// Make sure the new limits makes sense
	if (lowerLim == upperLim) {
		throw invalid_argument("The limit range cannot be zero.");
	} else if (xLog && (lowerLim <= 0 || upperLim <= 0)) {
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

void ofxGPlot::setXLim(const array<float, 2>& newXLim) {
	setXLim(newXLim[0], newXLim[1]);
}

void ofxGPlot::setYLim(float lowerLim, float upperLim) {
	// Make sure the new limits makes sense
	if (lowerLim == upperLim) {
		throw invalid_argument("The limit range cannot be zero.");
	} else if (yLog && (lowerLim <= 0 || upperLim <= 0)) {
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

void ofxGPlot::setYLim(const array<float, 2>& newYLim) {
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

void ofxGPlot::setLogScale(const string& logType) {
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
		xAxis.setLimAndLogScale(xLim, xLog);
		topAxis.setLimAndLogScale(xLim, xLog);
		yAxis.setLimAndLogScale(yLim, yLog);
		rightAxis.setLimAndLogScale(yLim, yLog);

		// Update the layers
		mainLayer.setLimAndLog(xLim, yLim, xLog, yLog);

		for (ofxGLayer& layer : layerList) {
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

		for (ofxGLayer& layer : layerList) {
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

		for (ofxGLayer& layer : layerList) {
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
	// Make sure that the value makes sense
	if (expandFactor < 0) {
		throw invalid_argument("The expansion factor should be positive.");
	}

	if (expandFactor != expandLimFactor) {
		expandLimFactor = expandFactor;

		// Update the plot limits
		updateLimits();
	}
}

void ofxGPlot::setBgColor(const ofColor& newBgColor) {
	bgColor = newBgColor;
}

void ofxGPlot::setBoxBgColor(const ofColor& newBoxBgColor) {
	boxBgColor = newBoxBgColor;
}

void ofxGPlot::setBoxLineColor(const ofColor& newBoxLineColor) {
	boxLineColor = newBoxLineColor;
}

void ofxGPlot::setBoxLineWidth(float newBoxLineWidth) {
	if (newBoxLineWidth <= 0) {
		throw invalid_argument("The line width should be larger than zero.");
	}

	boxLineWidth = newBoxLineWidth;
}

void ofxGPlot::setGridLineColor(const ofColor& newGridLineColor) {
	gridLineColor = newGridLineColor;
}

void ofxGPlot::setGridLineWidth(float newGridLineWidth) {
	if (newGridLineWidth <= 0) {
		throw invalid_argument("The line width should be larger than zero.");
	}

	gridLineWidth = newGridLineWidth;
}

void ofxGPlot::setPoints(const vector<ofxGPoint>& points) {
	mainLayer.setPoints(points);
	updateLimits();
}

void ofxGPlot::setPoints(const vector<ofxGPoint>& points, const string& layerId) {
	getLayer(layerId).setPoints(points);
	updateLimits();
}

void ofxGPlot::setPoint(int index, float x, float y, const string& label) {
	mainLayer.setPoint(index, x, y, label);
	updateLimits();
}

void ofxGPlot::setPoint(int index, float x, float y, const string& label, const string& layerId) {
	getLayer(layerId).setPoint(index, x, y, label);
	updateLimits();
}

void ofxGPlot::setPoint(int index, float x, float y) {
	mainLayer.setPoint(index, x, y);
	updateLimits();
}

void ofxGPlot::setPoint(int index, const ofxGPoint& newPoint) {
	mainLayer.setPoint(index, newPoint);
	updateLimits();
}

void ofxGPlot::setPoint(int index, const ofxGPoint& newPoint, const string& layerId) {
	getLayer(layerId).setPoint(index, newPoint);
	updateLimits();
}

void ofxGPlot::addPoint(float x, float y, const string& label) {
	mainLayer.addPoint(x, y, label);
	updateLimits();
}

void ofxGPlot::addPoint(float x, float y, const string& label, const string& layerId) {
	getLayer(layerId).addPoint(x, y, label);
	updateLimits();
}

void ofxGPlot::addPoint(float x, float y) {
	mainLayer.addPoint(x, y);
	updateLimits();
}

void ofxGPlot::addPoint(const ofxGPoint& newPoint) {
	mainLayer.addPoint(newPoint);
	updateLimits();
}

void ofxGPlot::addPoint(const ofxGPoint& newPoint, const string& layerId) {
	getLayer(layerId).addPoint(newPoint);
	updateLimits();
}

void ofxGPlot::addPoint(int index, float x, float y, const string& label) {
	mainLayer.addPoint(index, x, y, label);
	updateLimits();
}

void ofxGPlot::addPoint(int index, float x, float y, const string& label, const string& layerId) {
	getLayer(layerId).addPoint(index, x, y, label);
	updateLimits();
}

void ofxGPlot::addPoint(int index, float x, float y) {
	mainLayer.addPoint(index, x, y);
	updateLimits();
}

void ofxGPlot::addPoint(int index, const ofxGPoint& newPoint) {
	mainLayer.addPoint(index, newPoint);
	updateLimits();
}

void ofxGPlot::addPoint(int index, const ofxGPoint& newPoint, const string& layerId) {
	getLayer(layerId).addPoint(index, newPoint);
	updateLimits();
}

void ofxGPlot::addPoints(const vector<ofxGPoint>& newPoints) {
	mainLayer.addPoints(newPoints);
	updateLimits();
}

void ofxGPlot::addPoints(const vector<ofxGPoint>& newPoints, const string& layerId) {
	getLayer(layerId).addPoints(newPoints);
	updateLimits();
}

void ofxGPlot::removePoint(int index) {
	mainLayer.removePoint(index);
	updateLimits();
}

void ofxGPlot::removePoint(int index, const string& layerId) {
	getLayer(layerId).removePoint(index);
	updateLimits();
}

void ofxGPlot::setPointColors(const vector<ofColor>& pointColors) {
	mainLayer.setPointColors(pointColors);
}

void ofxGPlot::setPointColor(const ofColor& pointColor) {
	mainLayer.setPointColor(pointColor);
}

void ofxGPlot::setPointSizes(const vector<float>& pointSizes) {
	mainLayer.setPointSizes(pointSizes);
}

void ofxGPlot::setPointSize(float pointSize) {
	mainLayer.setPointSize(pointSize);
}

void ofxGPlot::setLineColor(const ofColor& lineColor) {
	mainLayer.setLineColor(lineColor);
}

void ofxGPlot::setLineWidth(float lineWidth) {
	mainLayer.setLineWidth(lineWidth);
}

void ofxGPlot::setHistBasePoint(const ofxGPoint& basePoint) {
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

void ofxGPlot::setLabelBgColor(const ofColor& labelBgColor) {
	mainLayer.setLabelBgColor(labelBgColor);
}

void ofxGPlot::setLabelSeparation(const array<float, 2>& labelSeparation) {
	mainLayer.setLabelSeparation(labelSeparation);
}

void ofxGPlot::setTitleText(const string& text) {
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

void ofxGPlot::setHorizontalAxesTicks(const vector<float>& ticks) {
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

void ofxGPlot::setVerticalAxesTicks(const vector<float>& ticks) {
	yAxis.setTicks(ticks);
	rightAxis.setTicks(ticks);
}

void ofxGPlot::setFontName(const string& fontName) {
	mainLayer.setFontName(fontName);
}

void ofxGPlot::setFontColor(const ofColor& fontColor) {
	mainLayer.setFontColor(fontColor);
}

void ofxGPlot::setFontSize(int fontSize) {
	mainLayer.setFontSize(fontSize);
}

void ofxGPlot::setFontProperties(const string& fontName, const ofColor& fontColor, int fontSize) {
	mainLayer.setFontProperties(fontName, fontColor, fontSize);
}

void ofxGPlot::setAllFontProperties(const string& fontName, const ofColor& fontColor, int fontSize) {
	xAxis.setAllFontProperties(fontName, fontColor, fontSize);
	topAxis.setAllFontProperties(fontName, fontColor, fontSize);
	yAxis.setAllFontProperties(fontName, fontColor, fontSize);
	rightAxis.setAllFontProperties(fontName, fontColor, fontSize);
	title.setFontProperties(fontName, fontColor, fontSize);

	mainLayer.setAllFontProperties(fontName, fontColor, fontSize);

	for (ofxGLayer& layer : layerList) {
		layer.setAllFontProperties(fontName, fontColor, fontSize);
	}
}

void ofxGPlot::setFontsMakeContours(bool makeContours) {
	xAxis.setFontsMakeContours(makeContours);
	topAxis.setFontsMakeContours(makeContours);
	yAxis.setFontsMakeContours(makeContours);
	rightAxis.setFontsMakeContours(makeContours);
	title.setFontMakeContours(makeContours);

	mainLayer.setFontsMakeContours(makeContours);

	for (ofxGLayer& layer : layerList) {
		layer.setFontsMakeContours(makeContours);
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

ofxGLayer& ofxGPlot::getLayer(const string& layerId) {
	if (mainLayer.isId(layerId)) {
		return mainLayer;
	}

	for (ofxGLayer& layer : layerList) {
		if (layer.isId(layerId)) {
			return layer;
		}
	}

	throw invalid_argument("Couldn't find a layer with id = " + layerId);
}

ofxGAxis& ofxGPlot::getXAxis() {
	return xAxis;
}

ofxGAxis& ofxGPlot::getTopAxis() {
	return topAxis;
}

ofxGAxis& ofxGPlot::getYAxis() {
	return yAxis;
}

ofxGAxis& ofxGPlot::getRightAxis() {
	return rightAxis;
}

ofxGTitle& ofxGPlot::getTitle() {
	return title;
}

vector<ofxGPoint> ofxGPlot::getPoints() const {
	return mainLayer.getPoints();
}

vector<ofxGPoint> ofxGPlot::getPoints(const string& layerId) const {
	if (mainLayer.isId(layerId)) {
		return mainLayer.getPoints();
	}

	for (const ofxGLayer& layer : layerList) {
		if (layer.isId(layerId)) {
			return layer.getPoints();
		}
	}

	throw invalid_argument("Couldn't find a layer with id = " + layerId);
}

const vector<ofxGPoint>& ofxGPlot::getPointsRef() const {
	return mainLayer.getPointsRef();
}

const vector<ofxGPoint>& ofxGPlot::getPointsRef(const string& layerId) const {
	if (mainLayer.isId(layerId)) {
		return mainLayer.getPointsRef();
	}

	for (const ofxGLayer& layer : layerList) {
		if (layer.isId(layerId)) {
			return layer.getPointsRef();
		}
	}

	throw invalid_argument("Couldn't find a layer with id = " + layerId);
}

ofxGHistogram& ofxGPlot::getHistogram() {
	return mainLayer.getHistogram();
}

ofxGHistogram& ofxGPlot::getHistogram(const string& layerId) {
	return getLayer(layerId).getHistogram();
}

void ofxGPlot::activateZooming(float factor, int increaseButton, int decreaseButton, int increaseKeyModifier,
		int decreaseKeyModifier) {
	zoomFactor = factor;
	increaseZoomButton = increaseButton;
	decreaseZoomButton = decreaseButton;
	increaseZoomKeyModifier = increaseKeyModifier;
	decreaseZoomKeyModifier = decreaseKeyModifier;
	zoomingIsActive = true;
}

void ofxGPlot::activateZooming(float factor, int increaseButton, int decreaseButton) {
	activateZooming(factor, increaseButton, decreaseButton, GRAFICA_NONE_MODIFIER, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::activateZooming(float factor) {
	activateZooming(factor, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_RIGHT, GRAFICA_NONE_MODIFIER, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::activateZooming() {
	activateZooming(1.3, OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_RIGHT, GRAFICA_NONE_MODIFIER, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::deactivateZooming() {
	zoomingIsActive = false;
}

void ofxGPlot::activateCentering(int button, int keyModifier) {
	centeringButton = button;
	centeringKeyModifier = keyModifier;
	centeringIsActive = true;
}

void ofxGPlot::activateCentering(int button) {
	activateCentering(button, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::activateCentering() {
	activateCentering(OF_MOUSE_BUTTON_LEFT, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::deactivateCentering() {
	centeringIsActive = false;
}

void ofxGPlot::activatePanning(int button, int keyModifier) {
	panningButton = button;
	panningKeyModifier = keyModifier;
	panningIsActive = true;
}

void ofxGPlot::activatePanning(int button) {
	activatePanning(button, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::activatePanning() {
	activatePanning(OF_MOUSE_BUTTON_LEFT, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::deactivatePanning() {
	panningIsActive = false;
	panningReferencePointIsSet = false;
}

void ofxGPlot::activatePointLabels(int button, int keyModifier) {
	labelingButton = button;
	labelingKeyModifier = keyModifier;
	labelingIsActive = true;
}

void ofxGPlot::activatePointLabels(int button) {
	activatePointLabels(button, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::activatePointLabels() {
	activatePointLabels(OF_MOUSE_BUTTON_LEFT, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::deactivatePointLabels() {
	labelingIsActive = false;
	mousePosIsSet = false;
}

void ofxGPlot::activateReset(int button, int keyModifier) {
	resetButton = button;
	resetKeyModifier = keyModifier;
	resetIsActive = true;
	resetLimitsAreSet = false;
}

void ofxGPlot::activateReset(int button) {
	activateReset(button, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::activateReset() {
	activateReset(OF_MOUSE_BUTTON_RIGHT, GRAFICA_NONE_MODIFIER);
}

void ofxGPlot::deactivateReset() {
	resetIsActive = false;
	resetLimitsAreSet = false;
}

void ofxGPlot::mouseEventHandler(ofMouseEventArgs& args) {
	if (zoomingIsActive || centeringIsActive || panningIsActive || labelingIsActive || resetIsActive) {
		ofMouseEventArgs::Type eventType = args.type;
		int button = args.button;
		float xPos = args.x;
		float yPos = args.y;
		float wheelCounter = args.scrollY;

		bool finishedPanning = false;
		bool finishedZoom = false;

		if (panningIsActive) {
			if (button == panningButton
					&& (panningKeyModifier == GRAFICA_NONE_MODIFIER
							|| (keyIsPressed && pressedKey == panningKeyModifier))) {
				if (eventType == ofMouseEventArgs::Type::Dragged) {
					if (panningReferencePointIsSet) {
						// Save the axes limits if it's the first mouse modification after the last reset
						if (resetIsActive && !resetLimitsAreSet) {
							xLimReset = xLim;
							yLimReset = yLim;
							resetLimitsAreSet = true;
						}

						align(panningReferencePoint, xPos, yPos);
					} else if (isOverBox(xPos, yPos)) {
						panningReferencePoint = getValueAt(xPos, yPos);
						panningReferencePointIsSet = true;
					}
				} else if (eventType == ofMouseEventArgs::Type::Released) {
					panningReferencePointIsSet = false;
					finishedPanning = true;
				}
			}
		}

		if (zoomingIsActive && !finishedPanning
				&& (eventType == ofMouseEventArgs::Type::Released || eventType == ofMouseEventArgs::Type::Scrolled)) {
			if (button == increaseZoomButton
					&& (increaseZoomKeyModifier == GRAFICA_NONE_MODIFIER
							|| (keyIsPressed && pressedKey == increaseZoomKeyModifier))) {
				if (isOverBox(xPos, yPos)) {
					// Save the axes limits if it's the first mouse modification after the last reset
					if (resetIsActive && !resetLimitsAreSet) {
						xLimReset = xLim;
						yLimReset = yLim;
						resetLimitsAreSet = true;
					}

					if (wheelCounter == 0 || (wheelCounter < 0 && increaseZoomButton == decreaseZoomButton)) {
						zoom(zoomFactor, xPos, yPos);
						finishedZoom = true;
					}
				}
			}

			if (button == decreaseZoomButton
					&& (decreaseZoomKeyModifier == GRAFICA_NONE_MODIFIER
							|| (keyIsPressed && pressedKey == decreaseZoomKeyModifier))) {
				if (isOverBox(xPos, yPos)) {
					// Save the axes limits if it's the first mouse modification after the last reset
					if (resetIsActive && !resetLimitsAreSet) {
						xLimReset = xLim;
						yLimReset = yLim;
						resetLimitsAreSet = true;
					}

					if (wheelCounter == 0 || (wheelCounter > 0 && increaseZoomButton == decreaseZoomButton)) {
						zoom(1 / zoomFactor, xPos, yPos);
						finishedZoom = true;
					}
				}
			}
		}

		if (centeringIsActive && !finishedPanning && !finishedZoom
				&& (eventType == ofMouseEventArgs::Type::Released || eventType == ofMouseEventArgs::Type::Scrolled)) {
			if (button == centeringButton
					&& (centeringKeyModifier == GRAFICA_NONE_MODIFIER
							|| (keyIsPressed && pressedKey == centeringKeyModifier))) {
				if (isOverBox(xPos, yPos)) {
					// Save the axes limits if it's the first mouse modification after the last reset
					if (resetIsActive && !resetLimitsAreSet) {
						xLimReset = xLim;
						yLimReset = yLim;
						resetLimitsAreSet = true;
					}

					center(xPos, yPos);
				}
			}
		}

		if (labelingIsActive) {
			if (button == labelingButton
					&& (labelingKeyModifier == GRAFICA_NONE_MODIFIER
							|| (keyIsPressed && pressedKey == labelingKeyModifier))) {
				if ((eventType == ofMouseEventArgs::Type::Pressed || eventType == ofMouseEventArgs::Type::Dragged)
						&& isOverBox(xPos, yPos)) {
					mousePos = {xPos, yPos};
					mousePosIsSet = true;
				} else {
					mousePosIsSet = false;
				}
			}
		}

		if (resetIsActive
				&& (eventType == ofMouseEventArgs::Type::Released || eventType == ofMouseEventArgs::Type::Scrolled)) {
			if (button == resetButton
					&& (resetKeyModifier == GRAFICA_NONE_MODIFIER || (keyIsPressed && pressedKey == resetKeyModifier))) {
				if (isOverBox(xPos, yPos)) {
					if (resetLimitsAreSet) {
						setXLim(xLimReset);
						setYLim(yLimReset);
						resetLimitsAreSet = false;
					}
				}
			}
		}
	}
}

void ofxGPlot::keyEventHandler(ofKeyEventArgs& args) {
	if (zoomingIsActive || centeringIsActive || panningIsActive || labelingIsActive || resetIsActive) {
		if (args.type == ofKeyEventArgs::Type::Pressed) {
			pressedKey = args.key;
			keyIsPressed = true;
		} else if (args.type == ofKeyEventArgs::Type::Released) {
			keyIsPressed = false;
		}
	}
}
