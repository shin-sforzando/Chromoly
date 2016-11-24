#include "ofApp.h"

void ofApp::setup()
{
  ofSetWindowTitle("Chromoly");
  ofSetVerticalSync(true);

  gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);

  chromaKey.threshold = 0.1;
  fbo.allocate(1440, 2560);
}

void ofApp::update()
{
}

void ofApp::draw()
{
  fbo.begin();
  ofSetColor(0);
  ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
  ofSetColor(255);
  if (isBackgroundLoaded) {
    backgroundImages[currentFrame].draw(0, 0);
  }
  if (isForegroundLoaded) {
    chromaKey.begin();
    foregroundImages[currentFrame].draw(1440 / 2 - 640 / 2, 651 + 93);
    chromaKey.end();
  }
  fbo.end();
  fbo.draw(0, 0, ofGetWidth(), ofGetWidth());
  ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 20, ofGetHeight() / 2);  // TODO: Just Debug!
}

void ofApp::keyPressed(int key)
{
  switch (key) {
    case 'x':
      // For Debug!
      break;
    case 'i':
      importForegrounds();
      isUpdated = true;
      break;
    case 'b':
      importBackgrounds();
      isUpdated = true;
      break;
    case 'e':
      exportPhotos();
      break;
    case OF_KEY_UP:
      chromaKey.threshold += 0.005;
      ofLog() << chromaKey.threshold;
      isUpdated = true;
      break;
    case OF_KEY_DOWN:
      chromaKey.threshold -= 0.005;
      ofLog() << chromaKey.threshold;
      isUpdated = true;
      break;
    case OF_KEY_RIGHT:
      currentFrame += 1;
      isUpdated     = true;
      break;
    case OF_KEY_LEFT:
      currentFrame -= 1;
      isUpdated     = true;
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
    ofLog() << f.getAbsolutePath(); // TODO: Just Debug!
    ofImage importing;
    importing.load(f.getAbsolutePath());
    foregroundImages.push_back(importing);
  }
  chromaKey.keyColor = foregroundImages[0].getColor(630, 630);
  isForegroundLoaded = true;
}

void ofApp::importBackgrounds()
{
  ofDirectory backgroundDirectory("/Users/suzuki/Desktop/NinaRicci/background");
  backgroundDirectory.allowExt("jpg");
  backgroundDirectory.listDir();
  backgroundImages.clear();
  for (ofFile f : backgroundDirectory.getFiles()) {
    ofLog() << f.getAbsolutePath(); // TODO: Just Debug!
    ofImage importing;
    importing.load(f.getAbsolutePath());
    backgroundImages.push_back(importing);
  }
  isBackgroundLoaded = true;
}

void ofApp::exportPhotos()
{
  ofDirectory exportDirectory("/Users/suzuki/Desktop/NinaRicci/export");
  ofPixels pixels;
  fbo.readToPixels(pixels);
  exportImage.setFromPixels(pixels);
  exportImage.save(exportDirectory.getAbsolutePath() + "/" + ofToString(currentFrame) + ".png", OF_IMAGE_QUALITY_BEST);
}
