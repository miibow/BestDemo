#include "BaseCell.h"
#include "Layer/BackLayer.h"
#include "Manager/AnimateManager.h"
#include "Resource/Resource.h"

BaseCell::BaseCell()
{
}

BaseCell::~BaseCell()
{
}

BaseCell* BaseCell::createWithItemId(int id)
{
	BaseCell* cell = new(std::nothrow) BaseCell();
	if (cell != nullptr && cell->initWithItemId(id))
	{
		cell->autorelease();
	}
	else
	{
		delete cell;
		cell = nullptr;
	}
	return cell;
}

bool BaseCell::initWithItemId(int id)
{
	if (!Button::init())
		return false;

	return true;
}
