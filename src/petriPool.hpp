#pragma once

#include <string>

namespace petri {
  
  using poolIdf = unsigned int;
  
  class Pool {
  public:
    std::string name;
    unsigned int nb_tokens;
    Pool(const std::string& name, unsigned int nb_tokens);
    Pool()                       = default;
    Pool(const Pool&)            = default;
    Pool& operator=(const Pool&) = default;
  };
  
}
