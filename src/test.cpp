#include <iostream>
#include <limits.h>
#include <opencv2/opencv.hpp>
#include <string>

#define PI 3.14159265

using namespace cv;

Mat initial_frame, hsv_frame, gauss_frame, erosion_frame, dilation_frame,
    gray_frame, gray_frame_1, canny_frame, out_frame;
Mat element;
VideoCapture cap;

int thresh_l_R = 0, thresh_l_G = 112, thresh_h_R = 255;
int thresh_l_B = 0, thresh_h_B = 51, thresh_h_G = 255;
int canny_low_thresh = 0, canny_ratio = 3, canny_kernel_size = 3;
int hl_thresh_detect = 50, hl_min_line_length = 50, hl_max_line_gap = 10;

int x, y, k;

std::vector<Vec4i> lines;

void op_image(std::string s, int obj);
void op_video(std::string s, int obj);
void img_proc_gate();
void img_proc_buckets();
void img_proc_yellow_flare();
void img_proc_red_flare();
void show();

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << "Usage: \n ./test a path b \n a is 1 for an image and 0 for a "
                 "video \n b is \n 1 for gate\n 2 for buckets\n 3 for yellow_flare\n 4 "
                 "for red_flare\n"
              << '\n';
	return 0;
  }
  int n = atoi(argv[1]);
  //---- thresholding window--------------
  namedWindow("Threshold", WINDOW_NORMAL);
  createTrackbar("Low B or H", "Threshold", &thresh_l_B, 255, NULL);
  createTrackbar("High B or H", "Threshold", &thresh_h_B, 255, NULL);
  createTrackbar("Low G or S", "Threshold", &thresh_l_G, 255, NULL);
  createTrackbar("High G or S", "Threshold", &thresh_h_G, 255, NULL);
  createTrackbar("Low R or V", "Threshold", &thresh_l_R, 255, NULL);
  createTrackbar("High R or V", "Threshold", &thresh_h_R, 255, NULL);
  createTrackbar("canny low threshold", "Threshold", &canny_low_thresh, 255,
                 NULL);
  createTrackbar("canny ratio", "Threshold", &canny_ratio, 20, NULL);
  createTrackbar("kernel size", "Threshold", &canny_kernel_size, 20, NULL);

  createTrackbar("HL thresh detect", "Threshold", &hl_thresh_detect, 255, NULL);
  createTrackbar("HL min Line Length ", "Threshold", &hl_min_line_length, 255,
                 NULL);
  createTrackbar("HL max line Gap", "Threshold", &hl_max_line_gap, 255, NULL);
  //------end thresholding window-------------

  if (n) {

    op_image(argv[2], atoi(argv[3]));
  }
  if (!n) {
    op_video(argv[2], atoi(argv[3]));
  }
  return 0;
}

void op_image(std::string s, int obj) {

  for (;;) {
    initial_frame = imread(s, CV_LOAD_IMAGE_COLOR);

    if (!initial_frame.data) {
      std::cout << "Could not open or find the image" << std::endl;
      return;
    }
    //----- Image Processing ---------
    switch (obj) {
    case 1:
      img_proc_gate();
      break;
    case 2:
      img_proc_buckets();
      break;
    case 3:
      img_proc_yellow_flare();
      break;
    case 4:
      img_proc_red_flare();
      break;
    }

    //-----End Image Processing ----------
    show();
    if (waitKey(30) >= 0)
      break;
  }
}

void op_video(std::string s, int obj) {

  cap.open(s);
  if (!cap.isOpened()) {
    std::cout << "Could not open or find the image" << std::endl;
    return;
  }
  for (;;) {
    cap >> initial_frame;

    //----- Image Processing ---------
    switch (obj) {
    case 1:
      img_proc_gate();
      break;
    case 2:
      img_proc_buckets();
      break;
    case 3:
      img_proc_yellow_flare();
      break;
    case 4:
      img_proc_red_flare();
      break;
    }
    //-----End IMage Processing ----------
    show();
    if (waitKey(500) >= 0)
      break;
  }
}


void img_proc_gate() { std::cout << "Processing gate " << std::endl; }
void img_proc_buckets() { std::cout << "Processing buckets " << std::endl; }
void img_proc_yellow_flare() { std::cout << "Processing yellow flare " << std::endl; }
void img_proc_red_flare() { std::cout << "Processing red flare " << std::endl; }


void show() {
  imshow("Input Frame", initial_frame);
  imshow("HSV image", hsv_frame);
  imshow("Gray image", gray_frame);
  // imshow("Output image", canny_frame);
}
