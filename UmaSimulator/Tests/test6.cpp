#include <iostream>
#include <iomanip> 
#include <sstream>
#include <fstream>
#include <cassert>
#include <thread>  // for std::this_thread::sleep_for
#include <chrono>  // for std::chrono::seconds
#include "../Game/Game.h"
#include "../Search/Search.h"
using namespace std;


void main_test6()
{
  const int threadNum = 16;
  const int searchN = 8192;
  Search search;
  vector<Evaluator> evaluators;
  for (int i = 0; i < threadNum; i++)
    evaluators.push_back(Evaluator(NULL, 128));
  int lastTurn = -1;
  while (true)
  {
    ifstream fs("./packets/currentGS.json");
    ostringstream tmp;
    tmp << fs.rdbuf();
    fs.close();

    string jsonStr = tmp.str();
    Game game;
    bool suc = game.loadGameFromJson(jsonStr);
    if (!suc)
    {
      cout << "error" << endl;
      continue;
    }
    if (game.turn == lastTurn)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(300));//����Ƿ��и���
      continue;
    }
    lastTurn = game.turn;
    if (game.venusIsWisdomActive)
      continue;
    game.print();
    cout << endl;
    cout << "������..." << endl;
    cout << endl;
    search.runSearch(game, evaluators.data(), searchN, TOTAL_TURN, 27000, threadNum);
    {
      auto policy = search.extractPolicyFromSearchResults(1);

      cout << "ʹ��Ů���ʣ�";
      cout << fixed << setprecision(1) << policy.useVenusPolicy * 100 << "% ";
      cout << endl;

      cout << "���������ǣ�";
      for (int i = 0; i < 5; i++)
        cout << fixed << setprecision(1) << policy.trainingPolicy[i] * 100 << "% ";
      cout << endl;

      cout << "��Ϣ�������������";
      for (int i = 0; i < 3; i++)
        cout << fixed << setprecision(1) << policy.trainingPolicy[5 + i] * 100 << "% ";
      cout << endl;

      cout << "�죬�����ƣ�";
      for (int i = 0; i < 3; i++)
        cout << fixed << setprecision(1) << policy.threeChoicesEventPolicy[i] * 100 << "% ";
      cout << endl;

      cout << "���Ů������Լ���ͨ�����";
      for (int i = 0; i < 6; i++)
        cout << fixed << setprecision(1) << policy.outgoingPolicy[i] * 100 << "% ";
      cout << endl;
    }

    cout << endl;
    for (int i = 0; i < 2; i++)
    {
      for (int j = 0; j < 8 + 4 + 6; j++)
      {
        cout
          //<< fixed << setprecision(1) << search.allChoicesValue[i][j].winrate * 100 << "%:" 
          << fixed << setprecision(0) << search.allChoicesValue[i][j].avgScoreMinusTarget << " ";
        if (j == 4 || j == 7 || j == 11)cout << endl;
      }
      cout << endl;
      cout << endl;
    }

  }

}