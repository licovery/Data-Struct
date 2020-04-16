#ifndef POKER_H
#define POKER_H

#include <vector>
#include <string>
#include <list>
#include <iostream>

using namespace std;

//struct Poker
struct Poker
{
    Poker(int value, int type, string belong = "UnKnow") : m_value(value), m_type(type), m_belong(belong)
    {
    }

    int m_value;     //牌面值
    int m_type;      //类型，单个或者对子
    string m_belong; //牌在谁手上

    static constexpr int SINGLE = 0;
    static constexpr int PAIR = 1;
};

bool operator==(const Poker &p1, const Poker &p2);
ostream &operator<<(ostream &out, const Poker &poker);

// struct Player
struct Player
{
    Player(const string &name, list<Poker> *game);
    void play(const Poker &p);     //出一张牌
    void addPoker(const Poker &p); //获得一张牌
    void undo();                   //把最近打出的一张牌拿回到手上
    bool isWin();
    vector<Poker> pokers();

    list<Poker> m_pokerInHand;
    string m_name;
    list<Poker> *m_game;
};
ostream &operator<<(ostream &out, const Player &player);

// strut PokerGame
struct PokerGame
{
    PokerGame(const string &computerName, const vector<Poker> &poker1,
              const string &playerName, const vector<Poker> &poker2);

    // 是否先手有必胜策略
    // curPlayer 当前出牌的人，
    // isLeader  上一次领先，当前是否可以随意出牌
    bool isFirstFinallyWin(int curPlayer, bool isLeader);
    bool isBetter(Poker p); // 是否比对手出的牌更大
    bool isGameOver();      //是否游戏结束

    Player m_first;     //先手     0
    Player m_second;    //后手     1
    list<Poker> m_game; //牌局

    static constexpr int FIRST = 0;
    static constexpr int SECOND = 1;
};

ostream &operator<<(ostream &out, const PokerGame &pokerGame);

#endif