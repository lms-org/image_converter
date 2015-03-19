# image_converter

The image converter module is doing the following in each cycle:
- Read an image from a channel A
- Convert this image into another image format (e.g. from YUYV to GREY)
- Write the converted image to a channel B

## Config
- **input_channel** - The module will read an image from this channel.
- **output_channel** - The module will write the converted image into this channel.
- **output_format** - Look into [imaging lib](https://github.com/syxolk/imaging) for supported formats and conversions.
