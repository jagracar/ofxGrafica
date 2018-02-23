#pragma once

#include "ofxGConstants.h"
#include "ofMain.h"

/**
 * @brief Axis label class
 *
 * @author Javier Graciá Carpio
 */
class ofxGAxisLabel {
public:

	/**
	 * @brief Constructor
	 *
	 * @param _type the axis type. It can be GRAFICA_X_AXIS, GRAFICA_Y_AXIS, GRAFICA_TOP_AXIS or GRAFICA_RIGHT_AXIS
	 * @param _dim the plot box dimensions in pixels
	 * @param _text the label text
	 */
	ofxGAxisLabel(ofxGAxisType _type = GRAFICA_X_AXIS, const array<float, 2>& _dim = { 100, 100 }, const string& _text =
			"");

	/**
	 * @brief Draws the axis label
	 */
	void draw() const;

	/**
	 * @brief Sets the plot box dimensions information
	 *
	 * @param xDim the new plot box x dimension
	 * @param yDim the new plot box y dimension
	 */
	void setDim(float xDim, float yDim);

	/**
	 * @brief Sets the plot box dimensions information
	 *
	 * @param newDim the new plot box dimensions information
	 */
	void setDim(const array<float, 2>& newDim);

	/**
	 * @brief Sets the label relative position in the axis
	 *
	 * @param newRelativePos the new relative position in the axis
	 */
	void setRelativePos(float newRelativePos);

	/**
	 * @brief Sets the axis label offset
	 *
	 * @param newOffset the new axis label offset
	 */
	void setOffset(float newOffset);

	/**
	 * @brief Sets if the axis label should be rotated or not
	 *
	 * @param newRotate true if the axis label should be rotated
	 */
	void setRotate(bool newRotate);

	/**
	 * @brief Sets the axis label text
	 *
	 * @param newText the new axis label text
	 */
	void setText(const string& newText);

	/**
	 * @brief Sets the axis label type of text alignment
	 *
	 * @param newTextAlignment the new type of text alignment. It can be GRAFICA_CENTER_ALIGN, GRAFICA_LEFT_ALIGN or
	 * GRAFICA_RIGHT_ALIGN
	 */
	void setTextAlignment(ofxGTextAlignment newTextAlignment);

	/**
	 * @brief Sets the font name
	 *
	 * @param newFontName the name of the new font
	 */
	void setFontName(const string& newFontName);

	/**
	 * @brief Sets the font color
	 *
	 * @param newFontColor the new font color
	 */
	void setFontColor(const ofColor& newFontColor);

	/**
	 * @brief Sets the font size
	 *
	 * @param newFontSize the new font size
	 */
	void setFontSize(int newFontSize);

	/**
	 * @brief Sets all the font properties at once
	 *
	 * @param newFontName the name of the new font
	 * @param newFontColor the new font color
	 * @param newFontSize the new font size
	 */
	void setFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize);

	/**
	 * @brief Sets if the font should be made of contours
	 *
	 * @param newFontMakeContours true if the font should be made of contours
	 */
	void setFontMakeContours(bool newFontMakeContours);

protected:

	/**
	 * @brief Draws the axis label as an X axis label
	 */
	void drawAsXLabel() const;

	/**
	 * @brief Draws the axis label as an Y axis label
	 */
	void drawAsYLabel() const;

	/**
	 * @brief Draws the axis label as an TOP axis label
	 */
	void drawAsTopLabel() const;

	/**
	 * @brief Draws the axis label as an RIGHT axis label
	 */
	void drawAsRightLabel() const;

	/**
	 * @brief The axis type
	 */
	ofxGAxisType type;

	/**
	 * @brief The plot box dimensions in pixels
	 */
	array<float, 2> dim;

	/**
	 * @brief The label text
	 */
	string text;

	/**
	 * @brief The label relative position in the axis
	 */
	float relativePos;

	/**
	 * @brief The label position in plot units
	 */
	float plotPos;

	/**
	 * @brief The label offset
	 */
	float offset;

	/**
	 * @brief Defines if the label should be rotated or not
	 */
	bool rotate;

	/**
	 * @brief The text alignment
	 */
	ofxGTextAlignment textAlignment;

	/**
	 * @brief The font name
	 */
	string fontName;

	/**
	 * @brief The font color
	 */
	ofColor fontColor;

	/**
	 * @brief The font size
	 */
	int fontSize;

	/**
	 * @brief Defines if the font should be made of contours
	 */
	bool fontMakeContours;

	/**
	 * @brief The trueType font
	 */
	ofTrueTypeFont font;
};
