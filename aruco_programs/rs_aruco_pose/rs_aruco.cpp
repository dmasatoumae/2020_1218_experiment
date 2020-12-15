#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

bool aruco_detect(cv::Mat image, cv::Vec3d &tvec, cv::Vec3d &rvec)
{   
    cv::FileStorage fs("./calibration_params_D455.yml", cv::FileStorage::READ);
    FILE *fp;
    cv::Mat camera_matrix, dist_coeffs;
    fs["camera_matrix"] >> camera_matrix;
    fs["distortion_coefficients"] >> dist_coeffs;
    bool detect_success = false;
    //std::cout << "camera_matrix\n" << camera_matrix << std::endl;
    //std::cout << "\ndist coeffs\n" << dist_coeffs << std::endl;
    
    int wide = 1280;
    int height = 720;

    float degree = 180/3.141592;
	cv::Point2f center;
	std::vector<cv::Point2f> pt;
    std::vector<std::vector<cv::Point2f> > corners;
    std::vector<int> ids;
    cv::Ptr<cv::aruco::DetectorParameters> parameters;

    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
cv::Ptr<cv::aruco::GridBoard> board = cv::aruco::GridBoard::create(3, 2, 0.036, 0.004, dictionary);
    
    cv::Mat imageCopy;
    //image = cv::imread("./image.jpg");
    image.copyTo(imageCopy);

    cv::aruco::detectMarkers(image, dictionary, corners, ids);

    if (ids.size() > 0) {
        cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
        //cv::Vec3d rvec, tvec;
        int valid = estimatePoseBoard(corners, ids, board, camera_matrix, dist_coeffs, rvec, tvec);
        // if at least one board marker detected
        if(valid > 0)
            detect_success = true;
            cv::aruco::drawAxis(imageCopy, camera_matrix, dist_coeffs, rvec, tvec, 0.1);
            std::cout << "rvec\n" << rvec*degree << std::endl;
            std::cout << "\ntvec\n" << tvec << std::endl;
    }

    if (image.empty() == true) {
		// 画像データが読み込めなかったときは終了する

	}

    cv::imshow("out", imageCopy);
    cv::waitKey(1);

    //cv::imshow("in", image);
    return detect_success;
}

int main(int argc, char *argv[])try
{
    std::cout<<"0"<<std::endl;
    cv::Vec3d tvec, rvec;
    rs2::pipeline pipe;
    pipe.start();
    std::cout<<"1"<<std::endl;
    while(cv::waitKey(1) < 0){
        std::cout<<"2"<<std::endl;
        rs2::frameset frames = pipe.wait_for_frames();
        rs2::frame color_frame = frames.get_color_frame();
        cv::Mat color(cv::Size(1280, 720), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);
        cv::cvtColor(color, color, cv::COLOR_RGB2BGR);
        bool detect_success = aruco_detect(color, tvec, rvec);
        cv::imshow("color", color);
        //cv::imshow("aruco",color_mat);

    }
    return EXIT_SUCCESS;
}

catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
