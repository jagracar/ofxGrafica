#pragma once

#include "ofMain.h"

/**
 * @brief Point class
 *
 * An ofxGPoint is composed of two coordinates (x, y) and a text label
 *
 * @author Javier Graciá Carpio
 */
class ofxGPoint {
public:

	/**
	 * @brief Default constructor
	 */
	ofxGPoint();

	/**
	 * @brief Constructor
	 *
	 * @param _x the x coordinate
	 * @param _y the y coordinate
	 * @param _label the text label
	 */
	ofxGPoint(float _x, float _y, const string& _label = "");

	/**
	 * @brief Constructor
	 *
	 * @param v the 2d vector containing the point coordinates
	 * @param _label the text label
	 */
	ofxGPoint(const glm::vec2& v, const string& _label = "");

	/**
	 * @brief Sets the point x and y coordinates and the label
	 *
	 * @param newX the new x coordinate
	 * @param newY the new y coordinate
	 * @param newLabel the new point text label
	 */
	void set(float newX, float newY, const string& newLabel);

	/**
	 * @brief Sets the point x and y coordinates and the label
	 *
	 * @param v the 2d vector with the new point coordinates
	 * @param newLabel the new point text label
	 */
	void set(const glm::vec2& v, const string& newLabel);

	/**
	 * @brief Sets the point x and y coordinates and the label
	 *
	 * @param p the point to use as a reference
	 */
	void set(const ofxGPoint& p);

	/**
	 * @brief Sets the point x coordinate
	 *
	 * @param newX the new x coordinate
	 */
	void setX(float newX);

	/**
	 * @brief Sets the point y coordinate
	 *
	 * @param newY the new y coordinate
	 */
	void setY(float newY);

	/**
	 * @brief Sets the point x and y coordinates
	 *
	 * @param newX the new x coordinate
	 * @param newY the new y coordinate
	 */
	void setXY(float newX, float newY);

	/**
	 * @brief Sets the point x and y coordinates
	 *
	 * @param v the 2d vector with the new point coordinates
	 */
	void setXY(const glm::vec2& v);

	/**
	 * @brief Sets the point x and y coordinates
	 *
	 * @param p the point with the new point coordinates
	 */
	void setXY(const ofxGPoint& p);

	/**
	 * @brief Sets the point text label
	 *
	 * @param newLabel the new point text label
	 */
	void setLabel(const string& newLabel);

	/**
	 * @brief Returns the point x coordinate
	 *
	 * @return the point x coordinate
	 */
	float getX() const;

	/**
	 * @brief Returns the point y coordinate
	 *
	 * @return the point y coordinate
	 */
	float getY() const;

	/**
	 * @brief Returns the point text label
	 *
	 * @return the point text label
	 */
	string getLabel() const;

	/**
	 * @brief Returns if the point coordinates are valid or not
	 *
	 * @return true if the point coordinates are valid
	 */
	bool getValid() const;

	/**
	 * @brief Returns if the point coordinates are valid or not
	 *
	 * @return true if the point coordinates are valid
	 */
	bool isValid() const;

protected:

	/**
	 * @brief The point x coordinate
	 */
	float x;

	/**
	 * @brief The point y coordinate
	 */
	float y;

	/**
	 * @brief The point text label
	 */
	string label;

	/**
	 * @brief Indicates if the point coordinates are valid or not
	 */
	bool valid;
};
