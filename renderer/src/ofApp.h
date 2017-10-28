#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxJSON.h"
#include "ofxCv.h"

const int width = 640;
const int height = 640;

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void loadFeatMatrix();
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
    
    vector<ofVec2f> y;
    vector<ofVec2f> yNew;
    vector<vector<float> > feat_matrix;

    ofxPanel gui;
    ofxFloatSlider lineAlpha;
    ofxFloatSlider stretchRate;
    ofxIntSlider refreshSec;
    ofxIntSlider distThreshold;
    
    ofVec2f sample;
    int sampleIndex;
    
    ofVboMesh stringsNew;
    bool drawGui;
    
    ofFbo fbo;
    
    ofxOscReceiver receiver;
    
    int curFrame;
    
    ofxJSONElement responsex, responsey, responsez;
    vector<ofPoint> points;

    ofxCv::KalmanPosition kalman;
	ofSerial serial;

    ofApp() :
    curFrame(0),
    sampleIndex(0) {
    }
};
