/*
By downloading, copying, installing or using the software you agree to this
license. If you do not agree to this license, do not download, install,
copy or use the software.

                          License Agreement
               For Open Source Computer Vision Library
                       (3-clause BSD License)

Copyright (C) 2013, OpenCV Foundation, all rights reserved.
Third party copyrights are property of their respective owners.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  * Neither the names of the copyright holders nor the names of the contributors
    may be used to endorse or promote products derived from this software
    without specific prior written permission.

This software is provided by the copyright holders and contributors "as is" and
any express or implied warranties, including, but not limited to, the implied
warranties of merchantability and fitness for a particular purpose are
disclaimed. In no event shall copyright holders or contributors be liable for
any direct, indirect, incidental, special, exemplary, or consequential damages
(including, but not limited to, procurement of substitute goods or services;
loss of use, data, or profits; or business interruption) however caused
and on any theory of liability, whether in contract, strict liability,
or tort (including negligence or otherwise) arising in any way out of
the use of this software, even if advised of the possibility of such damage.
*/


#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
using namespace cv;


int main(int argc, char *argv[]) {

    int markers_x            = 3;
    int markers_y            = 2;
    float marker_length      = 600;      
    float marker_separation  = 50;
    String out               = "board.jpg";


    const aruco::PREDEFINED_DICTIONARY_NAME dictionary_name = aruco::DICT_4X4_50;
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(dictionary_name);

    Ptr<aruco::GridBoard> board = aruco::GridBoard::create(markers_x, markers_y, marker_length ,marker_separation , dictionary);

    const int margins= 20;
    Size img_size;
    img_size.width  =  10092;
    img_size.height =  7128;

    //img_size.width  = markers_x * (marker_length + marker_separation) - marker_separation + 2 * margins;
    //img_size.height = markers_y * (marker_length + marker_separation) - marker_separation + 2 * margins;
    // show created board
    Mat board_img;
    std::cout<<"aa"<<std::endl;
    board->draw(img_size, board_img, margins);

    imwrite(out, board_img);
    imshow("board", board_img);
    waitKey(0);


    return 0;
}
