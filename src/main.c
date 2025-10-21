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

char* generate_svg(int x_position) {
    char *svg = malloc(1024);
    snprintf(svg, 1024,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">"
        "<rect x=\"%d\" y=\"490\" width=\"200\" height=\"100\" fill=\"#3498db\" rx=\"10\"/>"
        "</svg>",
        WIDTH, HEIGHT, x_position
    );
    return svg;
}

cairo_surface_t* render_svg_to_surface(const char *svg_data) {
    GError *error = NULL;
    
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);
    cairo_t *cr = cairo_create(surface);
    
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_paint(cr);
    
    RsvgHandle *handle = rsvg_handle_new_from_data((const guint8*)svg_data, strlen(svg_data), &error);
    if (error != NULL) {
        fprintf(stderr, "Error loading SVG: %s\n", error->message);
        g_error_free(error);
        cairo_destroy(cr);
        cairo_surface_destroy(surface);
        return NULL;
    }
    
    rsvg_handle_render_cairo(handle, cr);
    
    g_object_unref(handle);
    cairo_destroy(cr);
    
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
        int x_pos = (int)((float)frame / TOTAL_FRAMES * (WIDTH - 200));
        
        char *svg_data = generate_svg(x_pos);
        
        cairo_surface_t *surface = render_svg_to_surface(svg_data);
        free(svg_data);
        
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