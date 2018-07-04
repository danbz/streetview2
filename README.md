# based on ofxStreetView

forked from http://patriciogonzalezvivo.com 

this version extends to allow export of .ply files with corresponding .png panorama data and as triangulated .obj files.
it adds a simple control panel for the first 20 panoramas to allow rotation, point-cloud dot-size selection and a scaling factor for improving alignment of multiple panoramas.

the code is under constant revision and should be considered stable. 

# controls:

e : export ply and png file pairs named with (your filename)_(model or texture)_lat_long_panorotation.(ply or png)

s : export .obj file

f : toggle fullscreen mode

g: toggle gui

up/right/down/left arrow keys - load an additional panorama at 0,90,180,270 degrees to the current panorama

+ recurse through a tree of links listed in the data for exach panorama and load the panoaramas - one level at a time

# dependencies
ofxGui
ofxMercatorMap
ofxStreetView
ofxXmlSettings

[ ![stokes croft](http://buzzo.com/wp-content/uploads/2017/12/Screen-Shot-2017-12-17-at-19.45.12-e1513543600881.png) ](http://buzzo.com/streetview/)

[ ![stokes croft](http://buzzo.com/wp-content/uploads/2017/12/Screen-Shot-2017-12-17-at-20.54.26-e1513543978734.png) ](http://buzzo.com/wp-content/uploads/2017/12/Screen-Shot-2017-12-17-at-20.54.26-300x261.png/)

[ ![backfields lane](http://buzzo.com/wp-content/uploads/2017/12/Screen-Shot-2017-12-17-at-20.53.21-e1513544088818.png) ](http://buzzo.com/wp-content/uploads/2017/12/Screen-Shot-2017-12-17-at-20.53.21-300x210.png)



## How it works

By doing the call:

	http://cbk0.google.com/cbk?output=xml&panoid=[pano_id]&dm=1

We get information that looks like [this](http://maps.google.com/cbk?output=xml&cb_client=maps_sv&v=4&dm=1&hl=en&panoid=ki_KzVWkE87EgkPWg3QPXg) 

At <deptMap> you can seee a depth image encoded in base64 (and zlib compressed)

This addon will construct the panoramic image (that you can get with ```getTextureReference()``` ) and then construct a 3D Mesh using the DepthMap information.

## Credits
- [ StreetView Explorer of Paul Wagener](https://github.com/PaulWagener/Streetview-Explorer)
