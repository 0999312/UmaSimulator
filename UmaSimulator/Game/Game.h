#pragma once
#include <random>
#include <array>
#include "../GameDatabase/GameDatabase.h"

struct Game
{
  //����״̬����������ǰ�غϵ�ѵ����Ϣ
  int umaId;//�����ţ���KnownUmas.cpp
  int turn;//�غ�������0��ʼ����77����
  int vital;//������������vital������Ϊ��Ϸ��������е�
  int maxVital;//��������
  bool isQieZhe;//����
  bool isAiJiao;//����
  int fiveValue[5];//��ά���ԣ�1200���ϲ�����
  //int fiveValueUmaBonus[5];//��������ӳ�
  int fiveValueLimit[5];//��ά�������ޣ�1200���ϲ�����
  int skillPt;//���ܵ�
  int motivation;//�ɾ�����1��5�ֱ��Ǿ����������õ�
  int cardId[6];//6�ſ���id
  int cardJiBan[8];//����ſ��ֱ�012345�����³�6������7
  int trainLevelCount[5];//���ѵ���ĵȼ��ļ�����ʵ��ѵ���ȼ�=min(5,t/12+1)
  int zhongMaBlueCount[5];//����������Ӹ���������ֻ��3��
  int isRacing;//����غ��Ƿ��ڱ���
  //bool raceTurns[TOTAL_TURN];//��Щ�غ��Ǳ��� //��umaId�������GameDatabase::AllUmas����

  //Ů�����
  int venusLevelYellow;//Ů��ȼ�
  int venusLevelRed;
  int venusLevelBlue;

  int venusSpiritsCount;//�ײ��м�����Ƭ
  int venusSpiritsBottom[8];//�ײ���Ƭ��8*��ɫ+���ԡ���ɫ012��Ӧ�����ƣ�����123456��Ӧ����������pt
  int venusSpiritsUpper[4 + 2];//��˳��ֱ��ǵڶ���͵��������Ƭ�������ײ���Ƭһ�¡�*2����*3�ֳ���
  int venusAvailableWisdom;//�����Ů����ǣ�123�ֱ��ǻƺ�����0��û��
  bool venusIsWisdomActive;//�Ƿ�����ʹ�����

  //���ſ�ר��
  bool venusCardFirstClick;// �Ƿ��Ѿ���������ſ�
  bool venusCardUnlockOutgoing;// �Ƿ�������
  bool venusCardIsQingRe;// ����zone
  bool venusCardOutgoingUsed[5];// �ù���Щ���У������Ǻ�������������

  //��ǰ�غϵ�ѵ����Ϣ
  //0֧Ԯ����δ���䣬1֧Ԯ��������ϻ������ʼǰ��2ѵ������������������0�����̶ֹ��¼�������¼���������һ���غ�
  //stageInTurn=0ʱ�������������������ֵ��stageInTurn=1ʱ�����������������policy
  int stageInTurn;
  bool cardDistribution[5][8];//֧Ԯ���ֲ������ſ��ֱ�012345�����³�6������7
  bool cardHint[6];//���ſ��ֱ���û�������
  int spiritDistribution[5 + 3];//��Ƭ�ֲ�����������ѵ������Ϣ�������������
  int trainValue[5][7];//��һ�����ǵڼ���ѵ�����ڶ���������������������pt����
  int failRate[5];//ѵ��ʧ����

  //��Ϸ����
  void newGame(std::mt19937_64& rand, int newUmaId,int newCards[6],int newZhongMaBlueCount[5]);//������Ϸ�����֡�umaId��������
  void randomDistributeCards(std::mt19937_64& rand);//������俨��
  void calculateTrainingValue();//����ÿ��ѵ���Ӷ���

  //����ѵ����ı仯�����У�chosenTrain����ѡ���ѵ����01234�ֱ������������ǣ�5����Ϣ��6�������7�Ǳ�����useVenusIfFull�Ǽ���Ů���������Ƿ���Ů��chosenSpiritColor�Ǽ������Ů����ѡһ�¼���ѡ�����Ƭ��ɫ��chosenOutgoing����������ѡ��������Ŀ�������������ֱ���01234����ͨ�����5��
  //ע����ͨ�غ���14�ֿ��ܣ�5��ѵ��������һ��ѵ�����ܻ����Ů����ѡһ��������������Ϣ��������5�ֳ��У��������غ�ֻ�п�����Ů������ѡ��
  void applyTraining(std::mt19937_64& rand, int chosenTrain, bool useVenusIfFull, int chosenSpiritColor, int chosenOutgoing);
  void checkEventAfterTrain(std::mt19937_64& rand);//���̶��¼�������¼�����������һ���غ�
  int finalScore(int chosenOutgoing) const;//�����ܷ�

  //��������
  void addSpirit(int s);//�����Ƭ
  int getTrainingLevel(int item) const;//����ѵ���ȼ�����0��ʼ����Ϸ���k����������k-1������Ů����5��
  std::array<int, 6> venusSpiritsBonus() const;//������Ƭ�ӳ�
  bool isOutgoingLegal(int chosenOutgoing) const;//�������Ƿ��ǿ��Խ��е�
};

