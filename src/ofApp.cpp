#include "ofApp.h"

ofDirectory exportDirectory(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/export");

void ofApp::setup()
{
  ofSetWindowTitle("Chromoly (NINA RICCI 2016 Version)");
  ofSetVerticalSync(true);
  ofEnableAlphaBlending();
  ofSetFrameRate(previewFramerate);

  gui = new ofxDatGui(0, 0);
  gui->setTheme(new ofxDatGuiCustomFontSize);
  textVisitorNumber = gui->addTextInput("Visitor Number", ofToString(ofApp::getNewVisitorNumber()));
  textVisitorNumber->setInputType(ofxDatGuiInputType::NUMERIC);
  textVisitorNumber->onTextInputEvent(this, &ofApp::onTextVisitorNumberEvent);
  gui->addFRM();
  sliderPreviewFramerate = gui->addSlider("Preview fps", 0.0, 60.0);
  sliderPreviewFramerate->setPrecision(0);
  sliderPreviewFramerate->bind(previewFramerate);
  sliderPreviewFramerate->onSliderEvent(this, &ofApp::onSliderPreviewFramerateEvent);
  buttonReload = gui->addButton("Reload");
  buttonReload->onButtonEvent(this, &ofApp::onButtonReloadEvent);
  buttonReload->setLabelAlignment(ofxDatGuiAlignment::CENTER);
  buttonReload->setBorder(ofColor::red, 10);
  sliderCurrentFrame = gui->addSlider("Current Frame", 0.0, 30.0, 0.0);
  sliderCurrentFrame->setPrecision(0);
  sliderCurrentFrame->bind(currentFrame);
  folderChromakey = gui->addFolder("Chroma Key");
  colorPicker     = folderChromakey->addColorPicker(" - Key Color");
  colorPicker->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
  sliderThreshold = folderChromakey->addSlider(" - Threshold", 0, 1.0, 0.1);
  sliderThreshold->setPrecision(3);
  sliderThreshold->bind(chromaKey.threshold);
  folderChromakey->expand();
  folderWebOverlay      = gui->addFolder("For Web:");
  sliderWebCaptureFrame = folderWebOverlay->addSlider(" - Frame", 0.0, 30.0);
  sliderWebCaptureFrame->setPrecision(0);
  sliderWebCaptureFrame->bind(webCaptureFrame);
  sliderWebOverlayScale = folderWebOverlay->addSlider(" - Scale", 0.0, 1.0);
  sliderWebOverlayScale->bind(webOverlayScale);
  padWebPosition = folderWebOverlay->add2dPad(" - Position");
  padWebPosition->setBounds(ofRectangle(0, 0, WEB_WIDTH, WEB_HEIGHT));
  padWebPosition->setPoint(ofPoint(webOverlayX, webOverlayY));
  padWebPosition->on2dPadEvent(this, &ofApp::onPadWebPosition);
  folderSnsOverlay      = gui->addFolder("For SNS:");
  sliderSnsOverlayScale = folderSnsOverlay->addSlider(" - Scale", 0.0, 1.0);
  sliderSnsOverlayScale->bind(snsOverlayScale);
  padSnsPosition = folderSnsOverlay->add2dPad(" - Position");
  padSnsPosition->setBounds(ofRectangle(0, 0, SNS_WIDTH, SNS_HEIGHT));
  padSnsPosition->setPoint(ofPoint(snsOverlayX, snsOverlayY));
  padSnsPosition->on2dPadEvent(this, &ofApp::onPadSnsPosition);
  buttonExport = gui->addButton("Export");
  buttonExport->onButtonEvent(this, &ofApp::onButtonExportEvent);
  buttonExport->setLabelAlignment(ofxDatGuiAlignment::CENTER);
  ofApp::windowResized(1400, 718);

  fbo_android.allocate(ANDROID_WIDTH, ANDROID_HEIGHT);
  fbo_sns.allocate(SNS_WIDTH, SNS_HEIGHT);
  fbo_web.allocate(WEB_WIDTH, WEB_HEIGHT);

  importAndroidBackgrounds();
  importSnsBackgrounds();
  importWebBackground();
}

//  +-----+-----+---------+
//  | RAW | WEB |         |
//  +-----+-----+ ANDROID +
//  | GUI | SNS |         |
//  +-----+-----+---------+
void ofApp::update()
{
  // LEFT TOP:      RAW

  // LEFT BOTTOM:   GUI

  // MIDDLE TOP:    WEB
  fbo_web.begin();
  ofClear(0);
  if (isWebBackgroundLoaded) {
    webBackgroundImage.draw(0, 0);
  }
  if (isTargetLoaded) {
    chromaKey.begin();
    targetImages[webCaptureFrame].setAnchorPercent(0.5, 0.5);
    targetImages[webCaptureFrame].draw(webOverlayX, webOverlayY, WEB_WIDTH * webOverlayScale, WEB_HEIGHT * webOverlayScale);
    targetImages[webCaptureFrame].resetAnchor();
    chromaKey.end();
  }
  fbo_web.end();

  // MIDDLE BOTTOM: SNS
  fbo_sns.begin();
  ofClear(0);
  if (isSnsBackgroundLoaded) {
    snsBackgroundImages[currentFrame].draw(0, 0);
  }
  if (isTargetLoaded) {
    chromaKey.begin();
    targetImages[currentFrame].setAnchorPercent(0.5, 0.5);
    targetImages[currentFrame].draw(snsOverlayX, snsOverlayY, SNS_WIDTH * snsOverlayScale, SNS_HEIGHT * snsOverlayScale);
    targetImages[currentFrame].resetAnchor();
    chromaKey.end();
  }
  fbo_sns.end();

  // RIGHT:         ANDROID
  fbo_android.begin();
  ofSetColor(0);
  ofDrawRectangle(0, 0, fbo_android.getWidth(), fbo_android.getHeight());
  ofSetColor(255);
  if (isTargetLoaded) {
    chromaKey.begin();
    targetImages[currentFrame].draw(ANDROID_WIDTH / 2 - TARGET_WIDTH / 2, 651 - TARGET_HEIGHT);
    chromaKey.end();
  }
  if (isAndroidBackgroundLoaded) {
    androidBackgroundImages[currentFrame].draw(0, 651 + 93);
  }
  fbo_android.end();
}

void ofApp::draw()
{
  // LEFT TOP:      RAW
  if (isTargetLoaded) {
    targetImages[currentFrame].draw(0, 0, ofGetWidth() * leftPaneRatio, ofGetWidth() * leftPaneRatio);
  }

  // LEFT BOTTOM:   GUI

  // MIDDLE TOP:    WEB
  webCheckerImage.draw(ofGetWidth() * leftPaneRatio, 0, ofGetWidth() * middlePaneRatio, ofGetWidth() * middlePaneRatio);
  fbo_web.draw(ofGetWidth() * leftPaneRatio, 0, ofGetWidth() * middlePaneRatio, ofGetWidth() * middlePaneRatio);

  // MIDDLE BOTTOM: SNS
  fbo_sns.draw(ofGetWidth() * leftPaneRatio, ofGetHeight() / 2, ofGetWidth() * middlePaneRatio, ofGetWidth() * middlePaneRatio);
  if (isExporting) {
    ofApp::exportForSns();
  }

  // RIGHT:         ANDROID
  fbo_android.draw(ofGetWidth() * (1 - rightPaneRatio), 0, ofGetWidth() * rightPaneRatio, ofGetHeight());
  if (isExporting) {
    ofApp::exportForAndroid();
  }

  if (isTargetLoaded && currentFrame + 1 < min(targetImages.size() - 1, androidBackgroundImages.size() - 1)) {
    currentFrame++;
  } else {
    currentFrame = 0;
    if (isExporting) {
      ofApp::exportFinish();
    }
  }
}

void ofApp::keyPressed(int key)
{
  switch (key) {
    case 'x':
      // For Debug!
      ofApp::convertAndroidMovie();
      break;
    case 'i':
    case 'l':
    case 't':
    case 'r':
      importTargets();
      break;
    case 'w':
      importWebBackground();
      break;
    case 's':
      importSnsBackgrounds();
      break;
    case 'a':
      importAndroidBackgrounds();
      break;
    case 'b':
      importWebBackground();
      importSnsBackgrounds();
      importAndroidBackgrounds();
      break;
    case 'e':
      ofApp::exportStart();
      break;
    case OF_KEY_UP:
      chromaKey.threshold += 0.005;
      break;
    case OF_KEY_DOWN:
      chromaKey.threshold -= 0.005;
      break;
    case OF_KEY_RIGHT:
      webCaptureFrame++;
      if (targetImages.size() - 1 < webCaptureFrame) {
        webCaptureFrame = 0;
      }
      break;
    case OF_KEY_LEFT:
      webCaptureFrame--;
      if (webCaptureFrame < 0) {
        webCaptureFrame = targetImages.size() - 1;
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
  if (button == 2) {
    // Right Click
    if (isTargetLoaded && x < ofGetWidth() * leftPaneRatio && y < ofGetWidth() * leftPaneRatio) {
      // Inside RAW Area
      int actualX = x * (1.0 * desirableWidth / ofGetWidth());
      int actualY = y * (1.0 * desirableHeight / ofGetHeight());
      chromaKey.keyColor = targetImages[webCaptureFrame].getColor(actualX, actualY);
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
  ofApp::say(msg.message);
}

void ofApp::onTextVisitorNumberEvent(ofxDatGuiTextInputEvent e)
{
  if (e.text != "") {
    visitorNumber = ofToInt(e.text);
  } else {
    visitorNumber = ofApp::getNewVisitorNumber();
    textVisitorNumber->setText(ofToString(visitorNumber));
  }
}

void ofApp::onSliderPreviewFramerateEvent(ofxDatGuiSliderEvent e)
{
  ofSetFrameRate(previewFramerate);
}

void ofApp::onButtonReloadEvent(ofxDatGuiButtonEvent e)
{
  importTargets();
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
  chromaKey.keyColor = e.color;
}

void ofApp::onPadWebPosition(ofxDatGui2dPadEvent e)
{
  webOverlayX = e.x;
  webOverlayY = e.y;
}

void ofApp::onPadSnsPosition(ofxDatGui2dPadEvent e)
{
  snsOverlayX = e.x;
  snsOverlayY = e.y;
}

void ofApp::onButtonExportEvent(ofxDatGuiButtonEvent e)
{
  ofApp::exportStart();
}

void ofApp::importTargets()
{
  ofApp::say("Images loading has started.");
  ofDirectory targetDirectory(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/import");
  targetDirectory.allowExt("jpg");
  targetDirectory.listDir();
  vector <ofFile> reversed = targetDirectory.getFiles();
  if (reversed.size() < FRAME_NUM) {
    ofSystemAlertDialog("Error! It requires " + ofToString(FRAME_NUM) + " photos or more.");
  }
  targetImages.clear();
  reverse(reversed.begin(), reversed.end());
  for (int i = 0; i < FRAME_NUM; i++) {
    ofImage importing;
    ofImage cropped;
    importing.load(reversed[i].getAbsolutePath());
    ofLog() << reversed[i].getAbsolutePath();  // TODO: Just Debug!
    cropped.cropFrom(importing, (importing.getWidth() - TARGET_WIDTH) / 2, (importing.getHeight() - TARGET_HEIGHT) / 2, TARGET_WIDTH, TARGET_HEIGHT);
    targetImages.push_back(cropped);
  }
  chromaKey.keyColor = targetImages[0].getColor(0, 0);
  colorPicker->setColor(chromaKey.keyColor);
  sliderCurrentFrame->setMax(targetImages.size() - 1);
  sliderWebCaptureFrame->setMax(targetImages.size() - 1);
  isTargetLoaded = true;
  ofApp::say("Images loading is completed.");
}

void ofApp::importWebBackground()
{
  webBackgroundImage.load(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/background_web/web.png");
  webCheckerImage.load(ofFilePath::getUserHomeDir() + "/Desktop/NinaRicci/background_web/checker.png");
  isWebBackgroundLoaded = true;
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

void ofApp::exportStart()
{
  if (isTargetLoaded) {
    isExporting  = true;
    currentFrame = 0;
    sliderCurrentFrame->setBackgroundColor(ofColor(255, 0, 0));
    sliderCurrentFrame->setLabel("Exporting...");
    ofApp::exportForWeb();
  }
}

void ofApp::exportFinish()
{
  isExporting = false;
  sliderCurrentFrame->setBackgroundColor(ofColor(250, 250, 250));
  sliderCurrentFrame->setLabel("Current Frame");
  ofApp::say("Images exporting is completed.");
}

void ofApp::exportForWeb()
{
  ofPixels pixels;
  fbo_web.readToPixels(pixels);
  exportWebImage.setFromPixels(pixels);
  exportWebImage.save(exportDirectory.getAbsolutePath() + "/" + ofGetTimestampString("%Y%m%d") + "_" + ofToString(visitorNumber, 3, '0') + "/web.png", OF_IMAGE_QUALITY_BEST);
}

void ofApp::exportForSns()
{
  ofPixels pixels;
  fbo_sns.readToPixels(pixels);
  exportSnsImage.setFromPixels(pixels);
  exportSnsImage.save(exportDirectory.getAbsolutePath() + "/" + ofGetTimestampString("%Y%m%d") + "_" + ofToString(visitorNumber, 3, '0') + "/sns_" + ofToString(currentFrame, 3, '0') + ".png", OF_IMAGE_QUALITY_BEST);
}

void ofApp::exportForAndroid()
{
  ofPixels pixels;
  fbo_android.readToPixels(pixels);
  exportAndroidImage.setFromPixels(pixels);
  exportAndroidImage.save(exportDirectory.getAbsolutePath() + "/" + ofGetTimestampString("%Y%m%d") + "_" + ofToString(visitorNumber, 3, '0') + "/android_" + ofToString(currentFrame, 3, '0') + ".png", OF_IMAGE_QUALITY_BEST);
}

void ofApp::convertSnsMovie()
{
  string path = exportDirectory.getAbsolutePath() + "/" + ofGetTimestampString("%Y%m%d") + "_" + ofToString(visitorNumber, 3, '0');
  ofSystem("/usr/local/bin/ffmpeg -r 10 -i " + path + "/sns_%03d.png -pix_fmt yuv420p " + path + "/" + ofToString(visitorNumber, 3, '0') + "_sns.mp4");
}

void ofApp::convertAndroidMovie()
{
  string path = exportDirectory.getAbsolutePath() + "/" + ofGetTimestampString("%Y%m%d") + "_" + ofToString(visitorNumber, 3, '0');
  ofSystem("/usr/local/bin/ffmpeg -r 10 -i " + path + "/android_%03d.png -c:v libx264 -pix_fmt yuv420p -vf scale=1440:1396 " + path + "/" + ofToString(visitorNumber, 3, '0') + ".mp4");
}

void ofApp::uploadAll()
{
  ofSystem("/usr/local/bin/s3cmd sync --recursive --force --acl-public --guess-mime-type " + exportDirectory.getAbsolutePath() + " s3://nina-ricci/");
  ofApp::say("Movies uploading is completed.");
}

void ofApp::printQRcode()
{
  ofApp::say("QR code printing is completed.");
}

int ofApp::getNewVisitorNumber()
{
  exportDirectory.listDir();
  int newVisitorNumber = 0;
  for (ofFile f : exportDirectory.getFiles()) {
    if (f.isDirectory()) {
      if (ofIsStringInString(f.getBaseName(), ofGetTimestampString("%Y%m%d"))) {
        string str_number = f.getBaseName();
        ofLog() << str_number;
        ofStringReplace(str_number, ofGetTimestampString("%Y%m%d") + "_", "");
        int int_number = ofToInt(str_number);
        if (newVisitorNumber + 1 != int_number) {
          visitorNumber = newVisitorNumber + 1;

          return visitorNumber;
        } else {
          newVisitorNumber = int_number;
        }
      }
    }
  }

  return newVisitorNumber;
}

void ofApp::say(string msg)
{
  ofSystem("say -v Victoria " + msg);
}
