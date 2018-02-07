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

	mainLayer = ofxGLayer();
	//mainLayer = ofxGLayer(MAINLAYERID, dim, xLim, yLim, xLog, yLog);

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
