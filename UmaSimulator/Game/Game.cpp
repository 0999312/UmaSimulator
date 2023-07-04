#include <iostream>
#include <cassert>
#include "Game.h"
using namespace std;
void Game::newGame(mt19937_64& rand, int newUmaId, int newCards[6], int newZhongMaBlueCount[5])
{
  umaId = newUmaId;
  for (int i = 0; i < 6; i++)
    cardId[i] = newCards[i];
  assert(cardId[0] = SHENTUAN_ID);
  for (int i = 0; i < 5; i++)
    zhongMaBlueCount[i] = newZhongMaBlueCount[i];

  turn = 0;
  vital = 100;
  maxVital = 100;
  isQieZhe = false;
  isAiJiao = false; 
  failureRateBias = 0;
  skillPt = 120;

  for (int i = 0; i < 5; i++)
    fiveStatus[i] = GameDatabase::AllUmas[umaId].fiveStatusInitial[i]; //�������ʼֵ
  for (int i = 0; i < 5; i++)//֧Ԯ����ʼ�ӳ�
  {
    for (int j = 0; j < 5; j++)
      fiveStatus[j] = GameDatabase::AllSupportCards[cardId[i]].bonusBasic[j]; 
    skillPt += GameDatabase::AllSupportCards[cardId[i]].bonusBasic[5];
  }
  for (int i = 0; i < 5; i++)
    fiveStatus[i] += zhongMaBlueCount[i] * 7; //����

  for (int i = 0; i < 5; i++)
    fiveStatusLimit[i] = GameConstants::BasicFiveStatusLimit[i]; //ԭʼ��������
  for (int i = 0; i < 5; i++)
    fiveStatusLimit[i] += zhongMaBlueCount[i] * 7; //��������--����
  for (int i = 0; i < 5; i++)
    fiveStatusLimit[i] += rand()%10; //��������--�������������

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
  randomDistributeCards(rand); 
  calculateTrainingValue();
}

void Game::randomDistributeCards(std::mt19937_64& rand)
{
  //�Ƚ�6�ſ����䵽ѵ����
  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 8; j++)
      cardDistribution[i][j] = 0;

  double blueVenusHintBonus = 1 + 0.01 * GameConstants::BlueVenusLevelHintProbBonus[venusLevelBlue];

  for (int i = 0; i < 6; i++)
  {
    std::vector<int> probs = { 100,100,100,100,100,50 }; //�������ʣ����������Ǹ�
    int cardType = GameDatabase::AllSupportCards[cardId[i]].cardType;
    int deYiLv = GameDatabase::AllSupportCards[cardId[i]].deYiLv;
    if (cardType >= 0 && cardType < 5)//���������ǿ�
      probs[cardType] += deYiLv;
    else //���˿�����ĸ��ʽϸ�
      probs[5] += 50;


    std::discrete_distribution<> d(probs.begin(), probs.end());
    int whichTrain = d(rand);//���ĸ�ѵ��
    if (whichTrain < 5)//û��
      cardDistribution[whichTrain][i] = true;

    //�Ƿ���hint
    if (cardType >= 0 && cardType < 5)//���������ǿ�
    {
      double hintProb = 0.06 * blueVenusHintBonus * (1 + 0.01 * GameDatabase::AllSupportCards[cardId[i]].hintProbIncrease);
      bernoulli_distribution d(hintProb);
      cardHint[i] = d(rand);
    }
    else cardHint[i] = false;
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
  //������Ƭ
  if (turn < 2 || venusSpiritsBottom[7] != 0)//����Ƭ
  {
    for (int i = 0; i < 8; i++)
      spiritDistribution[i] = 0;
  }
  else
  {
    bool allowTwoSpirits = venusSpiritsBottom[6] != 0;//��������λ
    for (int i = 0; i < 8; i++)
    {
      std::discrete_distribution<> d(GameConstants::VenusSpiritTypeProb[i], GameConstants::VenusSpiritTypeProb[i+1]);
      int spiritType = d(rand) + 1;
      int spiritColor = rand() % 3;
      int spirit = spiritType + spiritColor * 8;

      //�����Ƿ�Ϊ˫��Ƭ
      bool twoSpirits = false;
      if (allowTwoSpirits)
      {
        if (i < 5)//���ѵ��
        {
          bool isShining = false;//�Ƿ�����
          for (int card = 0; card < 6; card++)
          {
            if (cardDistribution[i][card])//����������ѵ��
            {
              isShining |= GameDatabase::AllSupportCards[cardId[card]].getCardEffect(*this, i, cardJiBan[card]).youQing > 0;
            }
          }
          if (isShining)
          {
            if (i < 4)twoSpirits = true;
            else if (rand() % 5 < 2)twoSpirits = true;//������Ȧ40%˫��Ƭ
          }
        }
        else
        {
          if (rand() % 5 == 0)twoSpirits = true;//������20%˫��Ƭ
        }
      }
      if (twoSpirits)spirit += 32;//+32����������Ƭ
      spiritDistribution[i] = spirit;
    }

  }
}

void Game::calculateTrainingValue()
{
  for (int trainType = 0; trainType < 5; trainType++)
  {
    calculateTrainingValueSingle(trainType);
  }
}
void Game::addSpirit(std::mt19937_64& rand, int s)
{
  int place = -1;//�ڵڼ�����Ƭ��
  for (int i = 0; i < 8; i++)
  {
    if (venusSpiritsBottom[i] == 0)
    {
      place = i;
      break;
    }
  }
  if (place == -1)return;//��Ƭ������
  venusSpiritsBottom[place] = s;
  if (place % 2 == 1)//�ڶ���������Ƭ
  {
    int sL = venusSpiritsBottom[place - 1];
    int colorL = sL / 8;
    int typeL = sL % 8;
    int typeR = s % 8;

    int type = typeL;
    if (rand() % 5 == 0)
      type = typeR;//��20%�������Ҳ���Ƭ������
    int sU = type + 8 * colorL;//�ϲ���Ƭ
    int layer2Place = place / 2;
    venusSpiritsUpper[layer2Place] = sU;


    if(layer2Place%2==1)//������������Ƭ
    {
      int sL = venusSpiritsUpper[layer2Place - 1];
      int colorL = sL / 8;
      int typeL = sL % 8;
      int typeR = sU % 8;

      int type = typeL;
      if (rand() % 5 == 0)
        type = typeR;//��20%�������Ҳ���Ƭ������
      int sU2 = type + 8 * colorL;//�ϲ���Ƭ
      int layer3Place = 4 + layer2Place / 2;
      venusSpiritsUpper[layer3Place] = sU2;

    }
  }

  if (place == 7)//��Ƭ������
  {
    int wiseColor = -1;
    int color1 = venusSpiritsBottom[0] / 8;
    int color2 = venusSpiritsBottom[4] / 8;
    if (color1 == color2)//1��λ��5��λͬɫ
    {
      wiseColor = color1;
    }
    else//��һ���ĸ���
    {
      int count = 0; 
      for (int i = 0; i < 8; i++)
      {
        int c = venusSpiritsBottom[i] / 8;
        if (c == color1)count += 1;
        else if (c == color2)count -= 1;
      }
      if (count > 0)wiseColor = color1;
      else if (count < 0)wiseColor = color2;
      else//�������
      {
        wiseColor = rand() % 2 ? color1 : color2;
      }
    }

    venusAvailableWisdom = wiseColor + 1;//123�ֱ��Ǻ�����
  }
  calculateVenusSpiritsBonus();

}
void Game::activateVenusWisdom()
{
  if (venusAvailableWisdom == 0)return;
  venusIsWisdomActive = true;
  if (venusAvailableWisdom == 1)//����
  {
    if (venusLevelRed < 5)
      venusLevelRed += 1;
    vital += 50;
    if (vital > maxVital)vital = maxVital;
    motivation = 5;
    //������Ŀ�������ﴦ��
  }
  if (venusAvailableWisdom == 2)//����
  {
    if (venusLevelBlue < 5)
      venusLevelBlue += 1;
    for (int i = 0; i < 6; i++)
    {
      if (GameDatabase::AllSupportCards[cardId[i]].cardType < 5)
        cardHint[i] = true;
    }
    //������Ŀ�������ﴦ��
  }
  if (venusAvailableWisdom == 3)//����
  {
    if (venusLevelYellow < 5)
      venusLevelYellow += 1;
    //����ѵ���������ﴦ��
  }

  calculateTrainingValue();//���¼���ѵ��ֵ
}
void Game::clearSpirit()
{

}
int Game::calculateFailureRate(int trainType) const
{
  //������ϵ�ѵ��ʧ���ʣ����κ��� A*x^2 + B*x + C + 0.5 * trainLevel
  //���Ӧ����2%����
  static const double A = 0.0245;
  static const double B[5] = { -3.77,-3.74,-3.76,-3.81333,-2.42857 };
  static const double C[5] = { 130,127,129,133.5,74.5 };

  double f = A * vital * vital + B[trainType] * vital + C[trainType] + 0.5 * getTrainingLevel(trainType);
  int fr = round(f);
  if (fr < 0)fr = 0;
  if (fr > 99)fr = 99;//����ϰ���֣�ʧ�������99%
  fr += failureRateBias;
  if (fr < 0)fr = 0;
  if (fr > 100)fr = 100;
  return fr;
}
void Game::calculateVenusSpiritsBonus()
{
  for (int i = 0; i < 6; i++)
    spiritBonus[i] = 0;
  //����ײ�
  for (int i = 0; i < 8; i++)
  {
    int s = venusSpiritsBottom[i];
    int type = s % 8 - 1;//012345��Ӧ����������pt
    if (type == -1)//����Ƭ��
      break;
    int color = s / 8; //012��Ӧ������
    spiritBonus[type] += 1;
  }
  //����ڶ���
  for (int i = 0; i < 4; i++)
  {
    int s = venusSpiritsUpper[i];
    int sL = venusSpiritsBottom[i * 2];//������Ƭ
    int sR = venusSpiritsBottom[i * 2 + 1];//������Ƭ
    int type = s % 8 - 1;//012345��Ӧ����������pt
    if (type == -1)//����Ƭ��
      break;
    if (sL / 8 == sR / 8)//�����Ҳ���Ƭ��ɫ��ͬ
      spiritBonus[type] += 2;
    else
      spiritBonus[type] += 3;
  }

  //���������
  for (int i = 0; i < 2; i++)
  {
    int s = venusSpiritsUpper[i + 4];
    int sL = venusSpiritsUpper[i * 2];//������Ƭ
    int sR = venusSpiritsUpper[i * 2 + 1];//������Ƭ
    int type = s % 8 - 1;//012345��Ӧ����������pt
    if (type == -1)//����Ƭ��
      break;
    if (sL / 8 == sR / 8)//�����Ҳ���Ƭ��ɫ��ͬ
      spiritBonus[type] += 2;
    else
      spiritBonus[type] += 3;
  }
}
void Game::runRace(int basicFiveStatusBonus, int basicPtBonus)
{
  int cardRaceBonus = 0;
  for (int card = 0; card < 6; card++)
  {
    cardRaceBonus += GameDatabase::AllSupportCards[cardId[card]].saiHou;
  }
  double raceMultiply = 1 + 0.01 * cardRaceBonus;
  if (venusAvailableWisdom == 1 && venusIsWisdomActive)//����
    raceMultiply *= 1.35;
  int fiveStatusBonus = floor(raceMultiply * basicFiveStatusBonus);
  int ptBonus = floor(raceMultiply * basicPtBonus);
  for (int i = 0; i < 5; i++)fiveStatus[i] += fiveStatusBonus;
  skillPt += basicPtBonus;
}
void Game::calculateTrainingValueSingle(int trainType)
{
  //�������ˣ��������������Լ�ֵ
  failRate[trainType] = calculateFailureRate(trainType);//����ʧ����

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

  //6.�ɳ���
  double growthRates[6] = { 1,1,1,1,1,1 };
  for (int j = 0; j < 5; j++)
    growthRates[j] = 1.0 + 0.01 * GameDatabase::AllUmas[umaId].fiveStatusBonus[j];

  //�²�����ֵ
  int totalValue[6];
  for (int j = 0; j < 6; j++)
  {
    int v = int(totalMultiplying * basicValue[j] * growthRates[j]);//����ȡ����
    if (v > 100)v = 100;
    totalValue[j] = v;
  }


    
  //7.��Ƭ
  for (int j = 0; j < 6; j++)
  {
    if (totalValue[j] > 0)//��������
      totalValue[j] += spiritBonus[j];
  }

    

    
  //8.Ů��ȼ��ӳ�
  double venusMultiplying = 1.00 + 0.01 * (
    GameConstants::VenusLevelTrainBonus[venusLevelRed]
    + GameConstants::VenusLevelTrainBonus[venusLevelBlue]
    + GameConstants::VenusLevelTrainBonus[venusLevelYellow]
    );

  for (int j = 0; j < 6; j++)
  {
    totalValue[j] = int(venusMultiplying * totalValue[j]);
  }

  //����
  int vitalChange=GameConstants::TrainingBasicValue[trainType][trainLv][6];
  for (int i = 0; i < cardNum; i++)
    vitalChange += effects[i].vitalBonus;
  if (vitalChange < 0)//��������ʱ������Ů��ȼ�
  {
    vitalChange = round(vitalChange*(1- 0.01*GameConstants::RedVenusLevelVitalCostDown[venusLevelRed]));
  }


  for (int j = 0; j < 6; j++)
  {
    trainValue[trainType][j] = totalValue[j];
  }
  trainValue[trainType][6] = vitalChange;
}

void Game::applyTraining(std::mt19937_64& rand, int chosenTrain, bool useVenusIfFull, int chosenSpiritColor, int chosenOutgoing)
{
  if (useVenusIfFull && venusAvailableWisdom != 0)
    activateVenusWisdom();//��checkEventAfterTrain()��ر�Ů�������Ƭ
  if (isRacing)
  {
    if (turn != TOTAL_TURN - 1)//����GrandMaster
      runRace(GameConstants::NormalRaceFiveStatusBonus, GameConstants::NormalRacePtBonus);
    int newSpirit = (rand() % 6 + 1) + (rand() % 3) * 8;//�����������Ƭ
    addSpirit(rand, newSpirit);
    addSpirit(rand, newSpirit);
    return;//GUR,WBC,SWBC,GrandMaster�ĳ�������checkEventAfterTrain()�ﴦ�������������
  }
  if (chosenTrain == 5)//��Ϣ
  {
    int r = rand() % 100;
    if (r < 25)
      vital += 70;
    else if (r < 82)
      vital += 50;
    else
      vital += 30;
    if (vital > maxVital)
      vital = maxVital;

    int spirit = spiritDistribution[chosenTrain];
    addSpirit(rand, spirit % 32);
    if(spirit>32)addSpirit(rand, spirit % 32);//������Ƭ
  }
  else if (chosenTrain == 7)//����
  {
    TODO
  }
}

int Game::finalScore(int chosenOutgoing) const
{
  int total = 0;
  for (int i = 0; i < 5; i++)
    total += GameConstants::FiveStatusFinalScore[min(fiveStatus[i],fiveStatusLimit[i])];
  
  double scorePtRate = isQieZhe ? GameConstants::ScorePtRateQieZhe : GameConstants::ScorePtRate;
  total += scorePtRate * skillPt;
  return total;
}

int Game::getTrainingLevel(int item) const
{
  int level = trainLevelCount[item] / 12;
  if (level > 4)level = 4;
  if (venusIsWisdomActive && venusAvailableWisdom == 1)//��Ů��
    level = 5;
  return level;
}

bool Game::isOutgoingLegal(int chosenOutgoing) const
{
  assert(chosenOutgoing >= 0 && chosenOutgoing <= 5);
  if (chosenOutgoing == 5)return true;//��ͨ���
  //ʣ�µ����������
  if (!venusCardUnlockOutgoing)return false;
  if (venusCardOutgoingUsed[chosenOutgoing])return false;
  if (chosenOutgoing == 0 || chosenOutgoing == 1 || chosenOutgoing == 2)return true;
  else if (chosenOutgoing == 3)
    return venusCardOutgoingUsed[0] && venusCardOutgoingUsed[1] && venusCardOutgoingUsed[2];
  else if (chosenOutgoing == 4)
    return venusCardOutgoingUsed[3];
  else return false;//δ֪



}
