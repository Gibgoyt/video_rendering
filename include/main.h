#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <librsvg/rsvg.h>
#include <cairo/cairo.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>

#define WIDTH 1920
#define HEIGHT 1080
#define FPS 30
#define DURATION 5
#define TOTAL_FRAMES (FPS * DURATION)

typedef struct {
    AVFormatContext *fmt_ctx;
    AVCodecContext *codec_ctx;
    AVStream *stream;
    AVFrame *frame;
    AVPacket *pkt;
    struct SwsContext *sws_ctx;
    int frame_count;
} VideoEncoder;

int encoder_init(VideoEncoder *enc, const char *filename);
int encoder_write_frame(VideoEncoder *enc, cairo_surface_t *surface);
void encoder_finish(VideoEncoder *enc);
char* generate_svg(int x_position);
cairo_surface_t* render_svg_to_surface(const char *svg_data);

#endif