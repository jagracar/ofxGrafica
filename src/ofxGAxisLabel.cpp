#include "ofxGAxisLabel.h"

using namespace grafica;

ofxGAxisLabel::ofxGAxisLabel(int _type, const array<float, 2> &_dim) :
		dim(_dim) {

	type = (_type == GRAFICA_X || _type == GRAFICA_Y || _type == GRAFICA_TOP
			|| _type == GRAFICA_RIGHT) ? _type : GRAFICA_X;
	relativePos = 0.5;
	plotPos =
			(type == GRAFICA_X || type == GRAFICA_TOP) ?
					relativePos * dim[0] : -relativePos * dim[1];
	offset = 35;
	rotate = (type == GRAFICA_X || type == GRAFICA_TOP) ? false : true;

	text = "";
	textAlignment = GRAFICA_CENTER;
	fontName = "verdana.ttf";
	fontColor = ofColor(0);
	fontSize = 13;
	font.load(fontName, fontSize, true, true);
}
