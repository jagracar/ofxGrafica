#pragma once

#include "ofxGConstants.h"
#include "ofxGPoint.h"
#include "ofMain.h"

/**
 * @brief Histogram class
 *
 * @author Javier Graciá Carpio
 */
class ofxGHistogram {
public:

	/**
	 * @brief Constructor
	 *
	 * @param _type the histogram type. It can be GRAFICA_HORIZONTAL_HISTOGRAM or GRAFICA_VERTICAL_HISTOGRAM
	 * @param _dim the plot box dimensions in pixels
	 * @param _plotPoints the points positions in the plot reference system
	 */
	ofxGHistogram(ofxGHistogramType _type = GRAFICA_VERTICAL_HISTOGRAM, const array<float, 2>& _dim = { 100, 100 },
			const vector<ofxGPoint>& _plotPoints = { });

	/**
	 * @brief Draws the histogram
	 *
	 * @param plotBasePoint the histogram base point in the plot reference system
	 */
	void draw(const ofxGPoint& plotBasePoint) const;

	/**
	 * @brief Sets the type of histogram to display
	 *
	 * @param newType the new type of histogram to display. It can be GRAFICA_HORIZONTAL_HISTOGRAM or
	 * GRAFICA_VERTICAL_HISTOGRAM
	 */
	void setType(ofxGHistogramType newType);

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
	 * @brief Sets the histogram plot points
	 *
	 * @param newPlotPoints the new point positions in the plot reference system
	 */
	void setPlotPoints(const vector<ofxGPoint>& newPlotPoints);

	/**
	 * @brief Sets one of the histogram plot points
	 *
	 * @param index the point position
	 * @param newPlotPoint the new point positions in the plot reference system
	 */
	void setPlotPoint(vector<ofxGPoint>::size_type index, const ofxGPoint& newPlotPoint);

	/**
	 * @brief Adds a new plot point to the histogram
	 *
	 * @param newPlotPoint the new point position in the plot reference system
	 */
	void addPlotPoint(const ofxGPoint& newPlotPoint);

	/**
	 * @brief Adds a new plot point to the histogram
	 *
	 * @param index the position to add the point
	 * @param newPlotPoint the new point position in the plot reference system
	 */
	void addPlotPoint(vector<ofxGPoint>::size_type index, ofxGPoint& newPlotPoint);

	/**
	 * @brief Adds new plot points to the histogram
	 *
	 * @param newPlotPoints the new points positions in the plot reference system
	 */
	void addPlotPoints(const vector<ofxGPoint>& newPlotPoints);

	/**
	 * @brief Removes one of the points from the histogram
	 *
	 * @param index the point position
	 */
	void removePlotPoint(vector<ofxGPoint>::size_type index);

	/**
	 * @brief Sets the separations between the histogram elements
	 *
	 * @param newSeparations the new separations between the histogram elements
	 */
	void setSeparations(const vector<float>& newSeparations);

	/**
	 * @brief Sets the background colors of the histogram elements
	 *
	 * @param newBgColors the new background colors of the histogram elements
	 */
	void setBgColors(const vector<ofColor>& newBgColors);

	/**
	 * @brief Sets the line colors of the histogram elements
	 *
	 * @param newLineColors the new line colors of the histogram elements
	 */
	void setLineColors(const vector<ofColor>& newLineColors);

	/**
	 * @brief Sets the line widths of the histogram elements
	 *
	 * @param newLineWidths the new line widths of the histogram elements
	 */
	void setLineWidths(const vector<float>& newLineWidths);

	/**
	 * @brief Sets if the histogram should be visible or not
	 *
	 * @param newVisible true if the histogram should be visible
	 */
	void setVisible(bool newVisible);

	/**
	 * @brief Sets the histogram labels offset
	 *
	 * @param newLabelsOffset the new histogram labels offset
	 */
	void setLabelsOffset(float newLabelsOffset);

	/**
	 * @brief Sets if the histogram labels should be drawn or not
	 *
	 * @param newDrawLabels true if the histogram labels should be drawn
	 */
	void setDrawLabels(bool newDrawLabels);

	/**
	 * @brief Sets if the histogram labels should be rotated or not
	 *
	 * @param newRotateLabels true if the histogram labels should be rotated
	 */
	void setRotateLabels(bool newRotateLabels);

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
	 * @brief Updates the leftSides and rightSides arrays
	 */
	void updateArrays();

	/**
	 * @brief Draws the histogram labels
	 */
	void drawHistLabels() const;

	/**
	 * @brief The histogram type
	 */
	ofxGHistogramType type;

	/**
	 * @brief The plot box dimensions in pixels
	 */
	array<float, 2> dim;

	/**
	 * @brief The points positions in the plot reference system
	 */
	vector<ofxGPoint> plotPoints;

	/**
	 * @brief Defines if the histogram should be visible or not
	 */
	bool visible;

	/**
	 * @brief The separations between the histogram elements
	 */
	vector<float> separations;

	/**
	 * @brief The background colors of the histogram elements
	 */
	vector<ofColor> bgColors;

	/**
	 * @brief The line colors of the histogram elements
	 */
	vector<ofColor> lineColors;

	/**
	 * @brief The line widths of the histogram elements
	 */
	vector<float> lineWidths;

	/**
	 * @brief The left side sizes of the histogram elements
	 */
	vector<float> leftSides;

	/**
	 * @brief The right side sizes of the histogram elements
	 */
	vector<float> rightSides;

	/**
	 * @brief The labels offset
	 */
	float labelsOffset;

	/**
	 * @brief Defines if the labels should be drawn
	 */
	bool drawLabels;

	/**
	 * @brief Defines if the labels should be rotated
	 */
	bool rotateLabels;

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
