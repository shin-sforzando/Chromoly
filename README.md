Chromoly
========

Package for chroma key compositing with oF.

![Screenshot](screenshot.png)

    +-----+-----+---------+
    | RAW | WEB |         |
    +-----+-----+ ANDROID +
    | GUI | SNS |         |
    +-----+-----+---------+

Table of Contents
--------

- [Requirements](#requirements)
- [How to Setup](#how-to-setup)
	- [0. Directory Structure](#0-directory-structure)
	- [1. openFrameworks](#1-openframeworks)
	- [2. Camera Utility (Canon)](#2-camera-utility-canon)
		- [2.1 Resolution](#21-resolution)
		- [2.2 Export setting](#22-export-setting)
		- [2.3 Shooting](#23-shooting)
	- [3. S3cmd](#3-s3cmd)
	- [4. Printer](#4-printer)
- [How to Use](#how-to-use)
	- [Misc. 1: Annotate text on images using ImageMagick](#misc-1-annotate-text-on-images-using-imagemagick)
	- [Misc. 2: Flip all images using ImageMagick](#misc-2-flip-all-images-using-imagemagick)
	- [Misc. 3: Resize all images using ImageMagick](#misc-3-resize-all-images-using-imagemagick)
	- [Misc. 4: Convert mp4 using FFmpeg](#misc-4-convert-mp4-using-ffmpeg)
	- [Keyboard shortcut](#keyboard-shortcut)
	- [Option](#option)
- [Contributes](#contributes)
- [LICENSE](#license)

## Requirements
- Mac OS 10.11 or higher
- Xcode 8.1 or higher
- [openFrameworks](http://openframeworks.cc) 0.98 or higher
- [Homebrew](http://brew.sh)
    - [ImageMagick](http://www.imagemagick.org/)
    - [FFmpeg](https://www.ffmpeg.org)
    - [S3cmd](http://s3tools.org/s3cmd)
    - [QREncode](http://fukuchi.org/works/qrencode/)

## How to Setup
### 0. Directory Structure
    ~/Desktop
    ├── background_android
    ├── background_original
    ├── background_sns
    ├── background_web
    ├── backup_import
    ├── export
    ├── import
    └── of_v0.9.8_osx_release
        ├── addons
        │   ├── ofxChromaKey
        │   ├── ofxDatGui
        │   └── ofxXmlSettings
        │       ├── libs
        │       └── src
        └── apps
            └── myApps
                └── Chromoly

### 1. openFrameworks
1. Download openFrameworks latest osx release version.
1. Change direcotory to `myApps` and clone this repository.
1. Add `+x` to `setup.sh` and execute it.
    1. It will install some addons to appropriate path.

1. Prepare background images to appropriate path.

### 2. Camera Utility (Canon)

#### 2.1 Resolution
* It doesn't need RAW files
* Set "S2" to approx 2K x 2K

#### 2.2 Export setting
* Set import direcotory correctry like "import"
* Set rename setting to "<Shooting Time>|<Image Number>" like 1742480001.jpg

#### 2.3 Shooting
* Take continuous photos more than final movie rate

### 3. S3cmd
`s3cmd --configure`

### 4. Printer
`lpr -P Brother_QL-700 -o media=DC17 ~.png`

- DC17 = 39x48mm
- DC03 = 29x90mm
- DC08 = 29x42mm

## How to Use

1. Push "Reload" button to target images
1. Choose chroma key color
    * Use color picker
    * Right click inside TOP-LEFT area
1. Adjust chromakey threshold
    * Use slider
    * Use UP / DOWN key
1. Adjust frame number, scale and position for WEB
    * User slider or LEFT / RIGHT key to choose the frame number which will be WEB image
    * Use slider to adjust scale
    * Use 2D pad to adjust position
1. Adjust scale and position for SNS
    * Use slider to adjust scale
    * Use 2D pad to adjust position
1. Push "Export" button to export synthesized images

### Misc. 1: Annotate text on images using ImageMagick
```
for i in {00001..40};do
  mogrify -font Silom -pointsize 96 -gravity center -annotate 0 $i -fill red target_$i.JPG
done
```

### Misc. 2: Flip all images using ImageMagick
`mogrify -resize 1920x *.JPG`

### Misc. 3: Resize all images using ImageMagick
`mogrify -flip *.jpg`

### Misc. 4: Convert mp4 using FFmpeg
`ffmpeg -r 10 -i android_%03d.png -c:v libx264 -pix_fmt yuv420p -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" android_yuv420p.mp4`

### Keyboard shortcut

- b
    - Reload **B**ackground images
- l / r / t
    - **R**e**L**oad **T**arget images
- e
    - **E**xport synthesized images

### Option
* You can change preview framerate
    * User slider
    * If it was set '0', Chromoly will try to render as quick as possible

## Contributes
- Shin'ichiro SUZUKI (C"E"O / [szk-engineering](https://szk-engineering.com/))

## LICENSE
WTFPL
