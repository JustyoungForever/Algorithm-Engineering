// ThresholdMethods.h

#ifndef THRESHOLDMETHODS_H
#define THRESHOLDMETHODS_H

#include <opencv2/core.hpp>

// void thresholdIntegral(const cv::Mat &inputMat, cv::Mat &outputMat);
// cv::Mat binarizeImage(const cv::Mat& grayImage, const cv::Mat& localMean, const cv::Mat& localStdDev, int windowSize, double sigma_global_min, double sigma_global_max);
void calcMeanAndStdDev(const cv::Mat& image, cv::Mat& mean, cv::Mat& stddev, int windowSize);
cv::Mat binarizeImage(const cv::Mat& grayImage, const cv::Mat& threshold) ;
void calcThresholdMatrix(const cv::Mat& grayImage, const cv::Mat& localMean, const cv::Mat& localStdDev, cv::Mat& threshold);
// Prototype for the custom thresholding method
// void customBinarization(const cv::Mat& src, cv::Mat& dst, int windowSize);


#endif // THRESHOLDMETHODS_H
