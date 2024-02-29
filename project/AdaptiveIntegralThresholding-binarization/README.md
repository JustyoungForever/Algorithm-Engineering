ThresholdApp is an image processing tool designed to convert color or grayscale images into binary images. The program employs an adaptive thresholding method to process the input image and generate a binarized result.

# Installation Instructions
Ensure that your system has CMake and OpenCV libraries installed. You can compile the program with the following commands:
```bash
cmake .
make
```
After compilation, you will obtain the executable file `ThresholdApp`.

# Usage
The basic command line invocation format is:
```bash
./ThresholdApp <image_path>
```
where `<image_path>` is the path to the image file you wish to process.

# Command Line Options
The current version of `ThresholdApp` does not support additional command line options. New features and options may be added in future releases.

# Examples
To process an image named `image.jpg`, you can use the following command:
```bash
./ThresholdApp image.jpg
```
The processed image will be saved at the output location specified by the program.

# Error Messages

### Image Loading Error
- **Error Message:** `"Problem loading image!!!"` 
- **Details:** This message is displayed when the image fails to load.
- **Resolution:** Check that the image path is correct and that the file is accessible by the program.

### Command Line Arguments Error
- **Error Message:** `"Usage: <Executable> <ImagePath>"` 
- **Details:** If the user does not provide the correct command line arguments, the program will output this message.
- **Resolution:** Ensure that the program is invoked with the correct image path argument.

### OpenCV Function Error
- **Error Message:** `"OpenCV error: "` followed by the detailed error message.
- **Details:** This is the error message for an OpenCV exception, which will be followed by specific details of the exception.
- **Resolution:** Debug based on the exception details. This might involve checking the input image to make sure it meets the function’s expectations (e.g., image depth is CV_8U) or checking for other coding errors.

### Index Out of Range Error
- **Error Message:** `"Index out of range!"` 
- **Details:** This error occurs when calculating the window coordinates in the `calcMeanAndStdDev` function and the indices are out of the valid range of the image.
- **Resolution:** This typically means there is an issue with the window size or edge handling of the image. Check that the window size is appropriate and the image edges are handled correctly.

### Negative Standard Deviation Error
- **Error Message:** There is no direct error message, but if a negative value is calculated for the standard deviation, it is set to 0 by `stddev.at<double>(y, x) = variance < 0 ? 0 : sqrt(variance);`.
- **Details:** There is no direct prompt, but standard deviation should not be negative, indicating a calculation issue.
- **Resolution:** Check the formula for calculating variance to ensure there are no programming errors.

# Contact Information
If you have any questions or need assistance, please contact us at the following email: [zhiyuan.pan@uni-jena.de](mailto:zhiyuan.pan@uni-jena.de).