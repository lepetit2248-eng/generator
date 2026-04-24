#ifndef GAMMAR_H_INCLUDED
#define GAMMAR_H_INCLUDED
#include <string>
#include <cstring>
#include <iostream>
#include <list>
#include <iterator>
#include <tuple>
#include "patients.h"
#include "json.hpp"
using json = nlohmann::json;
//Structures

std::tuple<std::string,std::string> Instantiate_events_in_grammar(json grammar, structure_patient patient);
std::tuple<std::string,std::string> A_I_Enter(json grammar, structure_patient patient);
std::tuple<std::string,std::string> B_I_Induction(json grammar, structure_patient patient);
std::tuple<std::string,std::string> C_I_Procedure(json grammar, structure_patient patient);
std::tuple<std::string,std::string> D_I_Exit(json grammar, structure_patient patient);

#endif // GAMMAR_H_INCLUDED
