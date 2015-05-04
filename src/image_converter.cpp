#include <string>

#include "image_converter.h"
#include "lms/datamanager.h"
#include "lms/imaging/converter.h"

bool ImageConverter::initialize() {
    config = getConfig();

    outputFormat = lms::imaging::formatFromString(
                config->get<std::string>("output_format"));

    if(outputFormat == lms::imaging::Format::UNKNOWN) {
        logger.error("init") << "output_format is " << outputFormat;
        return false;
    }

    inputImagePtr = datamanager()
            ->readChannel<lms::imaging::Image>(this, "INPUT_IMAGE");
    outputImagePtr = datamanager()
            ->writeChannel<lms::imaging::Image>(this, "OUTPUT_IMAGE");

    return true;
}

bool ImageConverter::deinitialize() {
    return true;
}

bool ImageConverter::cycle() {
    logger.time("conversion");
    if(! lms::imaging::convert(*inputImagePtr, *outputImagePtr, outputFormat)) {
        logger.warn("cycle") << "Could not convert to " << outputFormat;
        return false;
    }
    logger.timeEnd("conversion");
    return true;
}
