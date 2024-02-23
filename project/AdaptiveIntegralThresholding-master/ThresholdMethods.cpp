// ThresholdMethods.cpp
#include <omp.h>
#include "ThresholdMethods.h"
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <chrono>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <limits>
using namespace std;
// Helper function to calculate mean and standard deviation within a window
void calcMeanAndStdDev(const cv::Mat& image, cv::Mat& mean, cv::Mat& stddev, int windowSize) {
    cv::Mat sum, sumSq;
    cv::integral(image, sum, sumSq, CV_64F);
    cout << "Inside calcMeanAndStdDev function" << endl;

    cv::integral(image, sum, sumSq, CV_64F);
    cout << "Sum and sumSq matrices should be of size: " << image.rows + 1 << "x" << image.cols + 1 << endl;
    cout << "Actual sizes are: sum - " << sum.rows << "x" << sum.cols << ", sumSq - " << sumSq.rows << "x" << sumSq.cols << endl;

    // Initialize mean and stddev with the correct size and type
    mean.create(image.size(), CV_64F);
    stddev.create(image.size(), CV_64F);
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {

            int x0 = std::max(0, x - windowSize / 2);
            int y0 = std::max(0, y - windowSize / 2);
            int x1 = std::min(image.cols - 1, x + windowSize / 2);
            int y1 = std::min(image.rows - 1, y + windowSize / 2);
            // It's crucial to ensure that all indices are within valid range
            if (x0 < 0 || x1 >= sum.cols || y0 < 0 || y1 >= sum.rows) {
                cerr << "Index out of range!" << endl;
                continue; // Skip this iteration to avoid segmentation fault
            }
            double area = (x1 - x0) * (y1 - y0);
            double sumVal = sum.at<double>(y1, x1) - sum.at<double>(y0, x1) - sum.at<double>(y1, x0) + sum.at<double>(y0, x0);
            double sumSqVal = sumSq.at<double>(y1, x1) - sumSq.at<double>(y0, x1) - sumSq.at<double>(y1, x0) + sumSq.at<double>(y0, x0);
            // Check if the variance is non-negative before taking the square root
            mean.at<double>(y, x) = sumVal / area;
            double variance = sumSqVal / area - mean.at<double>(y, x) * mean.at<double>(y, x);
            if (variance < 0) {
                cerr << "Negative variance encountered!" << endl;
                stddev.at<double>(y, x) = 0;
            } else {
                stddev.at<double>(y, x) = sqrt(variance);
            }
            stddev.at<double>(y, x) = sqrt(sumSqVal / area - mean.at<double>(y, x) * mean.at<double>(y, x));
        
        }
    }
        cout << "Mean and standard deviation calculated successfully." << endl;
}

// void calcThresholdMatrix(const cv::Mat& grayImage, const cv::Mat& localMean, const cv::Mat& localStdDev, cv::Mat& threshold) {
//     // 计算全局均值 mg
//     double mg = cv::mean(grayImage)[0];
//     cout << "Inside calcThresholdMatrix function" << endl;
//     // 初始化阈值矩阵
//     threshold = cv::Mat::zeros(grayImage.size(), CV_64F);
//             // 假设您已经计算过 sigmaMin 和 sigmaMax
//             double sigmaMin = *std::min_element(localStdDev.begin<double>(), localStdDev.end<double>());
//             double sigmaMax = *std::max_element(localStdDev.begin<double>(), localStdDev.end<double>());


//     // 遍历每个像素，计算阈值
//     for (int y = 0; y < grayImage.rows; ++y) {
//         for (int x = 0; x < grayImage.cols; ++x) {
//             // 获取局部窗口的均值 mw 和标准偏差 sigmaW
//             double mw = localMean.at<double>(y, x);
//             double sigmaW = localStdDev.at<double>(y, x);

//             // 计算自适应标准偏差 sigmaAdaptive
//             double sigmaMin = *std::min_element(localStdDev.begin<double>(), localStdDev.end<double>());
//             double sigmaMax = *std::max_element(localStdDev.begin<double>(), localStdDev.end<double>());
//             double sigmaAdaptive = (sigmaW - sigmaMin) / (sigmaMax - sigmaMin);

//             // 应用阈值化公式计算每个像素的阈值 T
//             double denominator = (mg + sigmaW) * (sigmaAdaptive + sigmaW);
//             if(denominator == 0) {
//                 // 避免除以零
//                 threshold.at<double>(y, x) = 0;
//             } else {
//                 double T = mw - (mw * mw - sigmaW * sigmaW) / denominator;
//                 threshold.at<double>(y, x) = T;
//             }
//         }
//     }
// }

void calcThresholdMatrix(const cv::Mat& grayImage, const cv::Mat& localMean, const cv::Mat& localStdDev, cv::Mat& threshold) {
    auto start = std::chrono::high_resolution_clock::now();
    double mg = cv::mean(grayImage)[0];
    cout << "Inside calcThresholdMatrix function" << endl;
    threshold = cv::Mat::zeros(grayImage.size(), CV_64F);
    // 定义块大小
    int blockSize = 16; // 根据您的具体情况可能需要调整

    double sigmaMin = *std::min_element(localStdDev.begin<double>(), localStdDev.end<double>());
    double sigmaMax = *std::max_element(localStdDev.begin<double>(), localStdDev.end<double>());

    #pragma omp parallel for collapse(2) schedule(dynamic, blockSize)
    for (int y = 0; y < grayImage.rows; y += blockSize) {
        for (int x = 0; x < grayImage.cols; x += blockSize) {
            // 确定每个块的实际尺寸，处理边界情况

            int blockHeight = std::min(blockSize, grayImage.rows - y);
            int blockWidth = std::min(blockSize, grayImage.cols - x);

            for (int i = y; i < y + blockHeight; ++i) {
                for (int j = x; j < x + blockWidth; ++j) {
                    
                    double mw = localMean.at<double>(i, j);
                    double sigmaW = localStdDev.at<double>(i, j);
                    if (sigmaW == 0) sigmaW = 1e-6;

                    double sigmaAdaptive = (sigmaW - sigmaMin) / (sigmaMax - sigmaMin);
                    double T;
                    double denominator = (mg + sigmaW) * (sigmaAdaptive + sigmaW);
                    if (denominator == 0)
                    {
                        threshold.at<double>(i, j) = 0;
                    } else {double T = mw - (mw * mw - sigmaW * sigmaW) / denominator;
                            threshold.at<double>(i, j) = T;
                    }
                }
            }
        }
    }
}

cv::Mat binarizeImage(const cv::Mat& grayImage, const cv::Mat& threshold) {
    cv::Mat binaryImage = cv::Mat::zeros(grayImage.size(), CV_8U); // 创建与输入图像相同尺寸的空白图像

    for (int y = 0; y < grayImage.rows; ++y) {
        for (int x = 0; x < grayImage.cols; ++x) {
            binaryImage.at<uchar>(y, x) = (grayImage.at<uchar>(y, x) <= threshold.at<double>(y, x)) ? 0 : 255;
        }
    }

    return binaryImage;
}




// cv::Mat binarizeImage(const cv::Mat& grayImage, const cv::Mat& localMean, const cv::Mat& localStdDev, int windowSize, double sigma_global_min, double sigma_global_max) {

//     CV_Assert(!grayImage.empty() && grayImage.channels() == 1);
//     if (grayImage.depth() != CV_8U) {
//         cerr << "Error: Image depth is not CV_8U" << endl;
//         exit(EXIT_FAILURE); // Or handle the error as appropriate
//     }
//     // Binarize image using local statistics
//     cv::Mat binaryImage = cv::Mat::zeros(grayImage.size(), CV_8U);
//     double m_g = cv::mean(grayImage)[0];
//     double range = sigma_global_max - sigma_global_min;
//     cout << "Starting binarization process..." << endl;
//     for (int y = 0; y < grayImage.rows; ++y) {
//         for (int x = 0; x < grayImage.cols; ++x) {
//             double m_w = localMean.at<double>(y, x);
//             double sigma_w = localStdDev.at<double>(y, x);
//             if (sigma_w < 0) {
//                 cerr << "Error: Negative standard deviation encountered at (" << y << ", " << x << ")" << endl;
//                 exit(EXIT_FAILURE); // Or handle the error as appropriate
//             }

//             double sigma_adaptive = (sigma_w - sigma_global_min) / (sigma_global_max - sigma_global_min);
//             double denominator = (m_g + sigma_w) * (sigma_adaptive + sigma_w);
//             if (denominator == 0) {
//                 cerr << "Error: Division by zero at (" << y << ", " << x << ")" << endl;
//                 exit(EXIT_FAILURE); // Or handle the error as appropriate
//             }

//             double T = m_w - (m_w * m_w - sigma_w) / denominator;
//             binaryImage.at<uchar>(y, x) = (grayImage.at<uchar>(y, x) < T) ? 0 : 255;

//             // Only print for the first few pixels to avoid cluttering the console
//             if (y < 5 && x < 5) {
//                 cout << "Threshold T at (" << y << ", " << x << ") is " << T << endl;
//             }
//         }
//     }
//     cout << "Binarization process completed successfully." << endl;
//     return binaryImage;
// }


// cv::Mat binarizeImage(const cv::Mat& grayImage, const cv::Mat& localMean, const cv::Mat& localStdDev, double sigmaMin, double sigmaMax) {
//     CV_Assert(!grayImage.empty() && grayImage.channels() == 1);

//     double mg = cv::mean(grayImage)[0]; // 全局均值
//     cv::Mat binaryImage = cv::Mat::zeros(grayImage.size(), CV_8U);

//     for (int y = 0; y < grayImage.rows; ++y) {
//         for (int x = 0; x < grayImage.cols; ++x) {
//             double mW = localMean.at<double>(y, x); // 窗口的均值
//             double sigmaW = localStdDev.at<double>(y, x); // 窗口的标准偏差
//             double sigmaAdaptive = (sigmaW - sigmaMin) / (sigmaMax - sigmaMin); // 自适应标准偏差

//             double T = mW - (mW * mW - sigmaW) / ((mg + sigmaW) * (sigmaAdaptive + sigmaW));
//             binaryImage.at<uchar>(y, x) = (grayImage.at<uchar>(y, x) < T) ? 0 : 255;
//         }
//     }

//     return binaryImage;
// }

// void customBinarization(const cv::Mat& src, cv::Mat& dst, int windowSize) {
//     CV_Assert(src.depth() == CV_8U);  
//     dst = cv::Mat::zeros(src.size(), src.type());
//     double epsilon = 1e-5;

//     for (int y = 0; y < src.rows; y++) {
//         for (int x = 0; x < src.cols; x++) {
//             int x0 = std::max(0, x - windowSize / 2);
//             int y0 = std::max(0, y - windowSize / 2);
//             int x1 = std::min(src.cols - 1, x + windowSize / 2);
//             int y1 = std::min(src.rows - 1, y + windowSize / 2);

//             
//             cv::Mat localArea = src(cv::Range(y0, y1), cv::Range(x0, x1));

//             double minVal, maxVal;
//             cv::minMaxLoc(localArea, &minVal, &maxVal);

//             
//             double contrast = (maxVal - minVal) / (maxVal + minVal + epsilon);

//             
//             double threshold = (maxVal + minVal) / 2;

//             dst.at<uchar>(y, x) = src.at<uchar>(y, x) > threshold ? 255 : 0;
//         }
//     }
// }