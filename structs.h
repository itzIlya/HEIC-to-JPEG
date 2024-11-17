/// All custom structs in the project are defined here


/// @brief stores the rgb image in image_data along with the stride of the image, height and width. The heif_image pointer also needs to be kept for additional use.
///
typedef struct RGB_IMAGE
{
    int image_height;
    int image_width;
    int stride;
    const struct heif_image * heif_image;
    const uint8_t * image_data;
}RGB_IMAGE;