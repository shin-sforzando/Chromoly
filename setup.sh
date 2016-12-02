#!/usr/bin/env bash

set -u
export LC_ALL=C

WORKING_DIR=$(cd $(dirname $0) && pwd)
OF_ROOT=$(cd $WORKING_DIR/../../.. && pwd)

cd $OF_ROOT/addons

# Install ofxChromaKey
git clone -o github https://github.com/tado/ofxChromaKey.git

# Install ofxDatGui
git clone -o github https://github.com/braitsch/ofxDatGui.git
cp -rf ofxDatGui/ofxbraitsch $WORKING_DIR/bin/data

cd $WORKING_DIR

## Install Homebrew
which brew || /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

## Install Curl
brew install curl --with-openssl || brew upgrade curl

## Install Git
brew install git --with-brewed-curl --with-brewed-openssl || brew upgrade git

## Install ImageMagick
brew install imagemagick || brew upgrade imagemagick

## Install ffmpeg
brew install ffmpeg || brew upgrade ffmpeg

## Install s3cmd
brew install s3cmd || brew upgrade s3cmd

## Install QREncode
brew install qrencode || brew upgrade qrencode
