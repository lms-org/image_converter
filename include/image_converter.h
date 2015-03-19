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
    const lms::type::ModuleConfig *config;

    lms::imaging::Format outputFormat;

    const lms::imaging::Image *inputImagePtr;
    lms::imaging::Image *outputImagePtr;
};

#endif /* IMAGE_CONVERTER_H */
