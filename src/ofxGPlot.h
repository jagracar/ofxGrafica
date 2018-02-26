#pragma once

#include "ofxGConstants.h"
#include "ofxGLayer.h"
#include "ofxGAxis.h"
#include "ofxGTitle.h"
#include "ofxGHistogram.h"
#include "ofMain.h"

/**
 * @brief Plot class
 *
 * It controls the rest of the graphical elements (layers, axes, title, limits).
 *
 * @author Javier Graciá Carpio
 */
class ofxGPlot {
public:

	/**
	 * @brief Constructor
	 *
	 * @param xPos the plot x position on the screen
	 * @param yPos the plot y position on the screen
	 * @param plotWidth the plot width (x outer dimension)
	 * @param plotHeight the plot height (y outer dimension)
	 */
	ofxGPlot(float xPos = 0, float yPos = 0, float plotWidth = 450, float plotHeight = 300);

	/**
	 * @brief Adds a layer to the plot
	 *
	 * @param newLayer the layer to add
	 */
	void addLayer(const ofxGLayer& newLayer);

	/**
	 * @brief Adds a new layer to the plot
	 *
	 * @param id the id to use for the new layer
	 * @param points the points to be included in the layer
	 */
	void addLayer(const string& id, const vector<ofxGPoint>& points);

	/**
	 * @brief Removes an exiting layer from the plot, provided it is not the plot main layer
	 *
	 * @param id the id of the layer to remove
	 */
	void removeLayer(const string& id);

	/**
	 * @brief Calculates the position of a point in the screen, relative to the plot reference system
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 *
	 * @return the x and y positions in the plot reference system
	 */
	array<float, 2> getPlotPosAt(float xScreen, float yScreen) const;

	/**
	 * @brief Calculates the position of a given (x, y) point in the screen
	 *
	 * @param xValue the x value
	 * @param yValue the y value
	 *
	 * @return the position of the (x, y) point in the screen
	 */
	array<float, 2> getScreenPosAtValue(float xValue, float yValue) const;

	/**
	 * @brief Returns the closest point in the main layer to a given screen position
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 *
	 * @return a pointer to the closest point in the plot main layer. nullptr if there is not a close point
	 */
	const ofxGPoint* getPointAt(float xScreen, float yScreen) const;

	/**
	 * @brief Returns the closest point in the specified layer to a given screen position
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 * @param layerId the layer id
	 *
	 * @return a pointer to the closest point in the specified layer. nullptr if there is not a close point
	 */
	const ofxGPoint* getPointAt(float xScreen, float yScreen, const string& layerId) const;

	/**
	 * @brief Adds a point to the main layer at a given screen position
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 */
	void addPointAt(float xScreen, float yScreen);

	/**
	 * @brief Adds a point to the specified layer at a given screen position
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 * @param layerId the layer id
	 */
	void addPointAt(float xScreen, float yScreen, const string& layerId);

	/**
	 * @brief Removes a point from the main layer at a given screen position
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 */
	void removePointAt(float xScreen, float yScreen);

	/**
	 * @brief Removes a point from the specified layer at a given screen position
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 * @param layerId the layer id
	 */
	void removePointAt(float xScreen, float yScreen, const string& layerId);

	/**
	 * @brief Returns the plot value at a given screen position
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 *
	 * @return the plot value
	 */
	array<float, 2> getValueAt(float xScreen, float yScreen) const;

	/**
	 * @brief Returns the relative plot position of a given screen position
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 *
	 * @return the relative position in the plot reference system
	 */
	array<float, 2> getRelativePlotPosAt(float xScreen, float yScreen) const;

	/**
	 * @brief Indicates if a given screen position is inside the main plot area
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 *
	 * @return true if the position is inside the main plot area
	 */
	bool isOverPlot(float xScreen, float yScreen) const;

	/**
	 * @brief Indicates if a given screen position is inside the plot box area
	 *
	 * @param xScreen x screen position
	 * @param yScreen y screen position
	 *
	 * @return true if the position is inside the plot box area
	 */
	bool isOverBox(float xScreen, float yScreen) const;

	/**
	 * @brief Calculates and updates the plot x and y limits
	 */
	void updateLimits();

	/**
	 * @brief Calculates the x limits of a given set of points, considering the plot properties (axis log scale, if the
	 * other axis limits are fixed, etc)
	 *
	 * @param points the points for which we want to calculate the x limits
	 *
	 * @return the x limits. Null if none of the points satisfies the plot properties
	 */
	array<float, 2> calculatePointsXLim(const vector<ofxGPoint>& points);

	/**
	 * @brief Calculates the y limits of a given set of points, considering the plot properties (axis log scale, if the
	 * other axis limits are fixed, etc)
	 *
	 * @param points the points for which we want to calculate the y limSits
	 *
	 * @return the y limits. Null if none of the points satisfies the plot properties
	 */
	array<float, 2> calculatePointsYLim(const vector<ofxGPoint>& points);

	/**
	 * @brief Moves the horizontal axes limits by a given amount specified in pixel units
	 *
	 * @param delta pixels to move
	 */
	void moveHorizontalAxesLim(float delta);

	/**
	 * @brief Moves the vertical axes limits by a given amount specified in pixel units
	 *
	 * @param delta pixels to move
	 */
	void moveVerticalAxesLim(float delta);

	/**
	 * @brief Centers the plot coordinates on the specified (x, y) point and zooms the limits range by a given factor
	 *
	 * @param factor the plot limits will be zoomed by this factor
	 * @param xValue the x plot value
	 * @param yValue the y plot value
	 */
	void centerAndZoom(float factor, float xValue, float yValue);

	/**
	 * @brief Zooms the limits range by a given factor
	 *
	 * @param factor the plot limits will be zoomed by this factor
	 */
	void zoom(float factor);

	/**
	 * @brief Zooms the limits range by a given factor keeping the same plot value at the specified screen position
	 *
	 * @param factor the plot limits will be zoomed by this factor
	 * @param xScreen x screen position in the parent Processing applet
	 * @param yScreen y screen position in the parent Processing applet
	 */
	void zoom(float factor, float xScreen, float yScreen);

	/**
	 * @brief Shifts the plot coordinates in a way that after that the given plot value will be at the specified screen
	 * position
	 *
	 * @param xValue the x plot value
	 * @param yValue the y plot value
	 * @param xScreen x screen position in the parent Processing applet
	 * @param yScreen y screen position in the parent Processing applet
	 */
	void align(float xValue, float yValue, float xScreen, float yScreen);

	/**
	 * @brief Shifts the plot coordinates in a way that after that the given plot value will be at the specified screen
	 * position
	 *
	 * @param value the x and y plot values
	 * @param xScreen x screen position in the parent Processing applet
	 * @param yScreen y screen position in the parent Processing applet
	 */
	void align(const array<float, 2>& value, float xScreen, float yScreen);

	/**
	 * @brief Centers the plot coordinates at the plot value that is at the specified screen position
	 *
	 * @param xScreen x screen position in the parent Processing applet
	 * @param yScreen y screen position in the parent Processing applet
	 */
	void center(float xScreen, float yScreen);

	/**
	 * @brief Initializes the histograms in all the plot layers
	 *
	 * @param histType the type of histogram to use. It can be GPlot.VERTICAL or GPlot.HORIZONTAL
	 */
	void startHistograms(ofxGHistogramType histType);

	/**
	 * @brief Stops updating and displaying the histograms in all the plot layers
	 */
	void stopHistograms();

	/**
	 * @brief Draws the plot on the screen with default parameters
	 */
	void defaultDraw();

	/**
	 * @brief Prepares the environment to start drawing the different plot components (points, axes, title, etc). Use
	 * endDraw() to return the sketch to its original state
	 */
	void beginDraw() const;

	/**
	 * @brief Returns the sketch to the state that it had before calling beginDraw()
	 */
	void endDraw() const;

	/**
	 * @brief Draws the plot background. This includes the box area and the margins
	 */
	void drawBackground() const;

	/**
	 * @brief Draws the box area. This doesn't include the plot margins
	 */
	void drawBox() const;

	/**
	 * @brief Draws the x axis
	 */
	void drawXAxis() const;

	/**
	 * @brief Draws the top axis
	 */
	void drawTopAxis() const;

	/**
	 * @brief Draws the y axis
	 */
	void drawYAxis() const;

	/**
	 * @brief Draws the right axis
	 */
	void drawRightAxis() const;

	/**
	 * @brief Draws the title
	 */
	void drawTitle() const;

	/**
	 * @brief Draws the points from all layers in the plot
	 */
	void drawPoints() const;

	/**
	 * @brief Draws the points from all layers in the plot
	 *
	 * @param pointShape the shape that should be used to represent the points
	 */
	void drawPoints(ofPath& pointShape) const;

	/**
	 * @brief Draws the points from all layers in the plot
	 *
	 * @param pointImg the image that should be used to represent the points
	 */
	void drawPoints(const ofImage& pointImg) const;

	/**
	 * @brief Draws a point in the plot
	 *
	 * @param point the point to draw
	 * @param pointColor color to use
	 * @param pointSize point size in pixels
	 */
	void drawPoint(const ofxGPoint& point, const ofColor& pointColor, float pointSize) const;

	/**
	 * @brief Draws a point in the plot
	 *
	 * @param point the point to draw
	 */
	void drawPoint(const ofxGPoint& point) const;

	/**
	 * @brief Draws a point in the plot
	 *
	 * @param point the point to draw
	 * @param pointShape the shape that should be used to represent the point
	 */
	void drawPoint(const ofxGPoint& point, const ofPath& pointShape) const;

	/**
	 * @brief Draws a point in the plot
	 *
	 * @param point the point to draw
	 * @param pointShape the shape that should be used to represent the points
	 * @param pointColor color to use
	 */
	void drawPoint(const ofxGPoint& point, ofPath& pointShape, const ofColor& pointColor) const;

	/**
	 * @brief Draws a point in the plot
	 *
	 * @param point the point to draw
	 * @param pointImg the image that should be used to represent the point
	 */
	void drawPoint(const ofxGPoint& point, const ofImage& pointImg) const;

	/**
	 * @brief Draws lines connecting the points from all layers in the plot
	 */
	void drawLines();

	/**
	 * @brief Draws a line in the plot, defined by two extreme points
	 *
	 * @param point1 first point
	 * @param point2 second point
	 * @param lineColor line color
	 * @param lineWidth line width
	 */
	void drawLine(const ofxGPoint& point1, const ofxGPoint& point2, const ofColor& lineColor, float lineWidth);

	/**
	 * @brief Draws a line in the plot, defined by two extreme points
	 *
	 * @param point1 first point
	 * @param point2 second point
	 */
	void drawLine(const ofxGPoint& point1, const ofxGPoint& point2);

	/**
	 * @brief Draws a line in the plot, defined by the slope and the cut in the y axis
	 *
	 * @param slope the line slope
	 * @param yCut the line y axis cut
	 * @param lineColor line color
	 * @param lineWidth line width
	 */
	void drawLine(float slope, float yCut, const ofColor& lineColor, float lineWidth);

	/**
	 * @brief Draws a line in the plot, defined by the slope and the cut in the y axis
	 *
	 * @param slope the line slope
	 * @param yCut the line y axis cut
	 */
	void drawLine(float slope, float yCut);

	/**
	 * @brief Draws an horizontal line in the plot
	 *
	 * @param value line horizontal value
	 * @param lineColor line color
	 * @param lineWidth line width
	 */
	void drawHorizontalLine(float value, const ofColor& lineColor, float lineWidth) const;

	/**
	 * @brief Draws an horizontal line in the plot
	 *
	 * @param value line horizontal value
	 */
	void drawHorizontalLine(float value) const;

	/**
	 * @brief Draws a vertical line in the plot
	 *
	 * @param value line vertical value
	 * @param lineColor line color
	 * @param lineWidth line width
	 */
	void drawVerticalLine(float value, const ofColor& lineColor, float lineWidth) const;

	/**
	 * @brief Draws a vertical line in the plot
	 *
	 * @param value line vertical value
	 */
	void drawVerticalLine(float value) const;

	/**
	 * @brief Draws filled contours connecting the points from all layers in the plot and a reference value
	 *
	 * @param contourType the type of contours to use. It can be GPlot.VERTICAL or GPlot.HORIZONTAL
	 * @param referenceValue the reference value to use to close the contour
	 */
	void drawFilledContours(ofxGContourType contourType, float referenceValue);

	/**
	 * @brief Draws the label of a given point
	 *
	 * @param point the point
	 */
	void drawLabel(const ofxGPoint& point) const;

	/**
	 * @brief Draws the labels of the points in the layers that are close to a given screen position
	 *
	 * @param xScreen x screen position in the parent Processing applet
	 * @param yScreen y screen position in the parent Processing applet
	 */
	void drawLabelsAt(float xScreen, float yScreen) const;

	/**
	 * @brief Draws the labels of the points in the layers that are close to the mouse position. In order to work, you
	 * need to activate first the points labeling with the command plot.activatePointLabels()
	 */
	void drawLabels() const;

	/**
	 * @brief Draws lines connecting the horizontal and vertical axis ticks
	 *
	 * @param gridType the type of grid to use. It could be GPlot.HORIZONTAL, GPlot.VERTICAL or GPlot.BOTH
	 */
	void drawGridLines(ofxGDirection gridType) const;

	/**
	 * @brief Draws the histograms of all layers
	 */
	void drawHistograms();

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
	 * @param verAlign text vertical alignment. It can be GRAFICA_CENTER_ALIGN, GRAFICA_TOP_ALIGN, GRAFICA_BOTTOM_ALIGN
	 */
	void drawAnnotation(const string& text, float x, float y, ofxGTextAlignment verAlign) const;

	/**
	 * @brief Draws a legend at the specified relative position
	 *
	 * @param text the text to use for each layer in the plot
	 * @param xRelativePos the plot x relative position for each layer in the plot
	 * @param yRelativePos the plot y relative position for each layer in the plot
	 */
	void drawLegend(const vector<string>& text, const vector<float>& xRelativePos,
			const vector<float>& yRelativePos) const;

	/**
	 * @brief Sets the plot position
	 *
	 * @param x the new plot x position on the screen
	 * @param y the new plot y position on the screen
	 */
	void setPos(float x, float y);

	/**
	 * @brief Sets the plot position
	 *
	 * @param newPos the new plot (x, y) position
	 */
	void setPos(const array<float, 2>& newPos);

	/**
	 * @brief Sets the plot outer dimensions
	 *
	 * @param xOuterDim the new plot x outer dimension
	 * @param yOuterDim the new plot y outer dimension
	 */
	void setOuterDim(float xOuterDim, float yOuterDim);

	/**
	 * @brief Sets the plot outer dimensions
	 *
	 * @param newOuterDim the new plot outer dimensions
	 */
	void setOuterDim(const array<float, 2>& newOuterDim);

	/**
	 * @brief Sets the plot margins
	 *
	 * @param bottomMargin the new plot bottom margin
	 * @param leftMargin the new plot left margin
	 * @param topMargin the new plot top margin
	 * @param rightMargin the new plot right margin
	 */
	void setMar(float bottomMargin, float leftMargin, float topMargin, float rightMargin);

	/**
	 * @brief Sets the plot margins
	 *
	 * @param newMar the new plot margins
	 */
	void setMar(const array<float, 4>& newMar);

	/**
	 * @brief Sets the plot box dimensions
	 *
	 * @param xDim the new plot box x dimension
	 * @param yDim the new plot box y dimension
	 */
	void setDim(float xDim, float yDim);

	/**
	 * @brief Sets the plot box dimensions
	 *
	 * @param newDim the new plot box dimensions
	 */
	void setDim(const array<float, 2>& newDim);

	/**
	 * @brief Sets the horizontal axes limits
	 *
	 * @param lowerLim the new axes lower limit
	 * @param upperLim the new axes upper limit
	 */
	void setXLim(float lowerLim, float upperLim);

	/**
	 * @brief Sets the horizontal axes limits
	 *
	 * @param newXLim the new horizontal axes limits
	 */
	void setXLim(const array<float, 2>& newXLim);

	/**
	 * @brief Sets the vertical axes limits
	 *
	 * @param lowerLim the new axes lower limit
	 * @param upperLim the new axes upper limit
	 */
	void setYLim(float lowerLim, float upperLim);

	/**
	 * @brief Sets the vertical axes limits
	 *
	 * @param newYLim the new vertical axes limits
	 */
	void setYLim(const array<float, 2>& newYLim);

	/**
	 * @brief Sets if the horizontal axes limits are fixed or not
	 *
	 * @param newFixedXLim the fixed condition for the horizontal axes
	 */
	void setFixedXLim(bool newFixedXLim);

	/**
	 * @brief Sets if the vertical axes limits are fixed or not
	 *
	 * @param newFixedYLim the fixed condition for the vertical axes
	 */
	void setFixedYLim(bool newFixedYLim);

	/**
	 * @brief Sets if the scale for the horizontal and vertical axes is logarithmic or not
	 *
	 * @param logType the type of scale for the horizontal and vertical axes
	 */
	void setLogScale(const string& logType);

	/**
	 * @brief Sets if the scale of the horizontal axes should be inverted or not
	 *
	 * @param newInvertedXScale true if the horizontal scale should be inverted
	 */
	void setInvertedXScale(bool newInvertedXScale);

	/**
	 * @brief Inverts the horizontal axes scale
	 */
	void invertXScale();

	/**
	 * @brief Sets if the scale of the vertical axes should be inverted or not
	 *
	 * @param newInvertedYScale true if the vertical scale should be inverted
	 */
	void setInvertedYScale(bool newInvertedYScale);

	/**
	 * @brief Inverts the vertical axes scale
	 */
	void invertYScale();

	/**
	 * @brief Sets if all the plot layers should be considered in the axes limits calculation
	 *
	 * @param includeAllLayers true if all layers should be considered and not only the main layer
	 */
	void setIncludeAllLayersInLim(bool includeAllLayers);

	/**
	 * @brief Sets the factor that is used to expand the axes limits
	 *
	 * @param expandFactor the new expansion factor
	 */
	void setExpandLimFactor(float expandFactor);

	/**
	 * @brief Sets the plot background color
	 *
	 * @param newBgColor the new plot background color
	 */
	void setBgColor(const ofColor& newBgColor);

	/**
	 * @brief Sets the box background color
	 *
	 * @param newBoxBgColor the new box background color
	 */
	void setBoxBgColor(const ofColor& newBoxBgColor);

	/**
	 * @brief Sets the box line color
	 *
	 * @param newBoxLineColor the new box background color
	 */
	void setBoxLineColor(const ofColor& newBoxLineColor);

	/**
	 * @brief Sets the box line width
	 *
	 * @param newBoxLineWidth the new box line width
	 */
	void setBoxLineWidth(float newBoxLineWidth);

	/**
	 * @brief Sets the grid line color
	 *
	 * @param newGridLineColor the new grid line color
	 */
	void setGridLineColor(const ofColor& newGridLineColor);

	/**
	 * @brief Sets the grid line width
	 *
	 * @param newGridLineWidth the new grid line width
	 */
	void setGridLineWidth(float newGridLineWidth);

	/**
	 * @brief Sets the points for the main layer
	 *
	 * @param points the new points for the main layer
	 */
	void setPoints(const vector<ofxGPoint>& points);

	/**
	 * @brief Sets the points for the specified layer
	 *
	 * @param points the new points for the main layer
	 * @param layerId the layer id
	 */
	void setPoints(const vector<ofxGPoint>& points, const string& layerId);

	/**
	 * @brief Sets one of the main layer points
	 *
	 * @param index the point position
	 * @param x the point new x coordinate
	 * @param y the point new y coordinate
	 * @param label the point new label
	 */
	void setPoint(int index, float x, float y, const string& label);

	/**
	 * @brief Sets one of the specified layer points
	 *
	 * @param index the point position
	 * @param x the point new x coordinate
	 * @param y the point new y coordinate
	 * @param label the point new label
	 * @param layerId the layer id
	 */
	void setPoint(int index, float x, float y, const string& label, const string& layerId);

	/**
	 * @brief Sets one of the main layer points
	 *
	 * @param index the point position
	 * @param x the point new x coordinate
	 * @param y the point new y coordinate
	 */
	void setPoint(int index, float x, float y);

	/**
	 * @brief Sets one of the main layer points
	 *
	 * @param index the point position
	 * @param newPoint the new point
	 */
	void setPoint(int index, const ofxGPoint& newPoint);

	/**
	 * @brief Sets one of the specified layer points
	 *
	 * @param index the point position
	 * @param newPoint the new point
	 * @param layerId the layer id
	 */
	void setPoint(int index, const ofxGPoint& newPoint, const string& layerId);

	/**
	 * @brief Adds a new point to the main layer points
	 *
	 * @param x the new point x coordinate
	 * @param y the new point y coordinate
	 * @param label the new point label
	 */
	void addPoint(float x, float y, const string& label);

	/**
	 * @brief Adds a new point to the specified layer points
	 *
	 * @param x the new point x coordinate
	 * @param y the new point y coordinate
	 * @param label the new point label
	 * @param layerId the layer id
	 */
	void addPoint(float x, float y, const string& label, const string& layerId);

	/**
	 * @brief Adds a new point to the main layer points
	 *
	 * @param x the new point x coordinate
	 * @param y the new point y coordinate
	 */
	void addPoint(float x, float y);

	/**
	 * @brief Adds a new point to the main layer points
	 *
	 * @param newPoint the point to add
	 */
	void addPoint(const ofxGPoint& newPoint);

	/**
	 * @brief Adds a new point to the specified layer points
	 *
	 * @param newPoint the point to add
	 * @param layerId the layer id
	 */
	void addPoint(const ofxGPoint& newPoint, const string& layerId);

	/**
	 * @brief Adds a new point to the main layer points
	 *
	 * @param index the position to add the point
	 * @param x the new point x coordinate
	 * @param y the new point y coordinate
	 * @param label the new point label
	 */
	void addPoint(int index, float x, float y, const string& label);

	/**
	 * @brief Adds a new point to the specified layer points
	 *
	 * @param index the position to add the point
	 * @param x the new point x coordinate
	 * @param y the new point y coordinate
	 * @param label the new point label
	 * @param layerId the layer id
	 */
	void addPoint(int index, float x, float y, const string& label, const string& layerId);

	/**
	 * @brief Adds a new point to the main layer points
	 *
	 * @param index the position to add the point
	 * @param x the new point x coordinate
	 * @param y the new point y coordinate
	 */
	void addPoint(int index, float x, float y);

	/**
	 * @brief Adds a new point to the main layer points
	 *
	 * @param index the position to add the point
	 * @param newPoint the point to add
	 */
	void addPoint(int index, const ofxGPoint& newPoint);

	/**
	 * @brief Adds a new point to the specified layer points
	 *
	 * @param index the position to add the point
	 * @param newPoint the point to add
	 * @param layerId the layer id
	 */
	void addPoint(int index, const ofxGPoint& newPoint, const string& layerId);

	/**
	 * @brief Adds new points to the main layer points
	 *
	 * @param newPoints the points to add
	 */
	void addPoints(const vector<ofxGPoint>& newPoints);

	/**
	 * @brief Adds new points to the specified layer points
	 *
	 * @param newPoints the points to add
	 * @param layerId the layer id
	 */
	void addPoints(const vector<ofxGPoint>& newPoints, const string& layerId);

	/**
	 * @brief Removes one of the main layer points
	 *
	 * @param index the point position
	 */
	void removePoint(int index);

	/**
	 * @brief Removes one of the specified layer points
	 *
	 * @param index the point position
	 * @param layerId the layer id
	 */
	void removePoint(int index, const string& layerId);

	/**
	 * @brief Sets the point colors for the main layer
	 *
	 * @param pointColors the point colors for the main layer
	 */
	void setPointColors(const vector<ofColor>& pointColors);

	/**
	 * @brief Sets the point color for the main layer
	 *
	 * @param pointColor the point color for the main layer
	 */
	void setPointColor(const ofColor& pointColor);

	/**
	 * @brief Sets the point sizes for the main layer
	 *
	 * @param pointSizes the point sizes for the main layer
	 */
	void setPointSizes(const vector<float>& pointSizes);

	/**
	 * @brief Sets the point size for the main layer
	 *
	 * @param pointSize the point sizes for the main layer
	 */
	void setPointSize(float pointSize);

	/**
	 * @brief Sets the line color for the main layer
	 *
	 * @param lineColor the line color for the main layer
	 */
	void setLineColor(const ofColor& lineColor);

	/**
	 * @brief Sets the line width for the main layer
	 *
	 * @param lineWidth the line with for the main layer
	 */
	void setLineWidth(float lineWidth);

	/**
	 * @brief Sets the base point for the histogram in the main layer
	 *
	 * @param basePoint the base point for the histogram in the main layer
	 */
	void setHistBasePoint(const ofxGPoint& basePoint);

	/**
	 * @brief Sets the histogram type for the histogram in the main layer
	 *
	 * @param histType the histogram type for the histogram in the main layer. It can be GRAFICA_HORIZONTAL_HISTOGRAM
	 * or GRAFICA_VERTICAL_HISTOGRAM
	 */
	void setHistType(ofxGHistogramType histType);

	/**
	 * @brief Sets if the histogram in the main layer is visible or not
	 *
	 * @param visible if true, the histogram is visible
	 */
	void setHistVisible(bool visible);

	/**
	 * @brief Sets if the labels of the histogram in the main layer will be drawn or not
	 *
	 * @param drawHistLabels if true, the histogram labels will be drawn
	 */
	void setDrawHistLabels(bool drawHistLabels);

	/**
	 * @brief Sets the label background color of the points in the main layer
	 *
	 * @param labelBgColor the label background color of the points in the main layer
	 */
	void setLabelBgColor(const ofColor& labelBgColor);

	/**
	 * @brief Sets the label separation of the points in the main layer
	 *
	 * @param labelSeparation the label separation of the points in the main layer
	 */
	void setLabelSeparation(const array<float, 2>& labelSeparation);

	/**
	 * @brief Set the plot title text
	 *
	 * @param text the plot title text
	 */
	void setTitleText(const string& text);

	/**
	 * @brief Sets the axis offset for all the axes in the plot
	 *
	 * @param offset the new axis offset
	 */
	void setAxesOffset(float offset);

	/**
	 * @brief Sets the tick length for all the axes in the plot
	 *
	 * @param tickLength the new tick length
	 */
	void setTicksLength(float tickLength);

	/**
	 * @brief Sets the approximate number of ticks in the horizontal axes. The actual number of ticks depends on the
	 * axes limits and the axes scale
	 *
	 * @param nTicks the new approximate number of ticks in the horizontal axes
	 */
	void setHorizontalAxesNTicks(int nTicks);

	/**
	 * @brief Sets the separation between the ticks in the horizontal axes
	 *
	 * @param ticksSeparation the new ticks separation in the horizontal axes
	 */
	void setHorizontalAxesTicksSeparation(float ticksSeparation);

	/**
	 * @brief Sets the horizontal axes ticks
	 *
	 * @param ticks the new horizontal axes ticks
	 */
	void setHorizontalAxesTicks(const vector<float>& ticks);

	/**
	 * @brief Sets the approximate number of ticks in the vertical axes. The actual number of ticks depends on the axes
	 * limits and the axes scale
	 *
	 * @param nTicks the new approximate number of ticks in the vertical axes
	 */
	void setVerticalAxesNTicks(int nTicks);

	/**
	 * @brief Sets the separation between the ticks in the vertical axes
	 *
	 * @param ticksSeparation the new ticks separation in the vertical axes
	 */
	void setVerticalAxesTicksSeparation(float ticksSeparation);

	/**
	 * @brief Sets the vertical axes ticks
	 *
	 * @param ticks the new vertical axes ticks
	 */
	void setVerticalAxesTicks(const vector<float>& ticks);

	/**
	 * @brief Sets the name of the font that is used in the main layer
	 *
	 * @param fontName the name of the font that will be used in the main layer
	 */
	void setFontName(const string& fontName);

	/**
	 * @brief Sets the color of the font that is used in the main layer
	 *
	 * @param fontColor the color of the font that will be used in the main layer
	 */
	void setFontColor(const ofColor& fontColor);

	/**
	 * @brief Sets the size of the font that is used in the main layer
	 *
	 * @param fontSize the size of the font that will be used in the main layer
	 */
	void setFontSize(int fontSize);

	/**
	 * @brief Sets the properties of the font that is used in the main layer
	 *
	 * @param fontName the name of the font that will be used in the main layer
	 * @param fontColor the color of the font that will be used in the main layer
	 * @param fontSize the size of the font that will be used in the main layer
	 */
	void setFontProperties(const string& fontName, const ofColor& fontColor, int fontSize);

	/**
	 * @brief Sets the properties of the font that will be used in all plot elements (layer, axes, title, histogram)
	 *
	 * @param fontName the name of the font that will be used in all plot elements
	 * @param fontColor the color of the font that will be used in all plot elements
	 * @param fontSize the size of the font that will be used in all plot elements
	 */
	void setAllFontProperties(const string& fontName, const ofColor& fontColor, int fontSize);

	/**
	 * @brief Sets if the fonts should be made of contours
	 *
	 * @param newFontMakeContours true if the fonts should be made of contours
	 */
	void setFontsMakeContours(bool newFontMakeContours);

	/**
	 * @brief Returns the plot position
	 *
	 * @return the plot position
	 */
	array<float, 2> getPos() const;

	/**
	 * @brief Returns the plot outer dimensions
	 *
	 * @return the plot outer dimensions
	 */
	array<float, 2> getOuterDim() const;

	/**
	 * @brief Returns the plot margins
	 *
	 * @return the plot margins
	 */
	array<float, 4> getMar() const;

	/**
	 * @brief Returns the box dimensions
	 *
	 * @return the box dimensions
	 */
	array<float, 2> getDim() const;

	/**
	 * @brief Returns the limits of the horizontal axes
	 *
	 * @return the limits of the horizontal axes
	 */
	array<float, 2> getXLim() const;

	/**
	 * @brief Returns the limits of the vertical axes
	 *
	 * @return the limits of the vertical axes
	 */
	array<float, 2> getYLim() const;

	/**
	 * @brief Returns true if the horizontal axes limits are fixed
	 *
	 * @return true, if the horizontal axes limits are fixed
	 */
	bool getFixedXLim() const;

	/**
	 * @brief Returns true if the vertical axes limits are fixed
	 *
	 * @return true, if the vertical axes limits are fixed
	 */
	bool getFixedYLim() const;

	/**
	 * @brief Returns true if the horizontal axes scale is logarithmic
	 *
	 * @return true, if the horizontal axes scale is logarithmic
	 */
	bool getXLog() const;

	/**
	 * @brief Returns true if the vertical axes scale is logarithmic
	 *
	 * @return true, if the vertical axes scale is logarithmic
	 */
	bool getYLog() const;

	/**
	 * @brief Returns true if the horizontal axes limits are inverted
	 *
	 * @return true, if the horizontal axes limits are inverted
	 */
	bool getInvertedXScale();

	/**
	 * @brief Returns true if the vertical axes limits are inverted
	 *
	 * @return true, if the vertical axes limits are inverted
	 */
	bool getInvertedYScale();

	/**
	 * @brief Returns the plot main layer
	 *
	 * @return the plot main layer
	 */
	ofxGLayer& getMainLayer();

	/**
	 * @brief Returns a layer with an specific id
	 *
	 * @param layerId the id of the layer to return
	 *
	 * @return the layer with the specified id
	 */
	ofxGLayer& getLayer(const string& layerId);

	/**
	 * @brief Returns the plot x axis
	 *
	 * @return the plot x axis
	 */
	ofxGAxis& getXAxis();

	/**
	 * @brief Returns the plot top axis
	 *
	 * @return the plot top axis
	 */
	ofxGAxis& getTopAxis();

	/**
	 * @brief Returns the plot y axis
	 *
	 * @return the plot y axis
	 */
	ofxGAxis& getYAxis();

	/**
	 * @brief Returns the plot right axis
	 *
	 * @return the plot right axis
	 */
	ofxGAxis& getRightAxis();

	/**
	 * @brief Returns the plot title
	 *
	 * @return the plot title
	 */
	ofxGTitle& getTitle();

	/**
	 * @brief Returns a copy of the points of the main layer
	 *
	 * @return a copy of the points of the main layer
	 */
	vector<ofxGPoint> getPoints() const;

	/**
	 * @brief Returns a copy of the points of the specified layer
	 *
	 * @param layerId the layer id
	 *
	 * @return a copy of the points of the specified layer
	 */
	vector<ofxGPoint> getPoints(const string& layerId) const;

	/**
	 * @brief Returns the points of the main layer
	 *
	 * @return the points of the main layer
	 */
	const vector<ofxGPoint>& getPointsRef() const;

	/**
	 * @brief Returns the points of the specified layer
	 *
	 * @param layerId the layer id
	 *
	 * @return the points of the specified layer
	 */
	const vector<ofxGPoint>& getPointsRef(const string& layerId) const;

	/**
	 * @brief Returns the histogram of the main layer
	 *
	 * @return the histogram of the main layer
	 */
	ofxGHistogram& getHistogram();

	/**
	 * @brief Returns the histogram of the specified layer
	 *
	 * @param layerId the layer id
	 *
	 * @return the histogram of the specified layer
	 */
	ofxGHistogram& getHistogram(const string& layerId);

	/**
	 * @brief Activates the option to zoom with the mouse using the specified buttons and the specified key modifiers
	 *
	 * @param factor the zoom factor to increase or decrease with each mouse click
	 * @param increaseButton the mouse button to increase the zoom. It could be OF_MOUSE_BUTTON_LEFT,
	 * OF_MOUSE_BUTTON_RIGHT or OF_MOUSE_BUTTON_MIDDLE. Select OF_MOUSE_BUTTON_LEFT to use the mouse wheel
	 * @param decreaseButton the mouse button to decrease the zoom. It could be OF_MOUSE_BUTTON_LEFT,
	 * OF_MOUSE_BUTTON_RIGHT or OF_MOUSE_BUTTON_MIDDLE. Select OF_MOUSE_BUTTON_LEFT to use the mouse wheel
	 * @param increaseKeyModifier the key modifier to use in conjunction with the increase zoom mouse button. It could
	 * be OF_KEY_LEFT_SHIFT, OF_KEY_LEFT_CONTROL, OF_KEY_LEFT_ALT, etc, or ofxGPlot::NONE if no key is needed
	 * @param decreaseKeyModifier the key modifier to use in conjunction with the decrease zoom mouse button. It could
	 * be OF_KEY_LEFT_SHIFT, OF_KEY_LEFT_CONTROL, OF_KEY_LEFT_ALT, etc, or ofxGPlot::NONE if no key is needed
	 */
	void activateZooming(float factor, int increaseButton, int decreaseButton, int increaseKeyModifier,
			int decreaseKeyModifier);

	/**
	 * @brief Activates the option to zoom with the mouse using the specified buttons
	 *
	 * @param factor the zoom factor to increase or decrease with each mouse click
	 * @param increaseButton the mouse button to increase the zoom. It could be OF_MOUSE_BUTTON_LEFT,
	 * OF_MOUSE_BUTTON_RIGHT or OF_MOUSE_BUTTON_MIDDLE. Select OF_MOUSE_BUTTON_LEFT to use the mouse wheel
	 * @param decreaseButton the mouse button to decrease the zoom. It could be OF_MOUSE_BUTTON_LEFT,
	 * OF_MOUSE_BUTTON_RIGHT or OF_MOUSE_BUTTON_MIDDLE. Select OF_MOUSE_BUTTON_LEFT to use the mouse wheel
	 */
	void activateZooming(float factor, int increaseButton, int decreaseButton);

	/**
	 * @brief Activates the option to zoom with the mouse using the LEFT and RIGHT buttons
	 *
	 * @param factor the zoom factor to increase or decrease with each mouse click
	 */
	void activateZooming(float factor);

	/**
	 * @brief Activates the option to zoom with the mouse using the LEFT and RIGHT buttons
	 */
	void activateZooming();

	/**
	 * @brief Deactivates the option to zoom with the mouse
	 */
	void deactivateZooming();

	/**
	 * @brief Activates the option to center the plot with the mouse using the specified button and the specified key modifier
	 *
	 * @param button the mouse button to use. It could be OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_RIGHT or
	 * OF_MOUSE_BUTTON_MIDDLE. Select OF_MOUSE_BUTTON_LEFT to use the mouse wheel
	 * @param keyModifier the key modifier to use in conjunction with the mouse button. It could be OF_KEY_LEFT_SHIFT,
	 * OF_KEY_LEFT_CONTROL, OF_KEY_LEFT_ALT, etc, or ofxGPlot::NONE if no key is needed
	 */
	void activateCentering(int button, int keyModifier);

	/**
	 * @brief Activates the option to center the plot with the mouse using the specified button
	 *
	 * @param button the mouse button to use. It could be OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_RIGHT or
	 * OF_MOUSE_BUTTON_MIDDLE. Select OF_MOUSE_BUTTON_LEFT to use the mouse wheel
	 */
	void activateCentering(int button);

	/**
	 * @brief Activates the option to center the plot with the mouse using the LEFT button
	 */
	void activateCentering();

	/**
	 * @brief Deactivates the option to center the plot with the mouse
	 */
	void deactivateCentering();

	/**
	 * @brief Activates the option to pan the plot with the mouse using the specified button and the specified key modifier
	 *
	 * @param button the mouse button to use. It could be OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_RIGHT or
	 * OF_MOUSE_BUTTON_MIDDLE
	 * @param keyModifier the key modifier to use in conjunction with the mouse button. It could be OF_KEY_LEFT_SHIFT,
	 * OF_KEY_LEFT_CONTROL, OF_KEY_LEFT_ALT, etc, or ofxGPlot::NONE if no key is needed
	 */
	void activatePanning(int button, int keyModifier);

	/**
	 * @brief Activates the option to pan the plot with the mouse using the specified button
	 *
	 * @param button the mouse button to use. It could be OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_RIGHT or
	 * OF_MOUSE_BUTTON_MIDDLE
	 */
	void activatePanning(int button);

	/**
	 * @brief Activates the option to pan the plot with the mouse using the LEFT button
	 */
	void activatePanning();

	/**
	 * @brief Deactivates the option to pan the plot with the mouse
	 */
	void deactivatePanning();

	/**
	 * @brief Activates the option to draw the labels of the points with the mouse using the specified button and the specified
	 * key modifier
	 *
	 * @param button the mouse button to use. It could be OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_RIGHT or
	 * OF_MOUSE_BUTTON_MIDDLE
	 * @param keyModifier the key modifier to use in conjunction with the mouse button. It could be OF_KEY_LEFT_SHIFT,
	 * OF_KEY_LEFT_CONTROL, OF_KEY_LEFT_ALT, etc, or ofxGPlot::NONE if no key is needed
	 */
	void activatePointLabels(int button, int keyModifier);

	/**
	 * @brief Activates the option to draw the labels of the points with the mouse using the specified button
	 *
	 * @param button the mouse button to use. It could be OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_RIGHT or
	 * OF_MOUSE_BUTTON_MIDDLE
	 */
	void activatePointLabels(int button);

	/**
	 * @brief Activates the option to draw the labels of the points with the mouse using the LEFT button
	 */
	void activatePointLabels();

	/**
	 * @brief Deactivates the option to draw the labels of the points with the mouse
	 */
	void deactivatePointLabels();

	/**
	 * @brief Activates the option to return to the value of the axes limits previous to any mouse interaction, using the
	 * specified button and the specified key modifier
	 *
	 * @param button the mouse button to use. It could be OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_RIGHT or
	 * OF_MOUSE_BUTTON_MIDDLE. Select OF_MOUSE_BUTTON_LEFT to use the mouse wheel
	 * @param keyModifier the key modifier to use in conjunction with the mouse button. It could be OF_KEY_LEFT_SHIFT,
	 * OF_KEY_LEFT_CONTROL, OF_KEY_LEFT_ALT, etc, or ofxGPlot::NONE if no key is needed
	 */
	void activateReset(int button, int keyModifier);

	/**
	 * @brief Activates the option to return to the value of the axes limits previous to any mouse interaction, using the
	 * specified button
	 *
	 * @param button the mouse button to use. It could be OF_MOUSE_BUTTON_LEFT, OF_MOUSE_BUTTON_RIGHT or
	 * OF_MOUSE_BUTTON_MIDDLE. Select OF_MOUSE_BUTTON_LEFT to use the mouse wheel
	 */
	void activateReset(int button);

	/**
	 * @brief Activates the option to return to the value of the axes limits previous to any mouse interaction, using the RIGHT
	 * button
	 */
	void activateReset();

	/**
	 * @brief Deactivates the option to return to the value of the axes limits previous to any mouse interaction using the
	 * mouse
	 */
	void deactivateReset();

protected:

	/**
	 * @brief Calculates the plot x limits
	 *
	 * @return the x limits
	 */
	array<float, 2> calculatePlotXLim();

	/**
	 * @brief Calculates the plot y limits
	 *
	 * @return the y limits
	 */
	array<float, 2> calculatePlotYLim();

	/**
	 * @brief Shifts the plot coordinates in a way that the value at a given plot position will have after that the
	 * specified new plot position
	 *
	 * @param valuePlotPos current plot position of the value
	 * @param newPlotPos new plot position of the value
	 */
	void shiftPlotPos(const array<float, 2>& valuePlotPos, const array<float, 2>& newPlotPos);

	/**
	 * @brief Mouse events (zooming, centering, panning, labeling) handler
	 *
	 * @param args the mouse event arguments
	 */
	void mouseEventHandler(ofMouseEventArgs& args);

	/**
	 * @brief Key events (zooming, centering, panning, labeling) handler
	 *
	 * @param args the key event arguments
	 */
	void keyEventHandler(ofKeyEventArgs& args);

	/**
	 * @brief The plot position
	 */
	array<float, 2> pos;

	/**
	 * @brief The plot outer dimensions
	 */
	array<float, 2> outerDim;

	/**
	 * @brief The plot margins
	 */
	array<float, 4> mar;

	/**
	 * @brief The box dimensions
	 */
	array<float, 2> dim;

	/**
	 * @brief The limits of the horizontal axes
	 */
	array<float, 2> xLim;

	/**
	 * @brief The limits of the vertical axes
	 */
	array<float, 2> yLim;

	/**
	 * @brief Defines if the horizontal axes limits are fixed
	 */
	bool fixedXLim;

	/**
	 * @brief Defines if the vertical axes limits are fixed
	 */
	bool fixedYLim;

	/**
	 * @brief Defines if the horizontal axes scale is logarithmic
	 */
	bool xLog;

	/**
	 * @brief Defines if the vertical axes scale is logarithmic
	 */
	bool yLog;

	/**
	 * @brief Defines if the horizontal axes limits are inverted
	 */
	bool invertedXScale;

	/**
	 * @brief Defines if the vertical axes limits are inverted
	 */
	bool invertedYScale;

	/**
	 * @brief Defines if all the plot layers should be considered in the axes limits calculation
	 */
	bool includeAllLayersInLim;

	/**
	 * @brief The factor that is used to expand the axes limits
	 */
	float expandLimFactor;

	/**
	 * @brief The plot background color
	 */
	ofColor bgColor;

	/**
	 * @brief The box background color
	 */
	ofColor boxBgColor;

	/**
	 * @brief The box line color
	 */
	ofColor boxLineColor;

	/**
	 * @brief The box line width
	 */
	float boxLineWidth;

	/**
	 * @brief The grid line color
	 */
	ofColor gridLineColor;

	/**
	 * @brief The grid line width
	 */
	float gridLineWidth;

	/**
	 * @brief The main layer
	 */
	ofxGLayer mainLayer;

	/**
	 * @brief The secondary layers
	 */
	vector<ofxGLayer> layerList;

	/**
	 * @brief The x axis
	 */
	ofxGAxis xAxis;

	/**
	 * @brief The top axis
	 */
	ofxGAxis topAxis;

	/**
	 * @brief The y axis
	 */
	ofxGAxis yAxis;

	/**
	 * @brief The right axis
	 */
	ofxGAxis rightAxis;

	/**
	 * @brief The title
	 */
	ofxGTitle title;

	/**
	 * @brief Defines if the zooming events are active
	 */
	bool zoomingIsActive;

	/**
	 * @brief The zoom factor
	 */
	float zoomFactor;

	/**
	 * @brief The mouse button to increase the zoom
	 */
	int increaseZoomButton;

	/**
	 * @brief The mouse button to decrease the zoom
	 */
	int decreaseZoomButton;

	/**
	 * @brief The key modifier to use in conjunction with the increase zoom mouse button
	 */
	int increaseZoomKeyModifier;

	/**
	 * @brief The key modifier to use in conjunction with the decrease zoom mouse button
	 */
	int decreaseZoomKeyModifier;

	/**
	 * @brief Defines if the centering events are active
	 */
	bool centeringIsActive;

	/**
	 * @brief The mouse button to center the plot
	 */
	int centeringButton;

	/**
	 * @brief The key modifier to use in conjunction with the centering mouse button
	 */
	int centeringKeyModifier;

	/**
	 * @brief Defines if the panning events are active
	 */
	bool panningIsActive;

	/**
	 * @brief The mouse button to pan the plot
	 */
	int panningButton;

	/**
	 * @brief The key modifier to use in conjunction with the panning mouse button
	 */
	int panningKeyModifier;

	/**
	 * @brief The panning reference point
	 */
	array<float, 2> panningReferencePoint;

	/**
	 * @brief Defines if the panning reference point is set or not
	 */
	bool panningReferencePointIsSet;

	/**
	 * @brief Defines if the labeling events are active
	 */
	bool labelingIsActive;

	/**
	 * @brief The mouse button to label the points
	 */
	int labelingButton;

	/**
	 * @brief The key modifier to use in conjunction with the labeling mouse button
	 */
	int labelingKeyModifier;

	/**
	 * @brief The mouse position to use for the points labeling
	 */
	array<float, 2> mousePos;

	/**
	 * @brief Defines if the labeling mouse position is set or not
	 */
	bool mousePosIsSet;

	/**
	 * @brief Defines if the reset events are active
	 */
	bool resetIsActive;

	/**
	 * @brief The mouse button to reset the plot
	 */
	int resetButton;

	/**
	 * @brief The key modifier to use in conjunction with the reset mouse button
	 */
	int resetKeyModifier;

	/**
	 * @brief The horizontal axes reset limits
	 */
	array<float, 2> xLimReset;

	/**
	 * @brief The vertical axes reset limits
	 */
	array<float, 2> yLimReset;

	/**
	 * @brief Defines if the reset limits are set or not
	 */
	bool resetLimitsAreSet;

	/**
	 * @brief The key that is currently pressed
	 */
	int pressedKey;

	/**
	 * @brief Defines if a key is pressed or not
	 */
	bool keyIsPressed;
};
