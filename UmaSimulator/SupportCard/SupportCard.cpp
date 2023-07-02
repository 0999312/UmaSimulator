#include <iostream>
#include "SupportCard.h"
#include "../GameDatabase/GameConstants.h"
#include "../Game/Game.h"

using namespace std;

CardTrainingEffect SupportCard::getCardEffect(const Game& game, int atTrain, int jiBan) const
{
  CardTrainingEffect effect =
  { 
    youQingBasic,
    ganJingBasic,
    xunLianBasic,
    {bonusBasic[0],bonusBasic[1],bonusBasic[2],bonusBasic[3],bonusBasic[4],bonusBasic[5]},
    wizVitalBonusBasic
  };

  bool isShining = true;//�Ƿ�����
  if (cardType < 5)//���������ǿ�
  {
    if (jiBan < 80)isShining = false;
    if (cardType != atTrain)isShining = false;
  }
  else if (cardID == SHENTUAN_ID)//����
  {
    if (!game.venusCardIsQingRe)
      isShining = false;
  }
  else cout << "δ֪��";

  if (game.venusIsWisdomActive && game.venusAvailableWisdom == 3)//��Ů��
    isShining = true;

  if (!isShining)
  {
    effect.youQing = 0;
  }
  if (!isShining || atTrain != 4)
    effect.vitalBonus = 0;

  //�������Ǹ��ֹ���
  //1.����
  if (cardID == 1)
  {
    if (jiBan < 100)
    {
      if (isShining)
        effect.youQing = 20;
      effect.ganJing = 0;
      effect.bonus[5] = 0;
    }
  }
  //2.�߷�
  //Ϊ�˼򻯣���Ϊ��ʼѵ���ӳ���4%����һ��������20%��Ҳ���ǵ�n���غ�4+n*(2/3)%
  else if (cardID == 2)
  {
    if (game.turn < 24)
      effect.xunLian = 4 + 0.6666667 * game.turn;
  }
  //3.����
  else if (cardID == 3)
  {
    //ɶ��û��
  }
  //4.��������
  else if (cardID == 4)
  {
    //ɶ��û��
  }
  //5.������
  else if (cardID == 5)
  {
    //ɶ��û��
  }
  //6.ˮ˾��
  else if (cardID == 6)
  {
    int traininglevel = game.getTrainingLevel(atTrain);
    effect.xunLian = 5 + traininglevel * 5;
    if (effect.xunLian > 25)effect.xunLian = 25;
  }
  //7.����˹
  else if (cardID == 7)
  {
    if (jiBan < 80)
    {
      effect.bonus[2] = 0;
    }
  }
  //8.���ʵ�
  else if (cardID == 8)
  {
    if (jiBan < 80)
    {
      effect.bonus[0] = 0;
    }
  }
  //9.������
  else if (cardID == 9)
  {
    //ɶ��û��
  }
  //10.�ٱ�Ѩ
  else if (cardID == 10)
  {
    if (jiBan < 100)
    {
      effect.bonus[0] = 0;
    }
  }
  //11.�ͺ���
  else if (cardID == 11)
  {
    //ɶ��û��
  }
  //12.�Ǻø��
  else if (cardID == 12)
  {
    if (jiBan < 80)
    {
      effect.bonus[0] = 0;
    }
  }
  //13.���ƽ��
  else if (cardID == 13)
  {
    if (jiBan < 100)
    {
      effect.bonus[3] = 0;
    }
  }
  //
  else 
  {
    cout << "δ֪��";
  }

  return effect;
}
