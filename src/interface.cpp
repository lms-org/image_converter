#include <image_converter.h>

extern "C" {
void* getInstance () {
    return new ImageConverter();
}
}
