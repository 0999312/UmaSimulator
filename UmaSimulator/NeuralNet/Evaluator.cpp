#include <cassert>
#include "Evaluator.h"

void Evaluator::evaluate(const Game* games, const float* targetScores, int mode, int gameNum)
{
  assert(gameNum <= maxBatchsize);
  if (model == NULL)//û�����磬��д�߼�
  {
    if (mode == 0)//value�������վֲſɼ���
    {
      for (int i = 0; i < gameNum; i++)
      {
        const Game& game = games[i];
        assert(game.isEnd() && "��������ʱ��ֻ����Ϸ������ſɼ���value");
        int score = game.finalScore();
        if (score >= targetScores[i])
          valueResults[i].winrate = 1.0;
        else valueResults[i].winrate = 0.0;
        valueResults[i].avgScoreMinusTarget = score - targetScores[i];
      }
    }
    else if (mode == 1)//policy����д�߼������ŵ�ѡ����1����������0
    {
      for (int i = 0; i < gameNum; i++)
      {
        policyResults[i] = handWrittenPolicy(games[i]);
      }
    }
  }
  else
  {
    assert(false && "��ûд");
  }
}

Evaluator::Evaluator(Model* model, int maxBatchsize):model(model), maxBatchsize(maxBatchsize)
{
  inputBuf.resize(NNINPUT_CHANNELS_V1 * maxBatchsize);
  outputBuf.resize(NNOUTPUT_CHANNELS_V1 * maxBatchsize);
  valueResults.resize(maxBatchsize);
  policyResults.resize(maxBatchsize);
  
}

ModelOutputPolicyV1 Evaluator::handWrittenPolicy(const Game& game0)
{
  ModelOutputPolicyV1 policy=
  {
    {0,0,0,0,0,0,0,0},
    0,
    {0,0,0,0,0,0},
    {0,0,0}
  };
  if(game0.isEnd())
    return policy;

  int chosenTrain = 0;
  bool useVenus = false;
  int chosenOutgoing = 0;
  int chosenSpiritColor = 0;

  Game game = game0;//����һ����������Ϊ�п���Ҫ��Ů��

  if (game.isRacing)//�����غ�
  {
    useVenus = game.venusAvailableWisdom == 1;
  }
  else//����ѵ���غ�
  {
    if (game.venusAvailableWisdom == 1)
    {
      useVenus = true;
      game.activateVenusWisdom();
    }

    if (game.vital < 50)//��Ů������������û�о���Ϣ
    {
      if (game.venusCardUnlockOutgoing && !game.venusCardOutgoingUsed[4] && !game.isXiaHeSu())
      {
        chosenTrain = 6;
        chosenOutgoing =
          (!game.venusCardOutgoingUsed[2]) ? 2 :
          (!game.venusCardOutgoingUsed[0]) ? 0 :
          (!game.venusCardOutgoingUsed[1]) ? 1 :
          (!game.venusCardOutgoingUsed[3]) ? 3 :
          4;
      }
      else
        chosenTrain = 5;
    }
    else
    {
      if (game.venusAvailableWisdom != 0 && !game.venusIsWisdomActive)
      {
        useVenus = true;
        game.activateVenusWisdom();
      }
      double bestValue = -10000;
      int bestTrain = -1;
      for (int item = 0; item < 5; item++)
      {
        const double jibanValue = 2;
        const double venusValue_first = 40;
        const double venusValue_beforeOutgoing = 10;
        const double venusValue_afterOutgoing = 20;
        const double venusValue_activated = 6;
        const double spiritValue = 25;
        const double vitalValue = 0.9;
        const double statusWeights[6] = { 1.0,1.0,1.0,1.0,1.0,0.4/1.8*(game.isQieZhe?GameConstants::ScorePtRateQieZhe:GameConstants::ScorePtRate)};


        double expectSpiritNum = int(game.spiritDistribution[item] / 32) + 1;
        double value = 0;
        assert(game.cardId[0] == SHENTUAN_ID && "���ſ����ڵ�һ��λ��");
        for (int head = 0; head < 6; head++)
        {
          if (!game.cardDistribution[item][head])
            continue;
          if (head == 0)
          {
            if (!game.venusCardFirstClick)
              value += venusValue_first;
            else if (!game.venusCardUnlockOutgoing)
            {
              expectSpiritNum += 0.5;
              value += venusValue_beforeOutgoing;
            }
            else if (!game.venusCardIsQingRe)
            {
              expectSpiritNum += 0.5;
              value += venusValue_afterOutgoing;
            }
            else
            {
              expectSpiritNum += 1;
              value += venusValue_activated;
            }
          }
          else
          {
            if (game.cardJiBan[head] < 80)
            {
              int jibanAdd = 7;
              if (game.cardHint[head])
                jibanAdd += 5;
              value += jibanAdd * jibanValue;
            }
          }
          if (game.cardHint[head])
          {
            for (int i = 0; i < 6; i++)
              value += GameDatabase::AllSupportCards[game.cardId[head]].hintBonus[i] * statusWeights[i];
          }

        }
        //����
        if (game.venusAvailableWisdom == 2 && game.venusIsWisdomActive)
        {
          auto blueBonus = game.calculateBlueVenusBonus(item);
          for (int i = 0; i < 6; i++)
            value += blueBonus[i] * statusWeights[i];
        }


        if (game.venusSpiritsBottom[7] > 0)
          expectSpiritNum = 0;
        else if (game.venusSpiritsBottom[6] > 0)
          expectSpiritNum = std::min(1.0, expectSpiritNum);
        else if (game.venusSpiritsBottom[5] > 0)
          expectSpiritNum = std::min(2.0, expectSpiritNum);
        value += spiritValue * expectSpiritNum;

        for (int i = 0; i < 6; i++)
          value += game.trainValue[item][i] * statusWeights[i];

        value += vitalValue * game.trainValue[item][6];

        if (value > bestValue)
        {
          bestValue = value;
          bestTrain = item;
        }
      }
      chosenTrain = bestTrain;
    }
  }
  policy.trainingPolicy[chosenTrain] = 1.0;
  if (useVenus)policy.useVenusPolicy = 1.0;
  policy.threeChoicesEventPolicy[chosenSpiritColor] = 1.0;
  policy.outgoingPolicy[chosenOutgoing] = 1.0;
  return policy;
}
