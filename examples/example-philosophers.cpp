#include <ctime>
#include <cstdlib>
#include <vector>
#include <iterator>
#include <fstream>
#include <petri.hpp>


void         make_philosophers_problem(petri::Network& net, unsigned int nb_philosophers);
unsigned int count_until_deadlock(unsigned int nb_philosophers);
double       average_count_until_deadlock(unsigned int nb_philosophers, unsigned int nb_samples);

int main(int argc, char* argv[]) {
  // randomize the random number generation.
  std::srand(std::time(0));

  // Let us disable the display of the executions.
  petri::Transition::verbose = false;
  
  // Let us use this network to display the problem with 4
  // philosophers.
  
  petri::Network display;
  make_philosophers_problem(display,4);
  std::ofstream file;
  file.open("philosophers.dot");
  file << display;
  file.close();
  std::cout << "File \"philosophers.dot\" generated." << std::endl;
  
  std::cout << "A problem with 50 philosophers has a deadlock after "
	    << count_until_deadlock(50) << " steps." << std::endl;

  // Let us generate a plot.

  std::cout << "making plot..." << std::endl;
  std::ofstream gnuplot;
  gnuplot.open("graph.plot");

  gnuplot << "set term svg" << std::endl
	  << "set output 'plot.svg'" << std::endl
	  << "set xlabel 'nb philosophers'" << std::endl
	  << "set ylabel 'avg deadlock time'" << std::endl
	  << "plot '-' with lines notitle" << std::endl;
  unsigned int nb_tests = 500;
  for(unsigned int i = 10; i < nb_tests; ++i) {
    std::cout << i+1 << '/' << nb_tests << "     \r" << std::flush;
    gnuplot << i << ' ' << average_count_until_deadlock(i,20) << std::endl;
  }
  std::cout << std::endl;
									  
  
  gnuplot.close();
  
  std::cout << std::endl
	    << "Run" << std::endl
	    << "  gnuplot graph.plot" << std::endl
	    << "  eog plot.svg" << std::endl;
  return 0;
}

double average_count_until_deadlock(unsigned int nb_philosophers,
				    unsigned int nb_samples) {
  double sum = 0;
  
  for(unsigned int i = 0; i < nb_samples; ++i)
    sum += count_until_deadlock(nb_philosophers);

  return sum/nb_samples;
}

unsigned int count_until_deadlock(unsigned int nb_philosophers) {
  petri::Network net;
  unsigned int steps;
  
  make_philosophers_problem(net,nb_philosophers);
  for(steps = 0; net(); ++steps);

  return steps;
}


void make_philosophers_problem(petri::Network& net,
			       unsigned int nb_philosophers) {

  if(nb_philosophers < 2)
    return;
  
  // Let us create the forks.
  
  std::vector<petri::poolIdf> forks;
  auto out_fork = std::back_inserter(forks);
  for(unsigned int i = 0; i < nb_philosophers; ++i)
    *(out_fork++) = net.makePool(std::string("fork_") + std::to_string(i+1), 1);

  // Let us create each philosopher

  auto fork_iter = forks.begin();
  for(unsigned int i = 0; i < nb_philosophers; ++i) {
    
    petri::poolIdf first_fork = *(fork_iter++);
    petri::poolIdf other_fork = 0;
    std::string    name       = std::string("Ph_") + std::to_string(i+1);
    
    if(fork_iter != forks.end())
      other_fork = *fork_iter;
    else
      other_fork = *(forks.begin());

    petri::poolIdf thinking   = net.makePool(name + " thinking", 1);
    petri::poolIdf got_first  = net.makePool(name + " got first", 0);
    petri::poolIdf got_both   = net.makePool(name + " got both", 0);

    petri::tranIdf take_first = net.makeTransition(name + " take first" );
    petri::tranIdf take_other = net.makeTransition(name + " take other");
    petri::tranIdf eats       = net.makeTransition(name + " eats");

    net.linkIn  (take_first , first_fork, 1);
    net.linkIn  (take_first , thinking  , 1);
    net.linkOut (take_first , got_first , 1);

    net.linkIn  (take_other, other_fork, 1);
    net.linkIn  (take_other, got_first , 1);
    net.linkOut (take_other, got_both  , 1);

    net.linkIn  (eats      , got_both  , 1);
    net.linkOut (eats      , first_fork, 1);
    net.linkOut (eats      , other_fork, 1);
    net.linkOut (eats      , thinking  , 1);
  }
}
