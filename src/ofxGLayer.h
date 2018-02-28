#pragma once

#include "ofxGConstants.h"
#include "ofxGPoint.h"
#include "ofxGHistogram.h"
#include "ofMain.h"

/**
 * @brief Layer class
 *
 * A ofxGLayer usually contains an array of points and a histogram
 *
 * @author Javier Graciá Carpio
 */
class ofxGLayer {
public:

	/**
	 * @brief Constructor
	 *
	 * @param _id the layer id
	 * @param _dim the plot box dimensions in pixels
	 * @param _xLim the horizontal limits
	 * @param _yLim the vertical limits
	 * @param _xLogScale the horizontal scale. True if it's logarithmic
	 * @param _yLogScale the vertical scale. True if it's logarithmic
	 */
	ofxGLayer(const string& _id = "defaultId", const array<float, 2>& _dim = { 100, 100 },
			const array<float, 2>& _xLim = { 0, 1 }, const array<float, 2>& _yLim = { 0, 1 }, bool _xLogScale = false,
			bool _yLogScale = false);

	/**
	 * @brief Checks if the layer's id is equal to a given id
	 *
	 * @param someId the id to check
	 *
	 * @return true if the provided id is equal to the layer's id
	 */
	bool isId(const string& someId) const;

	/**
	 * @brief Calculates the position of the x value in the plot reference system
	 *
	 * @param x the x value
	 *
	 * @return the x position in the plot reference system
	 */
	float valueToXPlot(float x) const;

	/**
	 * @brief Calculates the position of the y value in the plot reference system
	 *
	 * @param y the y value
	 *
	 * @return the y position in the plot reference system
	 */
	float valueToYPlot(float y) const;

	/**
	 * @brief Calculates the position of a given (x, y) point in the plot reference system
	 *
	 * @param x the x value
	 * @param y the y value
	 *
	 * @return the (x, y) position in the plot reference system
	 */
	array<float, 2> valueToPlot(float x, float y) const;

	/**
	 * @brief Calculates the position of a given point in the plot reference system
	 *
	 * @param point the point
	 *
	 * @return a copy of the point with its position transformed to the plot reference system
	 */
	ofxGPoint valueToPlot(const ofxGPoint& point) const;

	/**
	 * @brief Calculates the positions of a given set of points in the plot reference system
	 *
	 * @param pts the set of points
	 *
	 * @return a copy of the set of points with their positions transformed to the plot reference system
	 */
	vector<ofxGPoint> valueToPlot(const vector<ofxGPoint>& pts) const;

	/**
	 * @brief Returns the plot values at a given position in the plot reference system
	 *
	 * @param xPlot x position in the plot reference system
	 * @param yPlot y position in the plot reference system
	 *
	 * @return the (x, y) values at the (xPlot, yPlot) position
	 */
	array<float, 2> plotToValue(float xPlot, float yPlot) const;

	/**
	 * @brief Checks if a given (xPlot, yPlot) position in the plot reference system is inside the layer limits
	 *
	 * @param xPlot x position in the plot reference system
	 * @param yPlot y position in the plot reference system
	 *
	 * @return true if the (xPlot, yPlot) position is inside the layer limits
	 */
	bool isInside(float xPlot, float yPlot) const;

	/**
	 * @brief Checks if a given point in the plot reference system is inside the layer limits
	 *
	 * @param plotPoint the point in the plot reference system
	 *
	 * @return true if the point is inside the layer limits
	 */
	bool isInside(const ofxGPoint& plotPoint) const;

	/**
	 * @brief Checks if a given set of points in the plot reference system is inside the layer limits
	 *
	 * @param plotPts the set of points to check
	 *
	 * @return a boolean vector with the elements set to true if the point is inside the layer limits
	 */
	vector<bool> isInside(const vector<ofxGPoint>& plotPts) const;

	/**
	 * @brief Returns the position index of the closest point (if any) to a given position in the plot reference system
	 *
	 * @param xPlot x position in the plot reference system
	 * @param yPlot y position in the plot reference system
	 *
	 * @return the position index of closest point to the specified position. Returns points array size if there is no
	 * close point.
	 */
	vector<ofxGPoint>::size_type getPointIndexAtPlotPos(float xPlot, float yPlot) const;

	/**
	 * @brief Returns the closest point (if any) to a given position in the plot reference system
	 *
	 * @param xPlot x position in the plot reference system
	 * @param yPlot y position in the plot reference system
	 *
	 * @return a pointer to the closest point to the specified position. Returns nullptr if there is no close point.
	 */
	const ofxGPoint* getPointAtPlotPos(float xPlot, float yPlot) const;

	/**
	 * @brief Initializes the histogram
	 *
	 * @param histType the type of histogram to use. It can be GRAFICA_VERTICAL_HISTOGRAM or
	 * GRAFICA_HORIZONTAL_HISTOGRAM
	 */
	void startHistogram(ofxGHistogramType histType);

	/**
	 * @brief Stops updating and displaying the histogram
	 */
	void stopHistogram();

	/**
	 * @brief Draws the points inside the layer limits
	 */
	void drawPoints() const;

	/**
	 * @brief Draws the points inside the layer limits
	 *
	 * This method is considerably faster than drawPoints()
	 *
	 * @param pointColor the point color
	 */
	void drawPoints(const ofColor& pointColor) const;

	/**
	 * @brief Draws the points inside the layer limits
	 *
	 * @param pointShape the shape that should be used to represent the points
	 */
	void drawPoints(ofPath& pointShape) const;

	/**
	 * @brief Draws the points inside the layer limits
	 *
	 * @param pointImg the image that should be used to represent the points
	 */
	void drawPoints(const ofImage& pointImg) const;

	/**
	 * @brief Draws a point
	 *
	 * @param point the point to draw
	 * @param pointColor color to use
	 * @param pointSize point size in pixels
	 */
	void drawPoint(const ofxGPoint& point, const ofColor& pointColor, float pointSize) const;

	/**
	 * @brief Draws a point
	 *
	 * @param point the point to draw
	 */
	void drawPoint(const ofxGPoint& point) const;

	/**
	 * @brief Draws a point
	 *
	 * @param point the point to draw
	 * @param pointShape the shape that should be used to represent the point
	 */
	void drawPoint(const ofxGPoint& point, const ofPath& pointShape) const;

	/**
	 * @brief Draws a point
	 *
	 * @param point the point to draw
	 * @param pointShape the shape that should be used to represent the points
	 * @param pointColor color to use
	 */
	void drawPoint(const ofxGPoint& point, ofPath& pointShape, const ofColor& pointColor) const;

	/**
	 * @brief Draws a point
	 *
	 * @param point the point to draw
	 * @param pointImg the image that should be used to represent the point
	 */
	void drawPoint(const ofxGPoint& point, const ofImage& pointImg) const;

	/**
	 * @brief Draws lines connecting consecutive points in the layer
	 */
	void drawLines();

	/**
	 * @brief Draws a line between two points
	 *
	 * @param point1 first point
	 * @param point2 second point
	 * @param lc line color
	 * @param lw line width
	 */
	void drawLine(const ofxGPoint& point1, const ofxGPoint& point2, const ofColor& lc, float lw);

	/**
	 * @brief Draws a line between two points
	 *
	 * @param point1 first point
	 * @param point2 second point
	 */
	void drawLine(const ofxGPoint& point1, const ofxGPoint& point2);

	/**
	 * @brief Draws a line defined by the slope and the cut in the y axis
	 *
	 * @param slope the line slope
	 * @param yCut the line y axis cut
	 * @param lc line color
	 * @param lw line width
	 */
	void drawLine(float slope, float yCut, const ofColor& lc, float lw);

	/**
	 * @brief Draws a line defined by the slope and the cut in the y axis
	 *
	 * @param slope the line slope
	 * @param yCut the line y axis cut
	 */
	void drawLine(float slope, float yCut);

	/**
	 * @brief Draws an horizontal line
	 *
	 * @param value line horizontal value
	 * @param lc line color
	 * @param lw line width
	 */
	void drawHorizontalLine(float value, const ofColor& lc, float lw) const;

	/**
	 * @brief Draws an horizontal line
	 *
	 * @param value line horizontal value
	 */
	void drawHorizontalLine(float value) const;

	/**
	 * @brief Draws a vertical line
	 *
	 * @param value line vertical value
	 * @param lc line color
	 * @param lw line width
	 */
	void drawVerticalLine(float value, const ofColor& lc, float lw) const;

	/**
	 * @brief Draws a vertical line
	 *
	 * @param value line vertical value
	 */
	void drawVerticalLine(float value) const;

	/**
	 * @brief Draws a filled contour connecting consecutive points in the layer and a reference value
	 *
	 * @param contourType the type of contours to use. It can be GRAFICA_HORIZONTAL_CONTOUR or GRAFICA_VERTICAL_CONTOUR
	 * @param referenceValue the reference value to use to close the contour
	 */
	void drawFilledContour(ofxGContourType contourType, float referenceValue);

	/**
	 * @brief Draws the label of a given point
	 *
	 * @param point the point
	 */
	void drawLabel(const ofxGPoint& point) const;

	/**
	 * @brief Draws the label of the closest point in the layer to a given plot position
	 *
	 * @param xPlot x position in the plot reference system
	 * @param yPlot y position in the plot reference system
	 */
	void drawLabelAtPlotPos(float xPlot, float yPlot) const;

	/**
	 * @brief Draws the histogram
	 */
	void drawHistogram() const;

	/**
	 * @brief Draws a polygon defined by a set of points
	 *
	 * @param polygonPoints the points that define the polygon
	 * @param polygonColor the color to use to draw the polygon (contour and background)
	 */
	void drawPolygon(const vector<ofxGPoint>& polygonPoints, const ofColor& polygonColor);

	/**
	 * @brief Draws an annotation at a given plot value
	 *
	 * @param text the annotation text
	 * @param x x plot value
	 * @param y y plot value
	 * @param verAlign text vertical alignment. It can be GRAFICA_CENTER_ALIGN, GRAFICA_TOP_ALIGN or GRAFICA_BOTTOM_ALIGN
	 */
	void drawAnnotation(const string& text, float x, float y, ofxGTextAlignment verAlign) const;

	/**
	 * @brief Sets the layer dimensions
	 *
	 * @param xDim the new layer x dimension
	 * @param yDim the new layer y dimension
	 */
	void setDim(float xDim, float yDim);

	/**
	 * @brief Sets the layer dimensions
	 *
	 * @param newDim the new layer dimensions
	 */
	void setDim(const array<float, 2>& newDim);

	/**
	 * @brief Sets the horizontal limits
	 *
	 * @param xMin the minimum limit value
	 * @param xMax the maximum limit value
	 */
	void setXLim(float xMin, float xMax);

	/**
	 * @brief Sets the horizontal limits
	 *
	 * @param newXLim the new horizontal limits
	 */
	void setXLim(const array<float, 2>& newXLim);

	/**
	 * @brief Sets the vertical limits
	 *
	 * @param yMin the minimum limit value
	 * @param yMax the maximum limit value
	 */
	void setYLim(float yMin, float yMax);

	/**
	 * @brief Sets the vertical limits
	 *
	 * @param newYLim the new vertical limits
	 */
	void setYLim(const array<float, 2>& newYLim);

	/**
	 * @brief Sets the horizontal and vertical limits
	 *
	 * @param xMin the minimum horizontal limit value
	 * @param xMax the maximum horizontal limit value
	 * @param yMin the minimum vertical limit value
	 * @param yMax the maximum vertical limit value
	 */
	void setXYLim(float xMin, float xMax, float yMin, float yMax);

	/**
	 * @brief Sets the horizontal and vertical limits
	 *
	 * @param newXLim the new horizontal limits
	 * @param newYLim the new vertical limits
	 */
	void setXYLim(const array<float, 2>& newXLim, const array<float, 2>& newYLim);

	/**
	 * @brief Sets the horizontal and vertical limits and the horizontal and vertical scales
	 *
	 * @param xMin the minimum horizontal limit value
	 * @param xMax the maximum horizontal limit value
	 * @param yMin the minimum vertical limit value
	 * @param yMax the maximum vertical limit value
	 * @param newXLogScale the new horizontal scale
	 * @param newYLogScale the new vertical scale
	 */
	void setLimAndLogScale(float xMin, float xMax, float yMin, float yMax, bool newXLogScale, bool newYLogScale);

	/**
	 * @brief Sets the horizontal and vertical limits and the horizontal and vertical scales
	 *
	 * @param newXLim the new horizontal limits
	 * @param newYLim the new vertical limits
	 * @param newXLogScale the new horizontal scale
	 * @param newYLogScale the new vertical scale
	 */
	void setLimAndLogScale(const array<float, 2>& newXLim, const array<float, 2>& newYLim, bool newXLogScale,
			bool newYLogScale);

	/**
	 * @brief Sets the horizontal scale
	 *
	 * @param newXLogScale the new horizontal scale
	 */
	void setXLogScale(bool newXLogScale);

	/**
	 * @brief Sets the vertical scale
	 *
	 * @param newYLogScale the new vertical scale
	 */
	void setYLogScale(bool newYLogScale);

	/**
	 * @brief Sets the layer points
	 *
	 * @param newPoints the new points
	 */
	void setPoints(const vector<ofxGPoint>& newPoints);

	/**
	 * @brief Sets one of the layer points
	 *
	 * @param index the point position
	 * @param x the point new x coordinate
	 * @param y the point new y coordinate
	 * @param label the point new label
	 */
	void setPoint(vector<ofxGPoint>::size_type index, float x, float y, const string& label);

	/**
	 * @brief Sets one of the layer points
	 *
	 * @param index the point position
	 * @param x the point new x coordinate
	 * @param y the point new y coordinate
	 */
	void setPoint(vector<ofxGPoint>::size_type index, float x, float y);

	/**
	 * @brief Sets one of the layer points
	 *
	 * @param index the point position
	 * @param newPoint the new point
	 */
	void setPoint(vector<ofxGPoint>::size_type index, const ofxGPoint& newPoint);

	/**
	 * @brief Adds a new point to the layer points
	 *
	 * @param x the new point x coordinate
	 * @param y the new point y coordinate
	 * @param label the new point label
	 */
	void addPoint(float x, float y, const string& label);

	/**
	 * @brief Adds a new point to the layer points
	 *
	 * @param x the new point x coordinate
	 * @param y the new point y coordinate
	 */
	void addPoint(float x, float y);

	/**
	 * @brief Adds a new point to the layer points
	 *
	 * @param newPoint the point to add
	 */
	void addPoint(const ofxGPoint& newPoint);

	/**
	 * @brief Adds a new point to the layer points
	 *
	 * @param index the position to add the point
	 * @param x the new point x coordinate
	 * @param y the new point y coordinate
	 * @param label the new point label
	 */
	void addPoint(vector<ofxGPoint>::size_type index, float x, float y, const string& label);

	/**
	 * @brief Adds a new point to the layer points
	 *
	 * @param index the position to add the point
	 * @param x the new point x coordinate
	 * @param y the new point y coordinate
	 */
	void addPoint(vector<ofxGPoint>::size_type index, float x, float y);

	/**
	 * @brief Adds a new point to the layer points
	 *
	 * @param index the position to add the point
	 * @param newPoint the point to add
	 */
	void addPoint(vector<ofxGPoint>::size_type index, const ofxGPoint& newPoint);

	/**
	 * @brief Adds new points to the layer points
	 *
	 * @param newPoints the points to add
	 */
	void addPoints(const vector<ofxGPoint>& newPoints);

	/**
	 * @brief Removes one of the layer points
	 *
	 * @param index the point position
	 */
	void removePoint(vector<ofxGPoint>::size_type index);

	/**
	 * @brief Sets which points are inside the box
	 *
	 * @param newInside a boolean array with the information whether a point is inside or not
	 */
	void setInside(const vector<bool>& newInside);

	/**
	 * @brief Sets the points colors
	 *
	 * @param newPointColors the new point colors
	 */
	void setPointColors(const vector<ofColor>& newPointColors);

	/**
	 * @brief Sets the points color
	 *
	 * @param newPointColor the new point color
	 */
	void setPointColor(const ofColor& newPointColor);

	/**
	 * @brief Sets the points sizes
	 *
	 * @param newPointSizes the new point sizes
	 */
	void setPointSizes(const vector<float>& newPointSizes);

	/**
	 * @brief Sets the points size
	 *
	 * @param newPointSize the new point size
	 */
	void setPointSize(float newPointSize);

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
	 * @brief Sets the histogram base point
	 *
	 * @param newHistBasePoint the new histogram base point
	 */
	void setHistBasePoint(const ofxGPoint& newHistBasePoint);

	/**
	 * @brief Sets the histogram type
	 *
	 * @param histType the new histogram type. It can be GRAFICA_HORIZONTAL_HISTOGRAM or GRAFICA_VERTICAL_HISTOGRAM
	 */
	void setHistType(ofxGHistogramType histType);

	/**
	 * @brief Sets if the histogram is visible or not
	 *
	 * @param visible if true, the histogram is visible
	 */
	void setHistVisible(bool visible);

	/**
	 * @brief Sets if the histogram labels will be drawn or not
	 *
	 * @param drawHistLabels if true, the histogram labels will be drawn
	 */
	void setDrawHistLabels(bool drawHistLabels);

	/**
	 * @brief Sets the label background color
	 *
	 * @param newLabelBgColor the new label background color
	 */
	void setLabelBgColor(const ofColor& newLabelBgColor);

	/**
	 * @brief Sets the label separation
	 *
	 * @param newLabelSeparation the new label separation
	 */
	void setLabelSeparation(const array<float, 2>& newLabelSeparation);

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
	 * @brief Sets the font properties in the layer and the histogram
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
	 * @brief Returns the layer id
	 *
	 * @return the layer id
	 */
	string getId() const;

	/**
	 * @brief Returns the layer dimensions
	 *
	 * @return the layer dimensions
	 */
	array<float, 2> getDim() const;

	/**
	 * @brief Returns the layer horizontal limits
	 *
	 * @return the layer horizontal limits
	 */
	array<float, 2> getXLim() const;

	/**
	 * @brief Returns the layer vertical limits
	 *
	 * @return the layer vertical limits
	 */
	array<float, 2> getYLim() const;

	/**
	 * @brief Returns the layer horizontal scale
	 *
	 * @return the layer horizontal scale
	 */
	bool getXLogScale() const;

	/**
	 * @brief Returns the layer vertical scale
	 *
	 * @return the layer vertical scale
	 */
	bool getYLogScale() const;

	/**
	 * @brief Returns a copy of the layer points
	 *
	 * @return a copy of the layer points
	 */
	vector<ofxGPoint> getPoints() const;

	/**
	 * @brief Returns the layer points
	 *
	 * @return the layer points
	 */
	const vector<ofxGPoint>& getPointsRef() const;

	/**
	 * @brief Returns the layer point colors array
	 *
	 * @return the layer point colors array
	 */
	vector<ofColor> getPointColors() const;

	/**
	 * @brief Returns the layer point sizes array
	 *
	 * @return the layer point sizes array
	 */
	vector<float> getPointSizes() const;

	/**
	 * @brief Returns the layer line color
	 *
	 * @return the layer line color
	 */
	ofColor getLineColor() const;

	/**
	 * @brief Returns the layer line width
	 *
	 * @return the layer line width
	 */
	float getLineWidth() const;

	/**
	 * @brief Returns the layer histogram
	 *
	 * @return the layer histogram
	 */
	ofxGHistogram& getHistogram();

protected:

	/**
	 * @brief Updates the position of the layer points to the plot reference system
	 */
	void updatePlotPoints();

	/**
	 * @brief Returns the plot x value at a given position in the plot reference system
	 *
	 * @param xPlot x position in the plot reference system
	 *
	 * @return the x values at the xPlot position
	 */
	float xPlotToValue(float xPlot) const;

	/**
	 * @brief Returns the plot y value at a given position in the plot reference system
	 *
	 * @param yPlot y position in the plot reference system
	 *
	 * @return the y values at the yPlot position
	 */
	float yPlotToValue(float yPlot) const;

	/**
	 * @brief Updates the array list that tells if the points are inside the layer limits or not
	 */
	void updateInsideList();

	/**
	 * @brief Obtains the box intersections of the line that connects two given points
	 *
	 * @param plotPoint1 the first point in the plot reference system
	 * @param plotPoint2 the second point in the plot reference system
	 *
	 * @return the number of box intersections in the plot reference system
	 */
	int obtainBoxIntersections(const ofxGPoint& plotPoint1, const ofxGPoint& plotPoint2);

	/**
	 * @brief Returns only those cuts that are inside the box region and lie between the two given points
	 *
	 * @param cuts the axis cuts
	 * @param nCuts the number of cuts
	 * @param plotPoint1 the first point in the plot reference system
	 * @param plotPoint2 the second point in the plot reference system
	 *
	 * @return the number of cuts inside the box region and between the two points
	 */
	int getValidCuts(array<array<float, 2>, 4>& cuts, int nCuts, const ofxGPoint& plotPoint1,
			const ofxGPoint& plotPoint2);

	/**
	 * @brief Removes duplicated cuts
	 *
	 * @param cuts the box cuts
	 * @param nCuts the number of cuts
	 * @param tolerance maximum distance after which the points can't be duplicates
	 *
	 * @return the number of cuts without the duplications
	 */
	int removeDuplicatedCuts(array<array<float, 2>, 4>& cuts, int nCuts, float tolerance);

	/**
	 * @brief Removes cuts that are equal to a given point
	 *
	 * @param cuts the box cuts
	 * @param nCuts the number of cuts
	 * @param plotPoint the point to compare with
	 * @param tolerance maximum distance after which the points can't be equal
	 *
	 * @return the number of cuts without the point duplications
	 */
	int removePointFromCuts(array<array<float, 2>, 4>& cuts, int nCuts, const ofxGPoint& plotPoint, float tolerance);

	/**
	 * @brief Obtains the shape points of the horizontal contour that connects consecutive layer points and a reference
	 * value
	 *
	 * @param referenceValue the reference value to use to close the contour
	 *
	 * @return the shape points
	 */
	vector<ofxGPoint> getHorizontalShape(float referenceValue);

	/**
	 * @brief Obtains the shape points of the vertical contour that connects consecutive layer points and a reference
	 * value
	 *
	 * @param referenceValue the reference value to use to close the contour
	 *
	 * @return the shape points
	 */
	vector<ofxGPoint> getVerticalShape(float referenceValue);

	/**
	 * @brief The layer id
	 */
	string id;

	/**
	 * @brief The plot box dimensions in pixels
	 */
	array<float, 2> dim;

	/**
	 * @brief The horizontal limits
	 */
	array<float, 2> xLim;

	/**
	 * @brief The vertical limits
	 */
	array<float, 2> yLim;

	/**
	 * @brief Defines if the horizontal scale is logarithmic or not
	 */
	bool xLogScale;

	/**
	 * @brief Defines if the vertical scale is logarithmic or not
	 */
	bool yLogScale;

	/**
	 * @brief The layer points
	 */
	vector<ofxGPoint> points;

	/**
	 * @brief The layer points in the plot reference system
	 */
	vector<ofxGPoint> plotPoints;

	/**
	 * @brief Defines if the points are inside the layer limits
	 */
	vector<bool> inside;

	/**
	 * @brief The points colors
	 */
	vector<ofColor> pointColors;

	/**
	 * @brief The points sizes
	 */
	vector<float> pointSizes;

	/**
	 * @brief The line color
	 */
	ofColor lineColor;

	/**
	 * @brief The line width
	 */
	float lineWidth;

	/**
	 * @brief The layer histogram
	 */
	ofxGHistogram hist;

	/**
	 * @brief The histogram base point
	 */
	ofxGPoint histBasePoint;

	/**
	 * @brief Defines if the histogram is active and visible or not
	 */
	bool histIsActive;

	/**
	 * @brief The label background color
	 */
	ofColor labelBgColor;

	/**
	 * @brief The label separations
	 */
	array<float, 2> labelSeparation;

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

	/**
	 * @brief Utility array containing line cuts with the plot box
	 */
	array<array<float, 2>, 4> cuts;
};
