#include<iostream>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/ximgproc.hpp>


using namespace cv;
using namespace std;



cv::Mat reziseImg(cv::Mat& img)
{
	cv::Mat img_resized;

	cv::resize(img, img_resized, Size(400, 300), cv::INTER_AREA);

	return img_resized;

	//if (img.rows || img.cols > 750) {
	//	float scale_percent = 50.0; // percent of original size
	//	int width = int(img.cols * scale_percent / 100);
	//	int height = int(img.rows * scale_percent / 100);
	//	cout << "Image width: " << img.cols << " Image height: " << img.rows << endl;
	//	cout << "Image resized"  << endl;
	//	cout << (img.rows || img.cols > 750) << endl;
	//	//resize image
	//	cv::resize(img, img_resized, Size(width, height), cv::INTER_AREA);

	//	//return img_resized;
	//}
	//else {
	//	cout << "image OKAY" << endl;
	//	img_resized = img;
	//	//return img;
	//}

	//return img_resized;
	
}




void show(string window_name, cv::Mat img)
{
	cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
	//cv::moveWindow(window_name, 700, 150);
	cv::imshow(window_name, img);
	//cv::resizeWindow(window_name, 600, 600);
	//cout << window_name + " channels: " << img.channels() << endl;
	//cv::waitKey(0);
};





cv::Mat gray_image(cv::Mat& img)
{
	cv::Mat img_gray;
	cv::Mat img_resized;
	img_resized = reziseImg(img);
	cv::cvtColor(img_resized, img_gray, cv::COLOR_BGR2GRAY);
	//show("Gray image", img_gray);

	return img_gray;

}

cv::Mat smoothing(cv::Mat& img)
{	
	cv::Mat img_blur;
		
	cv::blur(gray_image(img), img_blur, Size(7,7));
	//show("Image blurring Averaging", img_blur);	

	return img_blur;
}

cv::Mat smoothingGaussian(cv::Mat& img)
{
	cv::Mat img_GaussBlur;	

	cv::GaussianBlur(gray_image(img), img_GaussBlur, Size(9,9), 3, 2);
	//show("Image Gaussian Blur", img_GaussBlur);

	return img_GaussBlur;
}

cv::Mat smoothingMedian(cv::Mat& img)
{
	cv::Mat img_MedianFilter;

	cv::medianBlur(gray_image(img), img_MedianFilter, 5);
	//show("Image Median Filter", img_MedianFilter);

	return img_MedianFilter;
}


cv::Mat edgeDetection(cv::Mat& img_gray)
{
	cv::Mat img_edges;
	
	cv::Canny(img_gray, img_edges, 35, 150);
	//show("Canny edges", img_edges);
	vector<Mat> contours;
	cv::findContours(img_edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	//cout << "Contours number: " << contours.size() << endl;

	return img_edges;

}


cv::Mat imgGlobalThresholding(cv::Mat& img)
{
	//global thresholding
	cv::Mat img_glob_norm;
	cv::threshold(img, img_glob_norm, 127, 255, cv::THRESH_BINARY);
	//cv::imshow("Global Normalization", img_glob_norm);

	return img_glob_norm;
}

cv::Mat imgOtsuThresholding(cv::Mat& img)
{
	//Otsu's thresholding
	cv::Mat img_Otsu_norm;
	cv::threshold(img, img_Otsu_norm, 0, 255, cv::THRESH_BINARY+cv::THRESH_OTSU);
	//cv::imshow("Otsu Normalization", img_Otsu_norm);

	return img_Otsu_norm;
}


cv::Mat imgNiblackThresholding(cv::Mat& img)
{
	//Niblack's thresholding
	cv::Mat img_Niblack_norm;
	cv::ximgproc::niBlackThreshold(img, img_Niblack_norm, 255, cv::THRESH_BINARY, 35, 0);
	//cv::imshow("Niblack Normalization", img_Niblack_norm);

	return img_Niblack_norm;
}


cv::Mat imgDilate(cv::Mat& img)
{
	cv::Mat img_dilate;
	cv::Mat dilate_kernel = cv::getStructuringElement(cv::MORPH_RECT, Size(2, 2));
	cv::dilate(img, img_dilate, dilate_kernel, Point(-1, -1));

	//cv::namedWindow("Erosion", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Erosion MORPH_RECT and 5x5", img_erode);

	return img_dilate;
}


cv::Mat imgOpen(cv::Mat& img)
{
	cv::Mat img_morphTransOpen;
	cv::Mat open_kernel = cv::getStructuringElement(cv::MORPH_RECT, Size(2, 2));
	
	cv::morphologyEx(img, img_morphTransOpen, cv::MORPH_GRADIENT, open_kernel);

	//cv::namedWindow("Erosion", cv::WINDOW_AUTOSIZE);
	//cv::imshow("Erosion MORPH_RECT and 5x5", img_erode);

	return img_morphTransOpen;
}



vector<Vec3f> imgDetectCircles(cv::Mat& img)
{
	vector<Vec3f> circles_hough;
	cv::HoughCircles(img, circles_hough, cv::HOUGH_GRADIENT, 1.5, 30, 80, 55, 10, 55);
	
	cout << "Coins detected: " << circles_hough.size() << endl;
	
	

	return circles_hough;
}


void drawCircles(cv::Mat img, vector<Vec3f> circles_hough) 
{

	cv::Mat img_draw;
	img_draw = img.clone();

	for (size_t i = 0; i < circles_hough.size(); i++)
	{
		Point center(cvRound(circles_hough[i][0]), cvRound(circles_hough[i][1]));
		int radius = cvRound(circles_hough[i][2]);
		// draw the circle center
		circle(img_draw, center, 5, Scalar(255, 0, 0), -1, 2, 0);
		// draw the circle outline
		circle(img_draw, center, radius, Scalar(0, 0, 255), 3, 2, 0);
	}
	
	show("circles", img_draw);

	


}
















int main() {

	string img_name;
	cout << "Images are located at imgs/ folder. " << endl;
	cout << "Please, type the image name: (max. 20 characters)" << endl;
	cin >> img_name;


	string img_path = "imgs/" + img_name;

	cout << "The path is: " << img_path << endl;

	
	cv::Mat	img = cv::imread(img_path);
	if (img.empty())
	{
		std::cout << "Could not read the image: imgs/" << img_path << std::endl;
		return -1;
	}

	
	cv::Mat img_formated = reziseImg(img);
	

	//show("Original Image", img);
	//show("Resized Image", img_formated);
	


	//**************************************************************//
	//                  Image Smoothing                             //
	//**************************************************************//
	
	cv::Mat img_blur, img_GaussBlur, img_MedianFilter;
	 
	//img_blur = smoothing(img);
	img_GaussBlur = smoothingGaussian(img);
	//img_MedianFilter = smoothingMedian(img);
	//show("Image blurring Averaging", img_blur);
	//show("Image Gaussian Blur", img_GaussBlur);




	//**************************************************************//
	//                  Canny Edge Detection                        //
	//**************************************************************//

	cv::Mat img_blurEdges, img_GaussBlurEdges, img_MedianFilterEdges;

	//img_blurEdges = edgeDetection(img_blur);
	img_GaussBlurEdges = edgeDetection(img_GaussBlur);
	//img_MedianFilterEdges = edgeDetection(img_MedianFilter);

	//show("Canny edges Blur Image", img_blurEdges);
	//show("Canny edges Gauss Smoothing Image", img_GaussBlurEdges);
	//show("Canny edges Median Filter Image", img_MedianFilterEdges);




	//**************************************************************//
	//                  Image Thresholding                          //
	//**************************************************************//

	cv::Mat img_glob_norm, img_Otsu_norm, img_Niblack_norm;

	//img_glob_norm = imgGlobalThresholding(img_GaussBlurEdges);
	img_Otsu_norm = imgOtsuThresholding(img_GaussBlurEdges);
	//img_Niblack_norm = imgNiblackThresholding(img_GaussBlurEdges);
	//show("Global Thresholding", img_glob_norm);
	//show("Otsu Thresholding", img_Otsu_norm);
	//show("Niblack Thresholding", img_Niblack_norm);





	//**************************************************************//
	//                Morphological Transformation                  //
	//**************************************************************//

	cv::Mat img_dilate, img_open;

	//img_dilate = imgDilate(img_glob_norm);
	img_open = imgOpen(img_Otsu_norm);

	//imshow("Dilatation MORPH_RECT and 5x5", img_dilate);
	imshow("Open Morphological Transformation", img_open);

	


	//**************************************************************//
	//                Detecting Circles			                    //
	//**************************************************************//


	vector<Vec3f> circles_hough;
	circles_hough = imgDetectCircles(img_open);
	


	//**************************************************************//
	//                Drawing Circles			                    //
	//**************************************************************//

	
	drawCircles(img_formated, circles_hough);














	cv::waitKey(0);


	
	return 0;



}