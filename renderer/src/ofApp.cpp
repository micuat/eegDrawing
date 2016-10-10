#include "ofApp.h"

#include "ofxNumpy.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofHideCursor();
    
    ofSetBackgroundAuto(false);
    
    receiver.setup(13000);

    videoPlayer.loadMovie(ofToDataPath("D05T01_Janine_sync_Center_Small.mp4"));
    videoPlayer.setLoopState(OF_LOOP_NORMAL);
    videoPlayer.play();
 
    // D01T01
    //std::string urlx = "http://api.piecemeta.com/streams/d1c05738-4b96-4548-837f-90dd9b37af08.json";
    //std::string urly = "http://api.piecemeta.com/streams/f166001a-42db-48f0-8514-a6fd636eed6c.json";
    //std::string urlz = "http://api.piecemeta.com/streams/5c77f56b-8fe3-4344-85e3-52fe16f78272.json";
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
    
    sampleIndex = 0;
    ofxNumpy::load("/Users/naoto/Documents/bci_art/tsneResult.npy", y);
    
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

    minXY.x = maxXY.x = y.at(0).x;
    minXY.y = maxXY.y = y.at(0).y;
    for (int i = 0; i < y.size(); i++)
    {
        ofVec2f p = y.at(i);
        minXY.x = min(minXY.x, p.x);
        maxXY.x = max(maxXY.x, p.x);
        minXY.y = min(minXY.y, p.y);
        maxXY.y = max(maxXY.y, p.y);
    }
    ofLogError() << minXY << " " << maxXY;
    
    strings.setMode(OF_PRIMITIVE_LINES);
    stringsNew.setMode(OF_PRIMITIVE_LINES);
    for (int i = 0; i < y.size(); i++)
    {
        for (int j = i; j < y.size(); j++)
        {
            ofVec2f p0 = y.at(i);
            ofVec2f p1 = y.at(j);
            p0.x = ofMap(p0.x, minXY.x, maxXY.x, -width * 0.5f + 10, width * 0.5f - 10);
            p0.y = ofMap(p0.y, minXY.y, maxXY.y, -height * 0.5f + 10, height * 0.5f - 10);
            p1.x = ofMap(p1.x, minXY.x, maxXY.x, -width * 0.5f + 10, width * 0.5f - 10);
            p1.y = ofMap(p1.y, minXY.y, maxXY.y, -height * 0.5f + 10, height * 0.5f - 10);
            float dist = p0.distance(p1);
            float distThreshold = 100;
            if (dist < distThreshold)
            {
                strings.addVertex(p0);
                strings.addVertex(p1);
                strings.addColor(ofFloatColor::fromHsb((float)i / y.size() * 0.75f, 1, 1, ofMap(dist, distThreshold, 0, 0, 0.5f)));
                strings.addColor(ofFloatColor::fromHsb((float)j / y.size() * 0.75f, 1, 1, ofMap(dist, distThreshold, 0, 0, 0.5f)));
                strings.addIndex(strings.getNumVertices() - 2);
                strings.addIndex(strings.getNumVertices() - 1);
            }
        }
    }
    
    gui.setup();
    gui.add(sliderChannel.setup("Channel", 0, 0, 15));
    gui.add(sliderUpperLimit.setup("Upper Limit", 2, 0, 6));
    gui.add(toggleColor.setup("Color", false));
    gui.add(refreshSec.setup("Refresh Sec", 5, 1, 20));
    gui.add(mouseDebug.setup("Mouse Debug", false));
    gui.add(distThreshold.setup("Distance", 150, 50, 300));
    gui.add(lineAlpha.setup("Line Alpha", 0.5f, 0, 1));
    gui.add(forceDuration.setup("Force Duration", 1000, 0, 5000));
    gui.add(forceIntensity.setup("Force Intensity", 30, 0, 100));
    gui.add(velocity.setup("Velocity", 0, 0, 1));
    gui.add(bFeedback.setup("Force Feedback", false));
    gui.loadFromFile("settings.xml");
    drawGui = false;
    
    fbo.allocate(width * 2, height * 2, GL_RGB);
    
    kalman.init(1e-4, 1e+3);
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
            samplePrev = sample;
            sample.x = m.getArgAsFloat(0);
            sample.y = m.getArgAsFloat(1);
            sampleIndex = m.getArgAsInt32(2);
            
            yNew.push_back(sample);
            
            ofVec2f p0 = sample;
            p0.x = ofMap(p0.x, minXY.x, maxXY.x, -width * 0.5f + 10, width * 0.5f - 10);
            p0.y = ofMap(p0.y, minXY.y, maxXY.y, -height * 0.5f + 10, height * 0.5f - 10);
            
            ofVec2f pn = kalman.getPrediction();
            pn.x = ofMap(pn.x, minXY.x, maxXY.x, 0, 1);
            pn.y = ofMap(pn.y, minXY.y, maxXY.y, 1, 0);
            for (int i = 0; i < yNew.size(); i++)
            {
                ofVec2f p1 = yNew.at(i);
                p1.x = ofMap(p1.x, minXY.x, maxXY.x, -width * 0.5f + 10, width * 0.5f - 10);
                p1.y = ofMap(p1.y, minXY.y, maxXY.y, -height * 0.5f + 10, height * 0.5f - 10);
                float dist = p0.distance(p1);
                if (dist < distThreshold)
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
                float closestDistance = 10000;
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
    ofTranslate(ofGetWidth() * 0.5f, ofGetHeight() * 0.5f);
    stringsNew.draw();
    
    for (int i = 0; i < y.size(); i++)
    {
        ofVec2f p = y.at(i);
        
        float radius = 5;
        if(toggleColor)
            ofSetColor(ofFloatColor::fromHsb(0, 1, 1, ofMap(feat_matrix.at(count).at(sliderChannel), 0, sliderUpperLimit, 0, 0.2f, true)));
        else
            ofSetColor(ofFloatColor::fromHsb((float)count / y.size() * 0.75f, 1, 1, 0.95f));
        
        ofVec2f newPos;
        newPos.x = ofMap(p.x, minXY.x, maxXY.x, -width * 0.5f + 10, width * 0.5f - 10);
        newPos.y = ofMap(p.y, minXY.y, maxXY.y, -height * 0.5f + 10, height * 0.5f - 10);
        
        ofCircle(newPos, radius);
        
        count++;
    }
    ofPopMatrix();
    
    if (yNew.size() >= refreshSec * 10)
    {
        ofSaveScreen(ofGetTimestampString() + ".png");
//        fbo.begin();
//        ofEnableAlphaBlending();
//        ofEnableAntiAliasing();
//        
//        ofTranslate(width * 0.5f * 2, height * 0.5f * 2);
//        ofScale(2, 2);
//        
//        count = 0;
//        for (auto& p : y)
//        {
//            ofSetColor(ofFloatColor::fromHsb((float)count / y.size() * 0.75f, 1, 1, 0.5f));
//            
//            ofVec2f newPos;
//            newPos.x = ofMap(p.x, minXY.x, maxXY.x, -width * 0.5f + 10, width * 0.5f - 10);
//            newPos.y = ofMap(p.y, minXY.y, maxXY.y, -height * 0.5f + 10, height * 0.5f - 10);
//            float radius = 5;
//            ofDrawCircle(newPos, radius);
//            count++;
//        }
//        
//        ofSetColor(255, 255);
//        stringsNew.draw();
//        fbo.end();
//        
//        ofImage image;
//        image.allocate(width * 2, height * 2, OF_IMAGE_COLOR);
//        fbo.readToPixels(image.getPixels());
//        image.resize(width, height);
//        image.update();
//        image.save(ofGetTimestampString() + ".png");
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
