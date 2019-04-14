#ifndef ARDU_DATAVIS_DRIVERGFX_H
#define ARDU_DATAVIS_DRIVERGFX_H

#include "Adafruit_GFX.h"

class DriverGFX : public Driver
{
public:
	DriverGFX();
	void drawPixel(int x, int y, Color color);
	void drawLine(int x0, int y0, int x1, int y1, Color color);
	void drawCircle(int x0, int y0, int r, Color color);
	void drawRect(int x, int y, int w, int h, Color color);
	void fillRect(int x, int y, int w, int h, Color color);
	void fillCircle(int x0, int y0, int r, Color color);
	int width();
	int height();

private:
	Adafruit_GFX tft;
	uint16_t colorTo16b(Color color);
};

#endif //ARDU_DATAVIS_DRIVER_H
