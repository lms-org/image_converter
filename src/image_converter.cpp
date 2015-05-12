#include <string>

#include "image_converter.h"
#include "lms/datamanager.h"
#include "lms/imaging/converter.h"
#include <lms/imaging/image_factory.h>
#include "lms/imaging/warp.h"

void test(){

}

bool ImageConverter::initialize() {
    config = getConfig();

    outputFormat = lms::imaging::formatFromString(
                config->get<std::string>("output_format"));
    //TODO just for testing
    //Not sure how to handle multiple filters, for example gauss first -> sobel values
    //Maybe we should go for one filter per converter at first :)
    std::string filterS = config->get<std::string>("filter");
        if(filterS == "gauss"){
             filterFunc= lms::imaging::op::gauss;
        }else if(filterS == "sobelX"){
            filterFunc = lms::imaging::op::sobelX;
        }else if(filterS == "sobelY"){
            filterFunc = lms::imaging::op::sobelY;
        }else if(filterS == "imageV2C"){
            filterFunc = lms::imaging::imageV2C;
        }else{
            filterFunc = nullptr;
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
    //TODO not that nice
    if(filterFunc != nullptr){
        filterFunc(*inputImagePtr,*outputImagePtr);
    }
    logger.timeEnd("conversion");
    return true;
}
