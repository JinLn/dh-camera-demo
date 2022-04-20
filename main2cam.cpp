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
#include <termio.h>
#include <fcntl.h>

using namespace std;

void Savestereoimg(cv::Mat leftimg, cv::Mat rightimg);
int kbhit(void);

int main() {
    bool isCalib = false, isSaveimg = false;
    char getcalibchar, getsavechar;

    cout << "Do you want to obtain images for calibration？[y/n]" << endl;
    cin >> getcalibchar;
    if (getcalibchar == 'y') {
        isCalib = true;
        cout << "Press press 'W' or 'w' to write the image !" << endl;
    }

    if (!isCalib) {
        cout << "Do you want to save images continuously？[y/n]" << endl;
        cin >> getsavechar;
        if (getsavechar == 'y') {
            isSaveimg = true;
        }
        else{
            return 0;
        }
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
        if (kbhit() && isCalib) {
            char ch = getchar();
            if (ch == 'w'||ch == 'W') {
                Savestereoimg(imgleft, imgright);
            }
            if (ch == 'q'||ch == 'Q')
                break;
        }

        /*连续保存图像置为true*/
        if (isSaveimg) {
            Savestereoimg(imgleft, imgright);
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

static int imgnums = 0;

void Savestereoimg(cv::Mat leftimg, cv::Mat rightimg) {
    char nameleft[150] = {}, nameright[150] = {};
    string folderPathl = "../saveimg/leftimg";
    string folderPathr = "../saveimg/rightimg";
    string commandl, commandr;
    commandl = "mkdir -p " + folderPathl;
    commandr = "mkdir -p " + folderPathr;
    system(commandl.c_str());
    system(commandr.c_str());

    sprintf(nameleft, "../saveimg/leftimg/%06d.png", imgnums);
    sprintf(nameright, "../saveimg/rightimg/%06d.png", imgnums++);
    imwrite(nameleft, leftimg);
    imwrite(nameright, rightimg);
    LOG(INFO) << "Save stereo images " << nameleft << "  " << nameright;
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}