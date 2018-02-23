#pragma once

#include "ofxGConstants.h"
#include "ofMain.h"

/**
 * @brief Title class
 *
 * @author Javier Graciá Carpio
 */
class ofxGTitle {
public:

	/**
	 * @brief Constructor
	 *
	 * @param _dim the plot box dimensions in pixels
	 * @param _text the title text
	 */
	ofxGTitle(const array<float, 2>& _dim = { 100, 100 }, const string& _text = "");

	/**
	 * @brief Draws the plot title
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
	 * @brief Sets the title relative position in the plot
	 *
	 * @param newRelativePos the new relative position in the plot
	 */
	void setRelativePos(float newRelativePos);

	/**
	 * @brief Sets the title offset
	 *
	 * @param newOffset the new title offset
	 */
	void setOffset(float newOffset);

	/**
	 * @brief Sets the title text
	 *
	 * @param newText the new title text
	 */
	void setText(const string& newText);

	/**
	 * @brief Sets the title type of text alignment
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
	 * @brief The plot box dimensions in pixels
	 */
	array<float, 2> dim;

	/**
	 * @brief The title text
	 */
	string text;

	/**
	 * @brief The title relative position in the plot
	 */
	float relativePos;

	/**
	 * @brief The title position in plot units
	 */
	float plotPos;

	/**
	 * @brief The title offset
	 */
	float offset;

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
