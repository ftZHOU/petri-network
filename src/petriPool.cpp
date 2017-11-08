#include "petriPool.hpp"

petri::Pool::Pool(const std::string& name,
		  unsigned int nb_tokens)
  : name(name), nb_tokens(nb_tokens) {}

