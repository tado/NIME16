#include "ofApp.h"
//#include "ofxSuperColliderServer.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //ofSetWindowPosition(1920, 0);
    
    ofSetFrameRate(60);
    ofBackground(0);
    ofSetLineWidth(1.0);
    pressed = false;
    //ofEnableBlendMode(OF_BLENDMODE_ADD);
    //cam.setFarClip(100000);
    //cam.setFov(110);
    //ofSetWindowPosition(1920, 0);
    
    sawNum = 0;
    zscaleRatio = currentZscale = 0.0;
    freqRatio = 1.12;
    
    //ofxSuperColliderServer::init(57110, 96000);
    //ofxSuperColliderServer::init();
    fx = new ofxSCSynth("fx");
    fx->create();
    sawFx = new ofxSCSynth("col_closefx");
    sawFx->create();
    sinFx = new ofxSCSynth("sinefx");
    sinFx->create();
    
    comb = new ofxSCSynth("col_comb");
    //comb->set("amp", 0.0);
    comb->create();
    
    zscale.addListener(this, &ofApp::zscaleChanged);
    fov.addListener(this, &ofApp::fovChanged);
    /*
     gui.setup();
     gui.add(freqRatio.setup("freqRatio", 1.3, 1.0, 2.0));
     gui.add(zscale.setup("z-scale", 0.0, 0.0, 10.0));
     gui.add(fov.setup("fov", 60, 10, 180));
     gui.loadFromFile("settings.xml");
     */
    cam.setFov(100);
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0; i < imageSynths.size(); i++) {
        imageSynths[i]->update();
    }
    if (imageSynths.size() < SYNTHNUM) {
        for (int i = 0; i < imageSynths.size(); i++) {
            imageSynths[i]->updateSynth();
        }
    } else {
        for (int i = imageSynths.size() - SYNTHNUM; i < imageSynths.size(); i++) {
            imageSynths[i]->updateSynth();
        }
    }
    
    
    currentZscale += (zscaleRatio - currentZscale) / 1000.0;
    for (int i = 0; i < imageSynths.size(); i++) {
        imageSynths[i]->zscaleRatio = currentZscale;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (pressed) {
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        ofSetColor(0, 63);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofEnableBlendMode(OF_BLENDMODE_ADD);
    } else {
        ofEnableDepthTest();
    }
    cam.begin();
    for (int i = 0; i < imageSynths.size(); i++) {
        imageSynths[i]->draw();
    }
    cam.end();
    ofDisableDepthTest();
    
    gui.draw();
    
    ofSetColor(255);
    ofDrawBitmapString(ofToString(saws.size()), ofGetWidth()-100, ofGetHeight()-20);
    
    //ofColor(255);
    //ofDrawCircle(mouseX, mouseY, 10);
}

void ofApp::dragEvent(ofDragInfo dragInfo){
    vector<ofImage> draggedImages;
    if( dragInfo.files.size() > 0 ){
        // get image data
        draggedImages.assign( dragInfo.files.size(), ofImage() );
        for(unsigned int k = 0; k < dragInfo.files.size(); k++){
            draggedImages[k].load(dragInfo.files[k]);
        }
        
        float size = 500;
        ofVec3f pos = ofVec3f(ofRandom(-size, size), ofRandom(-size, size), ofRandom(-size, size));
        ImageSynth *s = new ImageSynth(draggedImages[0], pos, freqRatio);
        s->zscale = zscale;
        imageSynths.push_back(s);
        if (imageSynths.size() > SYNTHNUM) {
            for (int i = 0; i < imageSynths[0]->filterSize; i++) {
                imageSynths[0]->synth[i]->free();
            }
        }
        if (imageSynths.size() > IMGNUM) {
            imageSynths.pop_front();
            //delete imageSynths[0];
        }
        //freqRatio *= 1.001;
        freqRatio += 0.01;
    }
}

ofApp::~ofApp(){
    fx->free();
    for (int j = 0; j < imageSynths.size(); j++) {
        for (int i = 0; i < imageSynths[j]->filterSize; i++) {
            imageSynths[j]->synth[i]->free();
        }
    }
    for (int i = 0; i < saws.size(); i++) {
        saws[i]->synth->free();
    }
    comb->free();
    imageSynths.clear();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'a') {
        if (imageSynths.size()>0) {
            for (int i = 0; i < imageSynths[0]->filterSize; i++) {
                imageSynths[0]->synth[i]->free();
            }
            imageSynths.pop_front();
        }
    }
    if (key == 's') {
        if (imageSynths.size()>0) {
            for (int i = 0; i < imageSynths[imageSynths.size()-1]->filterSize; i++) {
                imageSynths[imageSynths.size()-1]->synth[i]->free();
            }
            imageSynths.pop_back();
        }
    }
    if (key == 'z') {
        if (saws.size() < 19) {
            SawSynth *saw = new SawSynth(sawNum);
            sawNum++;
            zscaleRatio += 1.0;
            saws.push_back(saw);
        }
    }
    if (key == 'k') {
        for (int j = 0; j < imageSynths.size(); j++) {
            for (int i = 0; i < imageSynths[j]->filterSize; i++) {
                imageSynths[j]->synth[i]->free();
            }
        }
        for (int i = 0; i < saws.size(); i++) {
            saws[i]->synth->free();
        }
        comb->free();
        imageSynths.clear();
    }
    if (key == ' ') {
        ofSetBackgroundAuto(false);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        pressed = true;
        for (int i = 0; i < imageSynths.size(); i++) {
            imageSynths[i]->pressed = true;
        }
        //comb->set("amp", 1.0);
        comb->set("inamp", 1.0);
    }
}

void ofApp::zscaleChanged(float & zscale){
    for (int i = 0; i < imageSynths.size(); i++) {
        imageSynths[i]->zscale = zscale;
    }
}

void ofApp::fovChanged(float & fov){
    cam.setFov(fov);
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == ' ') {
        ofSetBackgroundAuto(true);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        pressed = false;
        for (int i = 0; i < imageSynths.size(); i++) {
            imageSynths[i]->pressed = false;
        }
        comb->set("inamp", 0.0);
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (pressed) {
        float deltime = ofMap(x, 0, ofGetWidth(), 0.005, 0.1);
        comb->set("delaytime", deltime);
    }
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

