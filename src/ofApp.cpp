#include "ofApp.h"
#include <fstream>

#include <iostream>

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofEnableDepthTest();
    
    //    viewLat = 50.7530769;//liege netherlands border post
    //    viewLong = 5.6960133;//liege netherlands border post
    
    pie = 3.14159265358979323846;
    viewLat = 51.462088;//stokes croft
    viewLong = -2.5901384;
    
    // viewLat = 22.2040435; // lo shing beach, lamma island, hong kong
    // viewLong = 114.1228474;
    //
    // viewLat = 22.2738285; // tai lung fung
    // viewLong = 114.1742357;
    //
    // viewLat = 22.3290091; // sham sui po
    // viewLong = 114.1600577;
    
    // viewLat = 51.5130679;
    // viewLong =-0.228804; // RCA white city
    
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
    
    ofxStreetView newStreet;
    streetview.push_back(newStreet);
    streetview[0].setLatLon(viewLat, viewLong);
    streetview[0].setZoom(3);
    
    b_drawPointCloud = b_showGui = true;
    b_enableLight = b_updateMesh = b_meshExists = false;;
    linkLevel = selectedView = 0;
    
    fileName = "streetmesh" + ofGetTimestampString() + ".obj";
    cout << fileName << endl;
    
    // obj.open(ofToDataPath(fileName),ofFile::WriteOnly);
    // obj.open(ofToDataPath(fileName),ofFile::ReadWrite);
    mesh = streetview[0].getDethMesh();
    
    gui.setup();
    string num;
    for(int i = 0; i < 20; i++){
        num = std::to_string(i);
        gui.add(showMesh[i].setup("showMesh "+num,1));
        //gui.add(xOffset[i].setup("latOffset"+num, 5, -20, 20));
        //gui.add(yOffset[i].setup("longOffset"+num, 5.0, -20, 20));
        gui.add(rotOffset[i].setup("Rotation offset"+num, 0, -180, 180));
    }

    gui.add(pointSize.setup("pointSize", 2, 1, 10));
    gui.add(scaleMeters.setup("scaleMeters", .4, 0.01, 2.0));
    
    rotOffset[0] = 0;
    xOffset[0] = 0;
    yOffset[0] = 0;
    
    ofPixels texturePixels;
    textureImage = streetview[0].getTexture(); //pull in pano texture
    textureImage.readToPixels(texturePixels);
    panoImage.setFromPixels(texturePixels);
    
    //mercator mapsetup // leftLon, bottomLat, rightLon, topLat
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
    //    if (b_updateMesh) {
    //        mesh.append(streetview.getDethMesh());
    //        b_updateMesh= false;
    //    }
    }

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    stringstream statusStream, statusStream2;
    if (b_enableLight) worldLight.enable();
    cam.begin();
    ofDrawAxis(100);
    ofDrawRotationAxes(1);
    
    // double doubleScale = scaleMeters;
    if (b_drawPointCloud) {
        glPointSize(pointSize);
        
        //int homeXOffset = (streetview[0].getLon() * 20037508.34)/180;
        //int homeYOffset = log(tan((90 + streetview[0].getLat()) * (pie/360) ))/ (pie/180) * (20037508.34/180);
        //  home = ofxGeo::Coordinate(streetview[0].getLat(), streetview[0].getLon());
        for(int i = 0; i < streetview.size(); i++){
            if ( streetview[i].bDataLoaded & streetview[i].bPanoLoaded) {
                
                //                    double latOffset = streetview[0].getLat()- streetview[i].getLat();
                //                    double longOffset = streetview[0].getLon()- streetview[i].getLon();
                
                // cout << latOffset << " lat:long offset " << longOffset << endl;
                // cout << "mesh " << i << " lat long pixel displacement " << latOffset << "," << longOffset << endl;
                // cavallo version
                // Data: Latitute and longitude of a location
                // Result: [x,z] offset from the center of the scene
                // int xoffset = (streetview[i].getLon() * 20037508.34)/180 - homeXOffset;
                // int zoffset = log(tan((90 + streetview[i].getLat()) * (pie/360) ))/ (pie/180) * (20037508.34/180) - homeYOffset;
                // end cavallo
                //ofTranslate( - longOffset * scaleMeters /2 , - latOffset * scaleMeters);
                //   newLocation = ofxGeo::Coordinate(streetview[i].getLat(), streetview[i].getLon());
                //    distanceHaversine = ofxGeo::Utils::distanceHaversine(home, newLocation);
                //    bearingHaversine = ofxGeo::Utils::bearingHaversine(home, newLocation);
                //if  (i > 0 ) ofRotateZ( streetview[i].getDirection()); // rotate to align pano shot direction
                //ofTranslate(streetview[i].getLon()*longOffset[i], streetview[i].getLat()*latOffset[i], 0);
                //ofRotateZ(98); //correct alignment of meshes
                
                ofPushMatrix();
                if (showMesh[i]) {
                    ofPoint pOrigin = merMap.getScreenLocationFromLatLon(streetview[0].getLat(), streetview[0].getLon()); //
                    ofPoint p = merMap.getScreenLocationFromLatLon(streetview[i].getLat(), streetview[i].getLon()); // translate to location using mermap
                    cout << "streetview " << i << " location " << p-pOrigin << endl;
                    ofSetColor(255,0,0);
                    ofTranslate((p.x - pOrigin.x) * scaleMeters, (p.y - pOrigin.y) * scaleMeters);
                    ofRotateZ(rotOffset[i]);
                    //ofRotateZ(-streetview[i].getDirection());
                    ofDrawRectangle(0,0, 8, 2);
                    ofSetColor(255);
                    streetview[i].draw();
                }
                ofPopMatrix();
                //     statusStream2 << "mesh " << i << " dist " << distanceHaversine << "m, bear " << bearingHaversine << ", rot " << home << " " << newLocation <<endl;
            }
            ofSetColor( 255, 255, 255);  //set render colour for unpainted points, faces and lines
            
        }
    } else {
        
        if (b_meshExists == false) {
            ofPixels texturePixels;
            textureImage = streetview[0].getTexture(); //pull in pano texture
            textureImage.readToPixels(texturePixels);
            panoImage.setFromPixels(texturePixels);
            mesh = streetview[0].getDethMesh(); // load street view 0 into mesh object
            int width = (streetview[0].getDepthMapWidth() -1) *6  ;
            int height = streetview[0].getDepthMapHeight() -1 ;
            int vertNum = mesh.getNumVertices();
            //meshMaker.makeTriangles(mesh, width, height, panoImage);
            // meshMaker.setNormals(mesh);
            b_meshExists = true;
        }
        mesh.setMode(OF_PRIMITIVE_POINTS);
        glPointSize(1);
        //glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
        ofPushMatrix();
        //ofRotateZ(98); //correct alignment of meshes
        ofRotateZ(streetview[0].getDirection());
        
        ofScale(1, -1, -1);  // the projected points are 'upside down' and 'backwards'
        // ofTranslate(0, 0, 0); // center the points a bit
        glEnable(GL_DEPTH_TEST);
        //gluPerspective(57.0, 1.5, 0.1, 20000.0); // fov,
        glShadeModel(GL_TRIANGLES);
        mesh.drawVertices();
        glDisable(GL_DEPTH_TEST);
        ofPopMatrix();
    }
    cam.end();
    worldLight.disable();
    
    statusStream << streetview[0].getPanoId() << " lat " << viewLat << " lon " << viewLong << " dir " << streetview[0].getDirection() << streetview[0].getAddress() << ", " << streetview[0].getRegion() << "meshes " <<streetview.size() << " linkLvl " << linkLevel;
    if (b_showGui) {
        ofSetColor(255, 255, 255);
        ofDrawBitmapString(statusStream.str(), 20,  20);
        ofDrawBitmapString(statusStream2.str(), 20,  40);
        gui.draw();
    }
}

//--------

void ofApp::loadLinks(){
    //load next level of related links from the central start streetview
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

//-----------
void ofApp::calculateVector() {
    
//    // geo calc need to add into vector creation in loading new meshes...
//    // plus add in new code for mesh transforms and appending on export...
//    
//    home = ofxGeo::Coordinate(streetview[0].getLat(), streetview[0].getLon());
//    
//    int i =  streetview.size()-1;
//    
//    newLocation = ofxGeo::Coordinate(streetview[i].getLat(), streetview[i].getLon());
//    distanceSpherical = ofxGeo::Utils::distanceSpherical(home, newLocation);
//    distanceHaversine = ofxGeo::Utils::distanceHaversine(home, newLocation);
//    bearingHaversine = ofxGeo::Utils::bearingHaversine(home, newLocation);
//    midpoint = ofxGeo::Utils::midpoint(home, newLocation);
//    string closeLink = streetview[i].getCloseLinkTo(0) ;
//    string thisLink = streetview[0].getPanoId();
//    cout <<  "home to mesh " << i << " distanceHaversine: " << distanceHaversine << " bearing: " << bearingHaversine << " this link: " << thisLink << " closest link: "<< closeLink << endl;
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
    ofFileDialogResult saveFileResult = ofSystemSaveDialog(ofGetTimestampString() + ".obj", "Export your mesh as an .obj file");
    if (saveFileResult.bSuccess){
        ofColor c;
//        mesh.clear();
//        for(int i = 0; i < streetview.size(); i++){ //build new mesh to export
//            
//            // ofRotateZ(streetview[i].getDirection()+rotOffset[i]);
//            // ofTranslate(streetview[i].getLon()*longOffset[i], streetview[i].getLat()*latOffset[i], 0);
//            streetview[i].getTexture().bind();
//            mesh.append(streetview[i].getDethMesh());
//            streetview[i].getTexture().unbind();
//        }
        
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
        ofPixels pixels;
        ofTexture texture = streetview[0].getTexture();
        texture.readToPixels(pixels);
        
        ofImage image;
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
        
        ofSystemAlertDialog("saving " + ofToString(streetview.size()) +" views to sequence of .ply and .png files, please wait...");
        
        
        
        
        for(int i = 0; i < streetview.size(); i++){
            if ( streetview[i].bDataLoaded & streetview[i].bPanoLoaded) {
                float lat = streetview[i].getLat();
                float lon = streetview[i].getLon();
                float rot = streetview[i].getDirection();
                
                if (!file.doesFileExist((saveFileResult.filePath + "model" + "_" + ofToString(lat) + "_" + ofToString(lon) + "_" + ofToString(rot) + ".ply"))) {
                    // add in here xml export to contain all streetview data available...
                    mesh = streetview[i].getDethMesh();
                    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
                    mesh.save(saveFileResult.filePath + "model" + "_" + ofToString(lat) + "_" + ofToString(lon) + "_" + ofToString(rot) + ".ply"); //= true saves as binary file / false saves as ascii
                    cout << "wrote ply file to .ply number " << i << endl;
                    ofPixels pixels;
                    ofTexture texture = streetview[i].getTexture();
                    texture.readToPixels(pixels);
                    ofImage image;
                    image.setFromPixels(pixels);
                    image.save(saveFileResult.filePath + "texture" + "_" + ofToString(lat) + "_" + ofToString(lon) + "_" + ofToString(rot) + ".png");
                    cout << "wrote tex file to .png number " << i << endl;
                } else {
                    cout << "file exists: " << (saveFileResult.filePath + "texture" + "_" + ofToString(lat) + "_" + ofToString(lon) + "_" + ofToString(rot) + ".png") << endl;
                }
            }
        }
    }
}

//--------------------------------------------------------------

void ofApp::saveCacheFiles(){
    ofBuffer cacheBuffer;
    string panoId;
    
    ofSetDataPathRoot("../../../data/");
    
    for(int i=0; i< streetview.size(); i++){
        panoId = streetview[i].getPanoId();
        fileName = "smCache_" + panoId + ".data";
        
        ofstream output_file(fileName, ios::binary);
        //output_file.write("hello", 100);
        output_file.write((char *)&streetview[i], sizeof(streetview[i]));
        output_file.close();
        
        //obj.open(ofToDataPath(fileName),ofFile::WriteOnly);
        // cacheBuffer.set((char*)streetview, sizeof(streetview));
        // cacheBuffer.set(streetview[1], sizeof(streetview[i]);
        // obj.writeFromBuffer(cacheBuffer);
        //obj.close();
        
        cout << "written cache file" << endl;
    }
}

//--------------------------------------------------------------

void colorMesh(ofMesh &mesh, ofImage &image){
    
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

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult){
    
    ofLogVerbose("getName(): "  + openFileResult.getName());
    ofLogVerbose("getPath(): "  + openFileResult.getPath());
    
    ofFile file (openFileResult.getPath());
    
    if (file.exists()){
        //Limiting this example to one image so we delete previous ones
        //        processedImages.clear();
        //        loadedImages.clear();
        
        ofLogVerbose("The file exists - now checking the type via file extension");
        string fileExtension = ofToUpper(file.getExtension());
        
        //We only want images
        if (fileExtension == "obj" || fileExtension == "OBJ") {
            
            cout << "its an obj file ok" << endl;
            //Save the file extension to use when we save out
            //originalFileExtension = fileExtension;
            
            //Load the selected image
            //            ofImage image;
            //            image.load(openFileResult.getPath());
            //            if (image.getWidth()>ofGetWidth() || image.getHeight() > ofGetHeight())
            //            {
            //                image.resize(image.getWidth()/2, image.getHeight()/2);
            //            }
            //loadedImages.push_back(image);
            
            //Make some short variables
            //            int w = image.getWidth();
            //            int h = image.getHeight();
            
            //Make a new image to save manipulation by copying the source
            //            ofImage processedImage = image;
            
            //Walk through the pixels
            //            for (int y = 0; y < h; y++){
            //
            //                //Create a vector to store and sort the colors
            //                vector<ofColor> colorsToSort;
            //
            //                for (int x = 0; x < w; x++){
            //
            //                    //Capture the colors for the row
            //                    ofColor color = image.getColor(x, y);
            //                    colorsToSort.push_back(color);
            //                }
            //
            //                //Sort the colors for the row
            //               // sort (colorsToSort.begin(), colorsToSort.end(), sortColorFunction);
            //
            //                for (int x = 0; x < w; x++)
            //                {
            //                    //Put the sorted colors back in the new image
            //                    processedImage.setColor(x, y, colorsToSort[x]);
            //                }
            //            }
            //Store the processed image
            // processedImages.push_back(processedImage);
        }
    }
    
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
           // b_enableLight = !b_enableLight;
            //calculateVector();
            
            break;
            
        case 's':
        case 'S':
            exportOBJ(mesh);
            break;
            
        case 'c':
        case 'C':
            saveCacheFiles();
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
            //Open the Open File Dialog
            openFileResult= ofSystemLoadDialog("Select an obj file");
            //Check if the user opened a file
            if (openFileResult.bSuccess){
                ofLogVerbose("User selected a file");
                //We have a file, check it and process it
                processOpenFileSelection(openFileResult);
            }
            else {
                ofLogVerbose("User hit cancel");
            }
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
