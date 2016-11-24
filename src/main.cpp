#include "ofMain.h"
#include "ofApp.h"

// use GLSL ver 3.2
int main()
{
  ofGLFWWindowSettings settings;
  settings.setGLVersion(3, 2);
  // settings.width  = 1440;
  // settings.height = 720;
  ofCreateWindow(settings);
  ofRunApp(new ofApp());
}

/*
 // use GLSL ver 2.1
 int main( ){
 ofSetupOpenGL(900,1200, OF_WINDOW);
 ofRunApp(new ofApp());
 }
 */
