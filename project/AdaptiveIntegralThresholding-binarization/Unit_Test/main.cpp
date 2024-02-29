#include <omp.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ThresholdMethods.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;
bool headless = false;
const int BLOCK_SIZE = 21;
const double CONST_C = 3.5;


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

//use OpenCV as a better example
    Mat gray;

    if (src.channels() == 3) {
        cvtColor(src, gray, COLOR_BGR2GRAY);
    } else {
        gray = src;
    }



    Mat blurred;
    GaussianBlur(gray, blurred, Size(5, 5), 0);
    

    Mat bw; 
      
    adaptiveThreshold(blurred, bw, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, BLOCK_SIZE, CONST_C);
    

    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(bw, bw, MORPH_OPEN, kernel);

  
    imshow("threshold_opencv", bw);
    imwrite("threshold_opencv.png", bw);




//my code
    int windowSize = 24 ;
      //now i want to get the runing-time for comparing
    double t = (double)getTickCount();
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

    //Calculate the minimum and maximum local standard deviation of the entire image
    double minLocalStdDev = DBL_MAX;
    double maxLocalStdDev = DBL_MIN;
    #pragma omp parallel for reduction(min:minLocalStdDev) reduction(max:maxLocalStdDev)

    for (int y = 0; y < localStdDev.rows; ++y) {
        for (int x = 0; x < localStdDev.cols; ++x) {
            double stdDevValue = localStdDev.at<double>(y, x);
            if (stdDevValue < minLocalStdDev) minLocalStdDev = stdDevValue;
            if (stdDevValue > maxLocalStdDev) maxLocalStdDev = stdDevValue;
        }
    }


    binaryImage = binarizeImage(src, threshold);
    t = ((double)getTickCount() - t) / getTickFrequency(); 

    cout << "binarizeImage function took " << t << " seconds." << endl;
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
