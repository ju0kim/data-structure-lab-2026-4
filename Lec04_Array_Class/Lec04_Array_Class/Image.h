#pragma once
#define Height 8
#define Width 8

class Image {

private:
	int pixels[Height][Width];

public:
	Image(int init_image[Height][Width]);

	int GetMaxBrightness();

};