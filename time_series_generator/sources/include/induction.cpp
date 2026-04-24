//===============================================================================================================================
// Name        : 
// Author      : Fatoumata Dama
// Version     : 1.0
// Copyright   :
// Description : 
//=================================================================================================================================

#include <fstream>
#include "serialization.cpp"
#include "utils.cpp"

#include "Impact.cpp"
#include "ImpactDrug.cpp"
#include "ImpactGest.cpp"

#include "Event.cpp"
#include "Action.cpp"
#include "Drug.cpp"
#include "Gesture.cpp"
#include "Descriptor.cpp"


int main(){

  //------------------------fix random generator seed
  srand(time(0));
  // Set horodatage
  int tmp[] = {0,30,60,90,120,150,390,450,540,720,750,780,810,1020,1080,1110,1140,1170,1200,1320,1590,1620,1680,1710,1830,1860,1890,2190,2520,2760,3600,4860,4890,5190,5790,5820,6180};
  vector<int> surgery_times (tmp, tmp + sizeof(tmp) / sizeof(int) );

  //-----------------------load and display event sequence and corresponding surgery times
  vector<Event *> sequence;
  getFromFile<binary_iarchive, vector<Event*>> (sequence, "../../sequence_generator/event_sequence.bin");

  cout << "-----------------------------------------------------------------" << endl;
  cout << "Sequence with " << to_string(sequence.size()) << " events and "
       << to_string(surgery_times.size())  << " surgery_times" << endl;
  cout << "-----------------------------------------------------------------" << endl;

  //---------------------induction
  //from enter_in_room to support_point_check, 20 events
  vector<Event *> induction;
  vector<int> ind_surgery_times;
  for (size_t i=0; i < 20; i++){
    induction.push_back(sequence[i]);
    ind_surgery_times.push_back(surgery_times[i]);
    
    /*cout << "time = " << to_string(surgery_times[i]/60.) << " mn" << endl;
    cout << induction[i]->toString();
    cout << "-------------------------------------------------------------------------" << endl;
    */
  }

  //--------------------take patient characteristics into account
  gesture_setting(induction);
  drug_setting(induction);
  
  cout << "-------------------------------------------------------------" << endl;
  for (size_t i=0; i < induction.size(); i++){
    cout << "time = " << to_string(surgery_times[i]) << " s" << endl;
    cout << induction[i]->toString();
    cout << "-------------------------------------------------------------------------" << endl;
    }


  //------------------------------------SIMULATION------------------------------------------------
  string parameter;
  float base_value;
  float begin_value;
  string file_name;
  
  //--------------------simulation FC
  parameter = "FC";
  base_value = 85;
  begin_value = 85;
  file_name = "../simulated_data/hernie_inguinale/induction/" + parameter + ".txt";
  
  cout << endl << "LOG-INFO " << parameter << " simulation begins ..." << endl;
  simulation(parameter, base_value, begin_value, induction, ind_surgery_times, file_name);

  //--------------------simulation PAM
  parameter = "PAM";
  base_value = 105;
  begin_value = 105;
  file_name = "../simulated_data/hernie_inguinale/induction/" + parameter + ".txt";

  cout << endl << "LOG-INFO " << parameter << " simulation begins ..." << endl;
  simulation(parameter, base_value, begin_value, induction, ind_surgery_times, file_name);
  
  //--------------------deletion
  for (Event *e: sequence){ delete e; }

}

