#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxSuperCollider.h"
#include "ofxUI.h"

class ofApp : public ofBaseApp{
    
public:
    ~ofApp();
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void guiEvent(ofxUIEventArgs &e);
    
    ofImage inputImage, synthImage;

    static const int filterSize = 100;
    ofxSCSynth *synth[filterSize];
    ofxSCSynth *fx;
    float scanX;
    int startFrame;
    
    float ratio;
    float freqOffset;
    
    ofxUICanvas *gui;
    
    bool dropped;
};
