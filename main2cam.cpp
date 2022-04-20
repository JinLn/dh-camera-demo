//
// Created by jinln on 2022/4/19.
//

#include <unistd.h>
#include <iostream>
#include <ctime>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "easylogging++/easylogging++.h"
#include "dh_camera.h"

using namespace std;
void Savestereoimg(cv::Mat leftimg, cv::Mat rightimg);

int main() {
    bool isSaveimg=false;
    char getchar;
    cout<<"Do you want to save the image？[y/n]"<<endl;
    cin>>getchar;
    if(getchar == 'y'){
        isSaveimg=true;
    }

    DHCamera camleft = DHCamera();
    DHCamera camright = DHCamera();

    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    el::Loggers::addFlag(el::LoggingFlag::MultiLoggerSupport);

    LOG(INFO) << "Waiting for camera...";
    // 使用相同参数配置
    camleft.OpenCamera("KN0210100034", "../config/KN0210100035.txt");
    camright.OpenCamera("KN0210100035", "../config/KN0210100035.txt");

    LOG(INFO) << "Connet camera";
    camleft.StartStream();
    camright.StartStream();

    cv::Mat imgleft, imgright, imghconcat;
    uint32_t frame_count_left = 0, frame_count_right = 0;
    uint32_t last_second_frame_cont_left = 0, last_second_frame_cont_right = 0;
    time_t start_time = time(nullptr), end_time;
    for (uint32_t fps_left = 0, fps_right = 0; cv::waitKey(1) != 'q'; ++frame_count_left, ++frame_count_right) {
        if (!camleft.GetImage(imgleft)) {
            --frame_count_left; // 都要就减一次
            --frame_count_right;
            continue;
        }
        if (!camright.GetImage(imgright)) {
            --frame_count_left; // 都要就减一次
            --frame_count_right;
            continue;
        }
        /*保存图像置为true*/
        if(isSaveimg){
            try{
                Savestereoimg(imgleft, imgright);
            } catch (...) {
                LOG(INFO) << "Save img error...";
            }
        }

        try {
            cv::hconcat(imgleft, imgright, imghconcat);
            cv::putText(imghconcat,
                        std::to_string(frame_count_left) + " FPS: " + std::to_string(fps_left),
                        cv::Point(0, 24),
                        cv::FONT_HERSHEY_DUPLEX,
                        1,
                        cv::Scalar(0, 255, 0),
                        1,
                        false);
            cv::putText(imghconcat,
                        std::to_string(frame_count_right) + " FPS: " + std::to_string(fps_right),
                        cv::Point(imghconcat.cols / 2, 24),
                        cv::FONT_HERSHEY_DUPLEX,
                        1,
                        cv::Scalar(0, 0, 255),
                        1,
                        false);
            cv::imshow("CAM_Double", imghconcat);
        } catch (...) {
            LOG(INFO) << "Image error...";
        }

        end_time = time(nullptr);
        if (end_time - start_time == 1) {
            fps_left = frame_count_left - last_second_frame_cont_left;
            last_second_frame_cont_left = frame_count_left;
            fps_right = frame_count_right - last_second_frame_cont_right;
            last_second_frame_cont_right = frame_count_right;
            start_time = time(nullptr);
        } else if (end_time - start_time > 1) {
            last_second_frame_cont_left = frame_count_left;
            last_second_frame_cont_right = frame_count_right;
            start_time = time(nullptr);
        }


    }
    imgleft.release();
    imgright.release();
    camleft.StopStream();
    camright.StopStream();
    camleft.CloseCamera();
    camright.CloseCamera();
    return 0;
}

int i=0;
void Savestereoimg(cv::Mat leftimg, cv::Mat rightimg){
    char nameleft[150] = {},nameright[150] = {};
    string folderPathl = "../saveimg/leftimg";
    string folderPathr = "../saveimg/rightimg";
    string commandl,commandr;
    commandl = "mkdir -p " + folderPathl;
    commandr = "mkdir -p " + folderPathr;
    system(commandl.c_str());
    system(commandr.c_str());

    sprintf(nameleft, "../saveimg/leftimg/%06d.png", i);
    sprintf(nameright, "../saveimg/rightimg/%06d.png", i++);
    imwrite(nameleft, leftimg);
    imwrite(nameright, rightimg);
    LOG(INFO) << "Save stereo images "<< nameleft<<"  "<<nameright;
}
