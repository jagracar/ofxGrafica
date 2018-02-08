#include "ofxGPlot.h"

const string ofxGPlot::MAINLAYERID = "main layer";

ofxGPlot::ofxGPlot(float xPos, float yPos, float plotWidth, float plotHeight) {
	pos = {xPos, yPos};
	outerDim = {plotWidth, plotHeight};
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

	bgColor = ofColor(255);
	boxBgColor = ofColor(245);
	boxLineColor = ofColor(210);
	boxLineWidth = 1;
	gridLineColor = ofColor(210);
	gridLineWidth = 1;

	mainLayer = ofxGLayer(MAINLAYERID, dim, xLim, yLim, xLog, yLog);

	xAxis = ofxGAxis();
	topAxis = ofxGAxis();
	yAxis = ofxGAxis();
	rightAxis = ofxGAxis();
	title = ofxGTitle();
	//xAxis = ofxGAxis(ofxGAxis::X_AXIS, dim, xLim, xLog);
	//topAxis = ofxGAxis(ofxGAxis::TOP_AXIS, dim, xLim, xLog);
	//yAxis = ofxGAxis(ofxGAxis::Y_AXIS, dim, yLim, yLog);
	//rightAxis = ofxGAxis(ofxGAxis::RIGHT_AXIS, dim, yLim, yLog);
	//title = ofxGTitle(dim);
}

array<float, 2> ofxGPlot::getPlotPosAt(float xScreen, float yScreen) {
	float xPlot = xScreen - (pos[0] + mar[1]);
	float yPlot = yScreen - (pos[1] + mar[2] + dim[1]);

	return {xPlot, yPlot};
}

array<float, 2> ofxGPlot::getScreenPosAtValue(float xValue, float yValue) {
	float xScreen = mainLayer.valueToXPlot(xValue) + (pos[0] + mar[1]);
	float yScreen = mainLayer.valueToYPlot(yValue) + (pos[1] + mar[2] + dim[1]);

	return {xScreen, yScreen};
}

ofxGPoint ofxGPlot::getPointAt(float xScreen, float yScreen) {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);

	return mainLayer.getPointAtPlotPos(plotPos[0], plotPos[1]);
}

ofxGPoint ofxGPlot::getPointAt(float xScreen, float yScreen,
		const string &layerId) {
	ofxGPoint p;

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

array<float, 2> ofxGPlot::getValueAt(float xScreen, float yScreen) {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);

	return mainLayer.plotToValue(plotPos[0], plotPos[1]);
}

array<float, 2> ofxGPlot::getRelativePlotPosAt(float xScreen, float yScreen) {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);

	return {plotPos[0] / dim[0], -plotPos[1] / dim[1]};
}

bool ofxGPlot::isOverPlot(float xScreen, float yScreen) {
	return (xScreen >= pos[0]) && (xScreen <= pos[0] + outerDim[0])
			&& (yScreen >= pos[1]) && (yScreen <= pos[1] + outerDim[1]);
}

bool ofxGPlot::isOverBox(float xScreen, float yScreen) {
	return (xScreen >= pos[0] + mar[1])
			&& (xScreen <= pos[0] + outerDim[0] - mar[3])
			&& (yScreen >= pos[1] + mar[2])
			&& (yScreen <= pos[1] + outerDim[1] - mar[0]);
}

void ofxGPlot::defaultDraw() {
	beginDraw();
	drawBackground();
	drawBox();
	//drawXAxis();
	//drawYAxis();
	//drawTitle();
	//drawLines();
	//drawPoints();
	endDraw();
}

void ofxGPlot::beginDraw() {
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(pos[0] + mar[1], pos[1] + mar[2] + dim[1], 0);
}

void ofxGPlot::endDraw() {
	ofPopMatrix();
	ofPopStyle();
}

void ofxGPlot::drawBackground() {
	ofPushStyle();
	ofSetRectMode(OF_RECTMODE_CORNER);
	ofFill();
	ofSetColor(bgColor);
	ofDrawRectangle(-mar[1], -mar[2] - dim[1], outerDim[0], outerDim[1]);
	ofPopStyle();
}

void ofxGPlot::drawBox() {
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
