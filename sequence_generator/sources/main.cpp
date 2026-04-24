//includeslibrary
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <tuple>
//Project function
#include "include/grammar.h"
#include "include/function.h"
#include "include/patients.h"
#include "include/json.hpp"
using namespace std;
using json = nlohmann::json;

int main (int argc, char **argv){
  if(argc==5){
    ///////////////////////////////////////////////////////////////////////
    //Variables declaration
    ///////////////////////////////////////////////////////////////////////

    //structure
    structure_patient patient;
    //string
    string event_sequence;
    string temp_event_sequence;
    string time_sequence;
    string temp_time_sequence;
    string drug;
    string gesture;

    //json
    json grammar;

    ///////////////////////////////////////////////////////////////////////
    //Parameters readings
    ///////////////////////////////////////////////////////////////////////

    //file surgery_parameters
    grammar=Read_grammar_json(argv[1]);
    //Print_grammar_recap(grammar);

    //file patient parameters
    patient=Read_patient_parameters(argv[2]);
    Print_patient(patient);

    //path to drug event database
    drug=argv[3];
    Check_drug_path(drug);


    //path to action event database
    gesture=argv[4];
    Check_action_path(gesture);



    ///////////////////////////////////////////////////////////////////////
    //Sequence generation
    ///////////////////////////////////////////////////////////////////////
    std::cout << "\n==================================="<< std::endl;
    std::cout << "Sequence generation "<< std::endl;
    std::cout << "==================================="<< std::endl;
    // 1st Step : Enter in operation room - A_I_enter
    std::tie(temp_event_sequence, temp_time_sequence) = A_I_Enter(grammar, patient);
    event_sequence=temp_event_sequence;
    time_sequence=temp_time_sequence;

    // 2nd Step : Induction
    std::tie(temp_event_sequence, temp_time_sequence)=B_I_Induction(grammar, patient);
    event_sequence+=temp_event_sequence;
    time_sequence+=temp_time_sequence;

    // 3rd Step : Procedure
    std::tie(temp_event_sequence, temp_time_sequence)=C_I_Procedure(grammar, patient);
    event_sequence+=temp_event_sequence;
    time_sequence+=temp_time_sequence;

    // 4th Step : Exit
    std::tie(temp_event_sequence, temp_time_sequence)=D_I_Exit(grammar, patient);
    event_sequence+=temp_event_sequence;
    time_sequence+=temp_time_sequence;

    ///////////////////////////////////////////////////////////////////////
    // Trace export
    ///////////////////////////////////////////////////////////////////////
    std::cout << "\n==================================="<< std::endl;
    std::cout << "Export "<< std::endl;
    std::cout << "==================================="<< std::endl;
    cout<<"\nSequence : " <<event_sequence<<"\n"<<endl;

    Export_event_sequence_text(event_sequence,time_sequence,drug,gesture,30);

    Export_event_sequence_bin(event_sequence,drug,gesture);

    return 1;
  }else{
    cout<<"Need 4 arguments : (1) path to the surgery grammar file (json format), (2) the path to the patient's parameters file, (3) the path to the drug event database and (4) the path to the gesture event database"<<endl;
    return 0;
  }
}
