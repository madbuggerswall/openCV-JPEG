// Ege Furkan Bilgin S004133 Department of Computer Science

#ifndef JPEG_HPP
#define JPEG_HPP

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

struct JPEG
{
	int height;
	int width;

	cv::Mat input;
	cv::Mat output;

	JPEG() = default;
	JPEG(cv::Mat input, const cv::Mat quant);
	~JPEG() = default;

	cv::Mat compress(cv::Mat source, const cv::Mat quant);
	cv::Mat decompress(cv::Mat compressed, const cv::Mat quant);
	int getOptimalDCTSize(int vecSize);
};
#endif