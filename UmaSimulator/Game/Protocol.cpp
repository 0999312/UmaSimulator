#include <iostream>
#include <sstream>
#include <cassert>
#include "../External/json.hpp"
#include "Game.h"
using namespace std;
using json = nlohmann::json;

bool Game::loadGameFromJson(std::string jsonStr)
{
  try
  {
    json j = json::parse(jsonStr);

    umaId = j["umaId"];
    if (!GameDatabase::AllUmaGameIdToSimulatorId.count(umaId))
      throw "δ֪����";
    umaId = GameDatabase::AllUmaGameIdToSimulatorId.at(umaId);

    turn = j["turn"];
    if (turn >= TOTAL_TURN && turn < 0)
      throw "�غ�������ȷ";

    vital = j["vital"];
    maxVital = j["maxVital"];
    isQieZhe = j["isQieZhe"];
    isAiJiao = j["isAiJiao"];
    failureRateBias = j["failureRateBias"];
    for (int i = 0; i < 5; i++)
      fiveStatus[i] = j["fiveStatus"][i];
    for (int i = 0; i < 5; i++)
      fiveStatusLimit[i] = j["fiveStatusLimit"][i];

    skillPt = j["skillPt"];
    motivation = j["motivation"];
    for (int i = 0; i < 6; i++)
    {
      int c = j["cardId"][i];
      if (!GameDatabase::AllSupportCardGameIdToSimulatorId.count(c))
        throw "δ֪֧Ԯ��";
      cardId[i] = GameDatabase::AllSupportCardGameIdToSimulatorId.at(c);
    }

    for (int i = 0; i < 8; i++)
      cardJiBan[i] = j["cardJiBan"][i];
    

    for (int i = 0; i < 5; i++)
      trainLevelCount[i] = j["trainLevelCount"][i];
    
    for (int i = 0; i < 5; i++)
      zhongMaBlueCount[i] = j["zhongMaBlueCount"][i];

    for (int i = 0; i < 6; i++)
      zhongMaExtraBonus[i] = j["zhongMaExtraBonus"][i];
    
    isRacing = j["isRacing"];
    venusLevelYellow = j["venusLevelYellow"];
    venusLevelRed = j["venusLevelRed"];
    venusLevelBlue = j["venusLevelBlue"];

    for (int i = 0; i < 8; i++)
      venusSpiritsBottom[i] = j["venusSpiritsBottom"][i];

    for (int i = 0; i < 6; i++)
      venusSpiritsUpper[i] = j["venusSpiritsUpper"][i];

    venusAvailableWisdom = j["venusAvailableWisdom"];
    venusIsWisdomActive = j["venusIsWisdomActive"];
    venusCardFirstClick = j["venusCardFirstClick"];
    venusCardUnlockOutgoing = j["venusCardUnlockOutgoing"];
    venusCardIsQingRe = j["venusCardIsQingRe"];
    venusCardQingReContinuousTurns = j["venusCardQingReContinuousTurns"];

    for (int i = 0; i < 5; i++)
      venusCardOutgoingUsed[i] = j["venusCardOutgoingUsed"][i];

    stageInTurn = j["stageInTurn"];
    for (int i = 0; i < 5; i++)
      for (int k = 0; k < 8; k++)
      {
        cardDistribution[i][k] = j["cardDistribution"][i][k];
      }

    for (int i = 0; i < 6; i++)
      cardHint[i] = j["cardHint"][i];

    for (int i = 0; i < 8; i++)
      spiritDistribution[i] = j["spiritDistribution"][i];


    if (cardId[0] != SHENTUAN_ID)//1��λ�������ţ���������λ�ã������Ż���1��λ
    {
      int s = -1;//����ԭλ��
      for (int i = 1; i < 6; i++)
      {
        if (cardId[i] == SHENTUAN_ID)
        {
          s = i;
          break;
        }
      }
      if (s == -1)
        throw "û������";

      std::swap(cardId[s], cardId[0]);
      std::swap(cardJiBan[s], cardJiBan[0]);

      for (int i = 0; i < 5; i++)
        std::swap(cardDistribution[i][s], cardDistribution[i][0]);

      std::swap(cardHint[s], cardHint[0]);
    }

    initRandomGenerators();
    calculateVenusSpiritsBonus();
    calculateTrainingValue();

  }
  catch (string e)
  {
    cout << "��ȡ��Ϸ��Ϣjson����" << e << endl;
    return false;
  }
  catch (...)
  {
    cout << "��ȡ��Ϸ��Ϣjson����δ֪����" << endl;
    return false;
  }
  return true;
}

