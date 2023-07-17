#pragma once
#include <vector>
#include "NNInput.h"
#include "../Game/Game.h"


struct ModelOutputPolicyV1
{
  float trainingPolicy[5 + 3];
  float useVenusPolicy;
  float outgoingPolicy[6];
  float threeChoicesEventPolicy[3];
};
static_assert(sizeof(ModelOutputPolicyV1) == sizeof(float) * NNOUTPUT_CHANNELS_POLICY_V1, "NNOUTPUT_CHANNELS_POLICY_V1����");


struct ModelOutputValueV1
{
  float winrate;
  float avgScoreMinusTarget;
  float extract(int i);
};
static_assert(sizeof(ModelOutputValueV1) == sizeof(float) * NNOUTPUT_CHANNELS_VALUE_V1, "NNOUTPUT_CHANNELS_VALUE_V1����");

struct ModelOutputV1
{
  ModelOutputValueV1 value;
  ModelOutputPolicyV1 policy;
};
static_assert(sizeof(ModelOutputV1) == sizeof(float) * NNOUTPUT_CHANNELS_V1,"NNOUTPUT_CHANNELS_V1����");

class Model
{
  //Model* model;
  //static lock;//���е�evaluator����һ��lock
  //void evaluate(const Game* gamesBuf, const float* otherInputsBuf, int gameNum);//����gamesBuf��gameNum����Ϸ������������outputBuf

};