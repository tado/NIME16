#pragma once
#include "ofMain.h"
#include "ofxSuperCollider.h"
#include "ofxCv.h"

class ImageSynth {
public:
    ImageSynth(ofImage image, ofVec3f pos, float freqRatio);
    ~ImageSynth();
    void update();
    void updateSynth();
    void draw();
    void drawWireframe();
    void createMesh();
    
    static const int filterSize = 50;
    ofxSCSynth *synth[filterSize];
    int scanX;
    int startFrame;
    ofImage inputImage, synthImage, depthImage;
    ofVec3f pos;
    float startTime;
    ofVec3f rot;
    ofVec3f rotSpeed;
    float sumLevel;
    float freqRatio;
    float zscale;
    float zscaleRatio;
    const float baseSpeed = 0.05;
    
    ofMesh mesh;
    int stepSize, xSteps, ySteps;
    bool pressed;
};