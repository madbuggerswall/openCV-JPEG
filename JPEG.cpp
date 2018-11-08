#include "JPEG.hpp"

JPEG::JPEG(cv::Mat input, cv::Mat quant)
{
	cv::Mat source = input.clone();

	this->height = source.size().height;
	this->width = source.size().width;
	
	//	Get optimal array sizes for dct().
	int optHight = getOptimalDCTSize(height);
	int optWidth = getOptimalDCTSize(width);

	//	Padding the array for a dct()-supported array size.
	cv::copyMakeBorder(source, source,
										 0, optHight - height,
										 0, optWidth - width,
										 cv::BORDER_CONSTANT, cv::Scalar::all(0));

	//	dct() - quantization - idct() 
	cv::Mat compressed = compress(source, quant);
	cv::Mat decompressed = decompress(compressed, quant);
	this->output = decompressed;
}

//	Applying dct() & quantization to every 8x8 pixel block.
cv::Mat JPEG::compress(cv::Mat source, cv::Mat quant)
{
	cv::Mat compressed = cv::Mat(cv::Size(width, height), CV_32FC1);

	source.convertTo(source, CV_32FC1);
	cv::cvtColor(source, source, CV_BGR2YCrCb);
	
	std::vector<cv::Mat> channels;
	split(source, channels); //Channels: Y - Cr - Cb

	for (int i = 0; i < height; i += 8){
		for (int j = 0; j < width; j += 8){
			for (int channel = 0; channel < channels.size(); channel++){
				cv::Mat block = channels[channel](cv::Rect(j, i, 8, 8));
				block.convertTo(block, CV_32FC1);
				cv::subtract(block, 128.0f, block); //	To centralize values around 0.
				cv::dct(block, block);
				cv::divide(block, quant, block);
				cv::add(block, 128.0f, block);
				block.convertTo(block, CV_16SC1);
				block.copyTo(channels[channel](cv::Rect(j, i, 8, 8)));
			}
		}
	}
	cv::merge(channels, compressed);
	return compressed;
}

//	Applying quantization & idct() to every 8x8 pixel block.
cv::Mat JPEG::decompress(cv::Mat compressed, cv::Mat quant)
{
	cv::Mat decompressed = cv::Mat(cv::Size(width, height), CV_16UC1);

	std::vector<cv::Mat> channels;
	split(compressed, channels); //Channels: Y - Cr - Cb

	for (int i = 0; i < height; i += 8){
		for (int j = 0; j < width; j += 8){
			for (int channel = 0; channel < channels.size(); channel++){
				cv::Mat block = channels[channel](cv::Rect(j, i, 8, 8));
				cv::subtract(block, 128.0f, block); //	To centralize values around 0.
				cv::multiply(block, quant, block);
				cv::idct(block, block);
				cv::add(block, 128.0f, block);
				block.convertTo(block, CV_16UC1);
				block.copyTo(channels[channel](cv::Rect(j, i, 8, 8)));
			}
		}
	}

	cv::merge(channels, decompressed);
	cv::cvtColor(decompressed, decompressed, CV_YCrCb2RGB);
	return decompressed;
}

//	Returns an even-sized array length or width since dct() only supports even-sized arrays.
//	Also recommended on OpenCV's documentation for increased dct() performance.
int JPEG::getOptimalDCTSize(int vecSize)
{
	return 2 * cv::getOptimalDFTSize((vecSize + 1) / 2);
}
