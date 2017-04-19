#ifndef SHAPE_HPP_INCLUDED
#define SHAPE_HPP_INCLUDED

#include <string>
#include <cmath>
#include <vector>
#include <memory>
using std::unique_ptr;
using std::make_unique;

class Shape {

public:
	virtual ~Shape() = default;
	virtual std::string generatePostScript() = 0;

	double height;
	double width;
	double x;
	double y;
};

class Circle : public Shape {
public:
	Circle(double radius) {
		height = radius * 2;
		width = radius * 2;
	}
	std::string generatePostScript() override
	{
		std::string circleString = "newpath\n";
		circleString += std::to_string(height / -2);
		circleString += " ";
		circleString += std::to_string(width / -2);
		circleString += " translate\n";
		circleString += std::to_string(height / 2);
		circleString += " ";
		circleString += std::to_string(width / 2);
		circleString += " ";
		circleString += std::to_string(width / 2);
		circleString += " 0 360 arc closepath\n";
		circleString += "stroke\n";

		circleString += std::to_string(height / 2);
		circleString += " ";
		circleString += std::to_string(width / 2);
		circleString += " translate\n";
		return circleString;
	}
};

//Given a number of sides and a length, create a shape with that number of sides
//with given length
class Polygon : public Shape {
public:
	Polygon() = default;
	Polygon(int numSides, double sideLength)
	{
		sideLength_g = sideLength;
		numSides_g = numSides;
		const double pi = 3.141592653589793238;

		if (numSides % 2 == 1)
		{
			height = sideLength*(1 + cos(pi / numSides)) / (2 * sin(pi / numSides));
			width = (sideLength*sin(pi*(numSides - 1) / (2 * numSides)) / (sin(pi / numSides)));
		}
		else if (numSides % 4 != 0) // numSides%2==0
		{
			height = sideLength * (cos(pi / numSides)) / (sin(pi / numSides));
			width = sideLength / (sin(pi / numSides));
		}
		else // numSides%2 == 0 && numSides%4 == 0
		{
			height = sideLength*(cos(pi / numSides)) / (sin(pi / numSides));
			width = height;
		}
	}
	std::string generatePostScript() override
	{

		int totalAngle = (numSides_g - 2) * 180;
		int anglePerSide = 180 - totalAngle / numSides_g;
		std::string sideAngle = std::to_string(anglePerSide);

		std::string myLength = std::to_string(sideLength_g);

		std::string polyString = "";
		polyString += std::to_string(sideLength_g / -2);
		polyString += " ";
		polyString += std::to_string(height / -2);
		polyString += " translate\n";
		polyString += "newpath\n";
		
		polyString += "0 0 moveto\n";

		for (int i = 0; i < numSides_g; i++)
		{
			polyString += myLength + " 0" + " lineto\n";
			polyString += myLength + " 0" + " translate\n";
			polyString += sideAngle + " rotate\n";

		}
		polyString += "closepath\n";
		polyString += "stroke\n";
		polyString += std::to_string(sideLength_g / 2);
		polyString += " ";
		polyString += std::to_string(height / 2);
		polyString += " translate\n";


		return polyString;
	}

private:
	double sideLength_g;
	double numSides_g;
};

class Rectangle : public Shape {
public:
	Rectangle(double w, double h) {
		height = h;
		width = w;
	}
	std::string generatePostScript() override
	{
		halfHeight_ = height / 2.0;
		halfWidth_ = width / 2.0;

		std::string rectangleString = "newpath\n";
		rectangleString += "0";
		rectangleString += " ";
		rectangleString += "0";
		rectangleString += " moveto\n";

		rectangleString += std::to_string(-1 * halfWidth_); //move to draw at origin
		rectangleString += " ";
		rectangleString += std::to_string(-1 * halfHeight_);
		rectangleString += " moveto\n";
		rectangleString += std::to_string(-1 * halfWidth_);
		rectangleString += " ";
		rectangleString += std::to_string(-1 * halfHeight_);
		rectangleString += " translate\n";

		rectangleString += std::to_string(width);
		rectangleString += " ";
		rectangleString += "0";     					// Bottom
		rectangleString += " ";
		rectangleString += "rlineto\n";

		rectangleString += "0";
		rectangleString += " ";
		rectangleString += std::to_string(height);      // Right
		rectangleString += " ";
		rectangleString += "rlineto\n";

		rectangleString += std::to_string(width*-1);
		rectangleString += " ";
		rectangleString += "0";      					// Top
		rectangleString += " ";
		rectangleString += "rlineto\n";

		rectangleString += "closepath\n";               // Left
		rectangleString += "stroke\n";

		rectangleString += std::to_string(halfWidth_);//move back from origin
		rectangleString += " ";
		rectangleString += std::to_string(halfHeight_);
		rectangleString += " moveto\n";
		rectangleString += std::to_string(halfWidth_);
		rectangleString += " ";
		rectangleString += std::to_string(halfHeight_);
		rectangleString += " translate\n";

		return rectangleString;
	}
private:
	double halfHeight_;
	double halfWidth_;
};

class Spacer : public Shape {
public:
	Spacer(double w, double h) {
		width = w;
		height = h;
		x = 0;
		y = 0;
	}
	std::string generatePostScript() override
	{
		std::string SpacerString = "newpath\n";
		SpacerString += std::to_string(x);
		SpacerString += " ";
		SpacerString += std::to_string(y);
		SpacerString += " moveto\n";

		SpacerString += std::to_string(width);
		SpacerString += " ";
		SpacerString += std::to_string(x);
		SpacerString += " rlineto\n";

		SpacerString += std::to_string(y);
		SpacerString += " ";
		SpacerString += std::to_string(height);
		SpacerString += " rlineto\n";

		SpacerString += std::to_string(-width);
		SpacerString += " ";
		SpacerString += std::to_string(x);
		SpacerString += " rlineto closepath\n";
		return SpacerString;
	}
};
//Create a 4 sided polygon with the given side length
class Square : public Polygon {
public:
	Square(double sideLength) : Polygon(4, sideLength) {}
};
//Create a 3 sided polygon with the given side length
class Triangle : public Polygon {
public:
	Triangle(double sideLength) : Polygon(3, sideLength) {}
};

class Custom : public Shape
{
public:
	Custom(double sideLength)
	{
		width = sideLength;
		height = sideLength;
	}
	std::string generatePostScript()
	{
		// Head
		std::string totalString = "";
		Square s_0(width);
		totalString += s_0.generatePostScript();

		totalString += std::to_string(width / 4);
		totalString += " ";
		totalString += std::to_string(height / 4);
		totalString += " translate\n";

		// Eyes
		Square s_1(width / 5);
		totalString += s_1.generatePostScript();

		totalString += std::to_string(-width / 4);
		totalString += " ";
		totalString += std::to_string(-height / 4);
		totalString += " translate\n";

		totalString += std::to_string(-width / 4);
		totalString += " ";
		totalString += std::to_string(height / 4);
		totalString += " translate\n";

		totalString += s_1.generatePostScript();

		totalString += std::to_string(width / 4);
		totalString += " ";
		totalString += std::to_string(-height / 4);
		totalString += " translate\n";

		// Mouth
		Rectangle mouth((width / 2), (height / 4));
		
		totalString += std::to_string(0);
		totalString += " ";
		totalString += std::to_string(-height / 4);
		totalString += " translate\n";
		
		totalString += mouth.generatePostScript();

		totalString += std::to_string(0);
		totalString += " ";
		totalString += std::to_string(height / 4);
		totalString += " translate\n";

		// Teeth
		Square teeth(width / 8);
		double halfTeeth = (width / 8);
		double quarterTeeth = halfTeeth/2;

		// Top row of teeth
		int scale = 0;
		for (int ii = 1; ii <= 4; ++ii) {
			totalString += std::to_string((-width / 4));
			totalString += " ";
			totalString += std::to_string((-height / 4));
			totalString += " translate\n";
			totalString += std::to_string((quarterTeeth*ii) + (quarterTeeth*scale));
			totalString += " ";
			totalString += std::to_string(quarterTeeth);
			totalString += " translate\n";
			totalString += teeth.generatePostScript();
			totalString += std::to_string((-quarterTeeth*ii) - (quarterTeeth*scale));
			totalString += " ";
			totalString += std::to_string(-quarterTeeth);
			totalString += " translate\n";
			totalString += std::to_string((width / 4));
			totalString += " ";
			totalString += std::to_string((height / 4));
			totalString += " translate\n";
			scale++;
		}
		// Bottom row of teeth
		scale = 0;
		for (int ii = 1; ii <= 4; ++ii) {
			totalString += std::to_string((-width / 4));
			totalString += " ";
			totalString += std::to_string((-height / 4));
			totalString += " translate\n";
			totalString += std::to_string((quarterTeeth*ii) + (quarterTeeth*scale));
			totalString += " ";
			totalString += std::to_string(-quarterTeeth);
			totalString += " translate\n";
			totalString += teeth.generatePostScript();
			totalString += std::to_string((-quarterTeeth*ii) - (quarterTeeth*scale));
			totalString += " ";
			totalString += std::to_string(quarterTeeth);
			totalString += " translate\n";
			totalString += std::to_string((width / 4));
			totalString += " ";
			totalString += std::to_string((height / 4));
			totalString += " translate\n";
			scale++;
		}
		return totalString;
	}
};
class Layered : public Shape
{
public:
	Layered(std::vector<unique_ptr<Shape>> shapeListGiven)
	{
		shapeList = std::move(shapeListGiven);
		width = 0;
		height = 0;

		for (unsigned int i = 0; i<shapeList.size(); ++i)
		{
			if (width < shapeList[i]->width)
			{
				width = shapeList[i]->width;
			}
			if (height < shapeList[i]->height)
			{
				height = shapeList[i]->height;
			}
		}
	}
	std::string generatePostScript()
	{
		std::string totalString = "";
		for (unsigned int i = 0; i<shapeList.size(); ++i)
		{
			totalString += shapeList[i]->generatePostScript();
		}
		return totalString;
	}

private:
	std::vector<unique_ptr<Shape>> shapeList;
};

class Scaled : public Shape
{
public:
	Scaled(Shape &shape, double fx, double fy) {
		std::string s = shape.generatePostScript();
		ScaleString = std::to_string(fx) + " " + std::to_string(fy) + " scale\n";
		ScaleString += s;
		ScaleString += std::to_string(1 / fx) + " " + std::to_string(1 / fy) + " scale\n";

		height = shape.height * fy;
		width = shape.width * fx;
	}
	std::string generatePostScript() override {
		return ScaleString;

	}

private:
	std::string ScaleString;
};

class Rotated : public Shape {
public:
	Rotated(Shape &shape, int rotationAngle)
		:refShape(shape), rotAngle(rotationAngle) {

		if (rotationAngle == 90 || rotationAngle == 270)
		{
			height = shape.width;
			width = shape.height;
		}
		else
		{
			height = shape.height;
			width = shape.width;
		}
	}

	std::string generatePostScript() override
	{
		std::string RotateString = std::to_string(rotAngle);
		RotateString += " rotate\n";
		RotateString += refShape.generatePostScript();

		return RotateString;
	}

private:
	Shape &refShape;
	int rotAngle;
};
//takes a vector and layers them based on the subclass.
//Several classes inherit from this
class Multi : public Shape {
public:
	// Ctor from vector of shapes.
	// Ctor accepts a vector of any size containing pointers to created shapes.
	//Multi(std::vector<unique_ptr<Shape>> mVec)
	Multi(int vSize)
	{
		//mStack = std::move(mVec);
		//mStack = mVec;
		vecSize = vSize;
	}

	std::string generatePostScript() override {
		std::string mString = "";

		// Vertical postscript generation loop.
		for (unsigned int i = 0; i<vecSize; ++i) {

			//Translate based on child functions
			mString += std::to_string(moveHorzStart(i));
			mString += " ";
			mString += std::to_string(moveVertStart(i));
			mString += " translate\n";
			//Get the postscript from the next shape
			//This is needed because unique_ptr does not allow for copy
			mString += getPost(i);
			mString += std::to_string(moveHorzEnd(i));
			mString += " ";
			mString += std::to_string(moveVertEnd(i));
			mString += " translate\n";
			mString += "\n";
		}
		
		return mString;
	}

	virtual double moveVertStart(int shapeNum)
	{
		return 0;
	}

	virtual double moveHorzStart(int shapeNum)
	{
		return 0;
	}

	virtual double moveVertEnd(int shapeNum)
	{
		return 0;
	}

	virtual double moveHorzEnd(int shapeNum)
	{
		return 0;
	}


	virtual std::string getPost(int shapeNum)
	{
		return 0;
	}

private:
	int vecSize;
};
//MultiLayered class inherits from multi, does not move shapes
class MultiLayered : public Multi {
public:
	MultiLayered(std::vector<unique_ptr<Shape>> mVec) : Multi(mVec.size()) {
		mStack = std::move(mVec);
		
		height = 0;
		width = 0;
		for (unsigned int i = 0; i<mStack.size(); ++i)
		{
			if (width < mStack[i]->width)
			{
				width = mStack[i]->width;
			}
			if (height < mStack[i]->height)
			{
				height = mStack[i]->height;
			}
		}

	}

	std::string getPost(int shapeNum) override
	{
		return mStack[shapeNum]->generatePostScript();
	}



private:
	std::vector<unique_ptr<Shape>> mStack;
};
//MultiHorizontal class inherits from multi, does not move shapes
class MultiHorizontal : public Multi {
public:
	MultiHorizontal(std::vector<unique_ptr<Shape>>& mVec) : Multi(mVec.size()) {
		mStack = std::move(mVec);
		height = 0;
		width = 0;
		// Find max height and total width of horizontal shape.
		for (unsigned int i = 0; i<mStack.size(); ++i) {
			width += std::move((mStack[i]->width) + 1);
			// Find max height
			if (mStack[i]->height > height) {
				height = mStack[i]->height;
			}
		}

	}
	//Move horizontally
	double moveVertStart(int shapeNum) override
	{
		return height;
	}
	double moveHorzStart(int shapeNum) override
	{
		return (mStack[shapeNum]->width / 2);
	}
	double moveVertEnd(int shapeNum) override
	{
		return -height;
	}
	double moveHorzEnd(int shapeNum) override
	{
		return (mStack[shapeNum]->width / 2) + 1;
	}
	std::string getPost(int shapeNum) override
	{
		return mStack[shapeNum]->generatePostScript();
	}
private:
	std::vector<unique_ptr<Shape>> mStack;
};
//MultiVertical inherits from vertical
class MultiVertical : public Multi {
public:
	MultiVertical(std::vector<unique_ptr<Shape>>& mVec) : Multi(mVec.size()) {
		mStack = std::move(mVec);
		height = 0;
		width = 0;
		// Find the total height and max width of the vertical shape.
		for (unsigned int i = 0; i<mStack.size(); ++i) {
			height += std::move((mStack[i]->height) + 1);
			// Find max width
			if (mStack[i]->width > width) {
				width = mStack[i]->width;
			}
		}

	}
	//Move vertically
	double moveVertStart(int shapeNum) override
	{
		return mStack[shapeNum]->height / 2;
	}
	double moveHorzStart(int shapeNum) override
	{
		return width;
	}
	double moveVertEnd(int shapeNum) override
	{
		return (mStack[shapeNum]->height / 2) + 1;
	}
	double moveHorzEnd(int shapeNum) override
	{
		return -width;
	}
	std::string getPost(int shapeNum) override
	{
		return mStack[shapeNum]->generatePostScript();
	}
private:
	std::vector<unique_ptr<Shape>> mStack;
};
/*
// Vertical shape class
// Creates a stack of shapes. Shapes are centered based on the widest shape.
//		Shapes do not overlap.
class Multi : public Shape {
public:
	// Ctor from vector of shapes.
	// Ctor accepts a vector of any size containing pointers to created shapes.
	//Multi(std::vector<unique_ptr<Shape>> mVec)
	Multi(std::vector<unique_ptr<Shape>> &mVec)
	{
		mStack = std::move(mVec);
		//mStack = mVec;
	}

	std::string generatePostScript() override {
		std::string mString = "";

		// Vertical postscript generation loop.
		for (unsigned int i = 0; i<mStack.size(); ++i) {
			mString += std::to_string(moveHorzStart(i));
			mString += " ";
			mString += std::to_string(moveVertStart(i));
			mString += " translate\n";
			mString += mStack[i]->generatePostScript();
			mString += std::to_string(moveHorzEnd(i));
			mString += " ";
			mString += std::to_string(moveVertEnd(i));
			mString += " translate\n";
			mString += "\n";
		}
		
		return mString;
	}

	virtual double moveVertStart(int shapeNum)
	{
		return 0;
	}

	virtual double moveHorzStart(int shapeNum)
	{
		return 0;
	}

	virtual double moveVertEnd(int shapeNum)
	{
		return 0;
	}

	virtual double moveHorzEnd(int shapeNum)
	{
		return 0;
	}

private:
	std::vector<unique_ptr<Shape>> mStack;
};


// MultiVertical shape class
// Creates a stack of shapes. Shapes are centered based on the widest shape.
//		Shapes do not overlap.
class MultiVertical : public Multi {
public:
	MultiVertical(std::vector<unique_ptr<Shape>>& mVec) : Multi(mVec) {
		mStack = std::move(mVec);
		height = 0;
		width = 0;
		// Find the total height and max width of the vertical shape.
		for (unsigned int i = 0; i<mStack.size(); ++i) {
			height += std::move((mStack[i]->height) + 1);
			// Find max width
			if (mStack[i]->width > width) {
				width = mStack[i]->width;
			}
		}
	
	}

	double moveVertStart(int shapeNum)
	{
		return mStack[shapeNum]->height / 2;
	}
	double moveHorzStart(int shapeNum)
	{
		return width;
	}
	double moveVertEnd(int shapeNum)
	{
		return (mStack[shapeNum]->height / 2)+1;
	}
	double moveHorzEnd(int shapeNum)
	{
		return -width;
	}
private:
	std::vector<unique_ptr<Shape>> mStack;
};

// MultiHorizontal shape class
// Creates a stack of shapes. Shapes are centered based on the widest shape.
//		Shapes do not overlap.
class MultiHorizontal : public Multi {
public:
	MultiHorizontal(std::vector<unique_ptr<Shape>>& mVec) : Multi(mVec) {
		mStack = std::move(mVec);
		height = 0;
		width = 0;
		// Find max height and total width of horizontal shape.
		for (unsigned int i = 0; i<mStack.size(); ++i) {
			width += std::move((mStack[i]->width) + 1);
			// Find max height
			if (mStack[i]->height > height) {
				height = mStack[i]->height;
			}
		}

	}

	double moveVertStart(int shapeNum) override
	{
		return height;
	}
	double moveHorzStart(int shapeNum) override
	{
		return (mStack[shapeNum]->width / 2);
	}
	double moveVertEnd(int shapeNum) override
	{
		return -height;
	}
	double moveHorzEnd(int shapeNum) override
	{
		return (mStack[shapeNum]->width / 2)+1;
	}
private:
	std::vector<unique_ptr<Shape>> mStack;
};

// Multilayered shape class
// Creates a stack of shapes.
//		Shapes overlap.
class MultiLayered : public Multi {
public:
	MultiLayered(std::vector<unique_ptr<Shape>> mVec) : Multi(mVec) {
		mStack = std::move(mVec);
		
		height = 0;
		width = 0;
		for (unsigned int i = 0; i<mStack.size(); ++i)
		{
			if (width < mStack[i]->width)
			{
				width = mStack[i]->width;
			}
			if (height < mStack[i]->height)
			{
				height = mStack[i]->height;
			}
		}

	}

	std::string generatePostScript() override {
		return Multi::generatePostScript();
	}


private:
	std::vector<unique_ptr<Shape>> mStack;
};
*/
// Vertical shape class
// Creates a stack of shapes. Shapes are centered based on the widest shape.
//		Shapes do not overlap.
class Vertical : public Shape {
public:
	// Ctor from vector of shapes.
	// Ctor accepts a vector of any size containing pointers to created shapes.
	Vertical(std::vector<unique_ptr<Shape>> vertVec)
	{
		vertStack = std::move(vertVec);
		height = 0;
		width = 0;
		// Find the total height and max width of the vertical shape.
		for (unsigned int i = 0; i<vertStack.size(); ++i) {
			height += std::move((vertStack[i]->height) + 1);
			// Find max width
			if (vertStack[i]->width > width) {
				width = vertStack[i]->width;
			}
		}
	}

	std::string generatePostScript() override {
		std::string vertString = "";

		// Vertical postscript generation loop.
		for (unsigned int i = 0; i<vertStack.size(); ++i) {
			vertString += std::to_string(width);
			vertString += " ";
			vertString += std::to_string(vertStack[i]->height / 2);
			vertString += " translate\n";
			vertString += vertStack[i]->generatePostScript();
			vertString += std::to_string(-width);
			vertString += " ";
			vertString += std::to_string((vertStack[i]->height / 2) + 1);
			vertString += " translate\n";
			vertString += "\n";
		}
		return vertString;
	}

private:
	std::vector<unique_ptr<Shape>> vertStack;
};

// Horizontal shape class
// Creates a row of shapes. Shapes are centered based on the tallest shape.
//		Shapes do not overlap.
class Horizontal : public Shape {
public:
	// Ctor from vector of shapes.
	// Ctor accepts a vector of any size containing pointers to created shapes.
	Horizontal(std::vector<unique_ptr<Shape>> horizontalVec)
	{
		horizontalStack = std::move(horizontalVec);
		height = 0;
		width = 0;
		// Find max height and total width of horizontal shape.
		for (unsigned int i = 0; i<horizontalStack.size(); ++i) {
			width += std::move((horizontalStack[i]->width) + 1);
			// Find max height
			if (horizontalStack[i]->height > height) {
				height = horizontalStack[i]->height;
			}
		}
	}

	std::string generatePostScript() override {

		std::string horizontalString = "";

		// Horizontal postscript generation loop.
		for (unsigned int i = 0; i<horizontalStack.size(); ++i) {
			horizontalString += std::to_string(horizontalStack[i]->width / 2);
			horizontalString += " ";
			horizontalString += std::to_string(height);
			horizontalString += " translate\n";
			horizontalString += horizontalStack[i]->generatePostScript();
			horizontalString += std::to_string((horizontalStack[i]->width / 2) + 1);
			horizontalString += " ";
			horizontalString += std::to_string(-height);
			horizontalString += " translate\n";
			horizontalString += "\n";
		}
		return horizontalString;
	}

private:
	std::vector<unique_ptr<Shape>> horizontalStack;

};

#endif // SHAPE_HPP_INCLUDED
