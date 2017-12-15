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
	serial.setup("COM18", 115200); // windows example

	lights.resize(1);
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

			pn.x += ofRandom(-0.1, 0.1);
			pn.y += ofRandom(-0.1, 0.1);
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
	//ofSetWindowPosition(2736 * 1.75f, 0);
	//ofSetWindowPosition(-10, -20);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 255);
    ofSetColor(255, 255);

    int count = 0;
    ofVec2f pPrev;
    
    ofFloatColor c;
    
    ofSetLineWidth(2.5f);
    
    ofPushMatrix();
	ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
	ofScale(width, height);
    stringsNew.draw();
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
    for (int i = 0; i < y.size(); i++)
    {
        ofVec2f p = y.at(i);
        
		float radius = 7.5;//5;
        ofSetColor(ofFloatColor::fromHsb((float)count / y.size() * 0.75f, 1, 1, 0.95f));
        
        ofVec2f newPos;
        newPos.x = ofMap(p.x, 0, 1, -width * 0.5f, width * 0.5f);
        newPos.y = ofMap(p.y, 0, 1, -height * 0.5f, height * 0.5f);
        
        ofDrawCircle(newPos, radius);
        
        count++;
    }
    ofPopMatrix();
    
    if (yNew.size() >= refreshSec * 10)
    {
        yNew.clear();
        stringsNew.clear();
        stringsNew.setMode(OF_PRIMITIVE_LINES);
    }
	ofBackground(255);
	ofRectangle bounds = voronoi.getBounds();
	ofSetLineWidth(0);
	ofNoFill();
	ofSetColor(220);
	ofDrawRectangle(bounds);

	//ofEnableLighting();

	lights.back().setSpotlight(lightCutoff);
	lights.back().setPosition(mouseX, mouseY, 1000);
	//light.lookAt(ofPoint(ofGetWidth()/2, ofGetHeight()/2, 0));
	lights.back().lookAt(ofPoint(mouseX, mouseY, 0));
	lights.back().setAmbientColor(ofFloatColor(lightAmbient));
	lights.back().setDiffuseColor(ofFloatColor(lightDiffuse));
	lights.back().setAttenuation(1, lightAttenuation);
	//for(auto& light: lights)
	//	light.enable();
	//cam.begin();
	ofSetupScreenOrtho(-1, -1, -1000, 1000);
	vector <ofxVoronoiCell> cells = voronoi.getCells();
	for (int i = 0; i<cells.size(); i++) {
		ofPoint screenP0 = p0 * ofGetHeight();
		screenP0.x += (ofGetWidth() - ofGetHeight())*0.5f;
		float closeness = ofMap(screenP0.distance(cells[i].pt), 0, 200, 1, 0, true);
		float rate = 0.02f;
		if (closeness < pointIntensity.at(i)) rate = 0.005f; // fades slowly
		pointIntensity.at(i) = pointIntensity.at(i) * (1 - rate) + closeness * rate;
		if (pointIntensity.at(i) < 0.1f) pointIntensity.at(i) = 0.1f;

		ofSetColor(ofColor::fromHsb(255. * i / cells.size(), 255., 255.));
		ofFill();
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		//mesh.addVertex(cells[i].pt);
		mesh.addVertices(cells[i].pts);
		//mesh.addVertex(cells[i].pts.back());
		//mesh.draw();

		mesh.clear();
		for (int j = 0; j < cells[i].pts.size(); j++) {
			mesh.addVertex(cells[i].pt);
			mesh.addVertex(cells[i].pts[j] + ofPoint(0, 0, -100));
			mesh.addVertex(cells[i].pts[(j+1)% cells[i].pts.size()] + ofPoint(0, 0, -100));
		}
		ofSetColor(255);
		ofSetColor(ofColor::fromHsb(255. * i / cells.size(), 255., pointIntensity.at(i) * 255.));
		mesh.draw();

	}
	//cam.end();
	//ofDisableLighting();

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
