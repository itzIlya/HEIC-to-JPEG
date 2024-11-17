## Description

A command-line micro utility for converting HEIC images to JPEG written in C. 

## Usage

### Dependencies

- libheif
- libjpeg

dependencies can be installed on Linux with the following commands (Ubuntu / Debian)

```bash
sudo apt-get update
sudo apt-get install libjpeg-dev libheif-dev
```

### Compiling

Run the following bash script in the project directory to build and compile the project. The CMake script supports Windows as well.

```bash
mkdir build
cd build
cmake ..
make
```

### Running

You can run the executable in the following format:

```bash
./main image_name.heic new_image_name.jpg
```

## Inspiration

Tbh I’m currently in the middle of midterms so anything that keeps me from studying can be regarded as an inspiration to write a barely half decent C. The other inspiration was my hatred towards this very ill supported format for supporting files. Every time I want to backup images from my iPhone to my laptop I have to convert all the images stored in HEIC to JPEG and since there aren’t any decent tools to convert HEIC images to JPEG I also decided to make another crappy tool to achieve the same awful results as everyone else.