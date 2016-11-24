Chromoly
========

Package for chroma key compositing with oF.

![Screenshot](https://dummyimage.com/700x359/000/fff.png)

Table of Contents
--------

<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

	- [Requirements](#requirements)
	- [How to Setup](#how-to-setup)
		- [1. openFrameworks](#1-openframeworks)
		- [2. Automator](#2-automator)
			- [2.1 Pre process](#21-pre-process)
			- [2.2 Post process](#22-post-process)
				- [2.2.1 QR code](#221-qr-code)
				- [2.2.2 Upload](#222-upload)
		- [3. Camera Utility (Canon)](#3-camera-utility-canon)
			- [3.1 Resolution](#31-resolution)
			- [3.2 Export setting](#32-export-setting)
			- [3.3 Shooting](#33-shooting)
	- [How to Use](#how-to-use)
	- [Contributes](#contributes)
	- [ToDo](#todo)
		- [High / FIXME](#high-fixme)
		- [Mid / TODO](#mid-todo)
		- [Low / XXX](#low-xxx)
	- [Misc.](#misc)

<!-- /TOC -->

## Requirements
- Mac OS 10.11 or higher
- Xcode 8.1 or higher
- [openFrameworks](http://openframeworks.cc) 0.98 or higher
- [Homebrew](http://brew.sh)
    - [ImageMagick](http://www.imagemagick.org/)
    - [FFmpeg](https://www.ffmpeg.org)
    - [QREncode](http://fukuchi.org/works/qrencode/)

## How to Setup
### 1. openFrameworks
1. Download openFrameworks latest osx release version.
1. Change direcotory to `myApps` and clone this repository.
1. Add `+x` to `setup.sh` and execute it.
1. It will install some addons to appropriate path.

### 2. Automator

#### 2.1 Pre process

#### 2.2 Post process

##### 2.2.1 QR code

##### 2.2.2 Upload

### 3. Camera Utility (Canon)

#### 3.1 Resolution
* Don't need RAW
* Set "S1 fine" to approx 3K x 2K
    - In case of EOS Kiss, it could be 2592 x 1728

#### 3.2 Export setting
* Set import direcotory correctry like "import"
* Set rename setting to "<Shooting Time>|<Image Number>" like 1742480001.jpg

#### 3.3 Shooting
* Take continuous photos more than final movie rate

## How to Use

### Make mp4
`ffmpeg -r 10 -i android_%03d.png -c:v libx264 -pix_fmt yuv420p -vf "scale=trunc(iw/2)*2:trunc(ih/2)*2" android_yuv420p.mp4`

## Contributes
- Shin'ichiro SUZUKI

## ToDo

### High / FIXME

### Mid / TODO

### Low / XXX
- [ ] Change Icon
- [ ] Measure import filename's overflow
- [ ] Implement Sound Notification

## Misc.
