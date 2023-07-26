#pragma once
#include <string>
#include <unordered_map>
#include "UmaData.h"
#include "../SupportCard/SupportCard.h"

class GameDatabase
{
public:
  static const int ALL_SUPPORTCARD_NUM = 51;
  static const std::string AllSupportCardNames[ALL_SUPPORTCARD_NUM];
  static const std::map<int, int> AllSupportCardGameIdToSimulatorId;
  static SupportCard AllSupportCards[ALL_SUPPORTCARD_NUM];

  // unordered_map ��Hash�洢����ѯ��̯���Ӷ�O(1)
  static std::unordered_map<int, UmaData> AllUmas;

  static void loadUmas(const std::string& dir);
};