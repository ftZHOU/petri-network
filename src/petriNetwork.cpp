#include <iostream>
#include <petriNetwork.hpp>
#include <iterator>
#include <cstdlib> // old style rand.

// This could be done with better C++ random generators.
unsigned int my_rand_uint(unsigned int max) {
  return (unsigned int)(max*(std::rand()/(1.0+RAND_MAX)));
}

bool petri::Network::operator()() {
  std::vector<petri::Transition*> activables;
  auto out = std::back_inserter(activables);

  for(auto& trans : transitions)
    if(trans)
      *(out++) = &trans;

  if(activables.size() > 0) {
    unsigned int rank         = my_rand_uint((unsigned int)(activables.size()));
    petri::Transition& picked =  *(activables[rank]);
    picked(); // execution;
    return true;
  }

  return false;
}

petri::poolIdf petri::Network::makePool(const std::string& pool_name, unsigned int nb_tokens) {
  pools.push_back({pool_name,nb_tokens});
  return (petri::poolIdf)(pools.size()-1);
}
    
petri::tranIdf petri::Network::makeTransition(const std::string& transition_name) {
  transitions.push_back({transition_name, this});
  return (petri::tranIdf)(transitions.size()-1);
  
}

void petri::Network::linkIn(tranIdf      transition,
			    poolIdf      pool,
			    unsigned int nb_tokens_in) {
  transitions[transition].addInput(pool, nb_tokens_in);
}

void petri::Network::linkOut(tranIdf      transition,
			     poolIdf      pool,
			     unsigned int nb_tokens_out) {
  transitions[transition].addOutput(pool, nb_tokens_out);
}


std::ostream& petri::operator<<(std::ostream& os, const petri::Network& net) {
  os << "digraph Petri {" << std::endl
     << std::endl
     << "  overlap=scale" << std::endl
     << std::endl
     << "  /* Pools */" << std::endl
     << std::endl;

  petri::poolIdf pool_idf = 0;
  for(auto& pool : net.pools)
    os << "  pool_" << (pool_idf++)
       << " [label=\"" << pool.name << " #(" << pool.nb_tokens
       << ")\", color=blue, style=bold];"<< std::endl;

  os << std::endl
     << "  /* Transitions */" << std::endl
     << std::endl;

  petri::tranIdf tran_idf;

  tran_idf = 0;
  for(auto& trans : net.transitions)
    os << "  transition_" << (tran_idf++)
       << " [label=\"" << trans.name
       << "\", shape=box, color=red, style=bold];" << std::endl;

  os << std::endl
     << "  /* Edges */" << std::endl
     << std::endl;

  tran_idf = 0;
  for(auto& trans : net.transitions) {
    for(auto elem : trans.inputs)
      os << "  pool_" << elem.first << " -> transition_" << tran_idf
	 << " [label=\"" << elem.second << "\"];" << std::endl;
    for(auto elem : trans.outputs)
      os << "  transition_" << tran_idf  << " -> pool_" << elem.first
	 << " [label=\"" << elem.second << "\"];" << std::endl;
    os << std::endl;
    ++tran_idf;
  }
  
  os << '}' << std::endl;
  
  return os;
}
