#ifndef __ITEM_H__
#define __ITEM_H__

#include "Common.h"

struct spDropItem 
{
	int _id;
	int _dropProb;
	int _relatedId;
};

struct spBaseItem
{
	int m_iId;
	int m_iSalePrice;
	int m_iSuperposition;
	int m_iRelatedId;
	int m_iExpendable;
	std::string m_sItemPic;
	std::string m_sRelatedName;
};

struct spFragment : public spDropItem
{
	std::string _name;
	int m_iCostNum;
	int m_iCostType;
	int m_iFragmentType;
	int m_iExchangeNums;
};

struct spTopUpItem
{
	int m_iId;
	std::string _name;
	std::string _description;
	int m_iType;
	int m_iCostNum;
};

struct chapterConfig
{
	int m_iId;
	std::string m_sName;
	std::string m_sIcon;
	std::vector<int> m_vStages;
};

struct stageConfig
{
	int m_iId;
	int m_iExp;
	int m_iGold;
	std::string m_sName;
	std::vector<std::pair<int, int>> m_vEnemySet;
	std::vector<int> m_vDropSet;

};

struct spTaskItem
{
	int m_iId;				// mission id
	int m_iGoal;			// goal num
	std::string m_sTitle;	// mission title
	std::string m_sDescription;		// mission description
	std::string m_sTaskPic;			// mission picture name
	std::string m_sNeedPath;		// mission index
	std::vector<int> m_vAwardIndex;		// mission award
	std::vector<int> m_vAssociation; // unlock missions set
};

struct spTaskAwardItem
{
	int m_iId;
	int m_iAwardNum;
	int m_iItemId;
	std::string m_sAwardPic;
};

struct spOwnedHero
{
	spOwnedHero() :m_iLevel(0) {}

	int m_iLevel;
	std::vector<int> _equipList;
};

#endif