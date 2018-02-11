#include "ofxGAxis.h"
#include "ofxGConstants.h"
#include "ofxGAxisLabel.h"
#include "ofMain.h"

ofxGAxis::ofxGAxis(ofxGAxisType _type, const array<float, 2>& _dim, const array<float, 2>& _lim, bool _log) :
		type(_type), dim(_dim), lim(_lim), log(_log) {
	// Do some sanity checks
	if (log && (lim[0] <= 0 || lim[1] <= 0)) {
		throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
	}

	// Format properties
	offset = 5;
	lineColor = ofColor(0);
	lineWidth = 1;

	// Ticks properties
	nTicks = 5;
	ticksSeparation = -1;
	ticksPrecission = 0;
	fixedTicks = false;
	tickLength = 3;
	smallTickLength = 2;
	expTickLabels = false;
	rotateTickLabels = (type == GRAFICA_X_AXIS || type == GRAFICA_TOP_AXIS) ? false : true;
	drawTickLabels = (type == GRAFICA_X_AXIS || type == GRAFICA_Y_AXIS) ? true : false;
	tickLabelOffset = 7;

	// Label properties
	lab = ofxGAxisLabel(type, dim);
	drawAxisLabel = true;

	// Font properties
	fontName = "SansSerif.ttf";
	fontColor = ofColor(0);
	fontSize = 9;
	font.load(fontName, fontSize);

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
	if (log) {
		obtainLogarithmicTicks();
	} else {
		obtainLinearTicks();
	}
}

void ofxGAxis::obtainLogarithmicTicks() {
	// Get the exponents of the first and last ticks in increasing order
	int firstExp = (lim[1] > lim[0]) ? floor(log10(lim[0])) : floor(log10(lim[1]));
	int lastExp = (lim[1] > lim[0]) ? ceil(log10(lim[1])) : ceil(log10(lim[0]));

	// Fill the ticks container
	ticks.clear();

	for (int exp = firstExp; exp < lastExp; ++exp) {
		float base = pow(10, exp);

		for (int i = 1; i <= 9; ++i) {
			ticks.push_back(i * base);
		}
	}

	ticks.push_back(pow(10, lastExp));
}

void ofxGAxis::obtainLinearTicks() {
	// Obtain the required precision for the ticks
	float step = 0;
	int nSteps = 0;

	if (ticksSeparation > 0) {
		step = (lim[1] > lim[0]) ? ticksSeparation : -ticksSeparation;
		ticksPrecission = obtainSigDigits(step);

		while (abs(roundPlus(step, ticksPrecission) - step) > abs(0.001 * step)) {
			++ticksPrecission;
		}

		nSteps = floor((lim[1] - lim[0]) / step);
	} else if (nTicks > 0) {
		step = (lim[1] - lim[0]) / nTicks;
		ticksPrecission = obtainSigDigits(step);
		step = roundPlus(step, ticksPrecission);

		if (step == 0 || abs(step) > abs(lim[1] - lim[0])) {
			ticksPrecission++;
			step = roundPlus((lim[1] - lim[0]) / nTicks, ticksPrecission);
		}

		nSteps = floor((lim[1] - lim[0]) / step);
	}

	// Calculate the linear ticks
	ticks.clear();

	if (nSteps > 0) {
		// Obtain the first tick
		float firstTick = lim[0] + ((lim[1] - lim[0]) - nSteps * step) / 2;

		// Subtract some steps to be sure we have all
		firstTick = roundPlus(firstTick - 2 * step, ticksPrecission);

		while ((lim[1] - firstTick) * (lim[0] - firstTick) > 0) {
			firstTick = roundPlus(firstTick + step, ticksPrecission);
		}

		// Calculate the rest of the ticks
		int n = floor(abs((lim[1] - firstTick) / step)) + 1;

		ticks.push_back(firstTick);

		for (int i = 1; i < n; ++i) {
			ticks.push_back(roundPlus(ticks.back() + step, ticksPrecission));
		}
	}
}

void ofxGAxis::updatePlotTicks() {
	float scaleFactor;
	plotTicks.clear();

	if (log) {
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

	if (log) {
		for (float tick : ticks) {
			if (tick > 0) {
				float logValue = log10(tick);
				bool isExactLogValue = abs(logValue - round(logValue)) < 0.0001;

				if (isExactLogValue) {
					logValue = round(logValue);

					if (expTickLabels) {
						tickLabels.push_back("1e" + to_string((int) logValue));
					} else {
						if (logValue > -3.1 && logValue < 3.1) {
							tickLabels.push_back((logValue >= 0) ? to_string((int) tick) : to_string(tick));
						} else {
							tickLabels.push_back("1e" + to_string((int) logValue));
						}
					}
				} else {
					tickLabels.push_back("");
				}
			} else {
				tickLabels.push_back("");
			}
		}
	} else {
		for (float tick : ticks) {
			ss.str("");
			ss << std::fixed << std::setprecision(ticksPrecission) << tick;
			tickLabels.push_back((fmod(tick, 1) == 0 && abs(tick) < 1e9) ? to_string((int) tick) : ss.str());
		}
	}
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
	ofFill();
	ofSetColor(lineColor);
	ofSetLineWidth(lineWidth);

	// Draw the ticks
	ofDrawLine(0, offset, dim[0], offset);

	for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
		if (ticksInside[i]) {
			if (log && tickLabels[i] == "") {
				ofDrawLine(plotTicks[i], offset, plotTicks[i], offset + smallTickLength);
			} else {
				ofDrawLine(plotTicks[i], offset, plotTicks[i], offset + tickLength);
			}
		}
	}

	// Draw the tick labels
	ofSetColor(fontColor);

	if (drawTickLabels) {
		if (rotateTickLabels) {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);

					ofPushMatrix();
					ofTranslate(plotTicks[i] + bounds.height / 2, offset + tickLabelOffset + bounds.width);
					ofRotateZ(-90);
					font.drawString(tickLabels[i], 0, 0);
					ofPopMatrix();
				}
			}
		} else {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);
					font.drawString(tickLabels[i], plotTicks[i] - bounds.width / 2,
							offset + tickLabelOffset + bounds.height);
				}
			}
		}
	}

	ofPopStyle();
}

void ofxGAxis::drawAsYAxis() const {
	ofPushStyle();
	ofFill();
	ofSetColor(lineColor);
	ofSetLineWidth(lineWidth);

	// Draw the ticks
	ofDrawLine(-offset, 0, -offset, -dim[1]);

	for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
		if (ticksInside[i]) {
			if (log && tickLabels[i] == "") {
				ofDrawLine(-offset, plotTicks[i], -offset - smallTickLength, plotTicks[i]);
			} else {
				ofDrawLine(-offset, plotTicks[i], -offset - tickLength, plotTicks[i]);
			}
		}
	}

	// Draw the tick labels
	ofSetColor(fontColor);

	if (drawTickLabels) {
		if (rotateTickLabels) {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);

					ofPushMatrix();
					ofTranslate(-offset - tickLabelOffset, plotTicks[i] + bounds.width / 2);
					ofRotateZ(-90);
					font.drawString(tickLabels[i], 0, 0);
					ofPopMatrix();
				}
			}
		} else {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);
					font.drawString(tickLabels[i], -offset - tickLabelOffset - bounds.width,
							plotTicks[i] + bounds.height / 2);
				}
			}
		}
	}

	ofPopStyle();
}

void ofxGAxis::drawAsTopAxis() const {
	ofPushStyle();
	ofFill();
	ofSetColor(lineColor);
	ofSetLineWidth(lineWidth);

	ofPushMatrix();
	ofTranslate(0, -dim[1]);

	// Draw the ticks
	ofDrawLine(0, -offset, dim[0], -offset);

	for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
		if (ticksInside[i]) {
			if (log && tickLabels[i] == "") {
				ofDrawLine(plotTicks[i], -offset, plotTicks[i], -offset - smallTickLength);
			} else {
				ofDrawLine(plotTicks[i], -offset, plotTicks[i], -offset - tickLength);
			}
		}
	}

	// Draw the tick labels
	ofSetColor(fontColor);

	if (drawTickLabels) {
		if (rotateTickLabels) {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);

					ofPushMatrix();
					ofTranslate(plotTicks[i] + bounds.height / 2, -offset - tickLabelOffset);
					ofRotateZ(-90);
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
	ofFill();
	ofSetColor(lineColor);
	ofSetLineWidth(lineWidth);

	ofPushMatrix();
	ofTranslate(dim[0], 0);

	// Draw the ticks
	ofDrawLine(offset, 0, offset, -dim[1]);

	for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
		if (ticksInside[i]) {
			if (log && tickLabels[i] == "") {
				ofDrawLine(offset, plotTicks[i], offset + smallTickLength, plotTicks[i]);
			} else {
				ofDrawLine(offset, plotTicks[i], offset + tickLength, plotTicks[i]);
			}
		}
	}

	// Draw the tick labels
	ofSetColor(fontColor);

	if (drawTickLabels) {
		if (rotateTickLabels) {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);

					ofPushMatrix();
					ofTranslate(offset + tickLabelOffset + bounds.height, plotTicks[i] + bounds.width / 2);
					ofRotateZ(-90);
					font.drawString(tickLabels[i], 0, 0);
					ofPopMatrix();
				}
			}
		} else {
			for (vector<float>::size_type i = 0; i < plotTicks.size(); ++i) {
				if (ticksInside[i] && tickLabels[i] != "") {
					ofRectangle bounds = font.getStringBoundingBox(tickLabels[i], 0, 0);
					font.drawString(tickLabels[i], offset + tickLabelOffset, plotTicks[i] + bounds.height / 2);
				}
			}
		}
	}

	ofPopMatrix();
	ofPopStyle();
}

void ofxGAxis::setDim(float xDim, float yDim) {
	if (xDim > 0 && yDim > 0) {
		dim = {xDim, yDim};
		updatePlotTicks();
		lab.setDim(dim);
	}
}

void ofxGAxis::setDim(const array<float, 2>& newDim) {
	setDim(newDim[0], newDim[1]);
}

void ofxGAxis::setLim(const array<float, 2>& newLim) {
	if (newLim[1] != newLim[0]) {
		// Make sure the new limits makes sense
		if (log && (newLim[0] <= 0 || newLim[1] <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		} else {
			lim = newLim;

			if (!fixedTicks) {
				updateTicks();
				updateTickLabels();
			}

			updatePlotTicks();
			updateTicksInside();
		}
	}
}

void ofxGAxis::setLimAndLog(const array<float, 2>& newLim, bool newLog) {
	if (newLim[1] != newLim[0]) {
		// Make sure the new limits makes sense
		if (newLog && (newLim[0] <= 0 || newLim[1] <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		} else {
			lim = newLim;
			log = newLog;

			if (!fixedTicks) {
				updateTicks();
				updateTickLabels();
			}

			updatePlotTicks();
			updateTicksInside();
		}
	}
}

void ofxGAxis::setLog(bool newLog) {
	if (newLog != log) {
		log = newLog;

		// Check if the old limits still make sense
		if (log && (lim[0] <= 0 || lim[1] <= 0)) {
			throw invalid_argument("The axis limits are negative and this is not allowed in logarithmic scale.");
		}

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
	if (newLineWidth > 0) {
		lineWidth = newLineWidth;
	}
}

void ofxGAxis::setNTicks(int newNTicks) {
	if (newNTicks >= 0) {
		nTicks = newNTicks;
		ticksSeparation = -1;

		if (!log) {
			fixedTicks = false;
			updateTicks();
			updatePlotTicks();
			updateTicksInside();
			updateTickLabels();
		}
	}
}

void ofxGAxis::setTicksSeparation(float newTicksSeparation) {
	ticksSeparation = newTicksSeparation;

	if (!log) {
		fixedTicks = false;
		updateTicks();
		updatePlotTicks();
		updateTicksInside();
		updateTickLabels();
	}
}

void ofxGAxis::setTicks(const vector<float>& newTicks) {
	fixedTicks = true;
	ticks = newTicks;
	updatePlotTicks();
	updateTicksInside();
	updateTickLabels();
}

void ofxGAxis::setTickLabels(const vector<string>& newTickLabels) {
	if (newTickLabels.size() == tickLabels.size()) {
		fixedTicks = true;
		tickLabels = newTickLabels;
	}
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
	font.load(fontName, fontSize);
}

void ofxGAxis::setFontColor(const ofColor& newFontColor) {
	fontColor = newFontColor;
}

void ofxGAxis::setFontSize(int newFontSize) {
	if (newFontSize > 0) {
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}

void ofxGAxis::setFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize) {
	if (newFontSize > 0) {
		fontName = newFontName;
		fontColor = newFontColor;
		fontSize = newFontSize;
		font.load(fontName, fontSize);
	}
}

void ofxGAxis::setAllFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize) {
	setFontProperties(newFontName, newFontColor, newFontSize);
	lab.setFontProperties(newFontName, newFontColor, newFontSize);
}

vector<float> ofxGAxis::getTicks() const {
	return ticks;
}

vector<float> &ofxGAxis::getTicksRef() {
	return ticks;
}

vector<float> ofxGAxis::getPlotTicks() const {
	return plotTicks;
}

vector<float> &ofxGAxis::getPlotTicksRef() {
	return plotTicks;
}

ofxGAxisLabel &ofxGAxis::getAxisLabel() {
	return lab;
}

