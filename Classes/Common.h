#ifndef __JUMPCOMMON_H__
#define __JUMPCOMMON_H__

#include "cocos2d.h"
#include "VisibleRect.h"
#include "SimpleAudioEngine.h"
#include "Manager/AnimateManager.h"
#include "Manager/AudioManager.h"

static CocosDenshion::SimpleAudioEngine* AudioPlayer = CocosDenshion::SimpleAudioEngine::getInstance();

#define SAFE_EXIT(p) do{if(p){p->removeFromParent(); CC_SAFE_RELEASE_NULL(p);}} while(0);

#define MINPROBABILITYUNIT 1E+3

#define MAXATTRNUM 6
#define MAXMAGICNUM 5
#define MAXEQUIPNUM 8

#define MAXMAGICLEVEL 100
#define MAXEQUIPLEVEL 100 
#define  DELTA 2.0f
#define  SCALEFACTORW  Xiyou_VisibleRect::getVisibleRect().size.width / 750.0f
#define  SCALEFACTORH  Xiyou_VisibleRect::getVisibleRect().size.height / 1334.0f

enum {
	BLUECRYSTAL = 203,
	REDCRYSTAL,
	WALK = 1888,
	DRAW,
	HEROSELECTE,
	EQUIPSELECTE,
	TREASUREBOX,
	SHOWUP,
	BORN
};

enum {
	ROLE = 0,
	ITEM,
};

enum {
	ISMOVE = 0,
	ISATTACK,
	ISIDLE,
	ISDEAD
};

#endif