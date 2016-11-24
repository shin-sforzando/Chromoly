#include "ofApp.h"

void ofApp::setup()
{
  ofSetWindowTitle("Chromoly (NINA RICCI 2016 Version)");
  ofSetVerticalSync(true);
  ofEnableAlphaBlending();

  gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);

  chromaKey.threshold = 0.1;
  fbo_android.allocate(1440, 1395);
  fbo_sns.allocate(640, 640);
  fbo_web.allocate(640, 640);
}

void ofApp::update()
{
}

void ofApp::draw()
{
  // LEFT TOP:      PREVIEW
  if (isTargetLoaded) {
    targetImages[currentFrame].draw(0, 0, ofGetWidth() * leftPaneRatio, ofGetWidth() * leftPaneRatio);
  }

  // LEFT BOTTOM:   GUI
  ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 36, ofGetHeight() / 2);  // TODO: Just Debug!
  ofDrawBitmapString(ofToString(currentFrame, 3, '0'), 36, ofGetHeight() / 2 + 50);  // TODO: Just Debug!

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
}

void ofApp::windowResized(int w, int h)
{
  ofSetWindowShape(ofGetHeight() * windowAspectRatio, ofGetHeight());
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::importTargets()
{
  ofDirectory foregroundDirectory("/Users/suzuki/Desktop/NinaRicci/import");
  foregroundDirectory.allowExt("jpg");
  foregroundDirectory.listDir();
  targetImages.clear();
  for (ofFile f : foregroundDirectory.getFiles()) {
    ofImage importing;
    importing.load(f.getAbsolutePath());
    targetImages.push_back(importing);
  }
  chromaKey.keyColor = targetImages[0].getColor(0, 0);
  isTargetLoaded     = true;
}

void ofApp::importAndroidBackgrounds()
{
  ofDirectory androidBackgroundDirectory("/Users/suzuki/Desktop/NinaRicci/background_android");
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
  ofDirectory snsBackgroundDirectory("/Users/suzuki/Desktop/NinaRicci/background_sns");
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
  webBackgroundImage.load("/Users/suzuki/Desktop/NinaRicci/background_web/web.png");
  isWebBackgroundLoaded = true;
}

void ofApp::exportForAndroid()
{
  ofDirectory exportDirectory("/Users/suzuki/Desktop/NinaRicci/export");
  ofPixels    pixels;
  fbo_android.readToPixels(pixels);
  exportAndroidImage.setFromPixels(pixels);
  exportAndroidImage.save(exportDirectory.getAbsolutePath() + "/android_" + ofToString(currentFrame, 3, '0') + ".png", OF_IMAGE_QUALITY_BEST);
}

void ofApp::exportForSns()
{
  ofDirectory exportDirectory("/Users/suzuki/Desktop/NinaRicci/export");
  ofPixels    pixels;
  fbo_sns.readToPixels(pixels);
  exportSnsImage.setFromPixels(pixels);
  exportSnsImage.save(exportDirectory.getAbsolutePath() + "/sns_" + ofToString(currentFrame, 3, '0') + ".png", OF_IMAGE_QUALITY_BEST);
}

void ofApp::exportForWeb()
{
  ofDirectory exportDirectory("/Users/suzuki/Desktop/NinaRicci/export");
  ofPixels    pixels;
  fbo_web.readToPixels(pixels);
  exportWebImage.setFromPixels(pixels);
  exportWebImage.save(exportDirectory.getAbsolutePath() + "/web.png", OF_IMAGE_QUALITY_BEST);
}
