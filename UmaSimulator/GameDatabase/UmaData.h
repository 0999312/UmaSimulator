#pragma once
#include "GameConstants.h"
#include "../External/json.hpp"
#include <vector>
using json = nlohmann::json;

template <typename T>
json arrayToJson(T* arr, int len)
{
	json j;
	for (int i = 0; i < len; ++i)
		j += arr[i];
	return j;
}

template <typename T>
int jsonToArray(const json& j, T* buf, int bufSize)
{
	int count = 0;
	if (!j.is_array() || bufSize <= 0)
		throw "Must be array";

	for (auto it : j) {
		buf[count++] = it;
		if (count >= bufSize) break;
	}
	return count;
}

std::string string_To_UTF8(const std::string& str);
std::string UTF8_To_string(const std::string& str);

//����Ĳ���
struct UmaData
{
  int star;//����
  bool races[TOTAL_TURN];//��Щ�غ��б���
  int fiveStatusBonus[5];//���Լӳ�
  int fiveStatusInitial[5];//��ʼ����
};

// �������ɱ������� [start, end]
struct FreeRaceData
{
	int startTurn;	// ��ʼ�غ�
	int endTurn;	// �����غϣ�������
	int count;		// ��Ҫ�Ĵ���

	// ����friend from_json, to_json��json��ʹ��
	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(FreeRaceData, startTurn, endTurn, count)
};

struct BonusData
{
	int triggerTurn;	// ����غϣ���0��ʼ���
	std::vector<int> statusBonus;	// ���ԣ�pt����������hintתΪpt������
	std::vector<int> raceTurns;	// ��Ҫ�����Ļغϱ�ţ�0..TOTAL_TURN)
	std::string description; // ��������

	// ����inline
	friend void to_json(json& j, const BonusData& me)
	{
		j["triggerTurn"] = me.triggerTurn;
		j["statusBonus"] = me.statusBonus;
		j["raceTurns"] = me.raceTurns;
		j["description"] = string_To_UTF8(me.description);
	}

	friend void from_json(const json& j, BonusData& me)
	{
		std::string st;
		j.at("triggerTurn").get_to(me.triggerTurn);
		j.at("statusBonus").get_to(me.statusBonus);
		j.at("raceTurns").get_to(me.raceTurns);
		j.at("description").get_to(st);
		me.description = UTF8_To_string(st);
	}
};

struct JsonUmaData : public UmaData
{
	std::string name;
	int gameId;
	std::vector<FreeRaceData> freeRaces;	// ���ɱ������䣨�����
	std::vector<int> preferRaces;	// ��������Ļغϱ��
	std::vector<int> preferReds;	// �����Ů��Ļغϱ��
	std::vector<BonusData> bonusData;	// ���ػغϽ����б�

	// ����inline
	friend void to_json(json& j, const JsonUmaData& me)
	{
		j["gameId"] = me.gameId;
		j["name"] = string_To_UTF8(me.name);
		j["star"] = me.star;
		j["races"] = arrayToJson(me.races, TOTAL_TURN);
		j["fiveStatusBonus"] = arrayToJson(me.fiveStatusBonus, 5);
		j["fiveStatusInitial"] = arrayToJson(me.fiveStatusInitial, 5);
		j["freeRaces"] = me.freeRaces;
		j["preferRaces"] = me.preferRaces;
		j["preferReds"] = me.preferReds;
		j["bonusData"] = me.bonusData;
	}

	friend void from_json(const json& j, JsonUmaData& me)
	{
		std::string st;
		j.at("gameId").get_to(me.gameId);
		j.at("name").get_to(st);
		me.name = UTF8_To_string(st);
		j.at("star").get_to(me.star);
		j.at("races").get_to(me.races);
		jsonToArray(j.at("fiveStatusBonus"), me.fiveStatusBonus, 5);
		jsonToArray(j.at("fiveStatusInitial"), me.fiveStatusInitial, 5);
		j.at("freeRaces").get_to(me.freeRaces);
		j.at("preferRaces").get_to(me.preferRaces);
		j.at("preferReds").get_to(me.preferReds);
		j.at("bonusData").get_to(me.bonusData);
	}
};
