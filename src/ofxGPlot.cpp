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

array<float, 2> ofxGPlot::getScreenPosAtValue(float xValue,
		float yValue) const {
	float xScreen = mainLayer.valueToXPlot(xValue) + (pos[0] + mar[1]);
	float yScreen = mainLayer.valueToYPlot(yValue) + (pos[1] + mar[2] + dim[1]);

	return {xScreen, yScreen};
}

ofxGPoint ofxGPlot::getPointAt(float xScreen, float yScreen) const {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);

	return mainLayer.getPointAtPlotPos(plotPos[0], plotPos[1]);
}

ofxGPoint ofxGPlot::getPointAt(float xScreen, float yScreen,
		const string &layerId) const {
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

array<float, 2> ofxGPlot::getValueAt(float xScreen, float yScreen) const {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);

	return mainLayer.plotToValue(plotPos[0], plotPos[1]);
}

array<float, 2> ofxGPlot::getRelativePlotPosAt(float xScreen,
		float yScreen) const {
	array<float, 2> plotPos = getPlotPosAt(xScreen, yScreen);

	return {plotPos[0] / dim[0], -plotPos[1] / dim[1]};
}

bool ofxGPlot::isOverPlot(float xScreen, float yScreen) const {
	return (xScreen >= pos[0]) && (xScreen <= pos[0] + outerDim[0])
			&& (yScreen >= pos[1]) && (yScreen <= pos[1] + outerDim[1]);
}

bool ofxGPlot::isOverBox(float xScreen, float yScreen) const {
	return (xScreen >= pos[0] + mar[1])
			&& (xScreen <= pos[0] + outerDim[0] - mar[3])
			&& (yScreen >= pos[1] + mar[2])
			&& (yScreen <= pos[1] + outerDim[1] - mar[0]);
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

}

void ofxGPlot::setPos(const array<float, 2> &newPos) {

}

void ofxGPlot::setOuterDim(float xOuterDim, float yOuterDim) {

}

void ofxGPlot::setOuterDim(const array<float, 2> &newOuterDim) {

}

void ofxGPlot::setMar(float bottomMargin, float leftMargin, float topMargin,
		float rightMargin) {

}

void ofxGPlot::setMar(const array<float, 4> &newMar) {

}

void ofxGPlot::setDim(float xDim, float yDim) {

}

void ofxGPlot::setDim(const array<float, 2> &newDim) {

}

void ofxGPlot::setXLim(float lowerLim, float upperLim) {

}

void ofxGPlot::setXLim(const array<float, 2> &newXLim) {

}

void ofxGPlot::setYLim(float lowerLim, float upperLim) {

}

void ofxGPlot::setYLim(const array<float, 2> &newYLim) {

}

void ofxGPlot::setFixedXLim(bool newFixedXLim) {

}

void ofxGPlot::setFixedYLim(bool newFixedYLim) {

}

void ofxGPlot::setLogScale(const string &logType) {

}

void ofxGPlot::setInvertedXScale(bool newInvertedXScale) {

}

void ofxGPlot::invertXScale() {

}

void ofxGPlot::setInvertedYScale(bool newInvertedYScale) {

}

void ofxGPlot::invertYScale() {

}

void ofxGPlot::setIncludeAllLayersInLim(bool includeAllLayers) {

}

void ofxGPlot::setExpandLimFactor(float expandFactor) {

}

void ofxGPlot::setBgColor(const ofColor &newBgColor) {

}

void ofxGPlot::setBoxBgColor(const ofColor &newBoxBgColor) {

}

void ofxGPlot::setBoxLineColor(const ofColor &newBoxLineColor) {

}

void ofxGPlot::setBoxLineWidth(float newBoxLineWidth) {

}

void ofxGPlot::setGridLineColor(const ofColor &newGridLineColor) {

}

void ofxGPlot::setGridLineWidth(float newGridLineWidth) {

}

void ofxGPlot::setPoints(const vector<ofxGPoint> &points) {
	mainLayer.setPoints(points);
	//updateLimits();
}

void ofxGPlot::setPoints(const vector<ofxGPoint> &points,
		const string &layerId) {

}

void ofxGPlot::setPoint(int index, float x, float y, const string &label) {

}

void ofxGPlot::setPoint(int index, float x, float y, const string &label,
		const string &layerId) {

}

void ofxGPlot::setPoint(int index, float x, float y) {

}

void ofxGPlot::setPoint(int index, const ofxGPoint &newPoint) {

}

void ofxGPlot::setPoint(int index, const ofxGPoint &newPoint,
		const string &layerId) {

}

void ofxGPlot::addPoint(float x, float y, const string &label) {

}

void ofxGPlot::addPoint(float x, float y, const string &label,
		const string &layerId) {

}

void ofxGPlot::addPoint(float x, float y) {

}

void ofxGPlot::addPoint(const ofxGPoint &newPoint) {

}

void ofxGPlot::addPoint(const ofxGPoint &newPoint, const string &layerId) {

}

void ofxGPlot::addPoint(int index, float x, float y, const string &label) {

}

void ofxGPlot::addPoint(int index, float x, float y, const string &label,
		const string &layerId) {

}

void ofxGPlot::addPoint(int index, float x, float y) {

}

void ofxGPlot::addPoint(int index, const ofxGPoint &newPoint) {

}

void ofxGPlot::addPoint(int index, const ofxGPoint &newPoint,
		const string &layerId) {

}

void ofxGPlot::addPoints(const vector<ofxGPoint> &newPoints) {

}

void ofxGPlot::addPoints(const vector<ofxGPoint> &newPoints,
		const string &layerId) {

}

void ofxGPlot::removePoint(int index) {

}

void ofxGPlot::removePoint(int index, const string &layerId) {

}

void ofxGPlot::setPointColors(const vector<ofColor> &pointColors) {

}

void ofxGPlot::setPointColor(const ofColor &pointColor) {

}

void ofxGPlot::setPointSizes(const vector<float> &pointSizes) {

}

void ofxGPlot::setPointSize(float pointSize) {

}

void ofxGPlot::setLineColor(const ofColor &lineColor) {

}

void ofxGPlot::setLineWidth(float lineWidth) {

}

void ofxGPlot::setHistBasePoint(const ofxGPoint &basePoint) {

}

void ofxGPlot::setHistType(int histType) {

}

void ofxGPlot::setHistVisible(bool visible) {

}

void ofxGPlot::setDrawHistLabels(bool drawHistLabels) {

}

void ofxGPlot::setLabelBgColor(const ofColor &labelBgColor) {

}

void ofxGPlot::setLabelSeparation(const array<float, 2> &labelSeparation) {

}

void ofxGPlot::setTitleText(const string &text) {

}

void ofxGPlot::setAxesOffset(float offset) {

}

void ofxGPlot::setTicksLength(float tickLength) {

}

void ofxGPlot::setHorizontalAxesNTicks(int nTicks) {

}

void ofxGPlot::setHorizontalAxesTicksSeparation(float ticksSeparation) {

}

void ofxGPlot::setHorizontalAxesTicks(const vector<float> &ticks) {

}

void ofxGPlot::setVerticalAxesNTicks(int nTicks) {

}

void ofxGPlot::setVerticalAxesTicksSeparation(float ticksSeparation) {

}

void ofxGPlot::setVerticalAxesTicks(const vector<float> &ticks) {

}

void ofxGPlot::setFontName(const string &fontName) {

}

void ofxGPlot::setFontColor(const ofColor &fontColor) {

}

void ofxGPlot::setFontSize(int fontSize) {

}

void ofxGPlot::setFontProperties(const string &fontName,
		const ofColor &fontColor, int fontSize) {

}

void ofxGPlot::setAllFontProperties(const string &fontName,
		const ofColor &fontColor, int fontSize) {

}

