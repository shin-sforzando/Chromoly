#pragma once

#include "ofMain.h"
#include "ofxChromaKey.h"
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp {
public:
  void setup();
  void update();
  void draw();
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);

  void importTargets();
  void importAndroidBackgrounds();
  void importSnsBackgrounds();
  void exportPhotos();

  bool isTargetLoaded = false;
  bool isAndroidBackgroundLoaded = false;
  bool isSnsBackgroundLoaded = false;
  float windowAspectRatio = (640 + 1440) / 1395.0;
  float widthRatio        = 640 / 1440.0;
  int currentFrame        = 0;

  int exportMovieFramerate = 10;
  int exportMovieSeconds   = 3;

  vector<ofImage> targetImages;
  vector<ofImage> androidBackgroundImages;
  vector<ofImage> snsBackgroundImages;
  ofImage exportImage;

  ofxChromaKey chromaKey;
  ofxDatGui *gui;
  ofFbo fbo;
};
