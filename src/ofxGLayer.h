#pragma once

#include "ofxGPoint.h"
#include "ofxGHistogram.h"
#include "ofMain.h"

class ofxGLayer {
public:
	// Constructor
	ofxGLayer(const string &_id = "defaultId", const array<float, 2> &_dim = { 0, 1 }, const array<float, 2> &_xLim = {
			0, 1 }, const array<float, 2> &_yLim = { 0, 1 }, bool _xLog = false, bool _yLog = false);

	// Special methods
	bool isId(const string &someId) const;
	float valueToXPlot(float x) const;
	float valueToYPlot(float y) const;
	array<float, 2> valueToPlot(float x, float y) const;
	ofxGPoint valueToPlot(const ofxGPoint &point) const;
	vector<ofxGPoint> &valueToPlot(const vector<ofxGPoint> &pts) const;
	array<float, 2> plotToValue(float xPlot, float yPlot) const;
	bool isInside(float xPlot, float yPlot) const;
	bool isInside(const ofxGPoint &plotPoint) const;
	vector<bool> &isInside(const vector<ofxGPoint> &plotPts) const;
	int getPointIndexAtPlotPos(float xPlot, float yPlot) const;
	ofxGPoint getPointAtPlotPos(float xPlot, float yPlot) const;
	void startHistogram(int histType);

	// Drawing methods
	void drawPoints() const;
	//void drawPoints(const PShape &pointShape) const;
	//void drawPoints(const PImage &pointImg) const;
	void drawPoint(const ofxGPoint &point, const ofColor &pointColor, float pointSize) const;
	void drawPoint(const ofxGPoint &point) const;
	//void drawPoint(const ofxGPoint &point, const PShape &pointShape) const;
	//void drawPoint(const ofxGPoint &point, const PShape &pointShape, const ofColor &pointColor) const;
	//void drawPoint(const ofxGPoint &point, const PImage &pointImg) const;
	void drawLines();
	void drawLine(const ofxGPoint &point1, const ofxGPoint &point2, int lc, float lw) const;
	void drawLine(const ofxGPoint &point1, const ofxGPoint &point2) const;
	void drawLine(float slope, float yCut, int lc, float lw) const;
	void drawLine(float slope, float yCut) const;
	void drawHorizontalLine(float value, int lc, float lw) const;
	void drawHorizontalLine(float value) const;
	void drawVerticalLine(float value, int lc, float lw) const;
	void drawVerticalLine(float value) const;
	void drawFilledContour(int contourType, float referenceValue) const;
	void drawLabel(const ofxGPoint &point) const;
	void drawLabelAtPlotPos(float xPlot, float yPlot) const;
	void drawHistogram() const;
	void drawPolygon(const vector<ofxGPoint> &polygonPoints, int polygonColor) const;
	void drawAnnotation(const string &text, float x, float y, int horAlign, int verAlign) const;

	// Setter methods
	void setDim(float xDim, float yDim);
	void setDim(const array<float, 2> &newDim);
	void setXLim(float xMin, float xMax);
	void setXLim(const array<float, 2> &newXLim);
	void setYLim(float yMin, float yMax);
	void setYLim(const array<float, 2> &newYLim);
	void setXYLim(float xMin, float xMax, float yMin, float yMax);
	void setXYLim(const array<float, 2> &newXLim, const array<float, 2> &newYLim);
	void setLimAndLog(float xMin, float xMax, float yMin, float yMax, bool newXLog, bool newYLog);
	void setLimAndLog(const array<float, 2> &newXLim, const array<float, 2> &newYLim, bool newXLog, bool newYLog);
	void setXLog(bool newXLog);
	void setYLog(bool newYLog);
	void setPoints(const vector<ofxGPoint> &newPoints);
	void setPoint(int index, float x, float y, const string &label);
	void setPoint(int index, float x, float y);
	void setPoint(int index, const ofxGPoint &newPoint);
	void addPoint(float x, float y, const string &label);
	void addPoint(float x, float y);
	void addPoint(const ofxGPoint &newPoint);
	void addPoint(int index, float x, float y, const string &label);
	void addPoint(int index, float x, float y);
	void addPoint(int index, const ofxGPoint &newPoint);
	void addPoints(const vector<ofxGPoint> &newPoints);
	void removePoint(int index);
	void setInside(const vector<bool> &newInside);
	void setPointColors(const vector<ofColor> &newPointColors);
	void setPointColor(const ofColor &newPointColor);
	void setPointSizes(const vector<float> &newPointSizes);
	void setPointSize(float newPointSize);
	void setLineColor(const ofColor &newLineColor);
	void setLineWidth(float newLineWidth);
	void setHistBasePoint(const ofxGPoint &newHistBasePoint);
	void setHistType(int histType);
	void setHistVisible(bool visible);
	void setDrawHistLabels(bool drawHistLabels);
	void setLabelBgColor(const ofColor &newLabelBgColor);
	void setLabelSeparation(const array<float, 2> &newLabelSeparation);
	void setFontName(const string &newFontName);
	void setFontColor(const ofColor &newFontColor);
	void setFontSize(int newFontSize);
	void setFontProperties(const string &newFontName, const ofColor &newFontColor, int newFontSize);
	void setAllFontProperties(const string &newFontName, const ofColor &newFontColor, int newFontSize);

	// Getter methods
	string getId() const;
	array<float, 2> getDim() const;
	array<float, 2> getXLim() const;
	array<float, 2> getYLim() const;
	bool getXLog() const;
	bool getYLog() const;
	vector<ofxGPoint> getPoints() const;
	vector<ofxGPoint> &getPointsRef();
	vector<ofColor> getPointColors() const;
	vector<float> getPointSizes() const;
	ofColor getLineColor() const;
	float getLineWidth() const;
	ofxGHistogram &getHistogram();

protected:
	void updatePlotPoints();
	float xPlotToValue(float xPlot) const;
	float yPlotToValue(float yPlot) const;
	void updateInsideList();
	int obtainBoxIntersections(const ofxGPoint &plotPoint1, const ofxGPoint &plotPoint2);
	int getValidCuts(array<array<float, 2>, 4> &cuts, int nCuts, const ofxGPoint &plotPoint1,
			const ofxGPoint &plotPoint2);
	int removeDuplicatedCuts(array<array<float, 2>, 4> &cuts, int nCuts, float tolerance);
	int removePointFromCuts(array<array<float, 2>, 4> &cuts, int nCuts, const ofxGPoint &plotPoint, float tolerance);
	vector<ofxGPoint> &getHorizontalShape(float referenceValue);
	vector<ofxGPoint> &getVerticalShape(float referenceValue);

	// General properties
	string id;
	array<float, 2> dim;
	array<float, 2> xLim;
	array<float, 2> yLim;
	bool xLog;
	bool yLog;

	// Points properties
	vector<ofxGPoint> points;
	vector<ofxGPoint> plotPoints;
	vector<bool> inside;
	vector<ofColor> pointColors;
	vector<float> pointSizes;

	// Line properties
	ofColor lineColor;
	float lineWidth;

	// Histogram properties
	ofxGHistogram hist;
	ofxGPoint histBasePoint;

	// Labels properties
	ofColor labelBgColor;
	array<float, 2> labelSeparation;
	string fontName;
	ofColor fontColor;
	int fontSize;
	ofTrueTypeFont font;

	// Helper variable
	array<array<float, 2>, 4> cuts;
};
