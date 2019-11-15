#include "Display.h"
#include "Colors.h"


Color colorBlack = { 0, 0, 0 };
Color colorDarkGray{ 50, 50, 50 };
Color colorLightGray{ 200, 200, 200 };
Color colorWhite{ 255, 255, 255 };

Color ColorPalette::getColor(float val)
{
	int idx1;               // |-- Our desired color will be between these two indexes in "color".
	int idx2;               // |
	float fractBetween = 0; // Fraction between "idx1" and "idx2" where our value is.
	Color color;

	if (val <= 0)
	{
		idx1 = idx2 = 0;
	} // accounts for an input <=0
	else if (val >= 1)
	{
		idx1 = idx2 = size - 1;
	} // accounts for an input >=0
	else
	{
		val = val * (size - 1);
		idx1 = floor(val);
		idx2 = idx1 + 1;
		fractBetween = val - float(idx1);
	}

	color.red = colors[idx2].red * fractBetween + colors[idx1].red * (1 - fractBetween);
	color.green = colors[idx2].green * fractBetween + colors[idx1].green * (1 - fractBetween);
	color.blue = colors[idx2].blue * fractBetween + colors[idx1].blue * (1 - fractBetween);

	return color;
}

ColorTheme::ColorTheme()
{
	colorPalette = &csBw;
	colorSource = ColorSource::computeFromZ;
	//ColorPalette &colorPalette, ColorSource colorSource) : colorPalette(&colorPalette), colorSource(colorSource){};
}

Color ColorTheme::project(Vector &dataPoint)
{
	switch (colorSource)
	{
	case ColorSource::computeFromX:
	{
		return colorPalette->getColor(dataPoint.x);
	}
	break;
	case ColorSource::computeFromY:
	{
		return colorPalette->getColor(dataPoint.y);
	}
	break;
	case ColorSource::computeFromZ:
	{
		return colorPalette->getColor(dataPoint.z);
	}
	break;

	case ColorSource::predefined:
	default:
	{
		return colorPalette->getColor(0.5);
	}
	break;
	}
}

Color ColorTheme::getColor(float val)
{
	return (colorPalette->getColor(val));
}

Pixel::Pixel()
    : x(0)
    , y(0)
{
}

Pixel::Pixel(float x, float y)
    : x(x)
    , y(y)
{
}

void DisplayDriver::begin(Adafruit_GFX *tft)
{
	this->tft = tft;
}

void DisplayDriver::drawPixel(Pixel c, Color color)
{
	tft->drawPixel(c.x * tft->width(),
	               (1.0 - c.y) * tft->height(),
	               colorTo16b(color));
}

void DisplayDriver::drawLine(Pixel a, Pixel b, Color color)
{
	//for now, color is decided by first pixel
	tft->drawLine(a.x * tft->width(),
	              (1.0 - a.y) * tft->height(),
	              b.x * tft->width(),
	              (1.0 - b.y) * tft->height(),
	              colorTo16b(color));
}

void DisplayDriver::drawCircle(Pixel c, float area, Color color)
{
	area *= tft->width() * tft->height();
	int radius = sqrt(area / M_PI);
	tft->drawCircle(c.x * tft->width(),
	                (1.0 - c.y) * tft->height(),
	                radius,
	                colorTo16b(color));
}

void DisplayDriver::drawTriangle(Pixel a, Pixel b, Pixel c, Color color)
{
	tft->drawTriangle(a.x * tft->width(),
	                  (1.0 - a.y) * tft->height(),
	                  b.x * tft->width(),
	                  (1.0 - b.y) * tft->height(),
	                  c.x * tft->width(),
	                  (1.0 - c.y) * tft->height(),
	                  colorTo16b(color));
}

void DisplayDriver::drawRectangle(Pixel bl, float w, float h, Color color)
// bl: bottom left vertex, w: width, h: height
{
	tft->drawRect(bl.x * tft->width(),
	              1.0 - (bl.y + h) * tft->height(),
	              w * tft->width(),
	              h * tft->height(),
	              colorTo16b(color));
}

void DisplayDriver::drawRectangle(Pixel bl, Pixel tr, Color color)
// bl: bottom left vertex, w: width, h: height
{
	drawRectangle(bl, tr.x - bl.x, tr.y - bl.y, color);
}

void DisplayDriver::drawRoundRectangle(Pixel bl, float w, float h, float area, Color color)
// bl: bottom left vertex, w: width, h: height, r: radius
{

	//account for mirrored boundaries
	if (w < 0)
	{
		bl.x += w;
		w = -w;
	}

	if (h < 0)
	{
		bl.y += h;
		h = -h;
	}

	area *= tft->width() * tft->height();
	int radius = sqrt(area / M_PI);

	tft->drawRoundRect(tft->width() * bl.x,
	                   tft->height() * (1.0 - (bl.y + h)),
	                   tft->width() * w,
	                   tft->height() * h,
	                   radius,
	                   colorTo16b(color));
}

void DisplayDriver::fillRectangle(Pixel bl, float w, float h, Color color)
{
	//std::cout << bl.x << " " << bl.y << " " << w << " " << h << std::endl;

	//account for mirrored boundaries
	if (w < 0)
	{
		bl.x += w;
		w = -w;
	}

	if (h < 0)
	{
		bl.y += h;
		h = -h;
	}

	tft->fillRect(tft->width() * bl.x,
	              tft->height() * (1.0 - (bl.y + h)),
	              tft->width() * w,
	              tft->height() * h,
	              colorTo16b(color));
}

void DisplayDriver::fillRectangle(Pixel bl, Pixel tr, Color color)
// bl: bottom left vertex, w: width, h: height
{
	fillRectangle(bl, tr.x - bl.x, tr.y - bl.y, color);
}

void DisplayDriver::fillCircle(Pixel c, float area, Color color)
{
	area *= tft->width() * tft->height();
	int radius = sqrt(area / M_PI);
	tft->fillCircle(c.x * tft->width(),
	                (1.0 - c.y) * tft->height(),
	                radius,
	                colorTo16b(color));
}

void DisplayDriver::fillTriangle(Pixel a, Pixel b, Pixel c, Color color)
{
	tft->fillTriangle(tft->width() * a.x,
	                  tft->height() * (1.0 - a.y),
	                  tft->width() * b.x,
	                  tft->height() * (1.0 - b.y),
	                  tft->width() * c.x,
	                  tft->height() * (1.0 - c.y),
	                  colorTo16b(color));
}

void DisplayDriver::fillRoundRectangle(Pixel bl, float w, float h, float area, Color color)
{
	area *= tft->width() * tft->height();
	int radius = sqrt(area / M_PI);

	tft->fillRoundRect(tft->width() * bl.x,
	                   tft->height() * (1.0 - (bl.y + h)),
	                   tft->width() * w,
	                   tft->height() * h,
	                   radius,
	                   colorTo16b(color));
}

void DisplayDriver::fillScreen(Color color)
{
	tft->fillScreen(colorTo16b(color));
}

int DisplayDriver::colorTo16b(Color color)
{
	int c = 0;
	int r = (color.red / 255.0) * 31;
	int g = (color.green / 255.0) * 63;
	int b = (color.blue / 255.0) * 31;
	c = (r << 11) | (g << 5) | (b);
	return (c);
}

DisplayBoundaries::DisplayBoundaries()
{
	reset();
}

DisplayBoundaries &DisplayBoundaries::reset()
{
	bottomLeft.x = 0.0;
	bottomLeft.y = 0.0;
	topRight.x = 1.0;
	topRight.y = 1.0;
	return *this;
}

DisplayBoundaries &DisplayBoundaries::set(Vector bottomLeft, Vector topRight)
{
	this->bottomLeft = bottomLeft;
	this->topRight = topRight;
	return *this;
}

DisplayBoundaries &DisplayBoundaries::addBorder(float top, float bottom, float left, float right)
{
	// account for horizzontalFlip
	if (width() > 0.0)
	{
		bottomLeft.x += left;
		topRight.x -= right;
	}
	else
	{
		bottomLeft.x -= left;
		topRight.x += right;
	}

	// account for verticalFlip
	if (height() > 0.0)
	{
		bottomLeft.y += bottom;
		topRight.y -= top;
	}
	else
	{
		bottomLeft.y -= bottom;
		topRight.y += top;
	}
	return *this;
}

DisplayBoundaries &DisplayBoundaries::crop(int rows, int columns, int index)
{
	float _width = width();
	float _height = height();

	_width /= columns;
	_height /= rows;

	bottomLeft.x = bottomLeft.x + (index % columns) * _width;
	bottomLeft.y = bottomLeft.y + (index / columns) * _height;

	topRight.x = bottomLeft.x + _width;
	topRight.y = bottomLeft.y + _height;
	return *this;
}

DisplayBoundaries &DisplayBoundaries::horizzontalFlip(void)
{
	swap(bottomLeft.x, topRight.x, float);
	return *this;
}

DisplayBoundaries &DisplayBoundaries::verticalFlip(void)
{
	swap(bottomLeft.y, topRight.y, float);
	return *this;
}

float DisplayBoundaries::width(void)
{
	return (topRight.x - bottomLeft.x);
}

float DisplayBoundaries::height(void)
{
	return (topRight.y - bottomLeft.y);
}

Vector DisplayBoundaries::getCenter(void)
{
	Vector center;
	center.x = (bottomLeft.x + topRight.x) / 2.0;
	center.y = (bottomLeft.y + topRight.y) / 2.0;
	return center;
}

Pixel DisplayBoundaries::project(Vector &dataPoint)
{
	Pixel p;
	p.x = (dataPoint.x * topRight.x + (1.0 - dataPoint.x) * bottomLeft.x);
	p.y = (dataPoint.y * topRight.y + (1.0 - dataPoint.y) * bottomLeft.y);
	return p;
}

RoundDisplayBoundaries::RoundDisplayBoundaries()
{
	DisplayBoundaries();
	reset();
}

DisplayBoundaries &RoundDisplayBoundaries::reset()
{
	DisplayBoundaries::reset();
	innerRadius = 0.0;
	outerRadius = 1.0;
	beginAngle = 0.0;
	endAngle = 1.0;
	return *this;
}

DisplayBoundaries &RoundDisplayBoundaries::set(Vector bottomLeft, Vector topRight)
{
	DisplayBoundaries::set(bottomLeft, topRight);
	return *this;
}

DisplayBoundaries &RoundDisplayBoundaries::addBorder(float top, float bottom, float left, float right)
{
	DisplayBoundaries::addBorder(top, bottom, left, right);
	return *this;
}

DisplayBoundaries &RoundDisplayBoundaries::addBorderRadial(float top, float bottom, float left, float right)
{
	float deltaRadius = outerRadius - innerRadius;	
	innerRadius += bottom * deltaRadius;
	outerRadius -= top * deltaRadius;

	float deltaAngle = endAngle - beginAngle;
	beginAngle += right * deltaAngle;
	endAngle -= left * deltaAngle;

	return *this;
}

DisplayBoundaries &RoundDisplayBoundaries::crop(int rows, int columns, int index)
{
	DisplayBoundaries::crop(rows, columns, index);
	return *this;
}

DisplayBoundaries &RoundDisplayBoundaries::cropRadial(int rows, int columns, int index)
{
	float deltaAngle = endAngle - beginAngle;
	float deltaRadius = outerRadius - innerRadius;

	deltaAngle /= columns;
	deltaRadius /= rows;

	beginAngle += (index % columns) * deltaAngle;
	endAngle = beginAngle + deltaAngle;

	innerRadius += (index / columns) * deltaRadius;
	outerRadius = innerRadius + deltaRadius;

	return *this;
}

DisplayBoundaries &RoundDisplayBoundaries::horizzontalFlip(void)
{
	DisplayBoundaries::horizzontalFlip();
	return *this;
}

DisplayBoundaries &RoundDisplayBoundaries::verticalFlip(void)
{
	DisplayBoundaries::verticalFlip();
	return *this;
}

DisplayBoundaries &RoundDisplayBoundaries::horizzontalFlipRadial(void)
{
	swap(beginAngle, endAngle, float);
	return *this;
}

DisplayBoundaries &RoundDisplayBoundaries::verticalFlipRadial(void)
{
	swap(innerRadius, outerRadius, float);
	return *this;
}

Pixel RoundDisplayBoundaries::project(Vector &dataPoint)
{
	Pixel p;

	float radius = innerRadius * (1.0 - dataPoint.y) + outerRadius * dataPoint.y;
	float angle = beginAngle * (1.0 - dataPoint.x) + endAngle * dataPoint.x;
	p.x = (getCenter().x + (radius*(width()/2)) * cos(angle*2*M_PI));
	p.y = (getCenter().y + (radius*(height()/2)) * sin(angle*2*M_PI));

	return p;
}

PlotObj::PlotObj()
{
}

void PlotObj::plot(DisplayDriver *displayDriver, DataSet *dataSet, DisplayBoundaries *boundaries, ColorTheme *theme)
{
}
