/* Requiered includes */
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <string>

/* Function Headers */
static std::vector<int>		detect(cv::Mat frame);
static cv::Point			findPupil(cv::Mat eye);

/* Global variables */
cv::VideoCapture capture;
SDL_DisplayMode screenMode;
cv::CascadeClassifier face_cascade;
cv::CascadeClassifier eyes_cascade;
int mouseX = 0, mouseY = 0;
bool enabled = false;

/**
 * Initializes and prepare the eye tracker
 * structure for later usages
 * 
 * @return	True or False if initialization was successful
 * */
bool tracker_init(void)
{
	// Initializes pre-made cascades models
	cv::String face_cascade_name = "assets/haarcascade_frontalface_alt.xml";
	cv::String eyes_cascade_name = "assets/haarcascade_eye_tree_eyeglasses.xml";
	if (!face_cascade.load(face_cascade_name) || !eyes_cascade.load(eyes_cascade_name))
	{
		std::cout << "Error while loading cascades\n";
		return false;
	}

	// Initialize SDL for mouse events
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
		return false;
	}

	// Get the screen information and get center
	if (SDL_GetDesktopDisplayMode(0, &screenMode) != 0)
	{
		std::cout << "Failed to get screen information: " << SDL_GetError() << std::endl;
		return (SDL_Quit(), false);
	}

	// Prepares the camera
	int camera_device = 0;
	capture.open(camera_device);
	if (!capture.isOpened())
	{
		std::cout << "--(!)Error opening video capture\n";
		return (SDL_Quit(), false);
	}

	enabled = true;
	return true;
}

/**
 * Stops and free memory of eyes trackers
 * */
void tracker_stop(void)
{
	enabled = false;
	if (capture.isOpened())
		capture.release();
	SDL_Quit();
}

/**
 * Retrieve the current eye tracker data available
 * 
 * @warning	-1 indicates error & -2 indicates not enabled
 * @return	Array of eyes related values (X, Y, Direction)
 * */
std::vector<int> tracker_getData(void)
{
	// If not ready, it returns -2
	if (!enabled) return {-2, -2, -2};

	// Get the current frame
	cv::Mat frame;
	if (!capture.read(frame)) return {-1, -1, -1};
	if (frame.empty()) return {-1, -1, -1};

	// Convert frame to grayscale
	cv::Mat frame_gray;
	cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);

	// Detect and retrieve
	return detect(frame_gray);
}

/**
 * Searchs and tries to detect for eyes
 * within the given frame. Otherwise returns
 * vector of negative values to indicate
 * none was found!
 * 
 * @return	Vector of 3 ints (X, Y, Direction)
 * */
static std::vector<int> detect(cv::Mat frame_gray)
{
	//-- Convert frame_gray to HUV color space
	cv::Mat frame_hsv;
	cv::cvtColor(frame_gray, frame_hsv, cv::COLOR_GRAY2BGR);
	cv::cvtColor(frame_hsv, frame_hsv, cv::COLOR_BGR2HSV);

	//-- Define lower and upper thresholds for dark eye color in HUV space
	cv::Scalar lower_threshold(0, 0, 0); // Lower threshold for dark eye color
	cv::Scalar upper_threshold(180, 255, 30); // Upper threshold for dark eye color

	//-- Apply the HUV filter to isolate dark eye color regions
	cv::Mat filtered_frame;
	cv::inRange(frame_hsv, lower_threshold, upper_threshold, filtered_frame);

	//-- Detect faces
	std::vector<cv::Rect> faces;
	face_cascade.detectMultiScale(frame_gray, faces);

	if (faces.empty())
		return {-1, -1, -1}; // No face detected

	for (const auto& face : faces)
	{
		//-- In each face, detect eyes
		cv::Mat faceROI = frame_gray(face);
		cv::Mat filtered_faceROI = filtered_frame(face);

		std::vector<cv::Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes);

		if (eyes.empty())
			continue; // No eyes detected, move to the next face

		for (const auto& eye : eyes)
		{
			cv::Mat eyeROI = faceROI(eye);
			cv::Mat filtered_eyeROI = filtered_faceROI(eye);

			cv::Point pupil = findPupil(filtered_eyeROI);
			cv::Point gazeDirection = cv::Point(eyeROI.cols / 2, eyeROI.rows / 2) - pupil;

			if (pupil.x < 0 || pupil.y < 0)
				pupil = cv::Point(-1, -1);

			int gaze = 0;
			if (abs(gazeDirection.x) > abs(gazeDirection.y))
			{
				// Horizontal direction
				gaze = gazeDirection.x > 0 ? 3 : 4; // 3: Left, 4: Right
			}
			else
			{
				// Vertical direction
				gaze = gazeDirection.y > 0 ? 1 : 2; // 1: Up, 2: Down
			}

			return {pupil.x, pupil.y, gaze};
		}
	}

	return {-1, -1, -1}; // Return -1 when no eye detected
}

/**
 * Searches for pupils and approximate position
 * 
 * @return	Point of position of found pupil
 * */
static cv::Point findPupil(cv::Mat eye)
{
	// Apply adaptive thresholding
	cv::Mat bw;
	cv::adaptiveThreshold(eye, bw, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 15, 10);

	// Apply morphological operations for noise removal
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
	cv::morphologyEx(bw, bw, cv::MORPH_OPEN, kernel);

	// Find the center of gravity
	cv::Moments mu = cv::moments(bw, true);
	cv::Point center(mu.m10 / mu.m00, mu.m01 / mu.m00);

	return center;
}
