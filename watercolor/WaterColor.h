#ifndef WATERCOLOR_H
#define WATERCOLOR_H

#include <opencv2/opencv.hpp>

class WaterColor
{
public:
    std::string processDir;
    std::string style;

    WaterColor(std::string pDir, std::string st);
	void deal(const cv::Mat &input, cv::Mat &output);
	void deal(cv::Mat &src, cv::Mat &dst);
};

#endif
