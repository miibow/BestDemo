#ifndef __BASECELL_H__
#define __BASECELL_H__

#include "Common.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "Data/DataInstance.h"
#include "Resource/Resource.h"

USING_NS_CC;
using namespace ui;

class BaseCell : public Button
{
public:
	BaseCell();
	~BaseCell();

	BaseCell* createWithItemId(int id);

	bool initWithItemId(int id);

};

#endif