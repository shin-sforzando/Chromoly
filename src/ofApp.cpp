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
}

void ofApp::keyPressed(int key)
{
  switch (key) {
    case 'x':
      // For Debug!
      break;
    case 'i':
      importPhotos();
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

void ofApp::importPhotos() {
  ofDirectory importDirectory("/Users/suzuki/Desktop/NinaRicci/import");
  importDirectory.allowExt("jpg");
  importDirectory.listDir();
  ofLog() << importDirectory.size();
  for(ofFile importing : importDirectory.getFiles()) {
    ofLog() << importing.getFileName();
  }
}

void ofApp::exportPhotos() {
  ofDirectory exportDirectory("/Users/suzuki/Desktop/NinaRicci/export");
}
