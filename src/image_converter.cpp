#include <string>

#include "image_converter.h"
#include "lms/datamanager.h"
#include "lms/imaging/converter.h"
#include <lms/imaging/image_factory.h>

void test(){

}

bool ImageConverter::initialize() {
    config = getConfig();

    outputFormat = lms::imaging::formatFromString(
                config->get<std::string>("output_format"));
    //TODO just for testing
    //Not sure how to handle multiple filters, for example gauss first -> sobel values
    //Maybe we should go for one filter per converter at first :)
    std::vector<std::string> filters = config->getArray<std::string>("filter");
    std::vector<void (*)(const lms::imaging::Image&,lms::imaging::Image&)> functions;
    for(std::string filter : filters){
        if(filter == "gauss"){
            functions.push_back(lms::imaging::op::gauss);
        }else if(filter == "sobelX"){
            functions.push_back(lms::imaging::op::sobelX);
        }else if(filter == "sobelY"){
            functions.push_back(lms::imaging::op::sobelY);
        }
    }

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
