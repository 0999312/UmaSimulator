#include "GameDatabase.h"
#include "UmaData.h"

// 0 ��
// 1 ���Ϸɣ����ǣ�
// 2 
// 3
// 4
//


const std::string GameDatabase::AllUmaNames[ALL_UMA_NUM] = {
  "��",
  "���Ϸ�(5��)",
  "���Ϸ�(4��)",
  "����(5��)",
  "op��(5��)",
  "С������(4��)",
};


const std::map<int, int> GameDatabase::AllUmaGameIdToSimulatorId =
{
  {0,0},
  {5101101,1},
  {4101101,2},
  {5102402,3},
  {5102401,4},
  {4109801,5},
};
const UmaData GameDatabase::AllUmas[ALL_UMA_NUM] =
{
    //0����
    {
      5,
      {
        false,false,false,false,false,false, false,false,false,false,false,false, false,false,false,false,false,false, false,false,false,false,false,false, //��һ��
        false,false,false,false,false,false, false,false,false,false,false,false, false,false,false,false,false,false, false,false,false,false,false,false, //�ڶ���
        false,false,false,false,false,false, false,false,false,false,false,false, false,false,false,false,false,false, false,false,false,false,false,false, //������
        false,false,false,false,false,true
      },
      {
        6,6,6,6,6
      },
      {
        110,110,110,110,110
      },

    },
    //1�����Ϸ�
    {
      5,
      {
        false,false,false,false,false,false, false,false,false,false,false,true, false,false,false,false,false,false, false,false,false,false,true,false, //��һ��
        false,false,false,false,false,false, false,false,false,true,false,false, false,false,false,false,false,false, false,false,false,true,false,true, //�ڶ���
        false,false,false,false,false,false, false,false,false,false,false,true, false,false,false,false,false,false, true,false,false,false,false,true, //������
        false,false,false,false,false,true
      },
      {
        20,0,10,0,0
      },
      {
        118,91,129,96,116
      },
    },
    //2�����Ϸɣ�4��
    {
      4,
      {
        false,false,false,false,false,false, false,false,false,false,false,true, false,false,false,false,false,false, false,false,false,false,true,false, //��һ��
        false,false,false,false,false,false, false,false,false,true,false,false, false,false,false,false,false,false, false,false,false,true,false,true, //�ڶ���
        false,false,false,false,false,false, false,false,false,false,false,true, false,false,false,false,false,false, true,false,false,false,false,true, //������
        false,false,false,false,false,true
      },
      {
        20,0,10,0,0
      },
      {
        107,83,117,87,106
      },
    },

    //3������
    {
      5,
      {
        false,false,false,false,false,false, false,false,false,false,false,true, false,false,false,false,false,false, false,false,false,false,false,false, //��һ��
        false,false,false,false,false,false, false,false,false,false,false,false, false,false,false,false,false,false, false,true,false,false,false,true, //�ڶ���
        false,false,false,false,false,true, false,true,false,false,false,true, false,false,false,false,false,false, false,true,false,false,false,true, //������
        false,false,false,false,false,true
      },
      {
        10,10,0,0,10
      },
      {
        102,123,100,110,115
      },
    },
    //4��op��
    {
      5,
      {
        false,false,false,false,false,false, false,false,false,false,false,true, false,false,false,false,false,false, false,false,false,false,false,false, //��һ��
        false,false,false,false,false,false, false,false,false,false,false,false, false,false,false,false,false,false, false,true,false,false,false,true, //�ڶ���
        false,false,false,false,false,true, false,true,false,false,false,true, false,false,false,false,false,false, false,true,false,false,false,true, //������
        false,false,false,false,false,true
      },
      {
        0,20,0,10,0
      },
      {
        94,130,86,123,117
      },
    },
    //5,С����
    {
      4,
      {
        false,false,false,false,false,false, false,false,false,false,false,true, false,false,false,false,false,false, false,false,false,false,false,true, //��һ��
        false,false,false,false,false,false, true,false,false,false,false,false, false,false,false,false,true,false, false,false,false,true,false,true, //�ڶ���
        false,false,false,true,false,false, false,false,true,false,false,true, false,false,false,false,true,false, true,false,true,false,false,true, //������
        false,false,false,false,false,true
      },
      {
        0,0,10,0,20
      },
      {
        104,97,103,96,100
      },
    },
};
