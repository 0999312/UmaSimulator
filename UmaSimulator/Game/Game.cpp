#include <iostream>
#include "Game.h"
using namespace std;
void Game::newGame(mt19937_64& rand, int newUmaId, int newCards[6], int newZhongMaBlueCount[5])
{
  umaId = newUmaId;
  for (int i = 0; i < 6; i++)
    cardId[i] = newCards[i];
  for (int i = 0; i < 5; i++)
    zhongMaBlueCount[i] = newZhongMaBlueCount[i];

  random_device rd;
  rand = mt19937_64(rd);
  turn = 0;
  vital = 100;
  maxVital = 100;
  isQieZhe = false;
  isAiJiao = false;
  skillPt = 120;

  for (int i = 0; i < 5; i++)
    fiveValue[i] = GameDatabase::AllUmas[umaId].fiveValueInitial[i]; //�������ʼֵ
  for (int i = 0; i < 5; i++)//֧Ԯ����ʼ�ӳ�
  {
    for (int j = 0; j < 5; j++)
      fiveValue[j] = GameDatabase::AllSupportCards[cardId[i]].bonusBasic[j]; 
    skillPt += GameDatabase::AllSupportCards[cardId[i]].bonusBasic[5];
  }
  for (int i = 0; i < 5; i++)
    fiveValue[i] += zhongMaBlueCount[i] * 7; //����

  for (int i = 0; i < 5; i++)
    fiveValueLimit[i] = GameConstants::BasicFiveValueLimit[i]; //ԭʼ��������
  for (int i = 0; i < 5; i++)
    fiveValueLimit[i] += zhongMaBlueCount[i] * 7; //��������--����
  for (int i = 0; i < 5; i++)
    fiveValueLimit[i] += rand()%10; //��������--�������������

  motivation = 3;
  for (int i = 0; i < 6; i++)
    cardJiBan[i] = GameDatabase::AllSupportCards[cardId[i]].initialJiBan;
  cardJiBan[6] = 0; 
  cardJiBan[7] = 0;
  for (int i = 0; i < 5; i++)
    trainLevelCount[i] = 0;
  isRacing = false;


  venusLevelYellow = 0;
  venusLevelRed = 0;
  venusLevelBlue = 0;
  venusSpiritsCount = 0;
  for (int i = 0; i < 8; i++)
    venusSpiritsBottom[i] = 0;
  for (int i = 0; i < 6; i++)
    venusSpiritsUpper[i] = 0;
  venusAvailableWisdom = 0;
  venusIsWisdomActive = false;


  venusCardFirstClick = false;
  venusCardUnlockOutgoing = false;
  venusCardIsQingRe = false;
  for (int i = 0; i < 5; i++)
    venusCardOutgoingUsed[i] = false;

  stageInTurn = 0;
  randomDistributeCardsAndCalculate();
}
