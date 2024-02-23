#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ThresholdMethods.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;
bool headless = false;



int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <ImagePath>" << endl;
        return -1;
    }

    cout << "Loading image..." << endl;
    Mat src = imread(argv[1], IMREAD_GRAYSCALE);

    if (src.empty()) {
        cerr << "Problem loading image!!!" << endl;
        return -1;
    } else {
        cout << "Image loaded successfully." << endl;
    }

    Mat gray;

    if (src.channels() == 3) {
        cvtColor(src, gray, COLOR_BGR2GRAY);
    } else {
        gray = src;
    }



    // 平滑滤波
    Mat blurred;
    GaussianBlur(gray, blurred, Size(5, 5), 0);
    
    // 自适应阈值
    Mat bw;
    int blockSize = 21;  // 假设文本较大，可以尝试更大或更小的值
    double C = 3.5;        // 可能需要微调
    adaptiveThreshold(blurred, bw, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, blockSize, C);
    
    // 形态学操作：使用一个小的矩形核进行开运算
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(bw, bw, MORPH_OPEN, kernel);

    // 显示和保存结果
    imshow("threshold_opencv", bw);
    imwrite("threshold_opencv.png", bw);
    //! [bin_1]

    // //! [bin_2]
    // cv::Mat bw2 = cv::Mat::zeros(gray.size(), CV_8UC1);
    // thresholdIntegral(gray, bw2);
    // // Save binary image
    // cv::imwrite("threshold_integral.png", bw2);

    // // Show binary image
    // if (headless)
    // {
    //     cv::imwrite("threshold_integral.png", bw2);
    // }
    // else
    // {
    //     cv::imshow("threshold_integral", bw2);
    // }
    // //! [bin_2]

    // // Check if image loaded successfully
    // if (gray.empty()) {
    //     std::cerr << "Failed to load image." << std::endl;
    //     return -1;
    // }
    // //! [bin_custom]
    // cv::Mat bwCustom;
    // int windowSize = src.rows/ 24; // 或者根据需要选择合适的窗口大小
    // customBinarization(gray, bwCustom, windowSize);
    // cv::imwrite("custom_binarization.png", bwCustom);

    // if (!headless) {
    //     cv::imshow("Custom Binarization", bwCustom);
    // }
    // //! [bin_custom]
    int windowSize = 20 ;
 
    Mat localMean, localStdDev, binaryImage;
    // Calculate local mean and standard deviation
    cout << "Calculating mean and standard deviation..." << endl;
    try {
        calcMeanAndStdDev(src, localMean, localStdDev, windowSize);
        cout << "Local mean and standard deviation calculated." << endl;
    } catch (const cv::Exception& e) {
        cerr << "OpenCV error: " << e.what() << endl;
        return -1;
    }   
    Mat threshold;
    calcThresholdMatrix(src, localMean, localStdDev, threshold);  

    // // Estimate global standard deviation
    // Scalar globalMean, globalStdDev;
    // meanStdDev(gray, globalMean, globalStdDev);
    // // Heuristics for sigma_min and sigma_max
    // double sigma_global_min = globalStdDev[0] * 0.5;
    // double sigma_global_max = globalStdDev[0] * 1.5;
    //Calculate the minimum and maximum local standard deviation of the entire image
    double minLocalStdDev = DBL_MAX;
    double maxLocalStdDev = DBL_MIN;
    for (int y = 0; y < localStdDev.rows; ++y) {
        for (int x = 0; x < localStdDev.cols; ++x) {
            double stdDevValue = localStdDev.at<double>(y, x);
            if (stdDevValue < minLocalStdDev) minLocalStdDev = stdDevValue;
            if (stdDevValue > maxLocalStdDev) maxLocalStdDev = stdDevValue;
        }
    }

// Now, calculate sigma_global_min and sigma_global_max based on the document's method
    // double sigma_global_min = minLocalStdDev;
    // double sigma_global_max = maxLocalStdDev;
    // // Apply the new binarization method
    // cout << "Binarizing image..." << endl;
    // try {
    //     binaryImage = binarizeImage(src, localMean, localStdDev, sigma_global_min, sigma_global_max);
    //     cout << "Image binarized." << endl;
    // } catch (const cv::Exception& e) {
    //     cerr << "OpenCV error: " << e.what() << endl;
    //     return -1;
    // }
    // Save and display the binary image if not headless
    binaryImage = binarizeImage(src, threshold);
    // Display and save the binary image if not empty
    cout << "Displaying and saving binary image..." << endl;
    if (!binaryImage.empty()) {

        std::ostringstream filenameStream;
        filenameStream << "binary_" << windowSize << ".png";
        std::string filename = filenameStream.str();
        

        cv::imwrite(filename, binaryImage);
        if (!headless) {
            cv::imshow("Binary Image", binaryImage);
            cv::waitKey(0);
        }
    } else {
        cerr << "Error: The binary image is empty." << endl;
    }

    return 0;
}
