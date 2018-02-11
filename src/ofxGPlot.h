#pragma once

#include "ofMain.h"
#include "ofxGLayer.h"
#include "ofxGAxis.h"
#include "ofxGTitle.h"

class ofxGPlot {
public:

	// Constants
	static const string MAINLAYERID;
	static const int NONE = 0;

	// Constructor
	ofxGPlot(float xPos = 0, float yPos = 0, float plotWidth = 450, float plotHeight = 300);

	// Other methods
	void addLayer(ofxGLayer &newLayer);
	void addLayer(const string &id, const vector<ofxGPoint> &points);
	void removeLayer(const string &id);
	array<float, 2> getPlotPosAt(float xScreen, float yScreen) const;
	array<float, 2> getScreenPosAtValue(float xValue, float yValue) const;
	ofxGPoint getPointAt(float xScreen, float yScreen) const;
	ofxGPoint getPointAt(float xScreen, float yScreen, const string &layerId) const;
	void addPointAt(float xScreen, float yScreen);
	void addPointAt(float xScreen, float yScreen, const string &layerId);
	void removePointAt(float xScreen, float yScreen);
	void removePointAt(float xScreen, float yScreen, const string &layerId);
	array<float, 2> getValueAt(float xScreen, float yScreen) const;
	array<float, 2> getRelativePlotPosAt(float xScreen, float yScreen) const;
	bool isOverPlot(float xScreen, float yScreen) const;
	bool isOverBox(float xScreen, float yScreen) const;
	void updateLimits();
	array<float, 2> calculatePlotXLim();
	array<float, 2> calculatePlotYLim();
	array<float, 2> calculatePointsXLim(const vector<ofxGPoint> &points);
	array<float, 2> calculatePointsYLim(const vector<ofxGPoint> &points);
	void moveHorizontalAxesLim(float delta);
	void moveVerticalAxesLim(float delta);
	void centerAndZoom(float factor, float xValue, float yValue);
	void zoom(float factor);
	void zoom(float factor, float xScreen, float yScreen);
	void shiftPlotPos(const array<float, 2> &valuePlotPos, const array<float, 2> &newPlotPos);
	void align(float xValue, float yValue, float xScreen, float yScreen);
	void align(const array<float, 2> & value, float xScreen, float yScreen);
	void center(float xScreen, float yScreen);
	void startHistograms(int histType);

	// Drawing methods
	void defaultDraw();
	void beginDraw() const;
	void endDraw() const;
	void drawBackground() const;
	void drawBox() const;
	void drawXAxis() const;
	void drawTopAxis() const;
	void drawYAxis() const;
	void drawRightAxis() const;
	void drawTitle() const;
	void drawPoints() const;
	//void drawPoints(const PShape &pointShape) const;
	//void drawPoints(const PImage &pointImg) const;
	void drawPoint(const ofxGPoint &point, const ofColor &pointColor, float pointSize) const;
	void drawPoint(const ofxGPoint &point) const;
	//void drawPoint(const ofxGPoint &point, const PShape &pointShape) const;
	//void drawPoint(const ofxGPoint &point, const PShape &pointShape, const ofColor &pointColor) const;
	//void drawPoint(const ofxGPoint &point, const PImage &pointImg) const;
	void drawLines();
	void drawLine(const ofxGPoint &point1, const ofxGPoint &point2, const ofColor &lineColor, float lineWidth) const;
	void drawLine(const ofxGPoint &point1, const ofxGPoint &point2) const;
	void drawLine(float slope, float yCut, const ofColor &lineColor, float lineWidth) const;
	void drawLine(float slope, float yCut) const;
	void drawHorizontalLine(float value, const ofColor &lineColor, float lineWidth) const;
	void drawHorizontalLine(float value) const;
	void drawVerticalLine(float value, const ofColor &lineColor, float lineWidth) const;
	void drawVerticalLine(float value) const;
	void drawFilledContours(int contourType, float referenceValue) const;
	void drawLabel(const ofxGPoint &point) const;
	void drawLabelsAt(float xScreen, float yScreen) const;
	void drawLabels() const;
	void drawGridLines(int gridType) const;
	void drawHistograms();
	void drawPolygon(const vector<ofxGPoint> &polygonPoints, const ofColor &polygonColor) const;
	void drawAnnotation(const string &text, float x, float y, int horAlign, int verAlign) const;
	void drawLegend(const vector<string> &text, const vector<float> &xRelativePos,
			const vector<float> &yRelativePos) const;

	// Setter methods
	void setPos(float x, float y);
	void setPos(const array<float, 2> &newPos);
	void setOuterDim(float xOuterDim, float yOuterDim);
	void setOuterDim(const array<float, 2> &newOuterDim);
	void setMar(float bottomMargin, float leftMargin, float topMargin, float rightMargin);
	void setMar(const array<float, 4> &newMar);
	void setDim(float xDim, float yDim);
	void setDim(const array<float, 2> &newDim);
	void setXLim(float lowerLim, float upperLim);
	void setXLim(const array<float, 2> &newXLim);
	void setYLim(float lowerLim, float upperLim);
	void setYLim(const array<float, 2> &newYLim);
	void setFixedXLim(bool newFixedXLim);
	void setFixedYLim(bool newFixedYLim);
	void setLogScale(const string &logType);
	void setInvertedXScale(bool newInvertedXScale);
	void invertXScale();
	void setInvertedYScale(bool newInvertedYScale);
	void invertYScale();
	void setIncludeAllLayersInLim(bool includeAllLayers);
	void setExpandLimFactor(float expandFactor);
	void setBgColor(const ofColor &newBgColor);
	void setBoxBgColor(const ofColor &newBoxBgColor);
	void setBoxLineColor(const ofColor &newBoxLineColor);
	void setBoxLineWidth(float newBoxLineWidth);
	void setGridLineColor(const ofColor &newGridLineColor);
	void setGridLineWidth(float newGridLineWidth);
	void setPoints(const vector<ofxGPoint> &points);
	void setPoints(const vector<ofxGPoint> &points, const string &layerId);
	void setPoint(int index, float x, float y, const string &label);
	void setPoint(int index, float x, float y, const string &label, const string &layerId);
	void setPoint(int index, float x, float y);
	void setPoint(int index, const ofxGPoint &newPoint);
	void setPoint(int index, const ofxGPoint &newPoint, const string &layerId);
	void addPoint(float x, float y, const string &label);
	void addPoint(float x, float y, const string &label, const string &layerId);
	void addPoint(float x, float y);
	void addPoint(const ofxGPoint &newPoint);
	void addPoint(const ofxGPoint &newPoint, const string &layerId);
	void addPoint(int index, float x, float y, const string &label);
	void addPoint(int index, float x, float y, const string &label, const string &layerId);
	void addPoint(int index, float x, float y);
	void addPoint(int index, const ofxGPoint &newPoint);
	void addPoint(int index, const ofxGPoint &newPoint, const string &layerId);
	void addPoints(const vector<ofxGPoint> &newPoints);
	void addPoints(const vector<ofxGPoint> &newPoints, const string &layerId);
	void removePoint(int index);
	void removePoint(int index, const string &layerId);
	void setPointColors(const vector<ofColor> &pointColors);
	void setPointColor(const ofColor &pointColor);
	void setPointSizes(const vector<float> &pointSizes);
	void setPointSize(float pointSize);
	void setLineColor(const ofColor &lineColor);
	void setLineWidth(float lineWidth);
	void setHistBasePoint(const ofxGPoint &basePoint);
	void setHistType(int histType);
	void setHistVisible(bool visible);
	void setDrawHistLabels(bool drawHistLabels);
	void setLabelBgColor(const ofColor &labelBgColor);
	void setLabelSeparation(const array<float, 2> &labelSeparation);
	void setTitleText(const string &text);
	void setAxesOffset(float offset);
	void setTicksLength(float tickLength);
	void setHorizontalAxesNTicks(int nTicks);
	void setHorizontalAxesTicksSeparation(float ticksSeparation);
	void setHorizontalAxesTicks(const vector<float> &ticks);
	void setVerticalAxesNTicks(int nTicks);
	void setVerticalAxesTicksSeparation(float ticksSeparation);
	void setVerticalAxesTicks(const vector<float> &ticks);
	void setFontName(const string &fontName);
	void setFontColor(const ofColor &fontColor);
	void setFontSize(int fontSize);
	void setFontProperties(const string &fontName, const ofColor &fontColor, int fontSize);
	void setAllFontProperties(const string &fontName, const ofColor &fontColor, int fontSize);

	// Getter methods
	array<float, 2> getPos() const;
	array<float, 2> getOuterDim() const;
	array<float, 4> getMar() const;
	array<float, 2> getDim() const;
	array<float, 2> getXLim() const;
	array<float, 2> getYLim() const;
	bool getFixedXLim() const;
	bool getFixedYLim() const;
	bool getXLog() const;
	bool getYLog() const;
	bool getInvertedXScale();
	bool getInvertedYScale();
	ofxGLayer &getMainLayer();
	ofxGLayer &getLayer(const string &id);
	ofxGAxis &getXAxis();
	ofxGAxis &getTopAxis();
	ofxGAxis &getYAxis();
	ofxGAxis &getRightAxis();
	ofxGTitle &getTitle();
	vector<ofxGPoint> getPoints() const;
	vector<ofxGPoint> getPoints(const string &layerId) const;
	vector<ofxGPoint> &getPointsRef();
	vector<ofxGPoint> &getPointsRef(const string &layerId);
	ofxGHistogram &getHistogram();
	ofxGHistogram &getHistogram(const string &layerId);

protected:

	// General properties
	array<float, 2> pos;
	array<float, 2> outerDim;
	array<float, 4> mar;
	array<float, 2> dim;
	array<float, 2> xLim;
	array<float, 2> yLim;
	bool fixedXLim;
	bool fixedYLim;
	bool xLog;
	bool yLog;
	bool invertedXScale;
	bool invertedYScale;
	bool includeAllLayersInLim;
	float expandLimFactor;

	// Format properties
	ofColor bgColor;
	ofColor boxBgColor;
	ofColor boxLineColor;
	float boxLineWidth;
	ofColor gridLineColor;
	float gridLineWidth;

	// Layers
	ofxGLayer mainLayer;
	vector<ofxGLayer> layerList;

	// Axes and title
	ofxGAxis xAxis;
	ofxGAxis topAxis;
	ofxGAxis yAxis;
	ofxGAxis rightAxis;
	ofxGTitle title;
};
