//
//  triangulateMesh.h
//  from volume-camera project
//
//  Created by Dan Buzzo on 24/08/2017.
//  this version 5/7/18
//  http://www.buzzo.com
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
