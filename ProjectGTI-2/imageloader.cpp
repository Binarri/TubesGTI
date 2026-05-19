
#include <assert.h>
#include <fstream>
#include "imageloader.h"

using namespace std;

// ============================
// CONSTRUCTOR & DESTRUCTOR
// ===========================

Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {}

Image::~Image() {
    delete[] pixels;
}

// =============================================================
// HELPER FUNCTIONS (Anonymous namespace - internal only)
// =============================================================

namespace {
    // Convert 4 bytes to int (little-endian)
    int toInt(const char* bytes) {
        return (int)(((unsigned char)bytes[3] << 24) |
                     ((unsigned char)bytes[2] << 16) |
                     ((unsigned char)bytes[1] << 8) |
                     (unsigned char)bytes[0]);
    }

    // Convert 2 bytes to short (little-endian)
    short toShort(const char* bytes) {
        return (short)(((unsigned char)bytes[1] << 8) |
                       (unsigned char)bytes[0]);
    }

    // Read int from file
    int readInt(ifstream &input) {
        char buffer[4];
        input.read(buffer, 4);
        return toInt(buffer);
    }

    // Read short from file
    short readShort(ifstream &input) {
        char buffer[2];
        input.read(buffer, 2);
        return toShort(buffer);
    }

    // ===========================================================
    // AUTO_ARRAY TEMPLATE CLASS (Memory management helper)
    // ===========================================================
    
    template<class T>
    class auto_array {
    private:
        T* array;
        mutable bool isReleased;
    public:
        explicit auto_array(T* array = NULL) : array(array), isReleased(false) {}
        
        auto_array(const auto_array<T> &aarray) {
            array = aarray.array;
            isReleased = aarray.isReleased;
            aarray.isReleased = true;
        }
        
        ~auto_array() {
            if (!isReleased && array != NULL) {
                delete[] array;
            }
        }
        
        T* get() const { 
            return array; 
        }
        
        T &operator*() const { 
            return *array; 
        }
        
        void operator=(const auto_array<T> &aarray) {
            if (!isReleased && array != NULL) {
                delete[] array;
            }
            array = aarray.array;
            isReleased = aarray.isReleased;
            aarray.isReleased = true;
        }
        
        T* operator->() const { 
            return array; 
        }
        
        T* release() {
            isReleased = true;
            return array;
        }
        
        T &operator[](int i) { 
            return array[i]; 
        }
    };
}

// ======================================
// MAIN BMP LOADING FUNCTION
// =====================================

Image* loadBMP(const char* filename) {
    // Open file in binary mode
    ifstream input;
    input.open(filename, ifstream::binary);
    
    // Check if file exists
    assert(!input.fail() || !"File tidak ditemukan!!!");
    
    // Read BMP signature (should be 'BM')
    char buffer[2];
    input.read(buffer, 2);
    assert((buffer[0] == 'B' && buffer[1] == 'M') || !"Bukan file bitmap!!!");
    
    // Skip file size and reserved fields (8 bytes)
    input.ignore(8);
    
    // Read data offset (where pixel data starts)
    int dataOffset = readInt(input);
    
    // Read header size
    int headerSize = readInt(input);
    
    int width, height;
    
    // Parse header based on size (we support BITMAPINFOHEADER = 40 bytes)
    switch (headerSize) {
        case 40:  // BITMAPINFOHEADER (most common)
            width = readInt(input);
            height = readInt(input);
            input.ignore(2);  // Skip color planes (must be 1)
            
            // Check bits per pixel (must be 24 for RGB)
            assert(readShort(input) == 24 || !"Gambar tidak 24 bits per pixel!");
            
            // Check compression (must be 0 = no compression)
            assert(readShort(input) == 0 || !"Gambar dikompres!");
            break;
            
        default:
            assert(!"Format bitmap tidak diketahui!");
    }
    
    // Calculate bytes per row (must be multiple of 4)
    int bytesPerRow = ((width * 3 + 3) / 4) * 4;
    int size = bytesPerRow * height;
    
    // Allocate buffer for raw pixel data
    auto_array<char> pixels(new char[size]);
    
    // Seek to pixel data and read
    input.seekg(dataOffset, ios_base::beg);
    input.read(pixels.get(), size);
    
    // Convert BGR to RGB and flip vertically (BMP stores bottom-to-top)
    auto_array<char> pixels2(new char[width * height * 3]);
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < 3; c++) {
                // Convert BGR to RGB and flip Y axis
                // pixels: BGR format, bottom-to-top
                // pixels2: RGB format, top-to-bottom
                pixels2[3 * (width * y + x) + c] = 
                    pixels[bytesPerRow * (height - 1 - y) + 3 * x + (2 - c)];
            }
        }
    }
    
    // Close file
    input.close();
    
    // Return new Image object with converted pixel data
    return new Image(pixels2.release(), width, height);
}
