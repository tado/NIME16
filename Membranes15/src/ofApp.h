#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxSuperCollider.h"
#include "ofxGui.h"
#include "ImageSynth.h"
#include "SawSynth.h"

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
    
    void zscaleChanged(float & zscale);
    void fovChanged(float & fov);
    
    ofxSCSynth *fx, *sawFx, *comb, *sinFx;
    deque<ImageSynth*> imageSynths;
    ofEasyCam cam;
    
    ofxPanel gui;
    //ofxFloatSlider freqRatio;
    ofxFloatSlider zscale;
    ofxFloatSlider fov;
    
    int sawNum;
    deque<SawSynth *> saws;
    float zscaleRatio, currentZscale;
    float freqRatio;
    bool pressed;
    
    static const int SYNTHNUM = 3;
    static const int IMGNUM = 6;
};
