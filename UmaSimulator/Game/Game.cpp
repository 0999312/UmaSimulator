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
  randomDistributeCardsAndCalculate(rand);
}

void Game::randomDistributeCardsAndCalculate(std::mt19937_64& rand)
{
  //�Ƚ�6�ſ����䵽ѵ����
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 8; j++)
      cardDistribution[i][j] = 0;
  for (int i = 0; i < 6; i++)
  {
    std::vector<int> probs = { 100,100,100,100,100,50 }; //�������ʣ����������Ǹ�
    int cardType = GameDatabase::AllSupportCards[cardId[i]].cardType;
    int deYiLv = GameDatabase::AllSupportCards[cardId[i]].deYiLv;
    if (cardType >= 0 && cardType < 5)//���������ǿ�
      probs[cardType] + deYiLv;
    else //���˿�����ĸ��ʽϸ�
      probs[5] += 50;


    std::discrete_distribution<> d(probs.begin(), probs.end());
    int whichTrain = d(rand);//���ĸ�ѵ��
    if (whichTrain < 5)//û��
      cardDistribution[whichTrain][i] = true;
  }
  //���³��ͼ���
  {
    std::vector<int> probs = { 100,100,100,100,100,100 }; //���������Ǹ�
    std::discrete_distribution<> d(probs.begin(), probs.end());
    int whichTrain = d(rand);//���ĸ�ѵ��
    if (whichTrain < 5)//û��
      cardDistribution[whichTrain][6] = true;
    whichTrain = d(rand);//���ĸ�ѵ��
    if (whichTrain < 5)//û��
      cardDistribution[whichTrain][7] = true;
  }

  //�������ˣ��������������Լ�ֵ
  for (int trainType = 0; trainType < 5; trainType++)
  {
    vector<CardTrainingEffect> effects;
    for (int card = 0; card < 6; card++)
    {
      if (cardDistribution[trainType][card])//����������ѵ��
      {
        effects.push_back(GameDatabase::AllSupportCards[cardId[card]].getCardEffect(*this, trainType, cardJiBan[card]));
      }
    }
    //�����Ů���ѵ��
    //1.��ͷ������
    int cardNum = effects.size();
    double cardNumMultiplying = 1 + 0.05 * cardNum;
    //2.��Ȧ(����ѵ��)���ʣ�ע���Ƿ������Ѿ���getCardEffect�￼�ǹ���
    double youQingMultiplying = 1;
    for (int i = 0; i < cardNum; i++)
      youQingMultiplying *= (1 + 0.01 * effects[i].youQing);
    //3.ѵ������
    double xunLianBonusTotal = 0;
    for (int i = 0; i < cardNum; i++)
      xunLianBonusTotal += effects[i].xunLian;
    double xunLianMultiplying = 1 + 0.01 * xunLianBonusTotal;
    //4.�ɾ�����
    double ganJingBasic = 0.1 * (motivation - 3);
    double ganJingBonusTotal = 0;
    for (int i = 0; i < cardNum; i++)
      ganJingBonusTotal += effects[i].ganJing;
    double ganJingMultiplying = 1 + ganJingBasic * (1 + 0.01 * ganJingBonusTotal);

    //�벻ͬ�����޹ص��ܱ���
    double totalMultiplying = cardNumMultiplying * youQingMultiplying * xunLianMultiplying * ganJingMultiplying;

    //5.����ֵ
    int trainLv = getTrainingLevel(trainType);
    int basicValue[6] = { 0,0,0,0,0,0 };
    for (int i = 0; i < cardNum; i++)
    {
      for (int j = 0; j < 6; j++)
        basicValue[j] += effects[i].bonus[j];
    }
    for (int j = 0; j < 6; j++)
    {
      int b = GameConstants::TrainingBasicValue[trainType][trainLv][j];
      if(b>0)//��������
        basicValue[j] += b;
      else
        basicValue[j] = 0;
    }

  }
}

int Game::getTrainingLevel(int item) const
{
  int level = trainLevelCount[item] / 12;
  if (level > 4)level = 4;
  if (venusIsWisdomActive && venusAvailableWisdom == 1)//��Ů��
    level = 5;
  return level;
}
