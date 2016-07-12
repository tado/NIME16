#include "ofApp.h"
//#include "ofxSuperColliderServer.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    //ofxSuperColliderServer::init(57110, 96000);
    //ofxSuperColliderServer::init();
    dropped = false;
    
    fx = new ofxSCSynth("fx");
    fx->create();
    
    for (int i = 0; i < filterSize; i++) {
        synth[i] = new ofxSCSynth("simpleSine");
        synth[i]->set("detune", ofRandom(0.9,1.15));
        synth[i]->create();
    }
    
    gui = new ofxUICanvas();
    gui->init(10, 10, 200, 200);
    gui->addSpacer();
    gui->addLabel("IMAGE -> AUDIO");
    gui->addSpacer();
    gui->addToggle("AUTO SCAN", true);
    gui->addSlider("SCAN SPEED", 0.0, 5.0, 1.0);
    gui->addSlider("FREQ RATIO", 1.0, 1.3, 1.15);
    gui->addSlider("FREQ OFFSET", 0, 400, 50);
    gui->addSlider("AMPLITUDE", 0.0, 0.1, 0.01);
    gui->addSpacer();
    gui->addButton("SAVE SETTINGS", false);
    gui->addSpacer();
    gui->loadSettings("main.xml");
    gui->autoSizeToFitWidgets();
    ofAddListener(gui->newGUIEvent,this,&ofApp::guiEvent);
    
    gui->setVisible(true);

    ofxUISlider *gratio = (ofxUISlider *)gui->getWidget("FREQ RATIO"); ratio = gratio->getValue();
    ofxUISlider *goffset = (ofxUISlider *)gui->getWidget("FREQ OFFSET"); freqOffset = goffset->getValue();
    
    for (int i = 0; i < filterSize; i++) {
        synth[i]->set("freq", powf(ratio, i) + freqOffset);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    ofxUIToggle *gautoscan = (ofxUIToggle *)gui->getWidget("AUTO SCAN"); bool autoScan = gautoscan->getValue();
    ofxUISlider *gscanSpeed = (ofxUISlider *)gui->getWidget("SCAN SPEED"); float scanSpeed = gscanSpeed->getValue();
    ofxUISlider *gamp = (ofxUISlider *)gui->getWidget("AMPLITUDE"); float amp = gamp->getValue();
    
    if (autoScan) {
        if (dropped) {
            scanX += scanSpeed;
        }
        if (scanX > ofGetWidth()) {
            scanX = 0;
        }
    } else {
        if (mouseX > 0 && mouseX < ofGetWidth()) {
            scanX = mouseX;
        }
    }
    for (int i = 0; i < filterSize; i++) {
        if (dropped) {
            synth[filterSize - i - 1]->set("mul", (1.0 - synthImage.getColor(scanX, i).getBrightness() / 255.0) * amp);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

    if (inputImage.getWidth() > 0) {
        ofSetColor(255);
        inputImage.draw(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(255, 0, 0);
        ofSetLineWidth(3.0);
        ofDrawLine(scanX, 0, scanX, ofGetHeight());
        ofSetLineWidth(1.0);
    }
    
    ofSetColor(255);
}

void ofApp::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    if(name == "SAVE SETTINGS"){
        gui->saveSettings("main.xml");
    }
    if (name == "FREQ RATIO") {
        ofxUISlider *gratio = (ofxUISlider *)gui->getWidget("FREQ RATIO"); ratio = gratio->getValue();
        for (int i = 0; i < filterSize; i++) {
            synth[i]->set("freq", powf(ratio, i) + freqOffset);
        }
    }
    if (name == "FREQ OFFSET") {
        ofxUISlider *goffset = (ofxUISlider *)gui->getWidget("FREQ OFFSET"); freqOffset = goffset->getValue();
        for (int i = 0; i < filterSize; i++) {
            synth[i]->set("freq", powf(ratio, i) + freqOffset);
        }
    }
}

void ofApp::dragEvent(ofDragInfo dragInfo){
    vector<ofImage> draggedImages;
    if( dragInfo.files.size() > 0 ){
        
        startFrame = ofGetFrameNum();
        
        // get image data
        draggedImages.assign( dragInfo.files.size(), ofImage() );
        for(unsigned int k = 0; k < dragInfo.files.size(); k++){
            draggedImages[k].load(dragInfo.files[k]);
        }
        draggedImages[0].resize(ofGetWidth(), ofGetHeight());
        inputImage = synthImage = draggedImages[0];
        
        // modify image
        synthImage.resize(ofGetWidth(), filterSize);
        synthImage.setImageType(OF_IMAGE_GRAYSCALE);
        synthImage.update();
        
        //reset scanX
        //scanX = 0;
        
        dropped = true;
    }
}

ofApp::~ofApp(){
    fx->free();
    for (int i = 0; i < filterSize; i++) {
        synth[i]->free();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

