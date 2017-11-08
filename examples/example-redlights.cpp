#include <ctime>
#include <cstdlib>
#include <fstream>
#include <petri.hpp>

int main(int argc, char* argv[]) {
  // randomize the random number generation.
  std::srand(std::time(0));
  
  
  petri::Network redlights;

  // Let us create the 7 pools
  
  petri::poolIdf red_1            = redlights.makePool("1 red",      1);
  petri::poolIdf red_2            = redlights.makePool("2 red",      1);
  petri::poolIdf orange_1         = redlights.makePool("1 orange",   0);
  petri::poolIdf orange_2         = redlights.makePool("2 orange",   0);
  petri::poolIdf green_1          = redlights.makePool("1 green",    0);
  petri::poolIdf green_2          = redlights.makePool("2 green",    0);
  petri::poolIdf all_red          = redlights.makePool("All is red", 1);

  // Then, we create the 6 transitions

  petri::tranIdf turns_green_1    = redlights.makeTransition("1 turns green" );
  petri::tranIdf turns_green_2    = redlights.makeTransition("2 turns green" );
  petri::tranIdf turns_orange_1   = redlights.makeTransition("1 turns orange");
  petri::tranIdf turns_orange_2   = redlights.makeTransition("2 turns orange");
  petri::tranIdf turns_red_1      = redlights.makeTransition("1 turns red"   );
  petri::tranIdf turns_red_2      = redlights.makeTransition("2 turns red"   );

  // Now, we connect each transition to input and output pools.

  redlights.linkIn (turns_green_1 , red_1    , 1);
  redlights.linkIn (turns_green_1 , all_red  , 1);
  redlights.linkOut(turns_green_1 , green_1  , 1);

  redlights.linkIn (turns_orange_1, green_1  , 1);
  redlights.linkOut(turns_orange_1, orange_1 , 1);

  redlights.linkIn (turns_red_1   , orange_1 , 1);
  redlights.linkOut(turns_red_1   , red_1    , 1);
  redlights.linkOut(turns_red_1   , all_red  , 1);

  redlights.linkIn (turns_green_2 , red_2    , 1);
  redlights.linkIn (turns_green_2 , all_red  , 1);
  redlights.linkOut(turns_green_2 , green_2  , 1);

  redlights.linkIn (turns_orange_2, green_2  , 1);
  redlights.linkOut(turns_orange_2, orange_2 , 1);

  redlights.linkIn (turns_red_2   , orange_2 , 1);
  redlights.linkOut(turns_red_2   , red_2    , 1);
  redlights.linkOut(turns_red_2   , all_red  , 1);
  
  std::ofstream file;
  file.open("redlights.dot");
  file << redlights;
  file.close();
	
  for(int steps = 0; steps < 100 && redlights(); ++steps);

  std::cout << "File \"redlights.dot\" generated." << std::endl;
  return 0;
}
