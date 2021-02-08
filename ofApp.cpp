#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofSetColor(255);

	this->number_of_targets = 500;
	for (int i = 0; i < this->number_of_targets; i++) {

		this->target_list.push_back(glm::vec2());
		this->color_list.push_back(glm::vec3(1, 1, 1));
	}

	this->shader.load("shader/shader.vert", "shader/shader.frag");

	this->deg_noise_seed = ofRandom(1000);
	this->radius_noise_seed = ofRandom(1000);
}

//--------------------------------------------------------------
void ofApp::update() {

	int radius = ofMap(ofNoise(this->radius_noise_seed, ofGetFrameNum() * 0.005), 0, 1, -300, 300);
	int next_radius = ofMap(ofNoise(this->radius_noise_seed, (ofGetFrameNum() + 1) * 0.005), 0, 1, -300, 300);

	int deg = ofMap(ofNoise(this->deg_noise_seed, ofGetFrameNum() * 0.005), 0, 1, 0, 360);
	int next_deg = ofMap(ofNoise(this->deg_noise_seed, (ofGetFrameNum() + 1) * 0.005), 0, 1, 0, 360);

	for (int deg_param = 0; deg_param < 360; deg_param += 90) {

		auto location = glm::vec2(ofGetWidth() * 0.5 + radius * cos((deg + deg_param) * DEG_TO_RAD), ofGetHeight() * 0.5 + radius * sin((deg + deg_param) * DEG_TO_RAD));
		auto next = glm::vec2(ofGetWidth() * 0.5 + next_radius * cos((next_deg + deg_param) * DEG_TO_RAD), ofGetHeight() * 0.5 + next_radius * sin((next_deg + deg_param) * DEG_TO_RAD));
		auto distance = location - next;
		distance *= 0.5;

		auto future = location + distance * 30;
		auto random_deg = ofRandom(360);
		future += glm::vec2(30 * cos(random_deg * DEG_TO_RAD), 30 * sin(random_deg * DEG_TO_RAD));
		auto future_distance = future - location;

		this->location_list.push_back(location);
		this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance));

		glm::vec3 tmp_color;
		switch ((int)ofRandom(3))
		{
		case 0:
			tmp_color += glm::vec3(0.6, 0.4, 0.4);
			break;
		case 1:
			tmp_color += glm::vec3(0.4, 0.6, 0.4);
			break;
		case 2:
			tmp_color += glm::vec3(0.4, 0.4, 0.6);
			break;
		}
		this->location_color_list.push_back(tmp_color);
		this->color_value_list.push_back(1);
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.01;
		this->color_value_list[i] *= 0.98;

		if (this->color_value_list[i] < 0.1 || glm::distance(glm::vec2(ofGetWidth() * 0.5, ofGetHeight() * 0.5), this->location_list[i]) > 720) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->location_color_list.erase(this->location_color_list.begin() + i);
			this->color_value_list.erase(this->color_value_list.begin() + i);
		}
	}

	for (int i = 0; i < this->target_list.size(); i++) {

		if (i < this->location_list.size()) {

			this->target_list[i] = this->location_list[i];
			this->color_list[i] = this->location_color_list[i] * this->color_value_list[i];
		}
		else {

			this->color_list[i] = glm::vec3(0);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofFill();
	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform2fv("targets", &this->target_list[0].x, this->number_of_targets);
	this->shader.setUniform3fv("colors", &this->color_list[0].x, this->number_of_targets);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}