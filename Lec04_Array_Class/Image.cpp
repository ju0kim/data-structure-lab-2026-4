#include "Image.h"
#include <iostream>

using namespace std;

//생성자
Image::Image(int init_image[Height][Width]) {
	for (int i = 0; i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			pixels[i][j] = init_image[i][j];
		}
	}
}

//최대 밝기 탐색

int Image::GetMaxBrightness() {
	int max_val = pixels[0][0];

	for (int i = 0;i < Height; i++) {
		for (int j = 0; j < Width; j++) {
			if (pixels[i][j] > max_val) {
				max_val = pixels[i][j];
			}
		}
	}

	return max_val;

}

int main() {
	//초기 이미지 값
	int input_data[Height][Width] = {
		{ 10,  30,  55,  80, 120, 160, 200, 230},
		{ 20,  45,  70, 100, 140, 180, 220, 210},
		{ 35,  60,  90, 130, 170, 210, 240, 190},
		{ 50,  80, 115, 150, 190, 230, 255, 170},
		{ 40,  65, 100, 140, 175, 215, 235, 150},
		{ 25,  50,  80, 115, 155, 195, 210, 130},
		{ 15,  35,  60,  90, 130, 165, 185, 110},
		{  5,  20,  40,  70, 105, 140, 160,  90}
	};

	//이미지 생성

	Image myImage(input_data);

	//함수호출

	int maxBrightness = myImage.GetMaxBrightness();
	cout << "R^2 image's max brightness: " << maxBrightness << endl;

}
