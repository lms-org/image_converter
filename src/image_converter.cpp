#include <string>

#include "image_converter.h"
#include "lms/datamanager.h"
#include "lms/imaging/converter.h"
#include <lms/imaging/image_factory.h>
#include "lms/imaging/warp.h"

bool ImageConverter::initialize() {
    outputFormat = lms::imaging::formatFromString(
                config().get<std::string>("output_format"));
    std::string filterS = config().get<std::string>("filter");
    scaleUpFactor = config().get<int>("scaleUp", -1);
    scaleDownFactor = config().get<int>("scaleDown", -1);

    // available operations: conversion, filtering, and scaling up/down
    if (outputFormat != lms::imaging::Format::UNKNOWN) {
        operation = Operation::CONVERSION;
    } else if(filterS == "gauss"){
        filterFunc= lms::imaging::op::gauss;
        operation = Operation::FILTER;
    }else if(filterS == "sobelX"){
        filterFunc = lms::imaging::op::sobelX;
        operation = Operation::FILTER;
    }else if(filterS == "sobelY"){
        filterFunc = lms::imaging::op::sobelY;
        operation = Operation::FILTER;
    }else if(filterS == "imageV2C" || filterS == "warp"){
        filterFunc = lms::imaging::imageV2C;
        operation = Operation::FILTER;
    } else if(scaleUpFactor != -1) {
        operation = Operation::SCALEUP;
    } else if (scaleDownFactor != -1) {
        operation = Operation::SCALEDOWN;
    } else {
        // in case the given parameters are incomplete/incorrect
        operation = Operation::NONE;
    }

    // print out used operation
    logger.debug("init") << "Operation: " <<
                            (operation == Operation::CONVERSION ? "CONVERSION " + formatToString(outputFormat) :
                            operation == Operation::FILTER ? "FILTER " + filterS:
                            operation == Operation::SCALEUP ? "SCALEUP " + std::to_string(scaleUpFactor) :
                            operation == Operation::SCALEDOWN ? "SCALEDOWN " + std::to_string(scaleDownFactor) :
                            "UNKNOWN/NONE");

    if(operation == Operation::NONE) {
        logger.error("init") << "unknown operation or incorrect parameters";
    }
    inputImagePtr = readChannel<lms::imaging::Image>("INPUT_IMAGE");
    outputImagePtr = writeChannel<lms::imaging::Image>("OUTPUT_IMAGE");
    return true;
}

bool ImageConverter::deinitialize() {
    return true;
}

bool ImageConverter::cycle() {
    logger.time("conversion");
    switch(operation) {
        case Operation::CONVERSION:
            if(! lms::imaging::convert(*inputImagePtr, *outputImagePtr, outputFormat)) {
                logger.warn("cycle") << "Could not convert to " << outputFormat;
                return false;
            }
            break;
        case Operation::FILTER:
            filterFunc(*inputImagePtr, *outputImagePtr);
            break;
        case Operation::SCALEUP:
            lms::imaging::scaleUp(*inputImagePtr, *outputImagePtr, scaleUpFactor);
            break;
        case Operation::SCALEDOWN:
            lms::imaging::scaleDown(*inputImagePtr, *outputImagePtr, scaleDownFactor);
            break;
        default:
            *outputImagePtr = *inputImagePtr;
    }

    logger.timeEnd("conversion");
    return true;
}
