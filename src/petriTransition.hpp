#pragma once

#include <utility>
#include <vector>
#include <string>

#include <petriPool.hpp>

namespace petri {

  using tranIdf = unsigned int;

  class Network; // permet de parler de pointeurs (rien de plus)
  
  class Transition {
  public:
    std::string name;
    std::vector<std::pair<poolIdf,unsigned int>> inputs;
    std::vector<std::pair<poolIdf,unsigned int>> outputs;
    Network* owner;

    static bool verbose;
    
    Transition(const std::string& name, Network* owner);
    Transition()                             = default;
    Transition(const Transition&)            = default;
    Transition& operator=(const Transition&) = default;
    
    void addInput (poolIdf  input_pool, unsigned int nb_token_required);
    void addOutput(poolIdf output_pool, unsigned int nb_token_produced);

    operator bool()   const;
    void operator()() const;
  };
  
}
