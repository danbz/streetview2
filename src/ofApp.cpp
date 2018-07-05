#include "ofApp.h"
#include <fstream>

#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    
    viewLat = 51.462088;//stokes croft
    viewLong = -2.5901384;
    
    // viewLat = 22.2040435; // lo shing beach, lamma island, hong kong
    // viewLong = 114.1228474;
    // viewLat = 22.2738285; // tai lung fung
    // viewLong = 114.1742357;
    // viewLat = 22.3290091; // sham sui po
    // viewLong = 114.1600577;
    // viewLat = 51.5130679;// RCA white cit
    // viewLat = 50.7530769;//liege netherlands border post
    // viewLong = 5.6960133;//liege netherlands border post
    // viewLong =-0.228804; y
    // viewLat = 50.3621444; //EDEN PROJECT
    // viewLong =-4.7448747;
    // 55.5893491,12.6428642 dragor
    // 22.2040435,114.1228474 lo shing beach, lamma island, hong kong
    // streetview.setLatLon(40.75732,-73.985951);  // Time Sq
    // streetview.setLatLon(40.768153,-73.981473); // Columbus Circus
    // streetview.setLatLon(40.751511,-73.993953);  // Penn Station
    // streetview.setLatLon(22.276499,114.1735439); // wanchai MTR Hong Kong;
    // streetview.setLatLon( 51.462088,-2.5901384 ); //stokes croft
    // streetview.setLatLon( 50.7530769,5.6964121 ); //liege netherlands border post
    // streetview.setLatLon( 50.7531791,5.6960133 ); //liege netherlands border post  2
    
    // download the first streetview from online
        ofxStreetView newStreet;
        streetview.push_back(newStreet);
        streetview[0].setLatLon(viewLat, viewLong);
        streetview[0].setZoom(3);
        ofPixels texturePixels;
        textureImage = streetview[0].getTexture(); //pull in pano texture
        textureImage.readToPixels(texturePixels);
        panoImage.setFromPixels(texturePixels);
    
    b_drawPointCloud = b_showGui = true;
    b_enableLight = b_updateMesh = b_meshExists = false;;
    linkLevel = 0;
    
    //mesh = streetview[0].getDethMesh();
    
    string num;
    gui.setup();
    gui.add(pointSize.setup("pointSize", 2, 1, 10));
    gui.add(scaleMeters.setup("scaleMeters", .33, 0.01, 2.0));
    
    for(int i = 0; i < 20; i++){
        num = std::to_string(i);
        gui.add(showMesh[i].setup("showMesh "+num,1));
        //gui.add(xOffset[i].setup("latOffset"+num, 5, -20, 20));
        //gui.add(yOffset[i].setup("longOffset"+num, 5.0, -20, 20));
        gui.add(rotOffset[i].setup("Rotation offset"+num, 0, -180, 180));
    }
    
    rotOffset[0] = 0;
    xOffset[0] = 0;
    yOffset[0] = 0;
    
    // mercator mapsetup // leftLon, bottomLat, rightLon, topLat
    merMapWidth = 2000;
    merMapHeight =1000;
    merMap.setup(merMapWidth,merMapHeight, -2.593, 51.46, -2.586, 51.465);
    //ofVec3f camStart = (merMapWidth/2, merMapHeight/2, -100);
    //cam.setUpAxis(camStart); //set up useful location for camera start
}

//--------------------------------------------------------------
void ofApp::update(){
    // iterate through vector of streetview objects
    for(int i = 0; i < streetview.size(); i++){
        streetview[i].update();
        streetview[i].setUseTexture(true);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(50);
    ofSetColor(200, 200, 200);
    stringstream statusStream, statusStream2;
    if (b_enableLight) worldLight.enable();
    cam.begin();
    ofDrawAxis(100);
    ofDrawRotationAxes(1);
    glPointSize(pointSize);
    
    if (b_drawPointCloud) {
        for(int i = 0; i < streetview.size(); i++){
            if ( streetview[i].bDataLoaded & streetview[i].bPanoLoaded) {
                //ofRotateZ(98); //correct alignment of meshes
                ofPushMatrix();
                if (showMesh[i]) {
                    ofPoint pOrigin = merMap.getScreenLocationFromLatLon(streetview[0].getLat(), streetview[0].getLon()); //
                    ofPoint p = merMap.getScreenLocationFromLatLon(streetview[i].getLat(), streetview[i].getLon()); // translate to location using mermap
                    ofSetColor(255,0,0);
                    ofTranslate((p.x - pOrigin.x) * scaleMeters, (p.y - pOrigin.y) * scaleMeters);
                    ofRotateZ(rotOffset[i]);
                    //ofRotateZ(streetview[i].getTiltYaw());
                    ofRotateZ(180-streetview[i].getDirection());
                    ofDrawRectangle(0,0, 50, 20);
                    ofSetColor(255);
                    streetview[i].draw();
                }
                ofPopMatrix();
            }
        }
    } else {
        
        //        if (b_meshExists == false) {
        //            ofPixels texturePixels;
        //            textureImage = streetview[0].getTexture(); //pull in pano texture
        //            textureImage.readToPixels(texturePixels);
        //            panoImage.setFromPixels(texturePixels);
        //            mesh = streetview[0].getDethMesh(); // load street view 0 into mesh object
        //            int width = (streetview[0].getDepthMapWidth() -1) *6  ;
        //            int height = streetview[0].getDepthMapHeight() -1 ;
        //            int vertNum = mesh.getNumVertices();
        //            // meshMaker.makeTriangles(mesh, width, height, panoImage);
        //            // meshMaker.setNormals(mesh);
        //            b_meshExists = true;
        //        }
        //mesh.setMode(OF_PRIMITIVE_POINTS);
        //glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
        ofPushMatrix();
        //ofRotateZ(98); //correct alignment of meshes
        // ofScale(1, -1, -1);  // the projected points are 'upside down' and 'backwards'
        // ofTranslate(0, 0, 0); // center the points a bit
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_TRIANGLES);
        //        mesh.drawVertices();
        ofDrawCircle(10, 10,  5);
        for (int i=0; i<localView.size(); i++){
            //ofDrawCircle(10*i+1, 10,  5);
            cout << "drawing local mesh "<< i << " of " << localView.size() << endl;
            
            ofPoint pOrigin = merMap.getScreenLocationFromLatLon(localView[0].lat, localView[0].lon); //
            ofPoint p = merMap.getScreenLocationFromLatLon(localView[i].lat, localView[i].lon); // translate to location using mermap
            ofSetColor(255,0,0);
            ofTranslate((p.x - pOrigin.x) * scaleMeters, (p.y - pOrigin.y) * scaleMeters);
            ofRotateZ(rotOffset[i]);
            ofRotateZ(180-localView[i].heading);
            ofDrawRectangle(0,0, 20, 10);
            ofSetColor(255);
            localView[i].mesh.setMode(OF_PRIMITIVE_POINTS);
            localView[i].image.bind();
            localView[i].mesh.draw();
            //localView[i].image.draw(0,0);
            localView[i].image.unbind();
        }
        ofPopMatrix();
    }
    cam.end();
    worldLight.disable();
    
    if (b_showGui) {
        // statusStream << streetview[0].getPanoId() << " lat " << viewLat << " lon " << viewLong << " dir " << streetview[0].getDirection() << streetview[0].getAddress() << ", " << streetview[0].getRegion() << "meshes " <<streetview.size() << " linkLvl " << linkLevel;
        // ofDrawBitmapString(statusStream.str(), 20,  20);
        // ofDrawBitmapString(statusStream2.str(), 20,  40);
        gui.draw();
    }
}

//--------

void ofApp::loadLinks(){ //load next level of related links from the central start streetview
    int numOfLinks, i, n, a;
    string newPanoName;
    bool b_panoExists=false;
    
    i = 0;
    int loopStart = linkLevel;
    int loopEnd = streetview.size();
    for (a = loopStart; a < loopEnd; a++) { // loop though non link-read panoramas
        numOfLinks = streetview[linkLevel].links.size(); //get number of links related to a loaded panormama
        cout << numOfLinks << " number of links related to streetview Pano number " << linkLevel << endl;
        if (numOfLinks>0) {
            for (n=0; n<numOfLinks; n++) { //iterate through links adding them
                newPanoName = streetview[linkLevel].links[n].pano_id;
                b_panoExists = false;
                cout << newPanoName << " = new pano name, number " << n+1 << " of " << numOfLinks << endl;
                while (i < streetview.size()) { // check if pano already loaded
                    if (newPanoName == streetview[i].getPanoId()) {
                        b_panoExists = true;
                        break;
                    }
                    i++;
                }
                if (!b_panoExists){
                    cout << " adding pano " << newPanoName << endl;
                    ofxStreetView newStreet;
                    streetview.push_back(newStreet);
                    streetview[i].setPanoId(newPanoName);
                    streetview[i].setZoom(3);
                    b_updateMesh=true;
                } else {
                    cout << "pano " <<  newPanoName << " is already loaded" << endl;
                }
            }
        }
    }
    linkLevel ++;
}

//--------------------------------------------------------------

void ofApp::loadNewStreet(int direction){
    string newPanoName;
    int i =0;
    ofxStreetView newStreet;
    newPanoName = streetview[streetview.size()-1].getCloseLinkTo(direction);
    while (i < streetview.size()) {
        if (newPanoName == streetview[i].getPanoId()) {
            cout << "pano is already loaded" << endl;
            ofSystemAlertDialog("pano is already loaded");
            return;
        }
        i++;
    }
    streetview.push_back(newStreet);
    streetview[i].setPanoId(newPanoName);
    streetview[i].setZoom(3);
    b_updateMesh=true;
}

//--------------------------------------------------------------

void ofApp::exportOBJ(ofMesh &mesh){
    ofPixels pixels;
    ofTexture texture;
    ofImage image;
    ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + ".obj", "Export your mesh as an .obj file");
    if (saveFileResult.bSuccess){
        ofColor c;
       
        obj.open(ofToDataPath(saveFileResult.filePath),ofFile::WriteOnly);
        obj << "#vertices\n";
        
        for(int i = 0 ; i < mesh.getNumVertices(); i++) {
            ofVec3f v = mesh.getVertex(i);
            c = mesh.getColor(i);
            obj << "v " + ofToString(v.x) + " " + ofToString(v.y) + " " + ofToString(v.z) + " " + ofToString(ofMap(c.r, 0, 255, 0, 1)) + " " + ofToString(ofMap(c.g, 0, 255, 0, 1)) + " " + ofToString(ofMap(c.b, 0, 255, 0, 1)) +"\n";
        }
        obj << "#faces\n";
        for(int i = 0 ; i < mesh.getNumIndices(); i += 3)
            obj << "f " + ofToString(mesh.getIndex(i)) + " " + ofToString(mesh.getIndex(i+1)) + " " + ofToString(mesh.getIndex(i+2)) + "\n";
        obj << "\n";
        obj.close();
        cout << "wrote obj file"  << endl;
        //to export texture file
        texture = streetview[0].getTexture();
        texture.readToPixels(pixels);
        image.setFromPixels(pixels);
        image.save(saveFileResult.filePath + "texture.png");
        cout << "wrote tex file to png" << endl;
    }
}

//---------------------------------------------------------------

void ofApp::exportPLY( ofMesh &mesh){ // export sequential .ply and .png files
    ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString(), "Export your meshes as a .ply and .png file sequence");
    if (saveFileResult.bSuccess){
        ofFile file (saveFileResult.getPath());
        ofSystemAlertDialog("saving " + ofToString(streetview.size()) +" views to sequence of .ply .png and .xml files, please press ok and wait a while...");
        
        for(int i = 0; i < streetview.size(); i++){
            string filenameP = saveFileResult.filePath  + "_" + streetview[i].getPanoId();
            cout << "filename is" << filenameP << endl;
            if ( streetview[i].bDataLoaded & streetview[i].bPanoLoaded) {
                if (!file.doesFileExist( filenameP + ".ply")) {
                    // add in here xml export to contain all streetview data available...
                    mesh = streetview[i].getDethMesh();
                    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
                    mesh.save(filenameP + ".ply"); //= true saves as binary file / false saves as ascii
                    cout << "wrote ply file to .ply number " << i << endl;
                    ofPixels pixels;
                    ofTexture texture = streetview[i].getTexture();
                    texture.readToPixels(pixels);
                    ofImage image;
                    image.setFromPixels(pixels);
                    image.save(filenameP + ".png");
                    cout << "wrote tex file to .png number " << i << endl;
                    // save XML details of ply scene
                    saveXMLData(filenameP, i);
                } else {
                    cout << "file exists: " << (filenameP + ".png") << endl;
                }
            }
        }
    }
}

//--------------------------------------------------------------

void ofApp::saveXMLData(string filePath, int i){ //put some some settings into an XML file
    
    float lat = streetview[i].getLat();
    float lon = streetview[i].getLon();
    float rot = streetview[i].getDirection();
    
    XMLsettings.addTag("Streetview panorama data");
    XMLsettings.setValue("panoId",  streetview[i].getPanoId());
    XMLsettings.setValue("latitude",  streetview[i].getLat());
    XMLsettings.setValue("longitude", streetview[i].getLon());
    XMLsettings.setValue("heading", streetview[i].getDirection());
    XMLsettings.setValue("yaw", streetview[i].getTiltYaw());
    XMLsettings.setValue("tilt", streetview[i].getTiltPitch());
    XMLsettings.setValue("elevation", streetview[i].getElevation());
    XMLsettings.setValue("groundHeight", streetview[i].getGroundHeight());
     XMLsettings.setValue("region", streetview[i].getRegion());
     XMLsettings.setValue("address", streetview[i].getAddress());
     XMLsettings.setValue("country", streetview[i].getCountry());
    XMLsettings.saveFile(filePath + ".xml"); //puts <panoramaID>.xml file in the current recordedframe folder
    string myXml;
    XMLsettings.copyXmlToString(myXml);
    cout << myXml <<endl ;
}

//--------------------------------------------------------------

bool ofApp::loadXMLData(string filePath, int i) { // load exifXML file from the selected folder and get the values out
    // add in the local view things here !
    if (XMLsettings.loadFile(filePath + "/exifSettings.xml")){
        XMLmodel = XMLsettings.getValue("exif:model", "");
        string thisScene ="Streetview panorama data";
        if (XMLmodel.find(thisScene) != string::npos){
            //cout << filePath << "/exifSettings.xml" << endl;
            localView[i].lat = XMLsettings.getValue("latitude", 0);
            localView[i].lon = XMLsettings.getValue("longitude", 0);
            localView[i].heading = XMLsettings.getValue("heading", 0);
            localView[i].panoId = XMLsettings.getValue("panoId", 0);
            localView[i].yaw = XMLsettings.getValue("yaw", 0);
            localView[i].tilt = XMLsettings.getValue("tilt", 0);
            localView[i].elevation = XMLsettings.getValue("elevation", 0);
            localView[i].groundheight = XMLsettings.getValue("groundheight", 0);
            //localView[i].rot = XMLsettings.getValue("linkdata", 0);
            string myXml;
            XMLsettings.copyXmlToString(myXml);
            cout << "loaded XML data: " << myXml <<endl ;
            return true;
        } else {
            ofSystemAlertDialog("Correct streetview XML metadata not found. Is the streetView.xml file corrupt?");
        }
    } else {
        ofSystemAlertDialog("No  XML metadata file found. Is this a streetview recording folder?");
        return false;
    }
}
//--------------------------------------------------------------

void ofApp::loadOBJ(ofMesh &mesh){
    
    //    //obj.open(ofToDataPath(name),ofFile::WriteOnly);
    //    obj << "#vertices\n";
    //
    //    for(int i = 0 ; i < mesh.getNumVertices(); i++) {
    //        ofVec3f v = mesh.getVertex(i);
    //        obj << "v " + ofToString(v.x) + " " + ofToString(v.y) + " " + ofToString(v.z) + "\n";
    //    }
    //    obj << "#faces\n";
    //    for(int i = 0 ; i < mesh.getNumIndices(); i += 3)
    //        obj << "f " + ofToString(mesh.getIndex(i)) + " " + ofToString(mesh.getIndex(i+1)) + " " + ofToString(mesh.getIndex(i+2)) + "\n";
    //    obj << "\n";
    //    obj.close();
    cout << "read obj file"  << endl;
}

//--------------------------------------------------------------

void ofApp::loadPlyData(){
    ofFileDialogResult result = ofSystemLoadDialog("Choose a folder of recorded Volca PNG data", true, ofToDataPath(""));
    if (result.getPath() != "") {
        string  filePath =result.getPath();
        //    ofFile file (openFileResult.getPath());
        //    if (file.exists()){
        //        string fileExtension = ofToUpper(file.getExtension());
        //        //cout << "its a file ok" << endl;
        //        ofDirectory directory  (openFileResult.getPath());
        //        ofFile file(ofToDataPath(_file + "/"));
        //        string path = file.getAbsolutePath();
        //        file.close();
        // if (fileExtension == "ply" || fileExtension == "PLY") {
        // count files in directory
        string path = ofToDataPath(filePath + "/");
        ofDirectory dir(path);
        dir.allowExt("ply");
        dir.allowExt("png");
        dir.allowExt("xml");
        dir.listDir();
        dir.sort();
        int numViewsToLoad = dir.size(); /// end count files in directory
        
        cout << "load ply file from directory of " << numViewsToLoad << endl;
        localView.clear();
        for (int i=0; i<numViewsToLoad/3;i++){
            streetviewsFromDisc newViewfromDisk;
            localView.push_back(newViewfromDisk);
            string plyFileToload = path + dir.getName(i*3);
            string textureFileToload = path + dir.getName(i*3+1);
            string xmlFileToload = path + dir.getName(i*3+2);

            cout << plyFileToload << " " << textureFileToload << " " << i << endl;
            localView[i].mesh.load(plyFileToload);
            localView[i].image.load(textureFileToload);
            loadXMLData(xmlFileToload, i);
            // parse lat lon and rotation from '_' seprated elements of file names
            string fileNameData =dir.getName(i*2);
    
            cout << "loaded png & ply " << endl;
        }
    }
}

void ofApp::loadViewsfromFile() {
    // load saved street views from disc
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
    switch (key) {
        case 'p':
        case 'P':
            b_drawPointCloud =!b_drawPointCloud;
            break;
            
        case 'f':
        case 'F':
            ofToggleFullscreen();
            break;
            
        case 'l':
        case 'L':
            b_enableLight = !b_enableLight;
            break;
            
        case 's':
        case 'S':
            exportOBJ(mesh);
            break;
            
        case 'c':
        case 'C':
            break;
            
        case 'e':
        case 'E':
            exportPLY(mesh);
            break;
            
        case 'g':
        case 'G':
            b_showGui=!b_showGui;
            break;
            
        case 'o':
        case 'O':
            loadPlyData();
            break;
            
        case OF_KEY_UP:
            loadNewStreet(0);
            break;
            
        case OF_KEY_DOWN:
            loadNewStreet(180);
            break;
            
        case OF_KEY_LEFT:
            loadNewStreet(90);
            break;
            
        case OF_KEY_RIGHT:
            loadNewStreet(270);
            break;
            
        case '+':
        case '=':
            loadLinks();
            break;
    }
}

//--------------------------------------------------------------

void ofApp::mouseMoved(int x, int y){
    
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
