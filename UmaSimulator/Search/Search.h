#pragma once
#include <vector>
#include "../Game/Game.h"
#include "../NeuralNet/Evaluator.h"
//һ����Ϸ��һ��search
class Search
{
public:
  std::vector<Game> gamesBuf;
  //2�����Ƿ�Ů��1Ϊ����0Ϊ��������û������
  //8������ѵ��+��Ϣ�������
  //4����Ů����ѡһ�¼�������״̬���������죬������ ��ע���������൱�ڻ���ˡ�ֻҪ��Ů��϶�������buff��������������ѵ��ʧ�ܣ�ѵ��ʧ���޷�������ѡһ�¼���
  //6����6�������ǰ5����Ů����������һ������ͨ���
  //�����غϣ��ڶ����±���0
  ModelOutputValueV1 allChoicesValue[2][8 + 4 + 6];

  //����ÿ�����ܵ���������2*(8+4+6)�֣���ÿ������ģ��eachSamplingNum�֣�ģ��maxDepth�غϺ󷵻����������������������ǴﵽtargetScore�ĸ��ʣ�
  void runSearch(const Game& game, Evaluator* evaluators, int eachSamplingNum, int maxDepth, int targetScore, int threadNum);

  //���㵥��ѡ�����ֵ
  ModelOutputValueV1 evaluateSingleAction(
    const Game& game, Evaluator* evaluators, int eachSamplingNum, int maxDepth, int targetScore,
    

    std::mt19937_64& rand,
    int chosenTrain,
    bool useVenus,
    int chosenSpiritColor,
    int chosenOutgoing,
    int threadNum,
    int forceThreeChoicesEvent);

  //�����������ѡ�����ѡ��policyҲ��һ������
  //mode=0�Ǹ���ʤ�ʣ�=1�Ǹ���ƽ����
  ModelOutputPolicyV1 extractPolicyFromSearchResults(int mode, float delta = 0);

  static void runOneTurnUsingPolicy(std::mt19937_64& rand, Game& game, const ModelOutputPolicyV1& policy, bool distributeCards);

};