#pragma once
#include <vector>
#include "../Game/Game.h"
#include "../NeuralNet/Evaluator.h"
//һ����Ϸ��һ��search
class Search
{
  Game rootGame;
  std::vector<Game> gamesBuf;
  //2�����Ƿ�Ů��1Ϊ����0Ϊ��������û������
  //8������ѵ��+��Ϣ�������
  //4����Ů����ѡһ�¼�������״̬���������죬������ ��ע���������൱�ڻ���ˡ�ֻҪ��Ů��϶�������buff��������������ѵ��ʧ�ܣ�ѵ��ʧ���޷�������ѡһ�¼���
  //6����6�������ǰ5����Ů����������һ������ͨ���
  double allChoicesValue[2][8 + 4 + 6];

  //����ÿ�����ܵ���������2*(8+4+6)�֣���ÿ������ģ��eachSamplingNum�֣�ģ��maxDepth�غϺ󷵻����������������������ǴﵽtargetScore�ĸ��ʣ�
  void runSearch(Evaluator* evaluator, int eachSamplingNum, int maxDepth, int targetScore);
};