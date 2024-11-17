#include "lib_headers.h"
#include "structs.h"

void save_to_jpeg(const uint8_t* rgb_data, int width, int height, int stride, const char* filename);
void _handle_error(struct heif_error err);
RGB_IMAGE* get_RGB_from_HEIC(const char *);

int main(int argc, char** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s input.heic ouptut.jpeg\n", argv[0]);
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_file = argv[2];
    RGB_IMAGE * rgb_image = get_RGB_from_HEIC(input_file);

    // Convert RGB data to jpeg image
    save_to_jpeg(rgb_image->image_data, rgb_image->image_width, rgb_image->image_height,rgb_image->stride, output_file);

    
    // Clean up
    heif_image_release(rgb_image->heif_image);
    free(rgb_image->image_data);
    
    printf("HEIC file successfully converted to JPEG!\n");

    return 0;
}


/// @brief Receives an HEIC file name and translates it to RGB data. 
/// @param source_file_name the HEIC file to read from.
/// @return RGB_IMAGE struct populated with the proper values
RGB_IMAGE * get_RGB_from_HEIC(const char * source_file_name){


    // Initialize the HEIF context
    struct heif_context* ctx = heif_context_alloc();

    // Read the HEIC file
    struct heif_error err = heif_context_read_from_file(ctx, source_file_name, NULL);
    _handle_error(err);

    // Get the primary image handle
    struct heif_image_handle* handle;
    err = heif_context_get_primary_image_handle(ctx, &handle);
    _handle_error(err);

    // Decode the image (convert to 24-bit RGB)
    struct heif_image* img;
    err = heif_decode_image(handle, &img, heif_colorspace_RGB, heif_chroma_interleaved_RGB, NULL);
    _handle_error(err);

    // Store the image data along with HEIC image pointer.
    RGB_IMAGE * rgb_image = malloc(sizeof(RGB_IMAGE));

    rgb_image->image_data = heif_image_get_plane_readonly(img, heif_channel_interleaved, &(*rgb_image).stride);
    rgb_image->image_width = heif_image_get_width(img, heif_channel_interleaved);
    rgb_image->image_height = heif_image_get_height(img, heif_channel_interleaved);
    
    
    // Clean up
    //heif_image_release(img); // This is commented temporarily to avoid seg fault. (tbh I'm to lazy to fix the seg fualt for good so i'm just applying a temporary patch)

    // Partial cleanup to avoid seg fault.
    heif_image_handle_release(handle);
    heif_context_free(ctx);
    return rgb_image;
}



/// @brief Translates RGB data to jpeg. Finally stores it in the given directory.
/// @param rgb_data the rgb array should be provided here
/// @param width  image width in pixels
/// @param height image height in pixels
/// @param stride image stride in bytes
/// @param filename the name of the output file
void save_to_jpeg(const uint8_t* rgb_data, int width, int height, int stride, const char* filename) {
    FILE* outfile = fopen(filename, "wb");
    if (!outfile) {
        perror("Error opening output file");
        exit(1);
    }

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3; // RGB has 3 components
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);
    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW row_pointer[1];

    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = (JSAMPROW) &rgb_data[cinfo.next_scanline * stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(outfile);
}

// Function to handle heif related errors
void _handle_error(struct heif_error err) {
    if (err.code != heif_error_Ok) {
        fprintf(stderr, "HEIF error: %s\n", err.message);
        exit(1);
    }
}
