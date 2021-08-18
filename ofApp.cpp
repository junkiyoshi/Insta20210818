#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(255);
	ofEnableDepthTest();
	ofDisableArbTex();

	this->frame_list_count = 60;

	this->cap.open("D:\\MP4\\production ID_4711774.mp4"); // Video URL https://www.pexels.com/video/video-of-streets-in-japan-during-daytime-4711774/
	this->cap_size = cv::Size(640, 360);

	this->rect_size = 40;
	for (int x = 0; x < this->cap_size.width; x += this->rect_size) {

		for (int y = 0; y < this->cap_size.height; y += this->rect_size) {

			auto image = make_unique<ofImage>();
			image->allocate(this->rect_size, this->rect_size, OF_IMAGE_COLOR);
			cv::Mat frame = cv::Mat(cv::Size(image->getWidth(), image->getHeight()), CV_MAKETYPE(CV_8UC3, image->getPixels().getNumChannels()), image->getPixels().getData(), 0);
			cv::Rect rect = cv::Rect(x, y, this->rect_size, this->rect_size);

			this->rect_images.push_back(move(image));
			this->cv_rects.push_back(rect);
			this->rect_frames.push_back(frame);
		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	cv::Mat frame, resize;

	this->cap >> frame;
	if (frame.empty()) { 
		
		this->cap.set(cv::CAP_PROP_POS_FRAMES, 0);
		return; 
	}

	cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
	cv::resize(frame, resize, this->cap_size);

	this->frame_list.push_front(resize);
	while (this->frame_list.size() > this->frame_list_count) {

		this->frame_list.pop_back();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofTranslate(this->cap_size.width * -0.5 + this->rect_size * 0.5, this->cap_size.height * -0.5 - this->rect_size * 0.5, 0);

	for (int i = 0; i < this->rect_frames.size(); i++) {

		float noise_seed = ofRandom(1000);
		int frame_index = ofMap(ofNoise(noise_seed, this->cv_rects[i].x * 0.005, this->cv_rects[i].y * 0.005, ofGetFrameNum() * 0.01), 0, 1, 0, this->frame_list.size());

		cv::Mat tmp_box_image(this->frame_list[frame_index], this->cv_rects[i]);
		tmp_box_image.copyTo(this->rect_frames[i]);

		this->rect_images[i]->update();
		this->rect_images[i]->getTexture().bind();
		ofSetColor(255);
		ofFill();
		ofDrawBox(glm::vec3(this->cv_rects[i].x, this->cap_size.height - this->cv_rects[i].y, 0), this->rect_size, this->rect_size, 5);
		this->rect_images[i]->unbind();

		ofSetColor(39);
		ofNoFill();
		ofDrawBox(glm::vec3(this->cv_rects[i].x, this->cap_size.height - this->cv_rects[i].y, 0), this->rect_size, this->rect_size, 5);
	}

	/*
	// Œ³“®‰æ‚Æ”ä‚×‚éŽž‚É‚ÍƒRƒƒ“ƒg‚ðŠO‚·

	ofTranslate(0, this->cap_size.height * 1.2);

	for (int i = 0; i < this->rect_frames.size(); i++) {

		int frame_index = 0;

		cv::Mat tmp_box_image(this->frame_list[frame_index], this->cv_rects[i]);
		tmp_box_image.copyTo(this->rect_frames[i]);

		this->rect_images[i]->update();
		this->rect_images[i]->getTexture().bind();
		ofSetColor(255);
		ofFill();
		ofDrawBox(glm::vec3(this->cv_rects[i].x, this->cap_size.height - this->cv_rects[i].y, 0), this->rect_size, this->rect_size, 5);
		this->rect_images[i]->unbind();

		ofSetColor(39);
		ofNoFill();
		ofDrawBox(glm::vec3(this->cv_rects[i].x, this->cap_size.height - this->cv_rects[i].y, 0), this->rect_size, this->rect_size, 5);
	}
	*/

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}