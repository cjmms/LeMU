#include "GameObject.hpp"


LeMU::GameObject LeMU::GameObject::createGameObject()
{
	static id_t currentID = 0;
	return GameObject{ currentID++ };
}