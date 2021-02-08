#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void mouseEntered(int x, int y) {};
	void mouseExited(int x, int y) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	int number_of_targets;
	vector<glm::vec2> target_list;
	vector<glm::vec3> color_list;

	ofShader shader;

	float deg_noise_seed;
	float radius_noise_seed;

	vector<glm::vec2> location_list;
	vector<glm::vec2> velocity_list;
	vector<glm::vec3> location_color_list;
	vector<float> color_value_list;
};