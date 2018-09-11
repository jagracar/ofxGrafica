#include "ofxGAxis.h"
#include "ofxGConstants.h"
#include "ofxGAxisLabel.h"
#include "ofMain.h"

ofxGAxis::ofxGAxis(ofxGAxisType _type, const array<float, 2>& _dim, const array<float, 2>& _lim, bool _logScale) :
		type(_type), dim(_dim), lim(_lim), logScale(_logScale) {
	// Do some sanity checks
	if (logScale && (lim[0] <= 0 || lim[1] <= 0)) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	// Format properties
	offset = 5;
	lineColor = ofColor(0);
	lineWidth = 1;

	// Ticks properties
	nTicks = 5;
	ticksSeparation = -1;
	fixedTicks = false;
	tickLength = 3;
	smallTickLength = 2;
	expTickLabels = false;
	rotateTickLabels = (type == GRAFICA_X_AXIS || type == GRAFICA_TOP_AXIS) ? false : true;
	drawTickLabels = (type == GRAFICA_X_AXIS || type == GRAFICA_Y_AXIS) ? true : false;
	tickLabelOffset = 8;

	// Label properties
	lab = ofxGAxisLabel(type, dim);
	drawAxisLabel = true;

	// Font properties
	fontName = OF_TTF_SANS;
	fontColor = ofColor(0);
	fontSize = 8;
	fontMakeContours = false;
	font.load(fontName, fontSize, true, true, fontMakeContours);

	// Update the tick containers
	updateTicks();
	updatePlotTicks();
	updateTicksInside();
	updateTickLabels();
}

int ofxGAxis::obtainSigDigits(float number) {
	return round(-log10(0.5 * abs(number)));
}

float ofxGAxis::roundPlus(float number, int sigDigits) {
	return round(number * pow(10, sigDigits)) / pow(10, sigDigits);
}

void ofxGAxis::updateTicks() {
	if (logScale) {
		obtainLogarithmicTicks();
	} else {
		obtainLinearTicks();
	}
}

void ofxGAxis::obtainLogarithmicTicks() {
	// Get the exponents of the first and last ticks in increasing order
	int firstExp = floor(log10(min(lim[0], lim[1])));
	int lastExp = ceil(log10(max(lim[0], lim[1])));

	// Fill the ticks container
	ticks.clear();

	for (int exp = firstExp; exp < lastExp; ++exp) {
		float base = pow(10, exp);

		for (int i = 1; i < 10; ++i) {
			ticks.push_back(i * base);
		}
	}

	ticks.push_back(pow(10, lastExp));
}

void ofxGAxis::obtainLinearTicks() {
	// Obtain the required tick precision
	float step = 0;
	int nSteps = 0;
	float sigDigits = 0;

	if (ticksSeparation > 0) {
		step = (lim[1] > lim[0]) ? ticksSeparation : -ticksSeparation;
		sigDigits = obtainSigDigits(step);

		while (abs(roundPlus(step, sigDigits) - step) > abs(0.001 * step)) {
			++sigDigits;
		}

		nSteps = floor((lim[1] - lim[0]) / step);
	} else if (nTicks > 0) {
		step = (lim[1] - lim[0]) / nTicks;
		sigDigits = obtainSigDigits(step);
		step = roundPlus(step, sigDigits);

		if (step == 0 || abs(step) > abs(lim[1] - lim[0])) {
			++sigDigits;
			step = roundPlus((lim[1] - lim[0]) / nTicks, sigDigits);
		}

		nSteps = floor((lim[1] - lim[0]) / step);
	}

	// Calculate the linear ticks
	ticks.clear();

	if (nSteps > 0) {
		// Obtain the first tick
		float firstTick = lim[0] + ((lim[1] - lim[0]) - nSteps * step) / 2;

		// Subtract some steps to be sure we have all
		firstTick = roundPlus(firstTick - 2 * step, sigDigits);

		while ((lim[1] - firstTick) * (lim[0] - firstTick) > 0) {
			firstTick = roundPlus(firstTick + step, sigDigits);
		}

		// Calculate the rest of the ticks
		int n = floor((lim[1] - firstTick) / step) + 1;

		for (int i = 0; i < n; ++i) {
			ticks.push_back(roundPlus(firstTick + i * step, sigDigits));
		}
	}
}

void ofxGAxis::updatePlotTicks() {
	float scaleFactor;
	plotTicks.clear();

	if (logScale) {
		if (type == GRAFICA_X_AXIS || type == GRAFICA_TOP_AXIS) {
			scaleFactor = dim[0] / log10(lim[1] / lim[0]);
		} else {
			scaleFactor = -dim[1] / log10(lim[1] / lim[0]);
		}

		for (float tick : ticks) {
			plotTicks.push_back(log10(tick / lim[0]) * scaleFactor);
		}
	} else {
		if (type == GRAFICA_X_AXIS || type == GRAFICA_TOP_AXIS) {
			scaleFactor = dim[0] / (lim[1] - lim[0]);
		} else {
			scaleFactor = -dim[1] / (lim[1] - lim[0]);
		}

		for (float tick : ticks) {
			plotTicks.push_back((tick - lim[0]) * scaleFactor);
		}
	}
}

void ofxGAxis::updateTicksInside() {
	ticksInside.clear();

	if (type == GRAFICA_X_AXIS || type == GRAFICA_TOP_AXIS) {
		for (float plotTick : plotTicks) {
			ticksInside.push_back((plotTick >= 0) && (plotTick <= dim[0]));
		}
	} else {
		for (float plotTick : plotTicks) {
			ticksInside.push_back((-plotTick >= 0) && (-plotTick <= dim[1]));
		}
	}
}

void ofxGAxis::updateTickLabels() {
	stringstream ss;
	tickLabels.clear();

	if (logScale) {
		for (float tick : ticks) {
			float logValue = log10(tick);
			bool isExactLogValue = abs(logValue - round(logValue)) < 0.0001;

			if (isExactLogValue) {
				logValue = round(logValue);

				if (expTickLabels) {
					tickLabels.push_back("1e" + to_string((int) logValue));
				} else {
					if (logValue > -3.1 && logValue < 3.1) {
						ss.str("");
						ss << std::defaultfloat << tick;
						tickLabels.push_back(ss.str());
					} else {
						tickLabels.push_back("1e" + to_string((int) logValue));
					}
				}
			} else {
				tickLabels.push_back("");
			}
		}
	} else {
		for (float tick : ticks) {
			ss.str("");
			ss << std::defaultfloat << tick;
			tickLabels.push_back(ss.str());
		}
	}
}

void ofxGAxis::moveLim(const array<float, 2>& newLim) {
	// Check that the new limit makes sense
	if (newLim[1] == newLim[0]) {
		throw invalid_argument("The limit range cannot be zero.");
	} else if (logScale && (newLim[0] <= 0 || newLim[1] <= 0)) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	// Update the limits
	lim = newLim;

	// Calculate the new ticks if they are not fixed
	if (!fixedTicks) {
		int n = ticks.size();

		if (logScale) {
			obtainLogarithmicTicks();
		} else if (n > 0) {
			// Obtain the ticks precision and the tick separation
			float step = 0;
			int sigDigits = 0;

			if (ticksSeparation > 0) {
				step = (lim[1] > lim[0]) ? ticksSeparation : -ticksSeparation;
				sigDigits = obtainSigDigits(step);

				while (abs(roundPlus(step, sigDigits) - step) > abs(0.001 * step)) {
					++sigDigits;
				}
			} else {
				step = (n == 1) ? lim[1] - lim[0] : ticks[1] - ticks[0];
				sigDigits = obtainSigDigits(step);
				step = roundPlus(step, sigDigits);

				if (step == 0 || abs(step) > abs(lim[1] - lim[0])) {
					++sigDigits;
					step = (n == 1) ? lim[1] - lim[0] : ticks[1] - ticks[0];
					step = roundPlus(step, sigDigits);
				}

				step = (lim[1] > lim[0]) ? abs(step) : -abs(step);
			}

			// Obtain the first tick
			float firstTick = ticks[0] + step * ceil((lim[0] - ticks[0]) / step);
			firstTick = roundPlus(firstTick, sigDigits);

			if ((lim[1] - firstTick) * (lim[0] - firstTick) > 0) {
				firstTick = ticks[0] + step * floor((lim[0] - ticks[0]) / step);
				firstTick = roundPlus(firstTick, sigDigits);
			}

			// Calculate the rest of the ticks
			n = floor(abs((lim[1] - firstTick) / step)) + 1;
			ticks.clear();

			for (int i = 0; i < n; ++i) {
				ticks.push_back(roundPlus(firstTick + i * step, sigDigits));
			}
		}

		// Obtain the new tick labels
		updateTickLabels();
	}

	// Update the rest of the arrays
	updatePlotTicks();
	updateTicksInside();
}

void ofxGAxis::draw() const {
	switch (type) {
	case GRAFICA_X_AXIS:
		drawAsXAxis();
		break;
	case GRAFICA_Y_AXIS:
		drawAsYAxis();
		break;
	case GRAFICA_TOP_AXIS:
		drawAsTopAxis();
		break;
	case GRAFICA_RIGHT_AXIS:
		drawAsRightAxis();
		break;
	}

	if (drawAxisLabel) {
		lab.draw();
	}
}

void ofxGAxis::drawAsXAxis() const {
	ofPushStyle();
	ofSetColor(lineColor);
	ofSetLineWidth(lineWidth);

	// Draw the ticks
	ofDrawLine(0, offset, dim[0], offset);

	for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
		if (ticksInside[i]) {
			if (logScale && tickLabels[i] == "") {
				ofDrawLine(plotTicks[i], offset, plotTicks[i], offset + smallTickLength);
			} else {
				ofDrawLine(plotTicks[i], offset, plotTicks[i], offset + tickLength);
			}
		}
	}

	// Draw the tick labels
	if (drawTickLabels) {
		ofSetColor(fontColor);

		if (rotateTickLabels) {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);

					ofPushMatrix();
					ofTranslate(plotTicks[i] + fontSize / 2.0, offset + tickLabelOffset + bounds.width);
					ofRotateZDeg(-90);
					font.drawString(tickLabels[i], 0, 0);
					ofPopMatrix();
				}
			}
		} else {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);
					font.drawString(tickLabels[i], plotTicks[i] - bounds.width / 2,
							offset + tickLabelOffset + fontSize);
				}
			}
		}
	}

	ofPopStyle();
}

void ofxGAxis::drawAsYAxis() const {
	ofPushStyle();
	ofSetColor(lineColor);
	ofSetLineWidth(lineWidth);

	// Draw the ticks
	ofDrawLine(-offset, 0, -offset, -dim[1]);

	for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
		if (ticksInside[i]) {
			if (logScale && tickLabels[i] == "") {
				ofDrawLine(-offset, plotTicks[i], -offset - smallTickLength, plotTicks[i]);
			} else {
				ofDrawLine(-offset, plotTicks[i], -offset - tickLength, plotTicks[i]);
			}
		}
	}

	// Draw the tick labels
	if (drawTickLabels) {
		ofSetColor(fontColor);

		if (rotateTickLabels) {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);

					ofPushMatrix();
					ofTranslate(-offset - tickLabelOffset, plotTicks[i] + bounds.width / 2);
					ofRotateZDeg(-90);
					font.drawString(tickLabels[i], 0, 0);
					ofPopMatrix();
				}
			}
		} else {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);
					font.drawString(tickLabels[i], -offset - tickLabelOffset - bounds.width,
							plotTicks[i] + fontSize / 2.0);
				}
			}
		}
	}

	ofPopStyle();
}

void ofxGAxis::drawAsTopAxis() const {
	ofPushStyle();
	ofSetColor(lineColor);
	ofSetLineWidth(lineWidth);

	ofPushMatrix();
	ofTranslate(0, -dim[1]);

	// Draw the ticks
	ofDrawLine(0, -offset, dim[0], -offset);

	for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
		if (ticksInside[i]) {
			if (logScale && tickLabels[i] == "") {
				ofDrawLine(plotTicks[i], -offset, plotTicks[i], -offset - smallTickLength);
			} else {
				ofDrawLine(plotTicks[i], -offset, plotTicks[i], -offset - tickLength);
			}
		}
	}

	// Draw the tick labels
	if (drawTickLabels) {
		ofSetColor(fontColor);

		if (rotateTickLabels) {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofPushMatrix();
					ofTranslate(plotTicks[i] + fontSize / 2.0, -offset - tickLabelOffset);
					ofRotateZDeg(-90);
					font.drawString(tickLabels[i], 0, 0);
					ofPopMatrix();
				}
			}
		} else {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);
					font.drawString(tickLabels[i], plotTicks[i] - bounds.width / 2, -offset - tickLabelOffset);
				}
			}
		}
	}

	ofPopMatrix();
	ofPopStyle();
}

void ofxGAxis::drawAsRightAxis() const {
	ofPushStyle();
	ofSetColor(lineColor);
	ofSetLineWidth(lineWidth);

	ofPushMatrix();
	ofTranslate(dim[0], 0);

	// Draw the ticks
	ofDrawLine(offset, 0, offset, -dim[1]);

	for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
		if (ticksInside[i]) {
			if (logScale && tickLabels[i] == "") {
				ofDrawLine(offset, plotTicks[i], offset + smallTickLength, plotTicks[i]);
			} else {
				ofDrawLine(offset, plotTicks[i], offset + tickLength, plotTicks[i]);
			}
		}
	}

	// Draw the tick labels
	if (drawTickLabels) {
		ofSetColor(fontColor);

		if (rotateTickLabels) {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);

					ofPushMatrix();
					ofTranslate(offset + tickLabelOffset + fontSize, plotTicks[i] + bounds.width / 2);
					ofRotateZDeg(-90);
					font.drawString(tickLabels[i], 0, 0);
					ofPopMatrix();
				}
			}
		} else {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);
					font.drawString(tickLabels[i], offset + tickLabelOffset, plotTicks[i] + fontSize / 2.0);
				}
			}
		}
	}

	ofPopMatrix();
	ofPopStyle();
}

void ofxGAxis::setDim(float xDim, float yDim) {
	if (xDim <= 0 || yDim <= 0) {
		throw invalid_argument("The dimensions should be larger than zero.");
	}

	dim = {xDim, yDim};
	updatePlotTicks();
	lab.setDim(dim);
}

void ofxGAxis::setDim(const array<float, 2>& newDim) {
	setDim(newDim[0], newDim[1]);
}

void ofxGAxis::setLim(const array<float, 2>& newLim) {
	// Check that the new limit makes sense
	if (newLim[1] == newLim[0]) {
		throw invalid_argument("The limit range cannot be zero.");
	} else if (logScale && (newLim[0] <= 0 || newLim[1] <= 0)) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	lim = newLim;

	if (!fixedTicks) {
		updateTicks();
		updateTickLabels();
	}

	updatePlotTicks();
	updateTicksInside();
}

void ofxGAxis::setLimAndLogScale(const array<float, 2>& newLim, bool newLogScale) {
	// Check that the new limit makes sense
	if (newLim[1] == newLim[0]) {
		throw invalid_argument("The limit range cannot be zero.");
	} else if (newLogScale && (newLim[0] <= 0 || newLim[1] <= 0)) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	lim = newLim;
	logScale = newLogScale;

	if (!fixedTicks) {
		updateTicks();
		updateTickLabels();
	}

	updatePlotTicks();
	updateTicksInside();
}

void ofxGAxis::setLogScale(bool newLogScale) {
	if (newLogScale != logScale) {
		// Check if the old limits still make sense
		if (newLogScale && (lim[0] <= 0 || lim[1] <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

		logScale = newLogScale;

		if (!fixedTicks) {
			updateTicks();
			updateTickLabels();
		}

		updatePlotTicks();
		updateTicksInside();
	}
}

void ofxGAxis::setOffset(float newOffset) {
	offset = newOffset;
}

void ofxGAxis::setLineColor(const ofColor& newLineColor) {
	lineColor = newLineColor;
}

void ofxGAxis::setLineWidth(float newLineWidth) {
	if (newLineWidth <= 0) {
		throw invalid_argument("The line width should be larger than zero.");
	}

	lineWidth = newLineWidth;
}

void ofxGAxis::setNTicks(int newNTicks) {
	if (newNTicks < 0) {
		throw invalid_argument("The number of ticks should be equal or larger than zero.");
	}

	nTicks = newNTicks;
	ticksSeparation = -1;
	fixedTicks = false;

	if (!logScale) {
		updateTicks();
		updatePlotTicks();
		updateTicksInside();
		updateTickLabels();
	}
}

void ofxGAxis::setTicksSeparation(float newTicksSeparation) {
	ticksSeparation = newTicksSeparation;
	fixedTicks = false;

	if (!logScale) {
		updateTicks();
		updatePlotTicks();
		updateTicksInside();
		updateTickLabels();
	}
}

void ofxGAxis::setTicks(const vector<float>& newTicks) {
	ticks = newTicks;
	fixedTicks = true;
	updatePlotTicks();
	updateTicksInside();
	updateTickLabels();
}

void ofxGAxis::setTickLabels(const vector<string>& newTickLabels) {
	if (newTickLabels.size() != tickLabels.size()) {
		throw invalid_argument("The number of ticks and tick labels should be the same.");
	}

	fixedTicks = true;
	tickLabels = newTickLabels;
}

void ofxGAxis::setFixedTicks(bool newFixedTicks) {
	if (newFixedTicks != fixedTicks) {
		fixedTicks = newFixedTicks;

		if (!fixedTicks) {
			updateTicks();
			updatePlotTicks();
			updateTicksInside();
			updateTickLabels();
		}
	}
}

void ofxGAxis::setTickLength(float newTickLength) {
	tickLength = newTickLength;
}

void ofxGAxis::setSmallTickLength(float newSmallTickLength) {
	smallTickLength = newSmallTickLength;
}

void ofxGAxis::setExpTickLabels(bool newExpTickLabels) {
	if (newExpTickLabels != expTickLabels) {
		expTickLabels = newExpTickLabels;
		updateTickLabels();
	}
}

void ofxGAxis::setRotateTickLabels(bool newRotateTickLabels) {
	rotateTickLabels = newRotateTickLabels;
}

void ofxGAxis::setDrawTickLabels(bool newDrawTicksLabels) {
	drawTickLabels = newDrawTicksLabels;
}

void ofxGAxis::setTickLabelOffset(float newTickLabelOffset) {
	tickLabelOffset = newTickLabelOffset;
}

void ofxGAxis::setDrawAxisLabel(bool newDrawAxisLabel) {
	drawAxisLabel = newDrawAxisLabel;
}

void ofxGAxis::setAxisLabelText(const string& text) {
	lab.setText(text);
}

void ofxGAxis::setFontName(const string& newFontName) {
	fontName = newFontName;
	font.load(fontName, fontSize, true, true, fontMakeContours);
}

void ofxGAxis::setFontColor(const ofColor& newFontColor) {
	fontColor = newFontColor;
}

void ofxGAxis::setFontSize(int newFontSize) {
	if (newFontSize <= 0) {
		throw invalid_argument("The font size should be larger than zero.");
	}

	fontSize = newFontSize;
	font.load(fontName, fontSize, true, true, fontMakeContours);
}

void ofxGAxis::setFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize) {
	if (newFontSize <= 0) {
		throw invalid_argument("The font size should be larger than zero.");
	}

	fontName = newFontName;
	fontColor = newFontColor;
	fontSize = newFontSize;
	font.load(fontName, fontSize, true, true, fontMakeContours);
}

void ofxGAxis::setAllFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize) {
	setFontProperties(newFontName, newFontColor, newFontSize);
	lab.setFontProperties(newFontName, newFontColor, newFontSize);
}

void ofxGAxis::setFontsMakeContours(bool newFontMakeContours) {
	fontMakeContours = newFontMakeContours;
	font.load(fontName, fontSize, true, true, fontMakeContours);
	lab.setFontMakeContours(fontMakeContours);
}

vector<float> ofxGAxis::getTicks() const {
	if (fixedTicks) {
		return ticks;
	} else {
		// Return only the ticks that are inside the plot
		vector<float> validTicks;

		for (vector<float>::size_type i = 0; i < ticks.size(); ++i) {
			if (ticksInside[i]) {
				validTicks.push_back(ticks[i]);
			}
		}

		return validTicks;
	}
}

const vector<float>& ofxGAxis::getTicksRef() const {
	return ticks;
}

vector<float> ofxGAxis::getPlotTicks() const {
	if (fixedTicks) {
		return plotTicks;
	} else {
		// Return only the plot ticks that are inside the plot
		vector<float> validPlotTicks;

		for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
			if (ticksInside[i]) {
				validPlotTicks.push_back(plotTicks[i]);
			}
		}

		return validPlotTicks;
	}
}

const vector<float>& ofxGAxis::getPlotTicksRef() const {
	return plotTicks;
}

ofxGAxisLabel& ofxGAxis::getAxisLabel() {
	return lab;
}
