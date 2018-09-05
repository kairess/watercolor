#include <iostream>
#include <unistd.h>
#include "WaterColor.h"
#include "ToolBox.h"
#include "Debug.h"
#include <opencv2/opencv.hpp>

int main(int argc, char *argv[])
{
    char *dir = getcwd(NULL, 0);
    printf("Current dir: %s\n", dir);

    std::string base_path = "/Users/visualcamp/Downloads/";

    std::string inName = "src-small.jpg";
    //    std::string inDir = "/Users/visualcamp/Development/Clanguage/waterColor_original/waterColor/waterColor/src/" + inName;
    std::string inDir = "/Users/visualcamp/Development/iris_tracking/colorlens/eye_00000000.png";
    std::string outName = "output.png";
    std::string outDir = base_path + outName;

    if (argc == 3)
    {
        inDir = std::string(argv[1]);
        outDir = std::string(argv[2]);
    }

    //read picture
    cv::Mat input = cv::imread(inDir), output;
    Debug() << "img size: " << input.rows << " " << input.cols;

    //process
    WaterColor watercolor;
    cv::imwrite(base_path + "process/src.jpg", input);
    watercolor.deal(input, output);

    //show & store result
    cv::imshow("src", input);
    cv::imshow("dst", output);
    cv::imwrite(outDir, output);
    cv::imwrite(base_path + "process/dst.png", output);
    cv::waitKey();
}
