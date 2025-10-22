#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstring>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#define WIDTH 1920
#define HEIGHT 1080
#define FPS 30
#define DURATION 5
#define TOTAL_FRAMES (FPS * DURATION)
#define PI 3.14159265359

#endif