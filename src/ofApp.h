#pragma once

#include "ofMain.h"
#include "ofxChromaKey.h"
#include "ofxDatGui.h"
#include "ofxXmlSettings.h"

#define TARGET_WIDTH   640
#define TARGET_HEIGHT  640
#define ANDROID_WIDTH  1440
#define ANDROID_HEIGHT 1395
#define SNS_WIDTH      640
#define SNS_HEIGHT     640
#define WEB_WIDTH      640
#define WEB_HEIGHT     640

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
  void importWebBackground();
  void exportForAndroid();
  void exportForSns();
  void exportForWeb();

  bool isTargetLoaded            = false;
  bool isAndroidBackgroundLoaded = false;
  bool isSnsBackgroundLoaded     = false;
  bool isWebBackgroundLoaded     = false;

  float desirableWidth    = 1.0 * (TARGET_WIDTH + WEB_WIDTH + ANDROID_WIDTH);
  float desirableHeight   = 1.0 * ANDROID_HEIGHT;
  float windowAspectRatio = desirableWidth / desirableHeight;
  float leftPaneRatio     = TARGET_WIDTH / desirableWidth;
  float middlePaneRatio   = WEB_WIDTH / desirableWidth;
  float rightPaneRatio    = ANDROID_WIDTH / desirableWidth;
  int currentFrame        = 0;

  int exportMovieFramerate = 10;
  int exportMovieSeconds   = 3;

  vector <ofImage> targetImages;
  vector <ofImage> androidBackgroundImages;
  vector <ofImage> snsBackgroundImages;
  ofImage webBackgroundImage;
  ofImage exportAndroidImage;
  ofImage exportSnsImage;
  ofImage exportWebImage;

  ofxChromaKey chromaKey;
  ofxDatGui *gui;
  ofFbo fbo_android;
  ofFbo fbo_sns;
  ofFbo fbo_web;
};
