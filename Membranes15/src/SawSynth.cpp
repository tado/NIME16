#include "SawSynth.h"
#include "ofApp.h"

SawSynth::SawSynth(int n){
    death = false;
    detune = ofRandom(-0.15, 0.15);
    nth = n;
    alpha = 1;
    alphaMax = 127;
    //startTime = ofGetElapsedTimef();
    
    synth = new ofxSCSynth("col_closesaw");
    synth->set("gate", 1);
    synth->set("amp", 0.75);
    synth->set("rq", 1.0);
    synth->set("n", nth);
    synth->set("detune", detune);
    synth->create();
    
    freq = 20.0 * powf(1.5, nth);
    height = float(ofGetHeight()) / freq * 20.0;
}

SawSynth::~SawSynth() {
    synth->free();
}

void SawSynth::update(){

}

void SawSynth::draw(){

}

void SawSynth::clear(){
    
}