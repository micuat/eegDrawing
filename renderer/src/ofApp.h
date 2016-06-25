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
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    vector<ofVec2f> y;
    vector<ofVec2f> yNew;
    vector<vector<float> > feat_matrix;
    vector<float> alphas;
    ofImage softPoint;
    
    ofVec2f minXY, maxXY;
    
    ofxPanel gui;
    ofxIntSlider sliderChannel;
    ofxFloatSlider sliderUpperLimit;
    ofxToggle toggleColor;
    ofxIntSlider sliderFrame;
    ofxIntSlider refreshSec;
    ofxIntSlider distThreshold;
    ofxFloatSlider lineAlpha;
    ofxToggle mouseDebug;
    ofxIntSlider forceDuration;
    ofxFloatSlider forceIntensity, velocity;
    ofxToggle bFeedback;
    
    ofVec2f sample;
    ofVec2f samplePrev;
    ofVec2f newPosPrev;
    int sampleIndex;
    
    ofVboMesh strings;
    ofVboMesh stringsNew;
    bool drawGui;
    
    ofFbo fbo;
    
    ofxOscReceiver receiver;
    
    ofVideoPlayer videoPlayer;
    int curFrame;
    
    ofxJSONElement responsex, responsey, responsez;
    vector<ofPoint> points;
    
    bool showVideo;
    bool showPoints;
    
    ofxCv::KalmanPosition kalman;

    ofApp() : showVideo(true), showPoints(true), curFrame(0) {
    }
};
