#include "WaterColor.h"
#include "ColorAdjustment.h"
#include "SaliencyDistance.h"
#include "Abstraction.h"
#include "WetInWet.h"
#include "HandTremorEffect.h"
#include "EdgeDarkening.h"
#include "Granulation.h"
#include "TurblenceFlow.h"
#include "PigmentVariation.h"
#include "ToolBox.h"
#include "Debug.h"
using namespace cv;

WaterColor::WaterColor(std::string pDir, std::string st) {
    processDir = pDir;
    style = st;
}

void texture(Mat &src, Mat &dst)
{
	vector<Mat> srcs;
	split(src, srcs);
	Mat texture = imread("texture.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat add(src.size(), CV_8U);

	for (int i = 0; i < add.rows; i++)
		for (int j = 0; j < add.cols; j++)
		{
			int x = texture.at<uchar>(i % texture.rows, j % texture.cols);
			x = max(x, 240);
			add.at<uchar>(i, j) = min(x, 255);
		}
	//imshow("add", add);
	//waitKey();
	srcs.push_back(add);
	merge(srcs, dst);
	//imwrite("aaa.png", dst);
}

void WaterColor::deal(Mat &src, Mat &dst)
{
	dst = src.clone();

	// Color transform
	Debug() << "color transform...";
	Mat myColorTransform;
	ColorAdjustment colorAdjustment;
    colorAdjustment.chooseOneStyle(style);   // preprocessed image style
	colorAdjustment.deal(src, myColorTransform);
	imwrite(processDir + "01_colorTransform.jpg", myColorTransform);

	// Saliency
	Debug() << "saliency...";
	Mat mySaliency, myDis;
	SaliencyDistance saliencyDiatance;
	saliencyDiatance.deal(myColorTransform, mySaliency, myDis);
	//    imwrite(path+"process/dis.jpg", myDis*255);
	imwrite(processDir + "02_saliency.jpg", mySaliency);

	// Abstraciton
	Debug() << "abstraction...";
	Mat myAbstraction;
	Abstraction abstraction;
	abstraction.deal(myColorTransform, mySaliency, myDis, myAbstraction);
	imwrite(processDir + "03_abstraciton.jpg", myAbstraction);

	// Add effects
	Debug() << "wet in wet...";
	Mat myWetInWet, myCanny;
	WetInWet wetInWet;
	wetInWet.deal(src, myAbstraction, myWetInWet, myCanny);
	imwrite(processDir + "04_wetinwet.jpg", myWetInWet);

	// Hand tremor
    Mat myHandTremor;
    HandTremorEffect handTremorEffect;
    handTremorEffect.deal(myWetInWet, myHandTremor, myCanny);
    imwrite(processDir + "05_handtremor.jpg", myHandTremor);

	// not implemented
	//    cv::Mat testtest;
	//    EdgeDarkening edgeDarkening;
	//    edgeDarkening.deal(myWetInWet, testtest);
	//    imwrite(processDir + "EdgeDarkening.jpg", testtest);
	//
	//    Granulation granulation;
	//    granulation.deal(myWetInWet, testtest);
	//    imwrite(processDir + "Granulation.jpg", testtest);
	//
	//    TurblenceFlow turblenceFlow;
	//    turblenceFlow.deal(myWetInWet, testtest);
	//    imwrite(processDir + "TurblenceFlow.jpg", testtest);
	//
	//    //After deal
	//    PigmentVariation pigmentVariation;
	//    pigmentVariation.deal(myWetInWet, testtest);
	//    imwrite(processDir + "PigmentVariation.jpg", testtest);
	//
	//    DoubleToImage(myHandTremor, myHandTremor);
	//    DoubleToImage(myHandTremor, myHandTremor);
	//    myHandTremor.convertTo(myHandTremor, CV_8UC3, 255.0);

	Mat myTexture;
	texture(myWetInWet, myTexture);
	imwrite(processDir + "09_texture.png", myTexture);

	dst = myTexture;
}
