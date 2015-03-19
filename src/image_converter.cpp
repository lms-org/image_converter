#include <string>

#include "image_converter.h"
#include "lms/datamanager.h"
#include "lms/imaging/converter.h"

bool ImageConverter::initialize() {
    config = getConfig();

    outputFormat = lms::imaging::formatFromString(config->get<std::string>("output_format"));

    if(outputFormat == lms::imaging::Format::UNKNOWN) {
        logger.error("init") << "output_format is " << outputFormat;
        return false;
    }

    std::string inputChannel = config->get<std::string>("input_channel");
    std::string outputChannel = config->get<std::string>("output_channel");

    if(inputChannel.empty()) {
        logger.error("init") << "input_channel is empty";
        return false;
    }
    if(outputChannel.empty()) {
        logger.error("init") << "output_channel is empty";
        return false;
    }

    inputImagePtr = datamanager()->readChannel<lms::imaging::Image>(this, inputChannel);
    outputImagePtr = datamanager()->writeChannel<lms::imaging::Image>(this, outputChannel);

    return true;
}

bool ImageConverter::deinitialize() {
    return true;
}

bool ImageConverter::cycle() {
    if(! lms::imaging::convert(*inputImagePtr, *outputImagePtr, outputFormat)) {
        logger.warn("cycle") << "Could not convert to " << outputFormat;
        return false;
    }
    return true;
}
