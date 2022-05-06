#include "ConstantBody.h"

GameEngine::ConstantBody::ConstantBody(GameObject* parent) : BaseComponent(parent) {
	std::cout << "Create ConstantBody" << std::endl;

}

GameEngine::ConstantBody::~ConstantBody() {
	std::cout << "Delete ConstantBody" << std::endl;

}

void GameEngine::ConstantBody::Update() {
	std::cout << "Update ConstantBody" << std::endl;

}