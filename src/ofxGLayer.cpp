#include "ofxGLayer.h"

ofxGLayer::ofxGLayer(const string &_id, const array<float, 2> &_dim,
		const array<float, 2> &_xLim, const array<float, 2> &_yLim, bool _xLog,
		bool _yLog) :
		id(_id), dim(_dim), xLim(_xLim), yLim(_yLim), xLog(_xLog), yLog(_yLog) {

	// Do some sanity checks
	if (xLog && (xLim[0] <= 0 || xLim[1] <= 0)) {
		ofLogWarning(
				"One of the limits is negative. This is not allowed in logarithmic scale.");
		ofLogWarning("Will set horizontal limits to (0.1, 10)");
		xLim = {0.1f, 10};
	}

	if (yLog && (yLim[0] <= 0 || yLim[1] <= 0)) {
		ofLogWarning(
				"One of the limits is negative. This is not allowed in logarithmic scale.");
		ofLogWarning("Will set vertical limits to (0.1, 10)");
		yLim = {0.1f, 10};
	}

	// Continue with the rest
	pointColors.push_back(ofColor(255, 0, 0, 150));
	pointSizes.push_back(7);

	lineColor = ofColor(0, 150);
	lineWidth = 1;

	//hist = null;
	histBasePoint = ofxGPoint(0, 0);

	labelBgColor = ofColor(255, 200);
	labelSeparation = {7, 7};
	fontName = "verdana.ttf";
	fontColor = ofColor(0);
	fontSize = 14;
	font.load(fontName, fontSize, true, true);
}

bool ofxGLayer::isId(const string &someId) {
	return id == someId;
}

float ofxGLayer::valueToXPlot(float x) {
	if (xLog) {
		return dim[0] * log10(x / xLim[0]) / log10(xLim[1] / xLim[0]);
	} else {
		return dim[0] * (x - xLim[0]) / (xLim[1] - xLim[0]);
	}
}

float ofxGLayer::valueToYPlot(float y) {
	if (yLog) {
		return -dim[1] * log(y / yLim[0]) / log(yLim[1] / yLim[0]);
	} else {
		return -dim[1] * (y - yLim[0]) / (yLim[1] - yLim[0]);
	}
}

array<float, 2> ofxGLayer::valueToPlot(float x, float y) {
	return {valueToXPlot(x), valueToYPlot(y)};
}

ofxGPoint ofxGLayer::valueToPlot(ofxGPoint point) {
	return ofxGPoint(valueToXPlot(point.getX()), valueToYPlot(point.getY()),
			point.getLabel());
}

float ofxGLayer::xPlotToValue(float xPlot) {
	if (xLog) {
		return exp(log(xLim[0]) + log(xLim[1] / xLim[0]) * xPlot / dim[0]);
	} else {
		return xLim[0] + (xLim[1] - xLim[0]) * xPlot / dim[0];
	}
}

float ofxGLayer::yPlotToValue(float yPlot) {
	if (yLog) {
		return exp(log(yLim[0]) - log(yLim[1] / yLim[0]) * yPlot / dim[1]);
	} else {
		return yLim[0] - (yLim[1] - yLim[0]) * yPlot / dim[1];
	}
}

array<float, 2> ofxGLayer::plotToValue(float xPlot, float yPlot) {
	return {xPlotToValue(xPlot), yPlotToValue(yPlot)};
}

bool ofxGLayer::isInside(float xPlot, float yPlot) {
	return (xPlot >= 0) && (xPlot <= dim[0]) && (-yPlot >= 0)
			&& (-yPlot <= dim[1]);
}

bool ofxGLayer::isInside(ofxGPoint plotPoint) {
	return plotPoint.isValid() ?
			isInside(plotPoint.getX(), plotPoint.getY()) : false;
}

ofxGPoint ofxGLayer::getPointAtPlotPos(float xPlot, float yPlot) {
	//int pointIndex = getPointIndexAtPlotPos(xPlot, yPlot);

	//return (pointIndex >= 0) ? points[pointIndex] : nullptr;

	return ofxGPoint();
}
