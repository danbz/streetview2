//
//  triangulateMesh.h
//  volume-camera
//
//  Created by Dan Buzzo on 24/08/2017.
//
//

#pragma once

#ifndef triangulateMesh_h
#define triangulateMesh_h

#include <stdio.h>
#include "ofMain.h"
#include "dataStructures.h"

#endif /* triangulateMesh_h */

class triangulateMesh
{
    public :
    
    triangulateMesh()
    {
    };
    
    void setup();
    void makeTriangles( ofMesh &mesh, int width, int height, ofImage image);
    void makeMesh( ofShortImage &filteredDepthImage, ofImage &filteredColorImage, ofMesh &mesh, volca volca, vRenderer &volcaRenderer);
    void setNormals( ofMesh &mesh);
    
    ~triangulateMesh() // destructor
    {
    };
    
};