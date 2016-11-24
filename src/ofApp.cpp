#include "ofApp.h"

void ofApp::setup()
{
  ofSetWindowTitle("Chromoly (NINA RICCI 2016 Version)");
  ofSetVerticalSync(true);
  ofEnableAlphaBlending();

  gui = new ofxDatGui(0, 0);
  gui->setTheme(new ofxDatGuiCustomFontSize);
  gui->addFRM();
  sliderCurrentFrame = gui->addSlider("Current Frame", 0, 30, 0);
  sliderCurrentFrame->setPrecision(0);
  sliderCurrentFrame->bind(currentFrame);
  colorPicker = gui->addColorPicker("Key Color");
  colorPicker->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
  sliderThreshold = gui->addSlider("Threshold", 0, 1.0, 0.1);
  sliderThreshold->setPrecision(3);
  sliderThreshold->bind(chromaKey.threshold);
  ofApp::windowResized(1400, 718);

  fbo_android.allocate(ANDROID_WIDTH, ANDROID_HEIGHT);
  fbo_sns.allocate(SNS_WIDTH, SNS_HEIGHT);
  fbo_web.allocate(WEB_WIDTH, WEB_HEIGHT);
}

void ofApp::update()
{
}

void ofApp::draw()
{
  //  +-----+-----+---------+
  //  | RAW | WEB |         |
  //  +-----+-----+ ANDROID +
  //  | GUI | SNS |         |
  //  +-----+-----+---------+
  // LEFT TOP:      RAW
  if (isTargetLoaded) {
    targetImages[currentFrame].draw(0, 0, ofGetWidth() * leftPaneRatio, ofGetWidth() * leftPaneRatio);
  }

  // LEFT BOTTOM:   GUI

  // MIDDLE TOP:    WEB
  if (isWebBackgroundLoaded) {
    webBackgroundImage.draw(ofGetWidth() * leftPaneRatio, 0, ofGetWidth() * middlePaneRatio, ofGetWidth() * middlePaneRatio);
  }

  // MIDDLE BOTTOM: SNS
  if (isSnsBackgroundLoaded) {
    snsBackgroundImages[currentFrame].draw(ofGetWidth() * leftPaneRatio, ofGetHeight() / 2, ofGetWidth() * middlePaneRatio, ofGetWidth() * middlePaneRatio);
  }

  // RIGHT:         ANDROID
  fbo_android.begin();
  ofSetColor(0);
  ofDrawRectangle(0, 0, fbo_android.getWidth(), fbo_android.getHeight());
  ofSetColor(255);
  if (isAndroidBackgroundLoaded) {
    androidBackgroundImages[currentFrame].draw(0, 0);
  }
  if (isTargetLoaded) {
    ofImage flipped;
    flipped.clone(targetImages[currentFrame]);
    flipped.mirror(true, false);
    chromaKey.begin();
    flipped.draw(ANDROID_WIDTH / 2 - TARGET_WIDTH / 2, 651 + 93);
    chromaKey.end();
  }
  fbo_android.end();
  fbo_android.draw(ofGetWidth() * (1 - rightPaneRatio), 0, ofGetWidth() * rightPaneRatio, ofGetHeight());
}

void ofApp::keyPressed(int key)
{
  switch (key) {
    case 'x':
      // For Debug!
      break;
    case 'i':
    case 'l':
    case 't':
      importTargets();
      break;
    case 'a':
      importAndroidBackgrounds();
      break;
    case 's':
      importSnsBackgrounds();
      break;
    case 'w':
      importWebBackground();
      break;
    case 'b':
      importAndroidBackgrounds();
      importSnsBackgrounds();
      importWebBackground();
      break;
    case 'e':
      exportForAndroid();
      exportForSns();
      exportForWeb();
      break;
    case OF_KEY_UP:
      chromaKey.threshold += 0.005;
      ofLog() << chromaKey.threshold;
      break;
    case OF_KEY_DOWN:
      chromaKey.threshold -= 0.005;
      ofLog() << chromaKey.threshold;
      break;
    case OF_KEY_RIGHT:
      if (isTargetLoaded) {
        currentFrame += 1;
        if (targetImages.size() - 1 < currentFrame) {
          currentFrame = 0;
        }
      }
      break;
    case OF_KEY_LEFT:
      if (isTargetLoaded) {
        currentFrame -= 1;
        if (currentFrame < 0) {
          currentFrame = targetImages.size() - 1;
        }
      }
      break;
    default:
      break;
  }
}

void ofApp::keyReleased(int key)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
}

void ofApp::mousePressed(int x, int y, int button)
{
}

void ofApp::mouseReleased(int x, int y, int button)
{
  if(button == 2) {
    // Right Click
    if (isTargetLoaded && x < ofGetWidth() * leftPaneRatio && y < ofGetWidth() * leftPaneRatio) {
      // Inside RAW Area
      int actualX = x * (1.0 * desirableWidth / ofGetWidth());
      int actualY = y * (1.0 * desirableHeight / ofGetHeight());
      chromaKey.keyColor = targetImages[currentFrame].getColor(actualX, actualY);
      colorPicker->setColor(chromaKey.keyColor);
    }
  }
}

void ofApp::windowResized(int w, int h)
{
  ofSetWindowShape(ofGetHeight() * windowAspectRatio, ofGetHeight());
  gui->setPosition(0, ofGetHeight() / 2);
  gui->setWidth(ofGetWidth() * leftPaneRatio);
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e){
  chromaKey.keyColor = e.color;
}

void ofApp::importTargets()
{
  ofDirectory targetDirectory(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/import");
  targetDirectory.allowExt("jpg");
  targetDirectory.listDir();
  targetImages.clear();
  for (ofFile f : targetDirectory.getFiles()) {
    ofImage importing;
    importing.load(f.getAbsolutePath());
    targetImages.push_back(importing);
  }
  chromaKey.keyColor = targetImages[0].getColor(0, 0);
  colorPicker->setColor(chromaKey.keyColor);
  sliderCurrentFrame->setMax(targetImages.size() - 1);
  isTargetLoaded     = true;
}

void ofApp::importAndroidBackgrounds()
{
  ofDirectory androidBackgroundDirectory(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/background_android");
  androidBackgroundDirectory.allowExt("jpg");
  androidBackgroundDirectory.listDir();
  androidBackgroundImages.clear();
  for (ofFile f : androidBackgroundDirectory.getFiles()) {
    ofImage importing;
    importing.load(f.getAbsolutePath());
    androidBackgroundImages.push_back(importing);
  }
  isAndroidBackgroundLoaded = true;
}

void ofApp::importSnsBackgrounds()
{
  ofDirectory snsBackgroundDirectory(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/background_sns");
  snsBackgroundDirectory.allowExt("jpg");
  snsBackgroundDirectory.listDir();
  snsBackgroundImages.clear();
  for (ofFile f : snsBackgroundDirectory.getFiles()) {
    ofImage importing;
    importing.load(f.getAbsolutePath());
    snsBackgroundImages.push_back(importing);
  }
  isSnsBackgroundLoaded = true;
}

void ofApp::importWebBackground()
{
  webBackgroundImage.load(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/background_web/web.png");
  isWebBackgroundLoaded = true;
}

void ofApp::exportForAndroid()
{
  ofDirectory exportDirectory(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/export");
  ofPixels    pixels;
  fbo_android.readToPixels(pixels);
  exportAndroidImage.setFromPixels(pixels);
  exportAndroidImage.save(exportDirectory.getAbsolutePath() + "/android_" + ofToString(currentFrame, 3, '0') + ".png", OF_IMAGE_QUALITY_BEST);
}

void ofApp::exportForSns()
{
  ofDirectory exportDirectory(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/export");
  ofPixels    pixels;
  fbo_sns.readToPixels(pixels);
  exportSnsImage.setFromPixels(pixels);
  exportSnsImage.save(exportDirectory.getAbsolutePath() + "/sns_" + ofToString(currentFrame, 3, '0') + ".png", OF_IMAGE_QUALITY_BEST);
}

void ofApp::exportForWeb()
{
  ofDirectory exportDirectory(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/export");
  ofPixels    pixels;
  fbo_web.readToPixels(pixels);
  exportWebImage.setFromPixels(pixels);
  exportWebImage.save(exportDirectory.getAbsolutePath() + "/web.png", OF_IMAGE_QUALITY_BEST);
}
