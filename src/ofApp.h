#pragma once

#include "ofMain.h"
#include "ofxStreetView.h"
#include "ofxGui.h"
#include "triangulateMesh.h"
#include "ofxMercatorMap.h"

struct streetviewsFromDisc {
    ofMesh mesh;
    ofImage image;
    float lat, lon, rot;
};

class ofApp : public ofBaseApp{
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void exportOBJ(ofMesh &mesh);
    void exportPLY(ofMesh &mesh);
    void loadOBJ(ofMesh &mesh);
    void processOpenFileSelection(ofFileDialogResult openFileResult);
    void loadNewStreet( int degrees);
    void loadLinks();
    void loadViewsfromFile();
    
    //gui
    ofxFloatSlider xOffset[20], yOffset[20], scaleMeters;
    ofxIntSlider rotOffset[20], pointSize;
    ofxToggle filled;
    ofxToggle showMesh[20];
    ofxPanel gui;
    
    ofLight worldLight;
    ofMesh mesh;
    ofFile obj;
    ofEasyCam cam;
    
    vector <ofxStreetView> streetview;
    vector <streetviewsFromDisc> localView;
    ofFileDialogResult openFileResult;
    
    double viewLat, viewLong;
    bool b_drawPointCloud, b_enableLight, b_updateMesh, b_showGui, b_meshExists;
    string fileName;
    int linkLevel;
    
    triangulateMesh meshMaker;
    
    ofImage panoImage;
    ofTexture textureImage;
    
    /// for mercator maps
    vector <ofPoint>    streetviewsCoordinates;
    ofxMercatorMap      merMap;
    int merMapWidth, merMapHeight;
    
    
};
