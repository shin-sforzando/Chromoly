//
//  ofxDatGuiCustomFontSize.h
//  Chromoly
//
//  Created by Shin'ichiro SUZUKI on 2016/11/25.
//
//

#pragma once
#include "ofxDatGuiTheme.h"

class ofxDatGuiCustomFontSize: public ofxDatGuiTheme {
public:
  ofxDatGuiCustomFontSize()
  {
    font.size = 9;
    layout.upperCaseLabels = false;
    init();
  }
};
