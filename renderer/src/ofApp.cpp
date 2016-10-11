#include "ofApp.h"

#include "ofxNumpy.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofHideCursor();
    ofSetBackgroundAuto(false);
    
    receiver.setup(13000);

    setupVideo();
    
    loadFeatMatrix();
    ofxNumpy::load("/Users/naoto/Documents/bci_art/tsneResult.npy", y);

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
    
    ofxNumpy::load("/Users/naoto/Documents/JR Sound Library/tsne.npy", soundTsne);
}

//--------------------------------------------------------------
void ofApp::setupVideo(){
    videoPlayer.loadMovie(ofToDataPath("D05T01_Janine_sync_Center_Small.mp4"));
    videoPlayer.setLoopState(OF_LOOP_NORMAL);
    videoPlayer.play();
    
    // D05T01
    std::string urlx = ofToDataPath("3eae1051-e185-4458-b04e-7bd510f6f076.json");
    std::string urly = ofToDataPath("9cc12a02-f7c9-4fd3-a3aa-1cabd1a83ef9.json");
    
    if (!responsex.open(urlx))
    {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON";
    }
    if (!responsey.open(urly))
    {
        ofLogNotice("ofApp::setup") << "Failed to parse JSON";
    }
    for (Json::ArrayIndex i = 0; i < responsex["frames"].size(); i++)
    {
        ofPoint p;
        p.x = ofMap(responsex["frames"][i].asFloat(), 0, 15, 0, 1);
        p.y = ofMap(responsey["frames"][i].asFloat(), 0, 15, 0, 1);
        points.push_back(p);
    }
}

//--------------------------------------------------------------
void ofApp::loadFeatMatrix(){
    cnpy::NpyArray t;
    size_t dim, n;
    double* data;
    string filename = "/Users/naoto/Documents/bci_art/t0.npy";
    t = cnpy::npy_load(filename);
    ofxNumpy::getSize(t, dim, n);
    data = t.data<double>();
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
            
            if(showVideo) {
                // find video frame
                float closestDistance = 1000000;
                int closestFrame = 0;
                for (int i = ofRandom(0, 10); i < points.size(); i+=10) {
                    float distanceSquared = points.at(i).distanceSquared(pn);
                    if(distanceSquared < closestDistance) {
                        closestDistance = distanceSquared;
                        closestFrame = i;
                    }
                }
                curFrame = closestFrame / 2;
            }
            
            // sound
            {
                ofVec2f sampleShrunk;
                sampleShrunk.x = ofMap(sample.x, 0, 1, (1 - stretchRate) * 0.5f, (1 + stretchRate) * 0.5f);
                sampleShrunk.y = ofMap(sample.y, 0, 1, (1 - stretchRate) * 0.5f, (1 + stretchRate) * 0.5f);
                float closestDistance = 100000000;
                int closestIndex = 0;
                for(int i = 0; i < soundTsne.size(); i++) {
                    ofVec2f yi = soundTsne.at(i);
                    float distance = yi.distanceSquared(sampleShrunk);
                    if(distance < closestDistance) {
                        closestIndex = i;
                        closestDistance = distance;
                    }
                }
                if(curSoundIndex != closestIndex) {
                    sound.loadSound("/Users/naoto/Documents/JR Sound Library/" + files[closestIndex]);
                    sound.play();
                    ofLogError() << closestIndex << " " << files[closestIndex];
                }
                curSoundIndex = closestIndex;
            }
        }
    }
    videoPlayer.setFrame(curFrame);
    
    videoPlayer.update();
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(showPoints) {
        ofBackground(0, 255);
        ofSetColor(255, 255);
    }
    else {
        ofSetColor(255, 25);
    }

    if(switchFlag >= 0) {
        ofPushStyle();
        ofSetColor(0, 255);
        ofRect(0, 0, ofGetWidth(), ofGetHeight());
        ofPopStyle();
        switchFlag++;
        if(switchFlag >= 2)
            switchFlag = -1;
    }
    
    if(showVideo) {
        videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    if(!showPoints) return;

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
        ofSaveScreen(ofGetTimestampString() + ".png");

        yNew.clear();
        stringsNew.clear();
        stringsNew.setMode(OF_PRIMITIVE_LINES);
    }
    
    if(showSounds)
    {
        ofPushMatrix();
        ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
        
        for (int i = 0; i < soundTsne.size(); i++)
        {
            ofVec3f p = soundTsne.at(i);
            
            float radius = 5;
            ofSetColor(ofFloatColor::fromHsb(p.z, 1, 1, 0.95f));
            
            ofCircle(ofMap(p.x, 0, 1, -width*0.5f, width*0.5f),
                     ofMap(p.y, 0, 1, -height*0.5f, height*0.5f), radius);
            
            if(i == curSoundIndex) {
                ofPushStyle();
                ofNoFill();
                ofSetColor(255, 255);
                ofCircle(ofMap(p.x, 0, 1, -width*0.5f, width*0.5f),
                         ofMap(p.y, 0, 1, -height*0.5f, height*0.5f), radius + 2);
                ofPopStyle();
            }
        }
        ofPopMatrix();
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
    if (key == '1')
    {
        showVideo = !showVideo;
        switchFlag = 0;
    }
    if (key == '2')
    {
        showPoints = !showPoints;
        switchFlag = 0;
    }
    if (key == '3')
    {
        showSounds = !showSounds;
        switchFlag = 0;
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
