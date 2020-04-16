
#include "poker.h"

ostream &operator<<(ostream &out, const Poker &poker)
{
    if (poker.m_type == Poker::SINGLE)
    {
        out << "[single ";
    }
    else if (poker.m_type == Poker::PAIR)
    {
        out << "[pair ";
    }
    out << poker.m_value << "]";
    return out;
}

bool operator==(const Poker &p1, const Poker &p2)
{
    return p1.m_value == p2.m_value &&
           p2.m_type == p2.m_type &&
           p1.m_belong == p2.m_belong;
}

Player::Player(const string &name, list<Poker> *game) : m_name(name), m_game(game)
{
}

void Player::play(const Poker &p) //出一张牌
{
    m_game->push_back(p);
    m_pokerInHand.remove(p);
}

void Player::addPoker(const Poker &p) //获得一张牌
{
    m_pokerInHand.emplace_back(p.m_value, p.m_type, m_name);
}

void Player::undo() //把最近打出的一张牌拿回到手上
{

    m_pokerInHand.push_back(m_game->back());
    m_game->pop_back();
}

bool Player::isWin()
{
    return m_pokerInHand.size() == 0;
}

vector<Poker> Player::pokers()
{
    return vector<Poker>(m_pokerInHand.begin(), m_pokerInHand.end());
}

ostream &operator<<(ostream &out, const Player &player)
{
    out << player.m_name << ": ";
    for (auto poker : player.m_pokerInHand)
    {
        out << poker << " ";
    }
    return out;
}

PokerGame::PokerGame(const string &computerName, const vector<Poker> &poker1,
                     const string &playerName, const vector<Poker> &poker2) : m_first(computerName, &m_game), m_second(playerName, &m_game)
{
    for (auto poker : poker1)
    {
        m_first.addPoker(poker);
    }

    for (auto poker : poker2)
    {
        m_second.addPoker(poker);
    }
}

// curPlayer 当前出牌的人，
// isLeader  上一次领先，当前是否可以随意出牌
bool PokerGame::isFirstFinallyWin(int curPlayer, bool isLeader)
{
    if (isGameOver())
    {
        if (m_first.isWin())
        {
            cout << "===win===" << endl
                 << *this;
            return true;
        }
        else
        {
            cout << "===lose===" << endl
                 << *this;
            return false;
        }
    }

    if (curPlayer == FIRST) //先手方出牌
    {
        int nextPlayer = SECOND; //换人出牌

        bool hasChange = false; // 是否有机会出牌
        vector<Poker> pokersInHand = m_first.pokers();
        //遍历手上的牌
        for (int i = 0; i < pokersInHand.size(); i++)
        {
            //选择出牌
            if (isLeader || isBetter(pokersInHand[i]))
            {
                hasChange = true;
                m_first.play(pokersInHand[i]);
                if (isFirstFinallyWin(nextPlayer, false)) //先手方出牌，只有有一个状态是必胜，则必胜
                {
                    m_first.undo();
                    return true;
                }
                m_first.undo();
            }
        }
        //没有机会出牌
        if (!hasChange)
        {
            if (isFirstFinallyWin(nextPlayer, true))
            {
                return true;
            }
        }
        // 当前状态下不存在必胜节点
        return false;
    }
    else //后手方出牌
    {
        int nextPlayer = FIRST; //换人出牌

        bool hasChange = false; // 是否有机会出牌
        vector<Poker> pokersInHand = m_second.pokers();
        for (int i = 0; i < pokersInHand.size(); i++)
        {
            if (isLeader || isBetter(pokersInHand[i]))
            {
                hasChange = true;
                m_second.play(pokersInHand[i]);
                if (!isFirstFinallyWin(nextPlayer, false)) //如果有一个节点，先手方不胜，那后手方就会选择这个节点，使得先手不胜
                {
                    m_second.undo();
                    return false;
                }
                m_second.undo();
            }
        }
        //没有机会出牌
        if (!hasChange)
        {
            if (!isFirstFinallyWin(nextPlayer, true))
            {
                return false;
            }
        }
        // 后手的任何选择都不会导致先手不胜，那么先手必胜
        return true;
    }
}

bool PokerGame::isBetter(Poker p) // 是否比对手出的牌更大
{
    if (m_game.empty())
    {
        return true;
    }

    auto lastPoker = m_game.back();

    if (p.m_type == lastPoker.m_type &&
        p.m_value > lastPoker.m_value)
    {
        return true;
    }
    return false;
}

bool PokerGame::isGameOver()
{
    return m_first.isWin() || m_second.isWin();
}

ostream &operator<<(ostream &out, const PokerGame &pokerGame)
{
    out << pokerGame.m_first << endl;
    out << pokerGame.m_second << endl;
    out << "Game: ";
    for (auto poker : pokerGame.m_game)
    {
        out << poker.m_belong << poker << " ";
    }
    out << endl;
    return out;
}

int main()
{
    vector<Poker> pokers{
        Poker(3, Poker::SINGLE),
        Poker(4, Poker::SINGLE),
        Poker(5, Poker::SINGLE),
        Poker(6, Poker::PAIR),
        Poker(7, Poker::PAIR),
        Poker(8, Poker::PAIR)};

    PokerGame pokerGame("AI", pokers, "Neo", pokers);
    cout << pokerGame.isFirstFinallyWin(PokerGame::FIRST, true) << endl;
    return 0;
}