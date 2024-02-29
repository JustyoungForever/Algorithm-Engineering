# Testing Report for Image Processing Functions

## Introduction

This document outlines the testing process and results for key image processing functions developed as part of our project. The functions under test include `calcThresholdMatrix`, `calcMeanAndStdDev`, and `binarizeImage`. These functions are critical for image analysis and processing tasks, such as calculating local mean and standard deviation within a window and binarizing images based on dynamically calculated threshold matrices.

## Test Environment

The tests were executed in a Linux environment on a laptop (model: LAPTOP-QSQ08ER3). The testing framework used is Catch2, which provides a rich set of features for unit testing C++ applications.

## Test Cases

Three primary test cases were designed to validate the correctness and functionality of the aforementioned image processing functions:

1. **Threshold Matrix Calculation** (`calcThresholdMatrix`): This test verifies that the function correctly calculates the threshold matrix based on local mean, local standard deviation, and global image mean values. The test ensures that the calculated threshold values match the expected logic derived from the function's algorithm.

2. **Mean and Standard Deviation Calculation** (`calcMeanAndStdDev`): This test checks whether the function accurately computes the mean and standard deviation for each pixel within a specified window size. The test is performed on a uniform image to ensure that the expected mean and standard deviation values are correctly calculated.

3. **Image Binarization** (`binarizeImage`): The final test case assesses the binarization function's ability to convert a grayscale image into a binary image using a pre-calculated threshold matrix. It ensures that pixels are correctly classified as either 0 or 255 based on their values relative to the threshold.

## Test Execution and Results

The test suite was compiled and executed using the following command:

```bash
./tests


Inside calcThresholdMatrix function
Inside calcMeanAndStdDev function
Sum and sumSq matrices should be of size: 11x11
Mean and standard deviation calculated successfully.
===============================================================================
All tests passed (325 assertions in 3 test cases)
```
## Analysis:
The test suite successfully passed all test cases, validating the correctness of the image processing functions. A total of 325 assertions were made across 3 test cases, all of which were confirmed to be correct. This comprehensive testing ensures that the functions perform as expected under various conditions and with different input data.

