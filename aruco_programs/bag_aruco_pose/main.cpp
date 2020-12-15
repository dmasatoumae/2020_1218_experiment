#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <librealsense2/rs.hpp>
#include <iostream>
#include <vector>
#include <numeric>
#include "./matplotlibcpp.h"

namespace plt = matplotlibcpp;

void accuracy_analysis(std::vector<double> target)
{
    double size;
    double mean;
    double var;
    double sd;
    double RMSE;
    double true_value = 0.04123;
    double dif_sum = 0.0;

    size = target.size();
    mean = std::accumulate(target.begin(), target.end(), 0.0) / target.size();
    var  = (std::inner_product(target.begin(), target.end(), target.begin(), 0.0) - mean * mean * size)/ (size - 1.0);
    sd = std::sqrt(var);
    for(int i = 0; i < target.size(); i++)
    {
        double dif = target[i]-true_value;
        dif_sum += dif*dif;    
    }
    RMSE = std::sqrt(dif_sum/target.size());
    std::cout<<"size = "<<size<<std::endl;
    std::cout<<"mean = "<<mean<<std::endl;
    std::cout<<"var  = "<<var<<std::endl;
    std::cout<<"sd   = "<<sd<<std::endl;
    std::cout<<"RMSE   = "<<RMSE<<std::endl;
    plt::plot(target);
    plt::save("./sample1.png");

}

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
            std::cout << "rvec\n" << rvec << std::endl;
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

void realsense_bag_play()
{
    int frame_number = 0;
    std::string bag_name = "/projects/20201210_182443.bag";
    std::vector<double> target;
    cv::Vec3d tvec, rvec;
    rs2::frameset frames;
    rs2::pipeline pipe;
    rs2::config cfg;
    cfg.enable_device_from_file(bag_name.c_str());
    pipe.start(cfg);
    //cv::namedWindow("out",cv::WINDOW_AUTOSIZE);
    while(frame_number<100)
    {
        if(pipe.poll_for_frames(&frames))
        {
            rs2::frameset frame = pipe.wait_for_frames();
            rs2::frame color_frame = frame.get_color_frame();
            cv::Mat color_mat = cv::Mat(cv::Size(1280, 720), CV_8UC3, const_cast<void *>( color_frame.get_data()));
            cv::cvtColor(color_mat, color_mat, cv::COLOR_RGB2BGR);
            bool detect_success = aruco_detect(color_mat, tvec, rvec);
            //cv::imshow("Color", color_mat);
            frame_number++;
            if (detect_success){
                target.push_back(tvec[1]);
            }
        }
    }
    pipe.stop();
    accuracy_analysis(target);
}


int main(int argc,char* argv[])
{
    realsense_bag_play();

    return 0;
}



    
