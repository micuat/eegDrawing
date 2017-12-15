#include "ofApp.h"

#include "ofxNumpy.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofHideCursor();
    ofSetBackgroundAuto(false);
    
    receiver.setup(13000);

    loadFeatMatrix();
    
    gui.setup();
    gui.add(doVideo.setup("Show video", false));
    gui.add(lineAlpha.setup("Line Alpha", 0.5f, 0, 1));
    gui.add(stretchRate.setup("Stretch Rate (Mapping)", 1, 0, 2));
	gui.add(lightAmbient.setup("Ambient", 0.01, 0, 1));
	gui.add(lightDiffuse.setup("Diffuse", 1, 0, 1));
	gui.add(lightAttenuation.setup("Attenuation", 0.001, 0, 0.01));
	gui.add(lightCutoff.setup("Cutoff", 30, 0, 45));
	gui.add(refreshSec.setup("Refresh Sec", 5, 1, 20));
    gui.add(distThreshold.setup("Distance", 150, 50, 300));
    gui.loadFromFile("settings.xml");
    drawGui = false;
    
    fbo.allocate(width * 2, height * 2, GL_RGB);
    
    kalman.init(1e-3, 1e+4);
    
	//serial.setup("COM13", 9600); // windows example
    //serial.setup("COM18", 115200); // windows example
    serial.setup("/dev/cu.usbmodemfd141", 115200); // windows example

	lights.resize(1);
    
    grabber.setup(1280, 720);
    grabbedImages.resize(1);
}

//--------------------------------------------------------------
void ofApp::loadFeatMatrix(){
    cnpy::NpyArray t;
    size_t dim, n;
    double* data;
    string filename = "/Users/naoto/Documents/bci_art/t0.npy";
    //string filename = "c:/Users/naoto/Documents/bci_art/t0.npy";
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

    ofxNumpy::load("/Users/naoto/Documents/bci_art/tsneResult.npy", y);
    //ofxNumpy::load("c:/Users/naoto/Documents/bci_art/tsneResult.npy", y);
	
	ofRectangle bounds = ofRectangle(10, 10, ofGetWidth() - 20, ofGetHeight() - 20);

	int pointCount = y.size();
	int seed = 33;
	vector<ofPoint> yPoints;
	for (auto p : y) {
		yPoints.push_back(ofPoint(p.x * ofGetHeight() + (ofGetWidth()-ofGetHeight())*0.5f, p.y * ofGetHeight(), 0));
	}

	voronoi.setBounds(bounds);
	voronoi.setPoints(yPoints);

	voronoi.generate();
	pointIntensity.resize(voronoi.getCells().size(), 0);
}

//--------------------------------------------------------------
void ofApp::update(){
    grabber.update();
    
	kalman.update(sample);
	ofVec2f pn = kalman.getPrediction();
	unsigned char *bytes = new unsigned char[3];
	bytes[0] = (unsigned char)ofMap(pn.x, 1, 0, 0, 127, true);
	bytes[1] = ',';
	bytes[2] = (unsigned char)ofMap(pn.y, 1, 0, 0, 127, true);
	serial.writeBytes(bytes, 3);
	serial.flush();

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
            
            p0 = sample;
            ofVec2f pn = kalman.getPrediction();

			pn.y = 1 - pn.y;
        }
		else if (m.getAddress() == "/muse/tsne/done") {
			// reload
			loadFeatMatrix();
			yNew.clear();
		}
        else if (m.getAddress() == "/muse/tsne/record") {
            int imageIndex = m.getArgAsInt32(0);
            if(imageIndex >= grabbedImages.size()) grabbedImages.resize(imageIndex + 1);
            grabbedImages.at(imageIndex).setFromPixels(grabber.getPixels());
        }
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
	//ofSetWindowPosition(2736 * 1.75f, 0);
	//ofSetWindowPosition(-10, -20);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 255);
    ofSetColor(255, 255);
    
    if(doVideo) {
        int ind = (ofGetFrameNum() / 8) % grabbedImages.size();
        grabbedImages.at(ind).draw(0, 0, ofGetWidth(), ofGetHeight());
        
        for(int i = ind; i < ind + 30 && i < grabbedImages.size(); i++) {
            auto p = y.at(i);
            auto screenP = ofPoint(p.x * ofGetHeight() + (ofGetWidth()-ofGetHeight())*0.5f, p.y * ofGetHeight(), 0);
            ofSetColor(ofColor::fromHsb(255. * i / grabbedImages.size(), 255., 255., 255));
            ofDrawCircle(screenP, 20);
        }
    }

    vector <ofxVoronoiCell> cells = voronoi.getCells();
    for (int i = 0; i<cells.size(); i++) {
        ofPoint screenP0 = p0 * ofGetHeight();
        screenP0.x += (ofGetWidth() - ofGetHeight())*0.5f;
        float closeness = ofMap(screenP0.distance(cells[i].pt), 0, 200, 1, 0, true);
        float rate = 0.02f;
        if (closeness < pointIntensity.at(i)) rate = 0.005f; // fades slowly
        pointIntensity.at(i) = pointIntensity.at(i) * (1 - rate) + closeness * rate;
        if (pointIntensity.at(i) < 0.1f) pointIntensity.at(i) = 0.1f;

        ofFill();
        ofMesh mesh;
        for (int j = 0; j < cells[i].pts.size(); j++) {
            mesh.addVertex(cells[i].pt);
            mesh.addVertex(cells[i].pts[j]);
            mesh.addVertex(cells[i].pts[(j+1)% cells[i].pts.size()]);
        }
        ofSetColor(255);
        int intensity = pointIntensity.at(i) * 255;
        int alpha = 255;
        if(doVideo) {
            intensity = 255;
            alpha = lineAlpha * 255;
        }
        ofSetColor(ofColor::fromHsb(255. * i / cells.size(), 255., intensity, alpha));
        mesh.draw();
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
