#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

VideoCapture cam(0);
/// Function header
void thresh_callback(int, void* );

/** @function main */
int main(  )
{
    while (true) {
        cam >> src;

        /// Convert image to gray and blur it
        cvtColor( src, src_gray, CV_BGR2GRAY );
        blur( src_gray, src_gray, Size(3,3) );

        /// Create Window
        namedWindow( "Source", CV_WINDOW_AUTOSIZE );
        imshow( "Source", src );

        createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
        thresh_callback( 0, 0 );
        if (waitKey(1)==27)break;
    }
    /// Load source image and convert it to gray



}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
    Mat src_copy = src.clone();
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using Threshold
    threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
    imshow("threshold_output",threshold_output);

    /// Find contours
    findContours( threshold_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Find the convex hull object for each contour
    vector<vector<Point> >hull( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    {  convexHull( Mat(contours[i]), hull[i], false ); }

    /// Draw contours + hull results
    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );

    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
    }

    /// Show in a window
    namedWindow( "Hull demo", CV_WINDOW_AUTOSIZE );
    imshow( "Hull demo", drawing );
}
