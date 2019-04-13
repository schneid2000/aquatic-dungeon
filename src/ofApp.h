#pragma once

#include "ofMain.h"
#include "map/level.h"
#include <iostream>

static const int kDisplaySize = 7;

class ofApp : public ofBaseApp{
	ofImage bridge;
	ofImage water;
	ofImage ceiling;
	ofImage floor;
	ofImage crabman_front;
	Level level = Level(true);

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		//Gets the image from a tile's type
		ofImage get_image_from_type(std::string type);

};
