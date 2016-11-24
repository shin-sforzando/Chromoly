#include "ofApp.h"

void ofApp::setup()
{
  ofSetWindowTitle("Chromoly");
  ofSetVerticalSync(true);

  gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);

  chromaKey.threshold = 0.1;
  fbo.allocate(1440, 1395, GL_RGBA);
}

void ofApp::update()
{
}

void ofApp::draw()
{
  // TOP LEFT
  if (isTargetLoaded) {
    targetImages[currentFrame].draw(0, 0, ofGetWidth() * widthRatio, ofGetHeight() / 2);
  }

  // BOTTOM LEFT

  // RIGHT
  fbo.begin();
  ofSetColor(0);
  ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
  ofSetColor(255);
  if (isAndroidBackgroundLoaded) {
    androidBackgroundImages[currentFrame].draw(0, 0);
  }
  if (isTargetLoaded) {
    chromaKey.begin();
    targetImages[currentFrame].draw(1440 / 2 - 640 / 2, 651 + 93);
    chromaKey.end();
  }
  fbo.end();
  fbo.draw(ofGetWidth() * widthRatio, 0, ofGetWidth() * (1 - widthRatio), ofGetHeight());

  ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 20, ofGetHeight() / 2);  // TODO: Just Debug!
}

void ofApp::keyPressed(int key)
{
  switch (key) {
    case 'x':
      // For Debug!
      ofLog() << "widthRatio       : " + ofToString(widthRatio);
      ofLog() << "windowAspectRatio: " + ofToString(windowAspectRatio);
      break;
    case 'i':
      importTargets();
      break;
    case 'b':
      importAndroidBackgrounds();
      break;
    case 'e':
      exportPhotos();
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
      currentFrame += 1;
      break;
    case OF_KEY_LEFT:
      currentFrame -= 1;
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
    ofLog() << f.getAbsolutePath(); // TODO: Just Debug!
    ofImage importing;
    importing.load(f.getAbsolutePath());
    targetImages.push_back(importing);
  }
  chromaKey.keyColor = targetImages[0].getColor(630, 630);
  isTargetLoaded     = true;
}

void ofApp::importAndroidBackgrounds()
{
  ofDirectory androidBackgroundDirectory("/Users/suzuki/Desktop/NinaRicci/background_android");
  androidBackgroundDirectory.allowExt("jpg");
  androidBackgroundDirectory.listDir();
  androidBackgroundImages.clear();
  for (ofFile f : androidBackgroundDirectory.getFiles()) {
    ofLog() << f.getAbsolutePath(); // TODO: Just Debug!
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
    ofLog() << f.getAbsolutePath(); // TODO: Just Debug!
    ofImage importing;
    importing.load(f.getAbsolutePath());
    snsBackgroundImages.push_back(importing);
  }
  isSnsBackgroundLoaded = true;
}

void ofApp::exportPhotos()
{
  ofDirectory exportDirectory("/Users/suzuki/Desktop/NinaRicci/export");
  ofPixels    pixels;
  fbo.readToPixels(pixels);
  exportImage.setFromPixels(pixels);
  exportImage.save(exportDirectory.getAbsolutePath() + "/" + ofToString(currentFrame) + ".png", OF_IMAGE_QUALITY_BEST);
}
