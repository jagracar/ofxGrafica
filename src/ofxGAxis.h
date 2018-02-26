#pragma once

#include "ofxGConstants.h"
#include "ofxGAxisLabel.h"
#include "ofMain.h"

/**
 * @brief Axis class
 *
 * @author Javier Graciá Carpio
 */
class ofxGAxis {
public:

	/**
	 * @brief Constructor
	 *
	 * @param _type the axis type. It can be GRAFICA_X_AXIS, GRAFICA_Y_AXIS, GRAFICA_TOP_AXIS or GRAFICA_RIGHT_AXIS
	 * @param _dim the plot box dimensions in pixels
	 * @param _lim the limits
	 * @param _logScale the axis scale. True if it's logarithmic
	 */
	ofxGAxis(ofxGAxisType _type = GRAFICA_X_AXIS, const array<float, 2>& _dim = { 100, 100 },
			const array<float, 2>& _lim = { 0, 1 }, bool _logScale = false);

	/**
	 * @brief Moves the axis limits
	 *
	 * @param newLim the new axis limits
	 */
	void moveLim(const array<float, 2>& newLim);

	/**
	 * @brief Draws the axis
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
	 * @brief Sets the axis limits
	 *
	 * @param newLim the new axis limits
	 */
	void setLim(const array<float, 2>& newLim);

	/**
	 * @brief Sets the axis limits and the axis scale
	 *
	 * @param newLim the new axis limits
	 * @param newLogScale the new axis scale
	 */
	void setLimAndLogScale(const array<float, 2>& newLim, bool newLogScale);

	/**
	 * @brief Sets the axis scale
	 *
	 * @param newLogScale the new axis scale
	 */
	void setLogScale(bool newLogScale);

	/**
	 * @brief Sets the axis offset with respect to the plot box
	 *
	 * @param newOffset the new axis offset
	 */
	void setOffset(float newOffset);

	/**
	 * @brief Sets the line color
	 *
	 * @param newLineColor the new line color
	 */
	void setLineColor(const ofColor& newLineColor);

	/**
	 * @brief Sets the line width
	 *
	 * @param newLineWidth the new line width
	 */
	void setLineWidth(float newLineWidth);

	/**
	 * @brief Sets the approximate number of ticks in the axis.
	 *
	 * The actual number of ticks depends on the axis limits and the axis scale
	 *
	 * @param newNTicks the new approximate number of ticks in the axis
	 */
	void setNTicks(int newNTicks);

	/**
	 * @brief Sets the separation between the ticks in the axis
	 *
	 * @param newTicksSeparation the new ticks separation
	 */
	void setTicksSeparation(float newTicksSeparation);

	/**
	 * @brief Sets the axis ticks
	 *
	 * @param newTicks the new axis ticks
	 */
	void setTicks(const vector<float>& newTicks);

	/**
	 * @brief Sets the axis ticks labels
	 *
	 * @param newTickLabels the new axis ticks labels
	 */
	void setTickLabels(const vector<string>& newTickLabels);

	/**
	 * @brief Sets if the axis ticks are fixed or not
	 *
	 * @param newFixedTicks true if the axis ticks should be fixed
	 */
	void setFixedTicks(bool newFixedTicks);

	/**
	 * @brief Sets the tick length
	 *
	 * @param newTickLength the new tick length
	 */
	void setTickLength(float newTickLength);

	/**
	 * @brief Sets the small tick length
	 *
	 * @param newSmallTickLength the new small tick length
	 */
	void setSmallTickLength(float newSmallTickLength);

	/**
	 * @brief Sets if the ticks labels should be displayed in exponential form or not
	 *
	 * @param newExpTickLabels true if the ticks labels should be in exponential form
	 */
	void setExpTickLabels(bool newExpTickLabels);

	/**
	 * @brief Sets if the ticks labels should be displayed rotated or not
	 *
	 * @param newRotateTickLabels true is the ticks labels should be rotated
	 */
	void setRotateTickLabels(bool newRotateTickLabels);

	/**
	 * @brief Sets if the ticks labels should be drawn or not
	 *
	 * @param newDrawTicksLabels true it the ticks labels should be drawn
	 */
	void setDrawTickLabels(bool newDrawTicksLabels);

	/**
	 * @brief Sets the tick label offset
	 *
	 * @param newTickLabelOffset the new tick label offset
	 */
	void setTickLabelOffset(float newTickLabelOffset);

	/**
	 * @brief Sets if the axis label should be drawn or not
	 *
	 * @param newDrawAxisLabel true if the axis label should be drawn
	 */
	void setDrawAxisLabel(bool newDrawAxisLabel);

	/**
	 * @brief Sets the axis label text
	 *
	 * @param text the new axis label text
	 */
	void setAxisLabelText(const string& text);

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
	 * @brief Sets the font properties in the axis and the axis label
	 *
	 * @param newFontName the new font name
	 * @param newFontColor the new font color
	 * @param newFontSize the new font size
	 */
	void setAllFontProperties(const string& newFontName, const ofColor& newFontColor, int newFontSize);

	/**
	 * @brief Sets if the fonts should be made of contours
	 *
	 * @param newFontMakeContours true if the fonts should be made of contours
	 */
	void setFontsMakeContours(bool newFontMakeContours);

	/**
	 * @brief Returns a copy of the axis ticks
	 *
	 * @return a copy of the axis ticks
	 */
	vector<float> getTicks() const;

	/**
	 * @brief Returns the axis ticks
	 *
	 * @return the axis ticks
	 */
	const vector<float>& getTicksRef() const;

	/**
	 * @brief Returns a copy of the axis ticks positions in the plot reference system
	 *
	 * @return a copy of the axis ticks positions in the plot reference system
	 */
	vector<float> getPlotTicks() const;

	/**
	 * @brief Returns the axis ticks positions in the plot reference system
	 *
	 * @return the axis ticks positions in the plot reference system
	 */
	const vector<float>& getPlotTicksRef() const;

	/**
	 * @brief Returns the axis label
	 *
	 * @return the axis label
	 */
	ofxGAxisLabel& getAxisLabel();

protected:

	/**
	 * @brief Calculates the optimum number of significant digits to use for a given number
	 *
	 * @param number the number
	 *
	 * @return the number of significant digits
	 */
	static int obtainSigDigits(float number);

	/**
	 * @brief Rounds a number to a given number of significant digits
	 *
	 * @param number the number to round
	 * @param sigDigits the number of significant digits
	 *
	 * @return the rounded number
	 */
	static float roundPlus(float number, int sigDigits);

	/**
	 * @brief Updates the axis ticks
	 */
	void updateTicks();

	/**
	 * @brief Calculates the axis ticks for the logarithmic scale
	 */
	void obtainLogarithmicTicks();

	/**
	 * @brief Calculates the axis ticks for the linear scale
	 */
	void obtainLinearTicks();

	/**
	 * @brief Updates the positions of the axis ticks in the plot reference system
	 */
	void updatePlotTicks();

	/**
	 * @brief Updates the array that indicates which ticks are inside the axis limits
	 */
	void updateTicksInside();

	/**
	 * @brief Updates the axis tick labels
	 */
	void updateTickLabels();

	/**
	 * @brief Draws the axis as an X axis
	 */
	void drawAsXAxis() const;

	/**
	 * @brief Draws the axis as a Y axis
	 */
	void drawAsYAxis() const;

	/**
	 * @brief Draws the axis as a TOP axis
	 */
	void drawAsTopAxis() const;

	/**
	 * @brief Draws the axis as a RIGHT axis
	 */
	void drawAsRightAxis() const;

	/**
	 * @brief The axis type
	 */
	ofxGAxisType type;

	/**
	 * @brief The plot box dimensions in pixels
	 */
	array<float, 2> dim;

	/**
	 * @brief The axis limits
	 */
	array<float, 2> lim;

	/**
	 * @brief Defines if the scale should be logarithmic or not
	 */
	bool logScale;

	/**
	 * @brief The axis offset with respect to the plot box
	 */
	float offset;

	/**
	 * @brief The line color
	 */
	ofColor lineColor;

	/**
	 * @brief The line width
	 */
	float lineWidth;

	/**
	 * @brief The approximate number of ticks in the axis
	 */
	int nTicks;

	/**
	 * @brief The separation between the ticks in the axis
	 */
	float ticksSeparation;

	/**
	 * @brief The axis ticks
	 */
	vector<float> ticks;

	/**
	 * @brief The ticks positions in the plot reference system
	 */
	vector<float> plotTicks;

	/**
	 * @brief Defines if the ticks are inside the axis limits
	 */
	vector<bool> ticksInside;

	/**
	 * @brief The axis ticks labels
	 */
	vector<string> tickLabels;

	/**
	 * @brief Defines if the axis ticks are fixed or not
	 */
	bool fixedTicks;

	/**
	 * @brief The ticks length
	 */
	float tickLength;

	/**
	 * @brief The small ticks length
	 */
	float smallTickLength;

	/**
	 * @brief Defines if the ticks labels should be displayed in exponential form
	 */
	bool expTickLabels;

	/**
	 * @brief Defines if the ticks labels should be displayed rotated
	 */
	bool rotateTickLabels;

	/**
	 * @brief Defines if the ticks labels should be drawn
	 */
	bool drawTickLabels;

	/**
	 * @brief The tick label offset
	 */
	float tickLabelOffset;

	/**
	 * @brief The axis label
	 */
	ofxGAxisLabel lab;

	/**
	 * @brief Defines if the axis label should be drawn
	 */
	bool drawAxisLabel;

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
