#include "ofApp.h"

void ofApp::setup()
{
  ofSetWindowTitle("Chromoly");
  ofSetVerticalSync(true);
  gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
}

void ofApp::update()
{
}

void ofApp::draw()
{
  if (backgroundImages.size() != 0) {
    backgroundImages[currentFrame].draw(0, 0, ofGetWidth(), ofGetWidth() / 2);
  }

  if (foregroundImages.size() != 0) {
    foregroundImages[currentFrame].draw(0, ofGetWidth() / 2, ofGetWidth(), ofGetWidth() / 2);
    if (foregroundImages.size() - 1 <= currentFrame) {
      currentFrame = 0;
    }
    currentFrame++;
  }
  ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 20, 20);
}

void ofApp::keyPressed(int key)
{
  switch (key) {
    case 'x':
      // For Debug!
      break;
    case 'i':
      importForegrounds();
      break;
    case 'b':
      importBackgrounds();
      break;
    case 'e':
      exportPhotos();
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
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::importForegrounds()
{
  ofDirectory foregroundDirectory("/Users/suzuki/Desktop/NinaRicci/import");
  foregroundDirectory.allowExt("jpg");
  foregroundDirectory.listDir();
  foregroundImages.clear();
  for (ofFile f : foregroundDirectory.getFiles()) {
    ofLog() << f.getAbsolutePath();
    ofImage importing;
    importing.load(f.getAbsolutePath());
    foregroundImages.push_back(importing);
  }
}

void ofApp::importBackgrounds()
{
  ofDirectory backgroundDirectory("/Users/suzuki/Desktop/NinaRicci/background");
  backgroundDirectory.allowExt("jpg");
  backgroundDirectory.listDir();
  backgroundImages.clear();
  for (ofFile f : backgroundDirectory.getFiles()) {
    ofLog() << f.getAbsolutePath();
    ofImage importing;
    importing.load(f.getAbsolutePath());
    backgroundImages.push_back(importing);
  }
}

void ofApp::exportPhotos()
{
  ofDirectory exportDirectory("/Users/suzuki/Desktop/NinaRicci/export");
}
