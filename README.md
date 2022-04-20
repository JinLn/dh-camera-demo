# by Jinln 2022/4/19 18:01:43

# 1. Prerequisites

## C++11 or C++0x Compiler
We use the new thread and chrono functionalities of C++11.

## OpenCV
We use [OpenCV](http://opencv.org) to manipulate images and features. Dowload and install instructions can be found at: http://opencv.org. **Required at leat 3.0. Tested with OpenCV 4.5.4**.
Please modify the program in the "cmakelists.txt" text
```
set(OpenCV_DIR /home/jinln/installProgram/opencv-4.5.4/build)
```
## 相机驱动 Galaxy_Linux-x86_Gige
[DaHeng](https://www.daheng-imaging.com/index.php?m=content&c=index&a=lists&catid=59&czxt=9&sylx=21&syxj=44#mmd)
下载**Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.2.2112.9071.tar.gz**,并提取
```
./Galaxy_camera.run
```
Please modify the program in the "cmakelists.txt" text
```
set(DH_CAM_PATH /home/jinln/installProgram/Galaxy_Linux-x86_Gige-U3_32bits-64bits_1.2.2112.9071/Galaxy_camera)
```

# 3. Building examples
```
cd dh-camera-demo
mkdir build
cd build
cmake ..
make -j8
```

# 4. Examples
1. 使用大恒相机采集图像的程序，可以采集单目或者双目图像
2. 所有的参数关于相机的参数都可以“../config/KN*.txt”文本中修改，具体修改选项请看“GlaxyView”中配置的选项
例如：ExposureAuto	Continuous/Off/Once
3. 硬件触发或软件触发也是在“../config/KN*.txt”文本中修改

```
./dh_camera_demo
./dh_camera_double
```

# 5. Quit examples
按q退出程序

