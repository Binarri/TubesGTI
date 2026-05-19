
#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED

class Image {
public:
    Image(char* ps, int w, int h);
    ~Image();
    char* pixels;    // Array pixel data (RGB)
    int width;       // Lebar gambar
    int height;      // Tinggi gambar
};

// Load file BMP dan return object Image
Image* loadBMP(const char* filename);

#endif
