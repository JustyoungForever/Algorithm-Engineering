#define CATCH_CONFIG_MAIN  // provide a main()
#include <omp.h>
#include <catch2/catch.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ThresholdMethods.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <sstream>

TEST_CASE("Threshold matrix calculation matches expected logic", "[calcThresholdMatrix]") {
    // Create a grayscale image, local mean, and local standard deviation matrices
    cv::Mat grayImage = cv::Mat::ones(5, 5, CV_64F) * 100;  // Global mean of the grayscale image is 100
    cv::Mat localMean = cv::Mat::ones(5, 5, CV_64F) * 120;  // Local mean is 120
    cv::Mat localStdDev = cv::Mat(5, 5, CV_64F);
    double stdDevValues[5][5] = {{10, 12, 15, 12, 10}, {12, 14, 15, 14, 12}, {15, 15, 15, 15, 15}, {12, 14, 15, 14, 12}, {10, 12, 15, 12, 10}};
    for(int i = 0; i < 5; ++i) {
        for(int j = 0; j < 5; ++j) {
            localStdDev.at<double>(i, j) = stdDevValues[i][j];
        }
    }
    cv::Mat threshold;

    // Calculate the threshold matrix
    calcThresholdMatrix(grayImage, localMean, localStdDev, threshold);

    double sigmaMin = 10;  // Minimum local standard deviation
    double sigmaMax = 15;  // Maximum local 
    double sigmaRange = sigmaMax - sigmaMin;  // Standard deviation range

    for (int y = 0; y < threshold.rows; ++y) {
        for (int x = 0; x < threshold.cols; ++x) {
            double mw = localMean.at<double>(y, x);
            double sigmaW = localStdDev.at<double>(y, x);
            double sigmaAdaptive = (sigmaW - sigmaMin) / sigmaRange;
            double mg = cv::mean(grayImage)[0];
            double denominator = (mg + sigmaW) * (sigmaAdaptive + sigmaW);

            double expectedThresholdValue = mw - (mw * mw - sigmaW * sigmaW) / denominator;

            REQUIRE(threshold.at<double>(y, x) == Approx(expectedThresholdValue).epsilon(0.01));
        }
    }
}

TEST_CASE("Mean and standard deviation calculation for uniform image", "[calcMeanAndStdDev]") {
    // Create a 10x10 matrix of all ones, type CV_64F
    cv::Mat src = cv::Mat::ones(10, 10, CV_64F);
    cv::Mat mean, stddev;
    int windowSize = 3;

    calcMeanAndStdDev(src, mean, stddev, windowSize);

    // Since the entire image is 1, the expected mean should be 1, and the standard deviation should be 0
    for (int y = 0; y < mean.rows; ++y) {
        for (int x = 0; x < mean.cols; ++x) {
            REQUIRE(mean.at<double>(y, x) == Approx(1).epsilon(0.01)); // Mean close to 1
            REQUIRE(stddev.at<double>(y, x) == Approx(0).margin(0.01)); // Standard deviation close to 0
        }
    }
}

TEST_CASE("Image binarization based on threshold matrix", "[binarizeImage]") {
    cv::Mat grayImage = cv::Mat::ones(10, 10, CV_8U) * 150;  // Create a grayscale image with all values 150
    cv::Mat threshold = cv::Mat::ones(10, 10, CV_64F) * 100;  // Create a threshold matrix with all values 100
    cv::Mat binaryImage = binarizeImage(grayImage, threshold);

    // Verify the binarized image as expected
    // Since the threshold is 100, all pixel values in the image should be converted to 255
    for (int y = 0; y < binaryImage.rows; ++y) {
        for (int x = 0; x < binaryImage.cols; ++x) {
            REQUIRE(binaryImage.at<uchar>(y, x) == 255);
        }
    }
}
