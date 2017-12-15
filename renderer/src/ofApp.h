#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxJSON.h"
#include "ofxCv.h"
#include "ofxVoronoi.h"

const int width = 800;// 640;
const int height = 800;// 640;

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
    ofxToggle doVideo;
    ofxFloatSlider lineAlpha;
	ofxFloatSlider stretchRate;
	ofxFloatSlider lightAmbient, lightDiffuse, lightAttenuation, lightCutoff;
	ofxIntSlider refreshSec;
    ofxIntSlider distThreshold;
    
    ofVideoGrabber grabber;
    vector<ofImage> grabbedImages;
    
    ofVec2f sample;
    int sampleIndex;
    
    bool drawGui;
    
    ofFbo fbo;
    
    ofxOscReceiver receiver;
    
    int curFrame;
    
    ofxJSONElement responsex, responsey, responsez;
    vector<ofPoint> points;
	vector<float> pointIntensity;
	ofVec2f p0;

	ofxVoronoi voronoi;
	vector<ofLight> lights;
	ofEasyCam cam;

    ofxCv::KalmanPosition kalman;
	ofSerial serial;
	int serialCount;

    ofApp() :
    curFrame(0),
		serialCount(0),
    sampleIndex(0),
	sample(0.5f, 0) {
    }
};
