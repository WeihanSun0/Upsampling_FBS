#include "upsampling.h"
#include "bin2cv.h"

#ifdef _DEBUG
#pragma comment (lib, ".\\lib\\opencv_world348d.lib")
#else
#pragma comment (lib, ".\\lib\\opencv_world348.lib")
#endif

int main(void) {

	PARAM param;

	cv::Mat guide = cv::imread("guide.bmp");
	cv::Mat sparse = cv::readBin("sparsedepth.bin");

	cv::Mat dense;
	upsampling(sparse, guide, param, dense);

	return 0;
}