#include "ofApp.h"

#include "ofxNumpy.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofHideCursor();
    ofSetBackgroundAuto(false);
    
    receiver.setup(13000);

    loadFeatMatrix();

    stringsNew.setMode(OF_PRIMITIVE_LINES);
    
    gui.setup();
    gui.add(lineAlpha.setup("Line Alpha", 0.5f, 0, 1));
    gui.add(stretchRate.setup("Stretch Rate (Mapping)", 1, 0, 2));
    gui.add(refreshSec.setup("Refresh Sec", 5, 1, 20));
    gui.add(distThreshold.setup("Distance", 150, 50, 300));
    gui.loadFromFile("settings.xml");
    drawGui = false;
    
    fbo.allocate(width * 2, height * 2, GL_RGB);
    
    kalman.init(1e-4, 1e+3);
    
	serial.setup("COM5", 115200); // windows example
}

//--------------------------------------------------------------
void ofApp::loadFeatMatrix(){
    cnpy::NpyArray t;
    size_t dim, n;
    double* data;
    string filename = "c:/Users/naoto/Documents/bci_art/t0.npy";
    t = cnpy::npy_load(filename);
    ofxNumpy::getSize(t, dim, n);
    data = t.data<double>();
	feat_matrix.clear();
    for (int i = 0; i < n / dim; i++)
    {
        vector<float> feat_vector(dim);
        for (int j = 0; j < dim; j++)
        {
            feat_vector.at(j) = *data;
            data++;
        }
        feat_matrix.push_back(feat_vector);
    }

	ofxNumpy::load("c:/Users/naoto/Documents/bci_art/tsneResult.npy", y);
}

//--------------------------------------------------------------
void ofApp::update(){
    kalman.update(sample);
    
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		// check for mouse moved message
		if(m.getAddress() == "/muse/tsne"){
            sample.x = m.getArgAsFloat(0);
            sample.y = m.getArgAsFloat(1);
            sampleIndex = m.getArgAsInt32(2);
            
            yNew.push_back(sample);
            
            ofVec2f p0 = sample;
            
            ofVec2f pn = kalman.getPrediction();

			unsigned char *bytes = new unsigned char[3];
			bytes[0] = (unsigned char)ofMap(pn.x, -width * 0.5f, width * 0.5f, 0, 255, true);
			bytes[1] = ',';
			bytes[2] = (unsigned char)ofMap(pn.y, height * 0.5f, -height * 0.5f, 80, 255, true);
			serial.writeBytes(bytes, 3);

			pn.y = 1 - pn.y;
            for (int i = 0; i < yNew.size(); i++)
            {
                ofVec2f p1 = yNew.at(i);
                float dist = p0.distanceSquared(p1);
                dist *= width * height; // correction to screen space
                if (dist < distThreshold * distThreshold)
                {
                    stringsNew.addVertex(p0);
                    stringsNew.addVertex(p1);
                    stringsNew.addColor(ofFloatColor::fromHsb((float)sampleIndex / y.size() * 0.75f, 0.5f, 1, lineAlpha));
                    stringsNew.addColor(ofFloatColor::fromHsb((float)sampleIndex / y.size() * 0.75f, 0.5f, 1, lineAlpha));
                    stringsNew.addIndex(stringsNew.getNumVertices() - 2);
                    stringsNew.addIndex(stringsNew.getNumVertices() - 1);
                }
            }
        }
		else if (m.getAddress() == "/muse/tsne/done") {
			// reload
			loadFeatMatrix();
			yNew.clear();
			stringsNew.clear();
			stringsNew.setMode(OF_PRIMITIVE_LINES);
		}
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 255);
    ofSetColor(255, 255);

    int count = 0;
    ofVec2f pPrev;
    
    ofFloatColor c;
    
    ofSetLineWidth(2);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.5f - width * 0.5f, ofGetHeight() * 0.5f - height * 0.5f);
    ofScale(width, height);
    stringsNew.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
    for (int i = 0; i < y.size(); i++)
    {
        ofVec2f p = y.at(i);
        
        float radius = 5;
        ofSetColor(ofFloatColor::fromHsb((float)count / y.size() * 0.75f, 1, 1, 0.95f));
        
        ofVec2f newPos;
        newPos.x = ofMap(p.x, 0, 1, -width * 0.5f, width * 0.5f);
        newPos.y = ofMap(p.y, 0, 1, -height * 0.5f, height * 0.5f);
        
        ofCircle(newPos, radius);
        
        count++;
    }
    ofPopMatrix();
    
    if (yNew.size() >= refreshSec * 10)
    {
        yNew.clear();
        stringsNew.clear();
        stringsNew.setMode(OF_PRIMITIVE_LINES);
    }
    
    if(drawGui)
        gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_TAB)
    {
        drawGui = !drawGui;
        if (drawGui)
            ofShowCursor();
        else
            ofHideCursor();
    }
    if (key == 'f')
    {
        ofToggleFullscreen();
    }
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

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
