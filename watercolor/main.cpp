#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "WaterColor.h"
#include "ToolBox.h"
#include "Debug.h"
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    char *dir = getcwd(NULL, 0);
    printf("Current dir: %s\n", dir);

    std::string basePath, outputPath;
    std::string inputPath = argv[1];

    std::string inputFilename;
    size_t sep = inputPath.find_last_of("\\/");
    
    inputFilename = inputPath.substr(sep + 1, inputPath.size() - sep - 1);
    basePath = inputPath.substr(0, sep + 1);

    outputPath = basePath + "out_" + inputFilename;
    
    const int dir_err = ::mkdir((basePath + inputFilename + "_process").c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err)
    {
        Debug() << "Error creating directory";
    }

    // read picture
    cv::Mat input = cv::imread(inputPath), output;
    Debug() << "img size: " << input.rows << " " << input.cols;

    // process
    WaterColor watercolor(basePath + inputFilename + "_process/");
    cv::imwrite(basePath + "process/src.jpg", input);
    watercolor.deal(input, output);

    // show & store result
    cv::imwrite(outputPath, output);
    cv::imwrite(basePath + "process/dst.png", output);
    
    cv::imshow("src", input);
    cv::imshow("dst", output);
    cv::waitKey();
}
