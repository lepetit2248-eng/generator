#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>    // std::sort
#include <vector>
#include <tuple>
#include <list>
#include <iterator>
#include "grammar.h"
#include "patients.h"
#include "function.h"
#include "json.hpp"
using json = nlohmann::json;

////////////////////////////////////////////////////////////////////////////////
// Enter
////////////////////////////////////////////////////////////////////////////////
std::tuple<std::string,std::string> Instantiate_events_in_grammar(json grammar, structure_patient patient){
  std::string event_sequence;
  std::string temp_event_sequence;
  std::string time_sequence;
  std::string temp_time_sequence;
  std::string event_id;
  std::vector<double> event_probability;
  json::iterator it;
  std::string::size_type sz;
  std::string code;
  std::string event_time_interval;
  //how many metaevents/event ?
  std::cout<<" - how many metaevents/event ? "<<grammar.size()<<std::endl;

  //for each metaevents/event
  for (it = grammar.begin(); it != grammar.end(); ++it) {
    std::cout<<"\t"<<it.key();
    //test type
    if(grammar[it.key()].type() == json::value_t::object){//is an object
      std::tie(temp_event_sequence, temp_time_sequence)=Instantiate_events_in_grammar(grammar[it.key()], patient);
      event_sequence+=temp_event_sequence;
      time_sequence+=temp_time_sequence;
    }else{//should be a tab
      if(grammar[it.key()].size()>0){//if there is event in the tab
        std::cout<<", there is "<<grammar[it.key()].size()<<" alternative event"<<std::endl;
        std::vector<double> event_probability;
        for (size_t i = 0; i < grammar[it.key()].size(); i++) { //create a vector containing each probability
          std::cout<<"\t\t("<<i<<") id : "<<grammar[it.key()][i]["id"]<<" Pb : "<<grammar[it.key()][i]["pb"]<<" Time : "<<grammar[it.key()][i]["time"]<<std::endl;
          code=grammar[it.key()][i]["pb"];
          event_probability.push_back(std::stod(code,&sz));
        }
        //check if the probability sum = 1
        double sum=0;
        for (size_t i = 0; i < event_probability.size(); i++){
          sum=sum+event_probability[i];
        }
        if(sum<1){//the progability sum is not egal to 1 and less to 1
          double null_pb=1-sum;
          std::cout<<"\t\t("<<event_probability.size()<<") id : NULL" <<" Pb : "<<null_pb<<std::endl;
          event_probability.push_back(null_pb);//so add a suplementary probability

        }else if(sum>1){//the progability sum is not egal to 1 and greater than 1
          std::cout<<"\t\t ERROR in grammar, Probability of events greater than 1"<<std::endl;
        }

        size_t choosed_value=choice(event_probability); // a revoir

        if(choosed_value<grammar[it.key()].size()){// IF the choosen value is not the added probability
          event_id=grammar[it.key()][choosed_value]["id"];
          event_time_interval=grammar[it.key()][choosed_value]["time"];
          time_sequence+=rand_time(event_time_interval);
          event_sequence+=event_id;
          
          if(event_id.substr(0,1)=="A"){
            if(event_id.substr(2,2)=="DG"){
              event_sequence+="-";
              event_sequence+=Drug_dose(event_id,patient);
            }else if(event_id.substr(2,2)=="GE"){

            }
          }
          event_sequence+="|";
          time_sequence+="|";
        }
      }else{
        std::cout<<", there is no event in this step"<<std::endl;
      }
    }
  }
  return std::make_tuple(event_sequence,time_sequence);
}

std::tuple<std::string,std::string>  A_I_Enter(json grammar, structure_patient patient){
  //variable declaration
  std::string event_sequence;
  std::string temp_event_sequence;
  std::string time_sequence;
  std::string temp_time_sequence;
  std::cout<<"A_I_enter"<<std::endl;
  std::cout<<"A_II_A_setting_up";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["A_I_enter"]["A_II_A_setting_up"], patient);
  event_sequence=temp_event_sequence;
  time_sequence=temp_time_sequence;
  std::cout<<"A_II_B_premedication_before_monitoring";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["A_I_enter"]["A_II_B_premedication_before_monitoring"], patient);
  event_sequence+=temp_event_sequence;
  time_sequence+=temp_time_sequence;
  std::cout<<"A_II_C_monitoring";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["A_I_enter"]["A_II_C_monitoring"], patient);
  event_sequence+=temp_event_sequence;
  time_sequence+=temp_time_sequence;
  std::cout<<"A_II_D_premedication_after_monitoring";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["A_I_enter"]["A_II_D_premedication_after_monitoring"], patient);
  event_sequence+=temp_event_sequence;
  time_sequence+=temp_time_sequence;
  std::cout<<"\n"<<std::endl;
  return std::make_tuple(event_sequence,time_sequence);
}

////////////////////////////////////////////////////////////////////////////////
// Induction
////////////////////////////////////////////////////////////////////////////////

std::tuple<std::string,std::string> B_I_Induction(json grammar, structure_patient patient){
  //variable declaration
  std::string event_sequence;
  std::string temp_event_sequence;
  std::string time_sequence;
  std::string temp_time_sequence;
  std::cout<<"B_I_induction"<<std::endl;
  std::cout<<"B_II_A_preoxygenation";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["B_I_induction"]["B_II_A_preoxygenation"], patient);
  event_sequence=temp_event_sequence;
  time_sequence=temp_time_sequence;
  std::cout<<"B_II_B_medication";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["B_I_induction"]["B_II_B_medication"], patient);
  event_sequence+=temp_event_sequence;
  time_sequence+=temp_time_sequence;
  std::cout<<"B_II_C_intubation";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["B_I_induction"]["B_II_C_intubation"], patient);
  event_sequence+=temp_event_sequence;
  time_sequence+=temp_time_sequence;
  std::cout<<"B_II_D_control_and_end_of_induction";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["B_I_induction"]["B_II_D_control_and_end_of_induction"], patient);
  event_sequence+=temp_event_sequence;
  time_sequence+=temp_time_sequence;
  std::cout<<"\n"<<std::endl;
  return std::make_tuple(event_sequence,time_sequence);
}

////////////////////////////////////////////////////////////////////////////////
// Procedure
////////////////////////////////////////////////////////////////////////////////


std::tuple<std::string,std::string> C_I_Procedure(json grammar, structure_patient patient){
  //variable declaration
  std::string event_sequence;
  std::string temp_event_sequence;
  std::string time_sequence;
  std::string temp_time_sequence;
  std::cout<<"C_I_procedure"<<std::endl;
  std::cout<<"C_II_A_preparation";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["C_I_procedure"]["C_II_A_preparation"], patient);
  event_sequence=temp_event_sequence;
  time_sequence=temp_time_sequence;
  std::cout<<"C_II_B_procedure";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["C_I_procedure"]["C_II_B_procedure"], patient);
  event_sequence+=temp_event_sequence;
  time_sequence+=temp_time_sequence;
  std::cout<<"\n"<<std::endl;
  return std::make_tuple(event_sequence,time_sequence);
}

////////////////////////////////////////////////////////////////////////////////
// Exit
////////////////////////////////////////////////////////////////////////////////

std::tuple<std::string,std::string> D_I_Exit(json grammar, structure_patient patient){
  //variable declaration
  std::string event_sequence;
  std::string temp_event_sequence;
  std::string time_sequence;
  std::string temp_time_sequence;
  std::cout<<"D_I_exit"<<std::endl;
  std::cout<<"D_II_A_decurarisation";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["D_I_exit"]["D_II_A_decurarisation"], patient);
  event_sequence=temp_event_sequence;
  time_sequence=temp_time_sequence;
  std::cout<<"D_II_B_ending";
  std::tie(temp_event_sequence, temp_time_sequence) =Instantiate_events_in_grammar(grammar["D_I_exit"]["D_II_B_ending"], patient);
  event_sequence+=temp_event_sequence;
  time_sequence+=temp_time_sequence;
  std::cout<<"\n"<<std::endl;
  event_sequence=event_sequence.substr(0,event_sequence.length()-1);
  return std::make_tuple(event_sequence,time_sequence);
}
