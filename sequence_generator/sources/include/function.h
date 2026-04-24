#ifndef FUNCTION_H_INCLUDED
#define FUNCTION_H_INCLUDED
#include <iostream>
#include <string>
#include <cstring>
#include "patients.h"
#include "json.hpp"
using json = nlohmann::json;
//Structures

//function
double Drug_dose(std::string event_id,structure_patient patient);
int choice(std::vector<double> event_probability);
std::string rand_time(std::string event_time_interval);
int uniforme_random_choice(int valeur_maximale);
structure_patient Read_patient_parameters(std::string file_patient_parameters_name);
json Read_grammar_json(std::string file_surgery_parameters_name);
void Print_patient(structure_patient patient);
void Recursive_json_print(json grammar, size_t tab);
void Print_grammar_recap(json grammar);
void Export_event_sequence_text(std::string event_sequence, std::string time_sequence,std::string drug, std::string gesture,int step);
void Export_event_sequence_bin(std::string event_sequence, std::string drug, std::string gesture);
void Check_action_path(std::string gesture);
void Check_drug_path(std::string drug);






#endif // FUNCTION_H_INCLUDED
