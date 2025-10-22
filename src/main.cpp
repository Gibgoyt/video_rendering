#include "../include/main.h"

// ============================================================================
// 3D Math Library
// ============================================================================

struct Vec3 {
    float x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
    
    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    
    Vec3 cross(const Vec3& v) const {
        return Vec3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }
    
    float length() const { return std::sqrt(x * x + y * y + z * z); }
    Vec3 normalize() const { float l = length(); return Vec3(x/l, y/l, z/l); }
};

struct Vec4 {
    float x, y, z, w;
    
    Vec4() : x(0), y(0), z(0), w(1) {}
    Vec4(float x, float y, float z, float w = 1) : x(x), y(y), z(z), w(w) {}
    Vec4(const Vec3& v, float w = 1) : x(v.x), y(v.y), z(v.z), w(w) {}
    
    Vec3 xyz() const { return Vec3(x, y, z); }
    
    // Perspective divide
    Vec3 toVec3() const { 
        if (w != 0) return Vec3(x/w, y/w, z/w);
        return Vec3(x, y, z);
    }
};

struct Mat4 {
    float m[4][4];
    
    Mat4() {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                m[i][j] = (i == j) ? 1.0f : 0.0f;
    }
    
    Vec4 operator*(const Vec4& v) const {
        return Vec4(
            m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z + m[0][3]*v.w,
            m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z + m[1][3]*v.w,
            m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z + m[2][3]*v.w,
            m[3][0]*v.x + m[3][1]*v.y + m[3][2]*v.z + m[3][3]*v.w
        );
    }
    
    Mat4 operator*(const Mat4& other) const {
        Mat4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }
    
    // Create rotation matrix around X axis
    static Mat4 rotationX(float angle) {
        Mat4 mat;
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat.m[1][1] = c;  mat.m[1][2] = -s;
        mat.m[2][1] = s;  mat.m[2][2] = c;
        return mat;
    }
    
    // Create rotation matrix around Y axis
    static Mat4 rotationY(float angle) {
        Mat4 mat;
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat.m[0][0] = c;   mat.m[0][2] = s;
        mat.m[2][0] = -s;  mat.m[2][2] = c;
        return mat;
    }
    
    // Create rotation matrix around Z axis
    static Mat4 rotationZ(float angle) {
        Mat4 mat;
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat.m[0][0] = c;  mat.m[0][1] = -s;
        mat.m[1][0] = s;  mat.m[1][1] = c;
        return mat;
    }
    
    // Create translation matrix
    static Mat4 translation(float x, float y, float z) {
        Mat4 mat;
        mat.m[0][3] = x;
        mat.m[1][3] = y;
        mat.m[2][3] = z;
        return mat;
    }
    
    // Create perspective projection matrix
    static Mat4 perspective(float fov, float aspect, float near, float far) {
        Mat4 mat;
        float tanHalfFov = std::tan(fov / 2.0f);
        
        mat.m[0][0] = 1.0f / (aspect * tanHalfFov);
        mat.m[1][1] = 1.0f / tanHalfFov;
        mat.m[2][2] = -(far + near) / (far - near);
        mat.m[2][3] = -(2.0f * far * near) / (far - near);
        mat.m[3][2] = -1.0f;
        mat.m[3][3] = 0.0f;
        
        return mat;
    }
};

// ============================================================================
// 3D Renderer
// ============================================================================

struct Color {
    uint8_t r, g, b, a;
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
};

class Framebuffer {
private:
    int width, height;
    std::vector<Color> pixels;
    std::vector<float> depthBuffer;
    
public:
    Framebuffer(int w, int h) : width(w), height(h) {
        pixels.resize(w * h, Color(0, 0, 0));
        depthBuffer.resize(w * h, 1.0f);
    }
    
    void clear(const Color& color) {
        std::fill(pixels.begin(), pixels.end(), color);
        std::fill(depthBuffer.begin(), depthBuffer.end(), 1.0f);
    }
    
    void setPixel(int x, int y, float depth, const Color& color) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        
        int idx = y * width + x;
        if (depth < depthBuffer[idx]) {
            depthBuffer[idx] = depth;
            pixels[idx] = color;
        }
    }
    
    const Color* getData() const { return pixels.data(); }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

class Renderer {
private:
    Framebuffer& framebuffer;
    Mat4 projectionMatrix;
    
    // Convert NDC coordinates to screen space
    void ndcToScreen(const Vec3& ndc, int& x, int& y, float& depth) {
        x = (int)((ndc.x + 1.0f) * 0.5f * framebuffer.getWidth());
        y = (int)((1.0f - ndc.y) * 0.5f * framebuffer.getHeight());
        depth = ndc.z;
    }
    
    // Draw a line using Bresenham's algorithm
    void drawLine(Vec3 p0, Vec3 p1, const Color& color) {
        int x0, y0, x1, y1;
        float d0, d1;
        ndcToScreen(p0, x0, y0, d0);
        ndcToScreen(p1, x1, y1, d1);
        
        int dx = std::abs(x1 - x0);
        int dy = std::abs(y1 - y0);
        int sx = (x0 < x1) ? 1 : -1;
        int sy = (y0 < y1) ? 1 : -1;
        int err = dx - dy;
        
        while (true) {
            float t = (dx + dy == 0) ? 0.5f : (float)std::abs(x0 - x0) / (float)(dx + dy);
            float depth = d0 + t * (d1 - d0);
            framebuffer.setPixel(x0, y0, depth, color);
            
            if (x0 == x1 && y0 == y1) break;
            
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x0 += sx; }
            if (e2 < dx) { err += dx; y0 += sy; }
        }
    }
    
    // Fill triangle using barycentric coordinates
    void fillTriangle(Vec3 p0, Vec3 p1, Vec3 p2, const Color& color) {
        int x0, y0, x1, y1, x2, y2;
        float d0, d1, d2;
        ndcToScreen(p0, x0, y0, d0);
        ndcToScreen(p1, x1, y1, d1);
        ndcToScreen(p2, x2, y2, d2);
        
        // Bounding box
        int minX = std::max(0, std::min({x0, x1, x2}));
        int maxX = std::min(framebuffer.getWidth() - 1, std::max({x0, x1, x2}));
        int minY = std::max(0, std::min({y0, y1, y2}));
        int maxY = std::min(framebuffer.getHeight() - 1, std::max({y0, y1, y2}));
        
        // Rasterize
        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                // Barycentric coordinates
                float w0 = (float)((x1 - x0) * (y - y0) - (y1 - y0) * (x - x0));
                float w1 = (float)((x2 - x1) * (y - y1) - (y2 - y1) * (x - x1));
                float w2 = (float)((x0 - x2) * (y - y2) - (y0 - y2) * (x - x2));
                
                if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                    float area = w0 + w1 + w2;
                    if (area != 0) {
                        w0 /= area;
                        w1 /= area;
                        w2 /= area;
                        float depth = d0 * w0 + d1 * w1 + d2 * w2;
                        framebuffer.setPixel(x, y, depth, color);
                    }
                }
            }
        }
    }
    
public:
    Renderer(Framebuffer& fb) : framebuffer(fb) {
        float fov = 60.0f * PI / 180.0f;
        float aspect = (float)WIDTH / (float)HEIGHT;
        projectionMatrix = Mat4::perspective(fov, aspect, 0.1f, 100.0f);
    }
    
    void drawTriangle(Vec3 v0, Vec3 v1, Vec3 v2, const Mat4& mvp, const Color& color, bool filled = true) {
        // Transform vertices
        Vec4 p0 = mvp * Vec4(v0);
        Vec4 p1 = mvp * Vec4(v1);
        Vec4 p2 = mvp * Vec4(v2);
        
        // Perspective divide
        Vec3 ndc0 = p0.toVec3();
        Vec3 ndc1 = p1.toVec3();
        Vec3 ndc2 = p2.toVec3();
        
        // Backface culling
        Vec3 edge1 = ndc1 - ndc0;
        Vec3 edge2 = ndc2 - ndc0;
        float cross = edge1.x * edge2.y - edge1.y * edge2.x;
        if (cross < 0) return; // Back-facing
        
        if (filled) {
            fillTriangle(ndc0, ndc1, ndc2, color);
        } else {
            drawLine(ndc0, ndc1, color);
            drawLine(ndc1, ndc2, color);
            drawLine(ndc2, ndc0, color);
        }
    }
    
    Mat4 getProjectionMatrix() const { return projectionMatrix; }
};

// ============================================================================
// Cube Definition
// ============================================================================

struct Cube {
    std::vector<Vec3> vertices;
    std::vector<std::vector<int>> faces;
    std::vector<Color> faceColors;
    
    Cube(float size = 1.0f) {
        float h = size / 2.0f;
        
        // 8 vertices of cube
        vertices = {
            Vec3(-h, -h, -h), Vec3( h, -h, -h), Vec3( h,  h, -h), Vec3(-h,  h, -h),
            Vec3(-h, -h,  h), Vec3( h, -h,  h), Vec3( h,  h,  h), Vec3(-h,  h,  h)
        };
        
        // 6 faces (2 triangles each)
        faces = {
            {0, 1, 2}, {0, 2, 3}, // Front
            {1, 5, 6}, {1, 6, 2}, // Right
            {5, 4, 7}, {5, 7, 6}, // Back
            {4, 0, 3}, {4, 3, 7}, // Left
            {3, 2, 6}, {3, 6, 7}, // Top
            {4, 5, 1}, {4, 1, 0}  // Bottom
        };
        
        // Colors for each face pair
        faceColors = {
            Color(231, 76, 60),  Color(231, 76, 60),   // Red
            Color(52, 152, 219), Color(52, 152, 219),  // Blue
            Color(46, 204, 113), Color(46, 204, 113),  // Green
            Color(241, 196, 15), Color(241, 196, 15),  // Yellow
            Color(155, 89, 182), Color(155, 89, 182),  // Purple
            Color(230, 126, 34), Color(230, 126, 34)   // Orange
        };
    }
    
    void render(Renderer& renderer, const Mat4& modelMatrix, bool wireframe = false) {
        Mat4 mvp = renderer.getProjectionMatrix() * modelMatrix;
        
        for (size_t i = 0; i < faces.size(); i++) {
            const auto& face = faces[i];
            renderer.drawTriangle(
                vertices[face[0]],
                vertices[face[1]],
                vertices[face[2]],
                mvp,
                faceColors[i],
                !wireframe
            );
        }
    }
};

// ============================================================================
// Video Encoder
// ============================================================================

class VideoEncoder {
private:
    AVFormatContext* fmtCtx;
    AVCodecContext* codecCtx;
    AVStream* stream;
    AVFrame* frame;
    AVPacket* pkt;
    SwsContext* swsCtx;
    int frameCount;
    
public:
    VideoEncoder() : fmtCtx(nullptr), codecCtx(nullptr), stream(nullptr),
                     frame(nullptr), pkt(nullptr), swsCtx(nullptr), frameCount(0) {}
    
    bool init(const char* filename) {
        avformat_alloc_output_context2(&fmtCtx, nullptr, nullptr, filename);
        if (!fmtCtx) return false;
        
        const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (!codec) return false;
        
        stream = avformat_new_stream(fmtCtx, nullptr);
        if (!stream) return false;
        
        codecCtx = avcodec_alloc_context3(codec);
        if (!codecCtx) return false;
        
        codecCtx->width = WIDTH;
        codecCtx->height = HEIGHT;
        codecCtx->time_base = {1, FPS};
        codecCtx->framerate = {FPS, 1};
        codecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
        codecCtx->gop_size = 10;
        codecCtx->max_b_frames = 1;
        
        av_opt_set(codecCtx->priv_data, "preset", "medium", 0);
        av_opt_set(codecCtx->priv_data, "crf", "23", 0);
        
        if (avcodec_open2(codecCtx, codec, nullptr) < 0) return false;
        
        avcodec_parameters_from_context(stream->codecpar, codecCtx);
        
        if (!(fmtCtx->oformat->flags & AVFMT_NOFILE)) {
            if (avio_open(&fmtCtx->pb, filename, AVIO_FLAG_WRITE) < 0) return false;
        }
        
        if (avformat_write_header(fmtCtx, nullptr) < 0) return false;
        
        frame = av_frame_alloc();
        frame->format = codecCtx->pix_fmt;
        frame->width = WIDTH;
        frame->height = HEIGHT;
        av_frame_get_buffer(frame, 0);
        
        pkt = av_packet_alloc();
        
        swsCtx = sws_getContext(
            WIDTH, HEIGHT, AV_PIX_FMT_RGBA,
            WIDTH, HEIGHT, AV_PIX_FMT_YUV420P,
            SWS_BILINEAR, nullptr, nullptr, nullptr
        );
        
        return true;
    }
    
    void writeFrame(const Framebuffer& fb) {
        const Color* pixels = fb.getData();
        
        // Convert RGBA to YUV420P
        std::vector<uint8_t> rgbaData(WIDTH * HEIGHT * 4);
        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            rgbaData[i * 4 + 0] = pixels[i].r;
            rgbaData[i * 4 + 1] = pixels[i].g;
            rgbaData[i * 4 + 2] = pixels[i].b;
            rgbaData[i * 4 + 3] = pixels[i].a;
        }
        
        const uint8_t* srcData[4] = { rgbaData.data(), nullptr, nullptr, nullptr };
        int srcLinesize[4] = { WIDTH * 4, 0, 0, 0 };
        
        sws_scale(swsCtx, srcData, srcLinesize, 0, HEIGHT, frame->data, frame->linesize);
        
        frame->pts = frameCount++;
        
        avcodec_send_frame(codecCtx, frame);
        
        while (avcodec_receive_packet(codecCtx, pkt) == 0) {
            av_packet_rescale_ts(pkt, codecCtx->time_base, stream->time_base);
            pkt->stream_index = stream->index;
            av_interleaved_write_frame(fmtCtx, pkt);
            av_packet_unref(pkt);
        }
    }
    
    void finish() {
        avcodec_send_frame(codecCtx, nullptr);
        
        AVPacket* pkt = av_packet_alloc();
        while (avcodec_receive_packet(codecCtx, pkt) == 0) {
            av_packet_rescale_ts(pkt, codecCtx->time_base, stream->time_base);
            pkt->stream_index = stream->index;
            av_interleaved_write_frame(fmtCtx, pkt);
            av_packet_unref(pkt);
        }
        av_packet_free(&pkt);
        
        av_write_trailer(fmtCtx);
        
        avcodec_free_context(&codecCtx);
        av_frame_free(&frame);
        av_packet_free(&this->pkt);
        sws_freeContext(swsCtx);
        
        if (!(fmtCtx->oformat->flags & AVFMT_NOFILE)) {
            avio_closep(&fmtCtx->pb);
        }
        
        avformat_free_context(fmtCtx);
    }
};

// ============================================================================
// Main
// ============================================================================

int main() {
    std::cout << "Initializing 3D renderer..." << std::endl;
    
    Framebuffer framebuffer(WIDTH, HEIGHT);
    Renderer renderer(framebuffer);
    Cube cube(2.0f);
    VideoEncoder encoder;
    
    if (!encoder.init("output_3d.mp4")) {
        std::cerr << "Failed to initialize encoder" << std::endl;
        return 1;
    }
    
    std::cout << "Rendering " << TOTAL_FRAMES << " frames..." << std::endl;
    
    for (int frameNum = 0; frameNum < TOTAL_FRAMES; frameNum++) {
        float t = (float)frameNum / (float)TOTAL_FRAMES;
        
        // Clear framebuffer
        framebuffer.clear(Color(20, 20, 30));
        
        // Create transformation matrices
        float angle = t * 2.0f * PI;
        Mat4 rotationX = Mat4::rotationX(angle * 0.7f);
        Mat4 rotationY = Mat4::rotationY(angle);
        Mat4 rotationZ = Mat4::rotationZ(angle * 0.3f);
        Mat4 translation = Mat4::translation(0, 0, -8.0f);
        
        // Combine transformations: Translate * RotateZ * RotateY * RotateX
        Mat4 modelMatrix = translation * rotationZ * rotationY * rotationX;
        
        // Render cube
        cube.render(renderer, modelMatrix, false);
        
        // Encode frame
        encoder.writeFrame(framebuffer);
        
        if (frameNum % 10 == 0) {
            std::cout << "Progress: " << frameNum << "/" << TOTAL_FRAMES << " frames" << std::endl;
        }
    }
    
    std::cout << "Finalizing video..." << std::endl;
    encoder.finish();
    
    std::cout << "Video saved as output_3d.mp4" << std::endl;
    std::cout << "A 3D cube rotating in all three axes!" << std::endl;
    
    return 0;
}