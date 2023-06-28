
#include <iostream>
using namespace std;
void main_test1()
{
  //��Ŀ��ÿ�غ����ȡ5���򣬺ڰ׸�����ȣ���������һ����ɫ����
  //һ��100���غ�
  //��������300����������100��Ŀ�����������
  const int MaxC1 = 1000, MaxC2 = 1000, MaxTurn = 300;
  const double probs[6] = { 
    1.0 / 32, 
    5.0 / 32, 
    10.0 / 32,
    10.0 / 32,
    5.0 / 32,
    1.0 / 32 };

  auto f = new double[MaxTurn+1][MaxC1+1][MaxC2+1]; //f��ʣ��turnʱ����ǰ�������ֱ��������c1,c2ʱ�����յ÷־������޵�����

  //turn=0ʱ
  for (int c1 = 0; c1 <= MaxC1; c1++)
    for (int c2 = 0; c2 <= MaxC2; c2++)
    {
      f[0][c1][c2] = c1 + c2;
    }

  //turn>0ʱ,���ƺ���
  for (int turn = 1; turn <= MaxTurn; turn++)
    for (int c1 = 0; c1 <= MaxC1; c1++)
      for (int c2 = 0; c2 <= MaxC2; c2++)
      {
        double avg = 0;

        for (int n = 0; n <= 5; n++)//n������
        {
          int c1new = max(0, c1 - n);
          int c2new = max(0, c2 - (5 - n));
          double bestChoice = min(f[turn - 1][c1new][c2], f[turn - 1][c1][c2new]);
          avg += bestChoice * probs[n];
        }

        f[turn][c1][c2] = avg;
      }


  //����turn=100ʱ��ȡ3��������2������ٽ���
  int turn = 300;
  for (int c2 = 0; c2 < MaxC2; c2++)
  {
    for (int c1 = 0; c1 < MaxC1; c1++)
    {
      int c1new = max(0, c1 - 3);
      int c2new = max(0, c2 - (5 - 3));
      if (f[turn - 1][c1new][c2] < f[turn - 1][c1][c2new])
      {
        cout << c2 << " " << c1 << endl;
        break;
      }
    }
  }

  //����c2=500��turn=100ʱ����ͬc1�£�ȡ3������2�����ƽ���ֵĲ���
  
  {
    int turn = 300;
    int c2 = 1000;
    for (int c1 = 0; c1 < MaxC1; c1++)
    {
      int c1new = max(0, c1 - 3);
      int c2new = max(0, c2 - (5 - 3));
      cout << c1 << " " << f[turn - 1][c1new][c2] - f[turn - 1][c1][c2new] << endl;
    }
  }

  for (int c = 0; c < MaxC1; c++)
  {
    cout << c << " " << f[turn][c][c] << endl;
  }

}