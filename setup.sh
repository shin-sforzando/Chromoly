#!/usr/bin/env bash

set -eu
export LC_ALL=C

WORKING_DIR=$(cd $(dirname $0) && pwd)
OF_ROOT=$(cd $WORKING_DIR/../../.. && pwd)

cd $OF_ROOT/addons

# Install ofxChromaKey
git clone -o github https://github.com/tado/ofxChromaKey.git

# Install ofxDatGui
git clone -o github https://github.com/braitsch/ofxDatGui.git
cp -r ofxDatGui/ofxbraitsch $WORKING_DIR/bin/data

cd $WORKING_DIR

## Install Homebrew
# /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"

## Install ImageMagick
brew install imagemagick

## Install QREncode
brew install qrencode
