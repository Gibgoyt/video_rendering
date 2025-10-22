#include "../include/main.h"

int encoder_init(VideoEncoder *enc, const char *filename) {
    int ret;
    const AVCodec *codec;
    
    avformat_alloc_output_context2(&enc->fmt_ctx, NULL, NULL, filename);
    if (!enc->fmt_ctx) {
        fprintf(stderr, "Could not create output context\n");
        return -1;
    }
    
    codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "H.264 codec not found\n");
        return -1;
    }
    
    enc->stream = avformat_new_stream(enc->fmt_ctx, NULL);
    if (!enc->stream) {
        fprintf(stderr, "Could not create stream\n");
        return -1;
    }
    
    enc->codec_ctx = avcodec_alloc_context3(codec);
    if (!enc->codec_ctx) {
        fprintf(stderr, "Could not allocate codec context\n");
        return -1;
    }
    
    enc->codec_ctx->width = WIDTH;
    enc->codec_ctx->height = HEIGHT;
    enc->codec_ctx->time_base = (AVRational){1, FPS};
    enc->codec_ctx->framerate = (AVRational){FPS, 1};
    enc->codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;
    enc->codec_ctx->gop_size = 10;
    enc->codec_ctx->max_b_frames = 1;
    
    av_opt_set(enc->codec_ctx->priv_data, "preset", "medium", 0);
    av_opt_set(enc->codec_ctx->priv_data, "crf", "23", 0);
    
    ret = avcodec_open2(enc->codec_ctx, codec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not open codec\n");
        return -1;
    }
    
    avcodec_parameters_from_context(enc->stream->codecpar, enc->codec_ctx);
    
    if (!(enc->fmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&enc->fmt_ctx->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open output file\n");
            return -1;
        }
    }
    
    ret = avformat_write_header(enc->fmt_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Could not write header\n");
        return -1;
    }
    
    enc->frame = av_frame_alloc();
    enc->frame->format = enc->codec_ctx->pix_fmt;
    enc->frame->width = WIDTH;
    enc->frame->height = HEIGHT;
    ret = av_frame_get_buffer(enc->frame, 0);
    if (ret < 0) {
        fprintf(stderr, "Could not allocate frame buffer\n");
        return -1;
    }
    
    enc->pkt = av_packet_alloc();
    
    enc->sws_ctx = sws_getContext(
        WIDTH, HEIGHT, AV_PIX_FMT_BGRA,
        WIDTH, HEIGHT, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, NULL, NULL, NULL
    );
    
    enc->frame_count = 0;
    
    return 0;
}

int encoder_write_frame(VideoEncoder *enc, cairo_surface_t *surface) {
    int ret;
    
    unsigned char *cairo_data = cairo_image_surface_get_data(surface);
    int stride = cairo_image_surface_get_stride(surface);
    
    const uint8_t *src_data[4] = { cairo_data, NULL, NULL, NULL };
    int src_linesize[4] = { stride, 0, 0, 0 };
    
    sws_scale(
        enc->sws_ctx,
        src_data, src_linesize, 0, HEIGHT,
        enc->frame->data, enc->frame->linesize
    );
    
    enc->frame->pts = enc->frame_count++;
    
    ret = avcodec_send_frame(enc->codec_ctx, enc->frame);
    if (ret < 0) {
        fprintf(stderr, "Error sending frame to encoder\n");
        return -1;
    }
    
    while (ret >= 0) {
        ret = avcodec_receive_packet(enc->codec_ctx, enc->pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            break;
        } else if (ret < 0) {
            fprintf(stderr, "Error encoding frame\n");
            return -1;
        }
        
        av_packet_rescale_ts(enc->pkt, enc->codec_ctx->time_base, enc->stream->time_base);
        enc->pkt->stream_index = enc->stream->index;
        
        ret = av_interleaved_write_frame(enc->fmt_ctx, enc->pkt);
        av_packet_unref(enc->pkt);
        
        if (ret < 0) {
            fprintf(stderr, "Error writing frame\n");
            return -1;
        }
    }
    
    return 0;
}

void encoder_finish(VideoEncoder *enc) {
    avcodec_send_frame(enc->codec_ctx, NULL);
    
    AVPacket *pkt = av_packet_alloc();
    int ret;
    while (1) {
        ret = avcodec_receive_packet(enc->codec_ctx, pkt);
        if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN)) {
            break;
        }
        av_packet_rescale_ts(pkt, enc->codec_ctx->time_base, enc->stream->time_base);
        pkt->stream_index = enc->stream->index;
        av_interleaved_write_frame(enc->fmt_ctx, pkt);
        av_packet_unref(pkt);
    }
    av_packet_free(&pkt);
    
    av_write_trailer(enc->fmt_ctx);
    
    avcodec_free_context(&enc->codec_ctx);
    av_frame_free(&enc->frame);
    av_packet_free(&enc->pkt);
    sws_freeContext(enc->sws_ctx);
    
    if (!(enc->fmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&enc->fmt_ctx->pb);
    }
    
    avformat_free_context(enc->fmt_ctx);
}

void draw_rounded_rectangle(cairo_t *cr, double x, double y, double width, double height, double radius) {
    cairo_new_sub_path(cr);
    cairo_arc(cr, x + radius, y + radius, radius, M_PI, 3 * M_PI / 2);
    cairo_arc(cr, x + width - radius, y + radius, radius, 3 * M_PI / 2, 0);
    cairo_arc(cr, x + width - radius, y + height - radius, radius, 0, M_PI / 2);
    cairo_arc(cr, x + radius, y + height - radius, radius, M_PI / 2, M_PI);
    cairo_close_path(cr);
}

cairo_surface_t* draw_phone_to_surface(int x_pos, float rot_x, float rot_z) {
    int phone_width = 340;
    int phone_height = 680;
    int y_center = (HEIGHT - phone_height) / 2;
    
    // Create Cairo surface
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);
    cairo_t *cr = cairo_create(surface);
    
    // Fill with white background
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);
    
    // Apply 3D transformations
    cairo_save(cr);
    
    // Move to phone center for rotation
    cairo_translate(cr, x_pos + phone_width/2, y_center + phone_height/2);
    
    // Apply Z-axis rotation (spinning)
    cairo_rotate(cr, rot_z * M_PI / 180.0);
    
    // Apply X-axis rotation simulation (flipping effect with Y scaling)
    float scale_y = fabs(cos(rot_x * M_PI / 180.0));
    if (scale_y < 0.1) scale_y = 0.1;  // Prevent complete flattening
    cairo_scale(cr, 1.0, scale_y);
    
    // Move back to draw from top-left
    cairo_translate(cr, -phone_width/2, -phone_height/2);
    
    // Draw phone outer frame
    cairo_set_source_rgb(cr, 0.176, 0.216, 0.282);  // #2D3748
    draw_rounded_rectangle(cr, 0, 0, 340, 680, 50);
    cairo_fill(cr);
    
    // Draw inner lighter border
    cairo_set_source_rgb(cr, 0.227, 0.271, 0.337);  // #3A4556
    draw_rounded_rectangle(cr, 4, 4, 332, 672, 46);
    cairo_fill(cr);
    
    // Draw main screen background
    cairo_set_source_rgb(cr, 0.102, 0.125, 0.173);  // #1A202C
    draw_rounded_rectangle(cr, 12, 12, 316, 656, 42);
    cairo_fill(cr);
    
    // Draw notch cutout (simplified as rectangle)
    cairo_set_source_rgb(cr, 0.176, 0.216, 0.282);  // #2D3748 - same as outer frame
    draw_rounded_rectangle(cr, 135, 0, 70, 36, 18);
    cairo_fill(cr);
    
    // Draw notch inner dark area
    cairo_set_source_rgb(cr, 0.102, 0.122, 0.180);  // #1A1F2E
    draw_rounded_rectangle(cr, 138, 8, 64, 25, 16);
    cairo_fill(cr);
    
    // Draw speaker grille
    cairo_set_source_rgb(cr, 0.059, 0.078, 0.098);  // #0F1419
    draw_rounded_rectangle(cr, 155, 18, 30, 4, 2);
    cairo_fill(cr);
    
    // Draw front camera (outer)
    cairo_set_source_rgb(cr, 0.059, 0.078, 0.098);  // #0F1419
    cairo_arc(cr, 180, 20, 4.5, 0, 2 * M_PI);
    cairo_fill(cr);
    
    // Draw front camera (inner)
    cairo_set_source_rgb(cr, 0.118, 0.161, 0.231);  // #1E293B
    cairo_arc(cr, 180, 20, 2.5, 0, 2 * M_PI);
    cairo_fill(cr);
    
    // Draw power button (right side)
    cairo_set_source_rgb(cr, 0.102, 0.125, 0.173);  // #1A202C
    draw_rounded_rectangle(cr, 335, 150, 5, 60, 2.5);
    cairo_fill(cr);
    
    // Draw volume buttons (left side)
    draw_rounded_rectangle(cr, 0, 120, 5, 45, 2.5);
    cairo_fill(cr);
    draw_rounded_rectangle(cr, 0, 175, 5, 45, 2.5);
    cairo_fill(cr);
    
    // Draw silent switch (left side)
    draw_rounded_rectangle(cr, 0, 80, 5, 25, 2.5);
    cairo_fill(cr);
    
    cairo_restore(cr);
    cairo_destroy(cr);
    
    // Flush to ensure all drawing is done
    cairo_surface_flush(surface);
    
    return surface;
}


int main() {
    VideoEncoder encoder;
    
    printf("Initializing video encoder...\n");
    if (encoder_init(&encoder, "output.mp4") < 0) {
        fprintf(stderr, "Failed to initialize encoder\n");
        return 1;
    }
    
    printf("Rendering frames...\n");
    
    for (int frame = 0; frame < TOTAL_FRAMES; frame++) {
        int phone_width = 340;
        int x_pos = (int)((float)frame / TOTAL_FRAMES * (WIDTH - phone_width));
        
        float progress = (float)frame / TOTAL_FRAMES;
        float rot_x = progress * 360.0f;
        float rot_z = progress * 360.0f;
        
        cairo_surface_t *surface = draw_phone_to_surface(x_pos, rot_x, rot_z);
        
        if (!surface) {
            fprintf(stderr, "Failed to render SVG for frame %d\n", frame);
            continue;
        }
        
        encoder_write_frame(&encoder, surface);
        
        cairo_surface_destroy(surface);
        
        if (frame % 10 == 0) {
            printf("Progress: %d/%d frames\n", frame, TOTAL_FRAMES);
        }
    }
    
    printf("Finalizing video...\n");
    encoder_finish(&encoder);
    
    printf("Video saved as output.mp4\n");
    printf("Duration: %d seconds, %d frames at %d fps\n", DURATION, TOTAL_FRAMES, FPS);
    
    return 0;
}