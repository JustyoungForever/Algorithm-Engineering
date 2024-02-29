// ThresholdMethods.h

#ifndef THRESHOLDMETHODS_H
#define THRESHOLDMETHODS_H

#include <opencv2/core.hpp>

void calcMeanAndStdDev(const cv::Mat& image, cv::Mat& mean, cv::Mat& stddev, int windowSize);
cv::Mat binarizeImage(const cv::Mat& grayImage, const cv::Mat& threshold) ;
void calcThresholdMatrix(const cv::Mat& grayImage, const cv::Mat& localMean, const cv::Mat& localStdDev, cv::Mat& threshold);



#endif // THRESHOLDMETHODS_H
