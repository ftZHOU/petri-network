#include <iostream>
#include <petriTransition.hpp>
#include <petriNetwork.hpp>


bool petri::Transition::verbose = true;

petri::Transition::Transition(const std::string& name, petri::Network* owner)
  : name(name), inputs(), outputs(), owner(owner) {}


void petri::Transition::addInput(petri::poolIdf input_pool, unsigned int nb_token_required) {
  inputs.push_back({input_pool,nb_token_required});
}

void petri::Transition::addOutput(petri::poolIdf output_pool, unsigned int nb_token_produced) {
  outputs.push_back({output_pool,nb_token_produced});
}

petri::Transition::operator bool() const {

  for(auto elem : inputs)
    if(owner->pools[elem.first].nb_tokens < elem.second)
      return false;
  return true;
}

void petri::Transition::operator()() const {
  for(auto elem : inputs)  owner->pools[elem.first].nb_tokens -= elem.second;
  for(auto elem : outputs) owner->pools[elem.first].nb_tokens += elem.second;
  if(verbose)
    std::cout << name << std::endl;
}
