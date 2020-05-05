#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class Move {
 public:
 // Take sc coins from heap sh and put tc coins to heap th.
 Move(int sh, int sc, int th, int tc);

 int getSource() const;
 int getSourceCoins() const;
 int getTarget() const;
 int getTargetCoins() const;

 friend ostream & operator << (ostream &out, const Move &move);

 private:
 int source_heap;
 int source_coins;
 int target_heap;
 int target_coins;
 };

 Move::Move(int sh, int sc, int th, int tc): source_heap(sh), source_coins(sc), target_heap(th), target_coins(tc) {}

 int Move::getSource() const {
    return source_heap;
 }

 int Move::getSourceCoins() const {
   return source_coins;
 }

 int Move::getTarget() const {
   return target_heap;
 }

 int Move::getTargetCoins() const {
   return target_coins;
 }

 ostream& operator<<(ostream &out, const Move &move){
   
   if(move.getTargetCoins() != 0)
     out << "takes" << move.getSourceCoins() << "coins from heap" << move.getSource() << "and puts" << move.getTargetCoins() << "coins to heap" << move.getTarget();
   else
     out << "takes" << move.getSourceCoins() << "coins from heap" << move.getSource() << "and puts nothing";
   return out;
 }

 class State {
 public:
 // State with h heaps, where the i-th heap starts with c[i] coins.
 State(int h, const int c[]);
 ~State();
 
 void next(const Move &move) throw(logic_error);
 bool winning() const;
 
 int getHeaps() const;
 int getCoins(int h) const throw(logic_error);

 friend ostream & operator << (ostream &out, const State &state);
 
 private:
 int numberOfHeaps;
 int *coins;
 };

 State::State(int h, const int c[]): numberOfHeaps(h){
   coins = new int[h];
   for(int j = 0; j < h; j++)
     coins[j] = c[j];
 }

 State::~State(){
   delete [] coins;
 }

int State::getHeaps() const {
  return numberOfHeaps;}


 int State::getCoins(int h) const throw(logic_error){
  if(h >= 0 && h < getHeaps())
    return coins[h];
  else 
    throw logic_error("Heaps: starting from 1, reaching 'heaps' ");   
}

 bool State::winning() const{
  int u = 0;
  for(int i = 0; i < getHeaps(); i++)
    u += getCoins(i);

  if(u > 0)
    return false;
  else
    return true;      
};

 void State::next(const Move &move) throw(logic_error){
  if((move.getSource() > getHeaps()) || (move.getSource() < 0) || (move.getTarget() < 0))
   throw logic_error("invalid heap"); 
  else if((move.getSourceCoins() > getCoins(move.getSource())) || (move.getSourceCoins() < 1) || (move.getTargetCoins() < 0) || (move.getTargetCoins() =< move.getTarget()))
   throw logic_error("Hmmm...Coin problem!");
  else {
    coins[move.getSource()] -= move.getSourceCoins();
    coins[move.getTarget()] += move.getTargetCoins();
  } 
};

 ostream& operator<<(ostream &out, const State &state){
  for(int j = 0; j < state.getHeaps(); j++)
  if(j + 1 != state.getHeaps())
     out << state.getCoins(j);
  else 
     out << state.getCoins(j) << ", ";
  return out;
};

class Player {
 public:
 Player(const string &n);
 virtual ~Player();

 virtual const string & getType() const = 0;
 virtual Move play(const State &s) = 0;

 friend ostream & operator << (ostream &out, const Player &player);

 protected:
 string player_Name;
 };

 Player::Player(const string &n): player_Name(n)
 {};

 Player::~Player(){
   player_Name.clear();
 };

 ostream& operator<<(ostream &out, const Player &player){
   out << player.getType() + " player " + player.player_Name;
   return out;
 };

class GreedyPlayer: public Player{
  public:
   GreedyPlayer(const string &name): Player(name){
     player_Type = "Greedy";
     };

   virtual ~GreedyPlayer() override{
     player_Name.clear();
     player_Type.clear();
   };

   virtual const string& getType() const override{
     return player_Type;
   };

   virtual Move play(const State &s) override{
     int x = s.getCoins(0);
     int u = 0;
     for(int i = 1; i < s.getHeaps(); i++){
       if(x < s.getCoins(i)){
        x = s.getCoins(i);
        u = i;}
     }
     Move greedy_play(u, x, 0, 0);
     return greedy_play;
   };
   private:
   string player_Type;
}; 

class SpartanPlayer: public Player{
  public:
   SpartanPlayer(const string &name): Player(name){
     player_Type = "Spartan";
     };

   virtual ~SpartanPlayer() override{
     player_Name.clear();
     player_Type.clear();
   };

   virtual const string& getType() const override{
     return player_Type;
   };

   virtual Move play(const State &s) override{
     int x = s.getCoins(0);
     int u = 0;
     for(int i = 1; i < s.getHeaps(); i++){
       if(x < s.getCoins(i)){
        x = s.getCoins(i);
        u = i;}
     }
   
     Move spartan_play(u, 1, 0, 0);
     return spartan_play;
   };
   private:
   string player_Type;
}; 

class SneakyPlayer: public Player{
  public:
   SneakyPlayer(const string &name): Player(name){
     player_Type = "Sneaky";
     };

   virtual ~SneakyPlayer() override{
     player_Name.clear();
     player_Type.clear();
   };

   virtual const string& getType() const override{
     return player_Type;
   };

   virtual Move play(const State &s) override{
     int j = 0;
     while(s.getCoins(j) == 0)
      j++;

     int x = s.getCoins(j);
     int u = 0;
     
     for(int i = 1; i < s.getHeaps(); i++){
       if((x > s.getCoins(i)) && (s.getCoins(i) != 0)){
        x = s.getCoins(i);
        u = i;}
     }
     Move sneaky_play(u, x, 0, 0);
     return sneaky_play;
   };
   private:
   string player_Type;
};

class RighteousPlayer: public Player{
  public:
   RighteousPlayer(const string &name): Player(name){
     player_Type = "Greedy";
     };

   virtual ~RighteousPlayer() override{
     player_Name.clear();
     player_Type.clear();
   };

   virtual const string& getType() const override{
     return player_Type;
   };

   virtual Move play(const State &s) override{
     int SC = s.getCoins(0);
     int SH = 0, TC = SC, TH = 0;
     for(int i = 1; i < s.getHeaps(); i++){
       if(SC > s.getCoins(i)){
        SC = s.getCoins(i);
        SH = i;}
       if(TC < s.getCoins(i)){
         TC = s.getCoins(i);
         TH = i;
       } 
     }
     Move righteous_play(SH, SC/2, TH, (SC/2 - 1));
     return righteous_play;
   };
   private:
   string player_Type;
}; 


