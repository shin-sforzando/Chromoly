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
    font.size = 10;
    font.file = "M+2VM+IPAG-circle.ttf";
    layout.upperCaseLabels = false;
    init();
  }
};
