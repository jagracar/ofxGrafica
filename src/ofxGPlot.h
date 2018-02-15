#pragma once

#include "ofxGConstants.h"
#include "ofxGLayer.h"
#include "ofxGAxis.h"
#include "ofxGTitle.h"
#include "ofxGHistogram.h"
#include "ofMain.h"

class ofxGPlot {
public:
	// Constants
	static const int NONE;

	// Constructors
	ofxGPlot(float xPos = 0, float yPos = 0, float plotWidth = 450, float plotHeight = 300);

	// Other methods
	void addLayer(ofxGLayer& newLayer);
	void addLayer(const string& id, const vector<ofxGPoint>& points);
	void removeLayer(const string& id);
	array<float, 2> getPlotPosAt(float xScreen, float yScreen) const;
	array<float, 2> getScreenPosAtValue(float xValue, float yValue) const;
	const ofxGPoint* getPointAt(float xScreen, float yScreen) const;
	const ofxGPoint* getPointAt(float xScreen, float yScreen, const string& layerId) const;
	void addPointAt(float xScreen, float yScreen);
	void addPointAt(float xScreen, float yScreen, const string& layerId);
	void removePointAt(float xScreen, float yScreen);
	void removePointAt(float xScreen, float yScreen, const string& layerId);
	array<float, 2> getValueAt(float xScreen, float yScreen) const;
	array<float, 2> getRelativePlotPosAt(float xScreen, float yScreen) const;
	bool isOverPlot(float xScreen, float yScreen) const;
	bool isOverBox(float xScreen, float yScreen) const;
	void updateLimits();
	array<float, 2> calculatePointsXLim(const vector<ofxGPoint>& points);
	array<float, 2> calculatePointsYLim(const vector<ofxGPoint>& points);
	void moveHorizontalAxesLim(float delta);
	void moveVerticalAxesLim(float delta);
	void centerAndZoom(float factor, float xValue, float yValue);
	void zoom(float factor);
	void zoom(float factor, float xScreen, float yScreen);
	void align(float xValue, float yValue, float xScreen, float yScreen);
	void align(const array<float, 2>& value, float xScreen, float yScreen);
	void center(float xScreen, float yScreen);
	void startHistograms(ofxGHistogramType histType);
	void stopHistograms();

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
	void drawPoints(ofPath& pointShape) const;
	void drawPoints(const ofImage& pointImg) const;
	void drawPoint(const ofxGPoint& point, const ofColor& pointColor, float pointSize) const;
	void drawPoint(const ofxGPoint& point) const;
	void drawPoint(const ofxGPoint& point, const ofPath& pointShape) const;
	void drawPoint(const ofxGPoint& point, ofPath& pointShape, const ofColor& pointColor) const;
	void drawPoint(const ofxGPoint& point, const ofImage& pointImg) const;
	void drawLines();
	void drawLine(const ofxGPoint& point1, const ofxGPoint& point2, const ofColor& lineColor, float lineWidth);
	void drawLine(const ofxGPoint& point1, const ofxGPoint& point2);
	void drawLine(float slope, float yCut, const ofColor& lineColor, float lineWidth);
	void drawLine(float slope, float yCut);
	void drawHorizontalLine(float value, const ofColor& lineColor, float lineWidth) const;
	void drawHorizontalLine(float value) const;
	void drawVerticalLine(float value, const ofColor& lineColor, float lineWidth) const;
	void drawVerticalLine(float value) const;
	void drawFilledContours(ofxGContourType contourType, float referenceValue);
	void drawLabel(const ofxGPoint& point) const;
	void drawLabelsAt(float xScreen, float yScreen) const;
	void drawLabels() const;
	void drawGridLines(ofxGDirection gridType) const;
	void drawHistograms();
	void drawPolygon(const vector<ofxGPoint>& polygonPoints, const ofColor& polygonColor);
	void drawAnnotation(const string& text, float x, float y, ofxGTextAlignment verAlign) const;
	void drawLegend(const vector<string>& text, const vector<float>& xRelativePos,
			const vector<float>& yRelativePos) const;

	// Setter methods
	void setPos(float x, float y);
	void setPos(const array<float, 2>& newPos);
	void setOuterDim(float xOuterDim, float yOuterDim);
	void setOuterDim(const array<float, 2>& newOuterDim);
	void setMar(float bottomMargin, float leftMargin, float topMargin, float rightMargin);
	void setMar(const array<float, 4>& newMar);
	void setDim(float xDim, float yDim);
	void setDim(const array<float, 2>& newDim);
	void setXLim(float lowerLim, float upperLim);
	void setXLim(const array<float, 2>& newXLim);
	void setYLim(float lowerLim, float upperLim);
	void setYLim(const array<float, 2>& newYLim);
	void setFixedXLim(bool newFixedXLim);
	void setFixedYLim(bool newFixedYLim);
	void setLogScale(const string& logType);
	void setInvertedXScale(bool newInvertedXScale);
	void invertXScale();
	void setInvertedYScale(bool newInvertedYScale);
	void invertYScale();
	void setIncludeAllLayersInLim(bool includeAllLayers);
	void setExpandLimFactor(float expandFactor);
	void setBgColor(const ofColor& newBgColor);
	void setBoxBgColor(const ofColor& newBoxBgColor);
	void setBoxLineColor(const ofColor& newBoxLineColor);
	void setBoxLineWidth(float newBoxLineWidth);
	void setGridLineColor(const ofColor& newGridLineColor);
	void setGridLineWidth(float newGridLineWidth);
	void setPoints(const vector<ofxGPoint>& points);
	void setPoints(const vector<ofxGPoint>& points, const string& layerId);
	void setPoint(int index, float x, float y, const string& label);
	void setPoint(int index, float x, float y, const string& label, const string& layerId);
	void setPoint(int index, float x, float y);
	void setPoint(int index, const ofxGPoint& newPoint);
	void setPoint(int index, const ofxGPoint& newPoint, const string& layerId);
	void addPoint(float x, float y, const string& label);
	void addPoint(float x, float y, const string& label, const string& layerId);
	void addPoint(float x, float y);
	void addPoint(const ofxGPoint& newPoint);
	void addPoint(const ofxGPoint& newPoint, const string& layerId);
	void addPoint(int index, float x, float y, const string& label);
	void addPoint(int index, float x, float y, const string& label, const string& layerId);
	void addPoint(int index, float x, float y);
	void addPoint(int index, const ofxGPoint& newPoint);
	void addPoint(int index, const ofxGPoint& newPoint, const string& layerId);
	void addPoints(const vector<ofxGPoint>& newPoints);
	void addPoints(const vector<ofxGPoint>& newPoints, const string& layerId);
	void removePoint(int index);
	void removePoint(int index, const string& layerId);
	void setPointColors(const vector<ofColor>& pointColors);
	void setPointColor(const ofColor& pointColor);
	void setPointSizes(const vector<float>& pointSizes);
	void setPointSize(float pointSize);
	void setLineColor(const ofColor& lineColor);
	void setLineWidth(float lineWidth);
	void setHistBasePoint(const ofxGPoint& basePoint);
	void setHistType(ofxGHistogramType histType);
	void setHistVisible(bool visible);
	void setDrawHistLabels(bool drawHistLabels);
	void setLabelBgColor(const ofColor& labelBgColor);
	void setLabelSeparation(const array<float, 2>& labelSeparation);
	void setTitleText(const string& text);
	void setAxesOffset(float offset);
	void setTicksLength(float tickLength);
	void setHorizontalAxesNTicks(int nTicks);
	void setHorizontalAxesTicksSeparation(float ticksSeparation);
	void setHorizontalAxesTicks(const vector<float>& ticks);
	void setVerticalAxesNTicks(int nTicks);
	void setVerticalAxesTicksSeparation(float ticksSeparation);
	void setVerticalAxesTicks(const vector<float>& ticks);
	void setFontName(const string& fontName);
	void setFontColor(const ofColor& fontColor);
	void setFontSize(int fontSize);
	void setFontProperties(const string& fontName, const ofColor& fontColor, int fontSize);
	void setAllFontProperties(const string& fontName, const ofColor& fontColor, int fontSize);

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
	ofxGLayer& getMainLayer();
	ofxGLayer& getLayer(const string& layerId);
	ofxGAxis& getXAxis();
	ofxGAxis& getTopAxis();
	ofxGAxis& getYAxis();
	ofxGAxis& getRightAxis();
	ofxGTitle& getTitle();
	vector<ofxGPoint> getPoints() const;
	vector<ofxGPoint> getPoints(const string& layerId) const;
	const vector<ofxGPoint>& getPointsRef() const;
	const vector<ofxGPoint>& getPointsRef(const string& layerId) const;
	ofxGHistogram& getHistogram();
	ofxGHistogram& getHistogram(const string& layerId);

	// Event methods
	void activateZooming(float factor, int increaseButton, int decreaseButton, int increaseKeyModifier,
			int decreaseKeyModifier);
	void activateZooming(float factor, int increaseButton, int decreaseButton);
	void activateZooming(float factor);
	void activateZooming();
	void deactivateZooming();
	void activateCentering(int button, int keyModifier);
	void activateCentering(int button);
	void activateCentering();
	void deactivateCentering();
	void activatePanning(int button, int keyModifier);
	void activatePanning(int button);
	void activatePanning();
	void deactivatePanning();
	void activatePointLabels(int button, int keyModifier);
	void activatePointLabels(int button);
	void activatePointLabels();
	void deactivatePointLabels();
	void activateReset(int button, int keyModifier);
	void activateReset(int button);
	void activateReset();
	void deactivateReset();
	//void mouseEvent(MouseEvent event);

protected:
	array<float, 2> calculatePlotXLim();
	array<float, 2> calculatePlotYLim();
	void shiftPlotPos(const array<float, 2>& valuePlotPos, const array<float, 2>& newPlotPos);

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
	vector<ofxGLayer*> layerList;

	// Axes and title
	ofxGAxis xAxis;
	ofxGAxis topAxis;
	ofxGAxis yAxis;
	ofxGAxis rightAxis;
	ofxGTitle title;

	// Mouse events
	bool zoomingIsActive;
	float zoomFactor;
	int increaseZoomButton;
	int decreaseZoomButton;
	int increaseZoomKeyModifier;
	int decreaseZoomKeyModifier;
	bool centeringIsActive;
	int centeringButton;
	int centeringKeyModifier;
	bool panningIsActive;
	int panningButton;
	int panningKeyModifier;
	array<float, 2> panningReferencePoint;
	bool labelingIsActive;
	int labelingButton;
	int labelingKeyModifier;
	array<float, 2>* mousePos;
	bool resetIsActive;
	int resetButton;
	int resetKeyModifier;
	array<float, 2> xLimReset;
	array<float, 2> yLimReset;
};
