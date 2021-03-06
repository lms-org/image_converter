#ifndef IMAGE_CONVERTER_H
#define IMAGE_CONVERTER_H

#include "lms/module.h"
#include "lms/imaging/image.h"
#include "lms/imaging/format.h"


class ImageConverter : public lms::Module {
public:
    bool initialize() override;
    bool deinitialize() override;
    bool cycle() override;
private:
    enum class Operation: std::uint8_t {
        NONE = 0,
        CONVERSION = 1,
        FILTER = 2,
        SCALEUP = 3,
        SCALEDOWN = 4
    } operation;
    lms::imaging::Format outputFormat;
    int scaleUpFactor;
    int scaleDownFactor;

    lms::ReadDataChannel<lms::imaging::Image> inputImagePtr;
    lms::WriteDataChannel<lms::imaging::Image> outputImagePtr;

    void (*filterFunc)(const lms::imaging::Image&, lms::imaging::Image&);
};

#endif /* IMAGE_CONVERTER_H */
