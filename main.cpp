// Ege Furkan Bilgin S004133 Department of Computer Science

#include <iostream>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "QTable.hpp"
#include "JPEG.hpp"

const cv::Mat quant10 = cv::Mat(8, 8, CV_32FC1, &QTable::q10);
const cv::Mat quant50 = cv::Mat(8, 8, CV_32FC1, &QTable::q50);
const cv::Mat quant80 = cv::Mat(8, 8, CV_32FC1, &QTable::q80);
const cv::Mat quant90 = cv::Mat(8, 8, CV_32FC1, &QTable::q90);


void convertAndWrite(cv::Mat image, std::string writePath, std::string fileName)
{
	JPEG image10 = JPEG(image, quant10);
	JPEG image50 = JPEG(image, quant50);
	JPEG image90 = JPEG(image, quant90);
	cv::imwrite(writePath + fileName + "10.jpeg", image10.output);
	cv::imwrite(writePath + fileName + "50.jpeg", image50.output);
	cv::imwrite(writePath + fileName + "90.jpeg", image90.output);
}

int main(int argc, char const *argv[])
{
	const std::string bearFilePath = "../data/bear_finland.png";
	const std::string natureFilePath = "../data/nature.png";
	const std::string patternFilePath = "../data/pattern.png";
	const std::string stateFilePath = "../data/sixty_state.png";
	
	cv::Mat bearIMG = cv::imread(bearFilePath, CV_LOAD_IMAGE_COLOR);
	cv::Mat natureIMG = cv::imread(natureFilePath, CV_LOAD_IMAGE_COLOR);
	cv::Mat patternIMG = cv::imread(patternFilePath, CV_LOAD_IMAGE_COLOR);
	cv::Mat stateIMG = cv::imread(stateFilePath, CV_LOAD_IMAGE_COLOR);

	if(bearIMG.empty())		{	std::cout << "Image empty." << std::endl; return -1;	}
	if(natureIMG.empty())	{	std::cout << "Image empty." << std::endl;	return -1;	}
	if(patternIMG.empty()){	std::cout << "Image empty." << std::endl;	return -1;	}
	if(stateIMG.empty())	{	std::cout << "Image empty." << std::endl;	return -1;	}

	convertAndWrite(bearIMG, "../output/", "bear");
	convertAndWrite(natureIMG, "../output/", "nature");
	convertAndWrite(patternIMG, "../output/", "pattern");
	convertAndWrite(stateIMG, "../output/", "state");
	std::cout << "JPEG encoding complete." << std::endl;

	return 0;
}