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
};


const UmaData GameDatabase::AllUmas[ALL_UMA_NUM] =
{
  //0����
  {
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
};
