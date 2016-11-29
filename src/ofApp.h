#pragma once

#include "ofMain.h"
#include "ofxChromaKey.h"
#include "ofxDatGui.h"
#include "ofxDatGuiCustomFontSize.h"
#include "ofxXmlSettings.h"

#define FRAME_NUM      36
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

  void onSliderPreviewFramerateEvent(ofxDatGuiSliderEvent e);
  void onButtonReloadEvent(ofxDatGuiButtonEvent e);
  void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
  void onPadWebPosition(ofxDatGui2dPadEvent e);
  void onPadSnsPosition(ofxDatGui2dPadEvent e);
  void onButtonExportEvent(ofxDatGuiButtonEvent e);
  void importTargets();
  void importWebBackground();
  void importSnsBackgrounds();
  void importAndroidBackgrounds();
  void exportForWeb();
  void exportForSns();
  void exportForAndroid();

  bool isTargetLoaded            = false;
  bool isWebBackgroundLoaded     = false;
  bool isSnsBackgroundLoaded     = false;
  bool isAndroidBackgroundLoaded = false;
  bool isExporting               = false;

  float desirableWidth    = 1.0 * (TARGET_WIDTH + WEB_WIDTH + ANDROID_WIDTH);
  float desirableHeight   = 1.0 * ANDROID_HEIGHT;
  float windowAspectRatio = desirableWidth / desirableHeight;
  float leftPaneRatio     = TARGET_WIDTH / desirableWidth;
  float middlePaneRatio   = WEB_WIDTH / desirableWidth;
  float rightPaneRatio    = ANDROID_WIDTH / desirableWidth;

  int previewFramerate = 10;
  int currentFrame     = 0;

  int webCaptureFrame   = 0;
  float webOverlayScale = 0.5;
  int webOverlayX       = WEB_WIDTH / 2;
  int webOverlayY       = WEB_HEIGHT / 2;
  float snsOverlayScale = 0.4;
  int snsOverlayX       = SNS_WIDTH / 2;
  int snsOverlayY       = SNS_HEIGHT / 2;


  vector <ofImage> targetImages;
  vector <ofImage> snsBackgroundImages;
  vector <ofImage> androidBackgroundImages;
  ofImage webBackgroundImage;
  ofImage webCheckerImage;
  ofImage exportWebImage;
  ofImage exportSnsImage;
  ofImage exportAndroidImage;

  ofxChromaKey chromaKey;

  ofxDatGui *gui;
  ofxDatGuiSlider *sliderPreviewFramerate;
  ofxDatGuiButton *buttonReload;
  ofxDatGuiSlider *sliderCurrentFrame;
  ofxDatGuiFolder *folderChromakey;
  ofxDatGuiColorPicker *colorPicker;
  ofxDatGuiSlider *sliderThreshold;
  ofxDatGuiFolder *folderWebOverlay;
  ofxDatGuiSlider *sliderWebCaptureFrame;
  ofxDatGuiSlider *sliderWebOverlayScale;
  ofxDatGui2dPad *padWebPosition;
  ofxDatGuiFolder *folderSnsOverlay;
  ofxDatGuiSlider *sliderSnsOverlayScale;
  ofxDatGui2dPad *padSnsPosition;
  ofxDatGuiButton *buttonExport;

  ofFbo fbo_web;
  ofFbo fbo_sns;
  ofFbo fbo_android;
};
