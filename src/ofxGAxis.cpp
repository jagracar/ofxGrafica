#include "ofxGAxis.h"

using namespace grafica;

ofxGAxis::ofxGAxis(int _type, array<float, 2> _dim, array<float, 2> _lim,
		bool _log) :
		dim(_dim), lim(_lim), log(_log) {

	type = (_type == GRAFICA_X || _type == GRAFICA_Y || _type == GRAFICA_TOP
			|| _type == GRAFICA_RIGHT) ? _type : GRAFICA_X;

	// Do some sanity checks
	if (log && (lim[0] <= 0 || lim[1] <= 0)) {
		ofLogWarning(
				"The limits are negative. This is not allowed in logarithmic scale.");
		ofLogWarning("Will set them to (0.1, 10)");

		if (lim[1] > lim[0]) {
			lim = {0.1, 10};
		} else {
			lim = {10, 0.1};
		}
	}

	offset = 5;
	lineColor = ofColor(0);
	lineWidth = 1;

	nTicks = 5;
	ticksSeparation = -1;
	fixedTicks = false;
	tickLength = 3;
	smallTickLength = 2;
	expTickLabels = false;
	rotateTickLabels = (type == GRAFICA_X || type == GRAFICA_TOP) ? false : true;
	drawTickLabels = (type == GRAFICA_X || type == GRAFICA_X) ? true : false;
	tickLabelOffset = 7;

	//lab = ofxGAxisLabel(type, dim);
	drawAxisLabel = true;

	fontName = "verdana.ttf";
	fontColor = ofColor(0);
	fontSize = 11;
	font.load(fontName, fontSize, true, true);

	// Update the arrayLists
	updateTicks();
	updatePlotTicks();
	updateTicksInside();
	updateTickLabels();
}

void ofxGAxis::updateTicks() {

}

void ofxGAxis::updatePlotTicks() {

}

void ofxGAxis::updateTicksInside() {

}

void ofxGAxis::updateTickLabels() {

}

void ofxGAxis::draw() {
	switch (type) {
	case GRAFICA_X:
		drawAsXAxis();
		break;
	case GRAFICA_Y:
		drawAsYAxis();
		break;
	case GRAFICA_TOP:
		drawAsTopAxis();
		break;
	case GRAFICA_RIGHT:
		drawAsRightAxis();
		break;
	}

	if (drawAxisLabel) {
		//lab.draw();
	}
}


void ofxGAxis::drawAsXAxis() {

}

void ofxGAxis::drawAsYAxis() {

}

void ofxGAxis::drawAsTopAxis() {

}

void ofxGAxis::drawAsRightAxis() {

}
