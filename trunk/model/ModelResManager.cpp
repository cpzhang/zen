#include "ModelResManager.h"

void IModelRes::onNoReference_()
{
	ModelResManager::getInstance()->release(ResID_);
}
