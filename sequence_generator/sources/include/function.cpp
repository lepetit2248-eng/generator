#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <cstring>
#include <fstream>
#include "function.h"
#include "patients.h"
#include "json.hpp"
#include "bdd/serialization.cpp"
#include "bdd/Event.cpp"
#include "bdd/Action.cpp"
#include "bdd/Drug.cpp"
#include "bdd/Gesture.cpp"
#include "bdd/Descriptor.cpp"

#include "bdd/Impact.cpp"
#include "bdd/ImpactGest.cpp"
#include "bdd/ImpactDrug.cpp"



using json = nlohmann::json;

/**
 * \brief choose an event using each event propability
 * \param vector with each event probability
 * \return vector inder (int) of the choosen probability
 */

 double Drug_dose(std::string event_id,structure_patient patient){
   return 0.0;
 }
/**
 * \brief choose an event using each event propability
 * \param vector with each event probability
 * \return vector inder (int) of the choosen probability
 */
int choice(std::vector<double> event_probability){
  double choose_value;
  double buffer;
  int choice;

  //random initialisation
  std::random_device rd;
  std::default_random_engine generator(rd());
  //distribution initialisation
  std::uniform_real_distribution<double> distribution(0,1);

  //buffer initialisation
  buffer = 0.0;

  choose_value = distribution(generator);//we choose a value in the distribution
  std::cout << choose_value << std::endl;
  for (size_t j = 0; j < event_probability.size(); j++) {//for each probability
    if (choose_value >= buffer && choose_value < buffer + event_probability[j]){
      std::cout << "Proba " << j << " picked" << std::endl;
      choice=j;
    }
    buffer += event_probability[j];
  }
  return choice;
}

/**
 * \brief choose a time interval value between the min and max value given
 * \param event_time_interval is either a sting of the min and max value separated by a hyphen or an int
 * \return an int corresponding to the time interval
 */
std::string rand_time(std::string event_time_interval){
  int min;
  int max;
  int choose_value;
  std::size_t found = event_time_interval.find("-");

  if (found!=std::string::npos){
    min=int(stoi(event_time_interval.substr(0,found))/30);
    max=int(stoi(event_time_interval.substr(found+1,event_time_interval.length()-found))/30);
    // std::cout<< min <<" "<<max<<std::endl;


  }else{
    std::cout<< "Time used is :" << event_time_interval << std::endl;
    return event_time_interval;
  }

  //random initialisation
  std::random_device rd;
  std::default_random_engine generator(rd());
  //distribution initialisation
  std::uniform_real_distribution<double> distribution(min,max);

  choose_value = distribution(generator);//we choose a value in the distribution
 
  std::cout<< "Time used is : " << std::to_string(int(int(choose_value)*30)) << std::endl;
  return std::to_string(int(int(choose_value)*30));

}

/**
 * \brief
 * \param
 * \return
 */
structure_patient Read_patient_parameters(std::string file_patient_parameters_name){
  //variable instantiation
  structure_patient patient;
  std::string variable;
  std::ifstream file_patient_parameters;

  file_patient_parameters.open(file_patient_parameters_name); //Open file stream
  if(file_patient_parameters){ //opening is ok!
   
    // Initialisation du générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());

    while(getline(file_patient_parameters, variable)) {
      if(variable=="#age"){
        getline(file_patient_parameters, variable);
        size_t space_pos = variable.find(" ");
        if(space_pos != std::string::npos) {
            int min_val = std::stoi(variable.substr(0, space_pos));
            int max_val = std::stoi(variable.substr(space_pos + 1));
            std::uniform_int_distribution<> distrib(min_val, max_val);
            patient.age = distrib(gen);
        } else {
            patient.age = std::stoi(variable);
        }
        
      }else  if(variable=="#sex"){
        getline(file_patient_parameters, variable);
        size_t space_pos = variable.find(" ");
        if(space_pos != std::string::npos) {
            char c1 = variable[0];
            char c2 = variable[space_pos + 1];
            std::uniform_int_distribution<> distrib(0, 1);
            patient.sex = (distrib(gen) == 0) ? c1 : c2;
        } else {
            patient.sex = variable[0];
        }
        
      }else  if(variable=="#hight"){
        getline(file_patient_parameters, variable);
        size_t space_pos = variable.find(" ");
        if(space_pos != std::string::npos) {
            int min_val = std::stoi(variable.substr(0, space_pos));
            int max_val = std::stoi(variable.substr(space_pos + 1));
            std::uniform_int_distribution<> distrib(min_val, max_val);
            patient.hight = distrib(gen);
        } else {
            patient.hight = std::stoi(variable);
        }
        
      }else  if(variable=="#weight"){
        getline(file_patient_parameters, variable);
        size_t space_pos = variable.find(" ");
        if(space_pos != std::string::npos) {
            int min_val = std::stoi(variable.substr(0, space_pos));
            int max_val = std::stoi(variable.substr(space_pos + 1));
            std::uniform_int_distribution<> distrib(min_val, max_val);
            patient.weight = distrib(gen);
        } else {
            patient.weight = std::stoi(variable);
        }
      }
    } // Fin du while
  } else { // <-- L'accolade manquante était celle juste avant ce else !
    std::cout << "ERROR: File \"patient_parameters\" opening impossible." << std::endl;
  }
  
  file_patient_parameters.close();
  return patient;
}

/**
 * \brief
 * \param
 * \return
 */
void Print_patient(structure_patient patient){
  std::cout << "\n==================================="<< std::endl;
  std::cout << "Patient parameters "<< std::endl;
  std::cout << "==================================="<< std::endl;
  std::cout << "Age : "<<patient.age <<" yo"<< std::endl;
  std::cout << "Sex : "<<patient.sex << std::endl;
  std::cout << "Hight : "<<patient.hight <<" cm"<< std::endl;
  std::cout << "Weight : "<<patient.weight <<" kg"<< std::endl;
  std::cout << "===================================\n"<< std::endl;

}
/**
 * \brief
 * \param
 * \return
 */
json Read_grammar_json(std::string file_surgery_parameters_name){
  std::ifstream  file_surgery_parameters;
  json j;
  file_surgery_parameters.open(file_surgery_parameters_name); //Open file stream
  if(file_surgery_parameters){ //opening is ok!
    file_surgery_parameters >> j;
  }else{
      std::cout << "ERROR: File \"surgery_parameters.json\" opening impossible." << std::endl;
    }
  file_surgery_parameters.close();
  return j;
}
/**
 * \brief
 * \param
 * \return
 */
void Recursive_json_print(json grammar,size_t tab=0){
  json::iterator it;
  if(grammar.is_object()){
    for (it = grammar.begin(); it != grammar.end(); ++it) {
      for (size_t i = 0; i < tab; i++) {
          std::cout << "| \t";
      }
      std::cout << it.key()<<" : " <<grammar[it.key()].size()<<" events"<< std::endl;
      Recursive_json_print(grammar[it.key()],tab+1);
    }
  }else if(grammar.is_array()){
    for (it = grammar.begin(); it != grammar.end(); ++it) {
      for (size_t i = 0; i < tab; i++) {
          std::cout << "| \t";
      }
      std::cout<<" : " <<*it<< std::endl;
    }
  }
}
/**
 * \brief
 * \param
 * \return
 */
void Print_grammar_recap(json grammar){
  std::cout << "\n==================================="<< std::endl;
  std::cout << "Grammar structure "<< std::endl;
  std::cout << "==================================="<< std::endl;
  Recursive_json_print(grammar);
  std::cout << "===================================\n"<< std::endl;
//  std::cout << grammar.dump(2)<< std::endl;
}

/**
 * \brief
 * \param
 * \return
 */
void Export_event_sequence_text(std::string event_sequence,std::string time_sequence,std::string drug, std::string gesture, int step){
  std::ofstream file_trace;//flux pour la sortie de la trace
  file_trace.open("event_sequence.txt"); //Open file stream
  std::string type;
  std::string id;
  int time_value;
  std::string time_value_str;
  int total_time=0;
  std::string event_file_name;

  bool stop=false;
  std::ifstream  file;
  if(file_trace){ //opening is ok!
    file_trace<<"Time,ID,Name";
    while (stop==false){
      file_trace<<"\n";
      time_value_str = time_sequence.substr(0,time_sequence.find("|"));
      const char *cstr = time_value_str.c_str();
      time_value= strtol(cstr,NULL,10);
      total_time+=time_value;
      file_trace<<total_time;
      // file_trace<<total_time;
      file_trace<<',';
      //get first field of the sequence corresponding to the event id
      id=event_sequence.substr(0,event_sequence.find("|"));

      file_trace<<id;
      file_trace<<',';
      //get the type of the event in the id
      type=id.substr(0,id.find("-"));
      //get the file name in the rest of the id
      event_file_name=id.substr(id.find("-")+1);
      if(type.size()==3){
        if(type[0]=='A'){//action event
          std::cout<<"Event type action(A) ";

          if(type[1]=='D' && type[2]=='G' ){//drug event
            std::cout<<"subtype drug(DG), \n";
            //create filename
            event_file_name=drug+"/"+event_file_name+".bin";
            //check
            file.open(event_file_name); //Open file stream
            if(file){ //opening is ok!
              //create the drug object
              Drug bin =  Drug();
              //formating the file name
              char file_name[event_file_name.size() + 1];
              strcpy(file_name, event_file_name.c_str());	// or pass &s[0]
              //get the data from the file
              getFromFile<binary_iarchive, Drug>(bin, file_name);//deserialisation
              file_trace<<bin.get_name() ;


            }else{
                std::cout << "ERROR: File "<< event_file_name<<" opening impossible." << std::endl;

            }
            file.close();

          }else if(type[1]=='G'  && type[2]=='E' ){//gesture event
            std::cout<<"subtype gesture(GE), \n";
            //create filename
            event_file_name=gesture+"/"+event_file_name+".bin";
            //check
            file.open(event_file_name); //Open file stream
            if(file){ //opening is ok!
              //create the gesture object
              Gesture bin = Gesture();
              //formating the file name
              char file_name[event_file_name.size() + 1];
              strcpy(file_name, event_file_name.c_str());	// or pass &s[0]
              //get the data from the file
              getFromFile<binary_iarchive, Gesture>(bin, file_name);//deserialisation
              file_trace<<bin.get_name() ;


            }else{
              std::cout << "ERROR: File "<< event_file_name<<" opening impossible." << std::endl;

            }
          file.close();
          }else{
            std::cout << "ERROR: impossible to read event id " <<id<< std::endl;

          }
        }
      }else if(type.size()==1){
        if(type[0]=='D'){//descriptor event
          std::cout<<"Event type descriptor(d),\n ";
          //create filename
          event_file_name+="descriptors/"+event_file_name+".bin";
          //check
          file.open(event_file_name); //Open file stream
          if(file){ //opening is ok!
            //create the descriptor object
            Descriptor bin = Descriptor();
            //formating the file name
            char file_name[event_file_name.size() + 1];
            strcpy(file_name, event_file_name.c_str());	// or pass &s[0]
            //get the data from the file
            getFromFile<binary_iarchive, Descriptor>(bin, file_name);//deserialisation
            file_trace<<bin.get_name() ;

            std::cout<<"id : "<<id<<" added."<<std::endl;
          }else{
              std::cout << "ERROR: File "<< event_file_name<<" opening impossible." << std::endl;

          }
        file.close();
        }else{
          std::cout << "ERROR: impossible to read event id " << id<<std::endl;

        }
      }else{
        std::cout << "ERROR: impossible to read event id " <<id<< std::endl;

      }
      if(event_sequence.find("|")!=std::string::npos){
        event_sequence=event_sequence.substr(event_sequence.find("|")+1);//get the rest of the content
        time_sequence=time_sequence.substr(time_sequence.find("|")+1);//get the rest of the content
      }else{
        stop=true;
      }
    }//while end



    std::cout << "\nSequence exported in text file : \"event_sequence.txt\"." << std::endl;

  }else{
    std::cout << "ERROR: File \"trace.txt\" impossible to write." << std::endl;
  }
  file_trace.close();
}
/**
 * \brief This function export a given event sequence to a event vector on binary format
 * \param event sequence - string
 * \return
 */
void Export_event_sequence_bin(std::string event_sequence , std::string drug, std::string gesture){
  bool stop=false;
  int error=0;
  int event_nb=0;
  std::string event_file_name;
  std::string type;
  std::string id;

  vector<Event*> event;

  std::ifstream  file;

  //is the sequence is not empty ?
  if(event_sequence==""){//the sequence is empty
    std::cout << "ERROR: event sequence is empty, impossible to serialize." << std::endl;
  }else{//the sequence is not empty

    std::cout<<"\n======== Sequence serialisation ========"<<std::endl;

    while (stop==false){

      //add 1 to the event count
      event_nb++;

      std::cout<<event_nb <<" - ";

      //get first field of the sequence corresponding to the event id
      id=event_sequence.substr(0,event_sequence.find("|"));
      //get the type of the event in the id
      type=id.substr(0,id.find("-"));
      //get the file name in the rest of the id
      event_file_name=id.substr(id.find("-")+1);

      if(type.size()==3){
        if(type[0]=='A'){//action event
          std::cout<<"Event type action(A), ";

          if(type[1]=='D' && type[2]=='G' ){//drug event
            std::cout<<"subtype drug(DG), ";
            //create filename
            event_file_name=drug+"/"+event_file_name+".bin";
            //check
            file.open(event_file_name); //Open file stream
            if(file){ //opening is ok!
              //create the drug object
              Drug drug_load_bin =  Drug();
              //formating the file name
              char file_name[event_file_name.size() + 1];
              strcpy(file_name, event_file_name.c_str());	// or pass &s[0]
              //get the data from the file
              getFromFile<binary_iarchive, Drug>(drug_load_bin, file_name);//deserialisation
              //create event pointer
              Drug *drug_copy = new Drug (drug_load_bin);
              //add to the vector
              event.push_back(drug_copy);
              std::cout<<"id : "<<id<<" added."<<std::endl;

            }else{
                std::cout << "ERROR: File "<< event_file_name<<" opening impossible." << std::endl;
                error++;
            }
            file.close();

          }else if(type[1]=='G'  && type[2]=='E' ){//gesture event
            std::cout<<"subtype gesture(GE), ";
            //create filename
            event_file_name=gesture+"/"+event_file_name+".bin";
            //check
            file.open(event_file_name); //Open file stream
            if(file){ //opening is ok!
              //create the gesture object
              Gesture gesture_load_bin = Gesture();
              //formating the file name
              char file_name[event_file_name.size() + 1];
              strcpy(file_name, event_file_name.c_str());	// or pass &s[0]
              //get the data from the file
              getFromFile<binary_iarchive, Gesture>(gesture_load_bin, file_name);//deserialisation
              //create event pointer
              Gesture *gesture_copy = new Gesture(gesture_load_bin);
              //add to the vector
              event.push_back(gesture_copy);
              //std::cout << gesture_copy->toString();
              std::cout<<"id : "<<id<<" added."<<std::endl;
            }else{
              std::cout << "ERROR: File "<< event_file_name<<" opening impossible." << std::endl;
              error++;
            }
          file.close();
          }else{
            std::cout << "ERROR: impossible to read event id " <<id<< std::endl;
            error++;
          }
        }
      }else if(type.size()==1){
        if(type[0]=='D'){//descriptor event
          std::cout<<"Event type descriptor(d), ";
          //create filename
          event_file_name+="descriptors/"+event_file_name+".bin";
          //check
          file.open(event_file_name); //Open file stream
          if(file){ //opening is ok!
            //create the descriptor object
            Descriptor descriptor_load_bin = Descriptor();
            //formating the file name
            char file_name[event_file_name.size() + 1];
            strcpy(file_name, event_file_name.c_str());	// or pass &s[0]
            //get the data from the file
            getFromFile<binary_iarchive, Descriptor>(descriptor_load_bin, file_name);//deserialisation
            //create event pointer
            Descriptor *descriptor_copy = new Descriptor(descriptor_load_bin);
            //add to the vector
            event.push_back(descriptor_copy);

            std::cout<<"id : "<<id<<" added."<<std::endl;
          }else{
              std::cout << "ERROR: File "<< event_file_name<<" opening impossible." << std::endl;
              error++;
          }
        file.close();
        }else{
          std::cout << "ERROR: impossible to read event id " << id<<std::endl;
          error++;
        }
      }else{
        std::cout << "ERROR: impossible to read event id " <<id<< std::endl;
        error++;
      }

      if(event_sequence.find("|")!=std::string::npos){
        event_sequence=event_sequence.substr(event_sequence.find("|")+1);//get the rest of the content
      }else{
        stop=true;
      }
    }//while end

    //////
    // vector serialisation
    //////

    //there is any errors during the event additon to the vector ?
    if(error==0){ //no, there is no error during the event additon to the vector
      std::cout << "All the "<<event_nb<<" event has been added to the event vector." << std::endl;
      std::cout << "\nSequence can now be serialized." << std::endl;
      saveIntoFile<binary_oarchive, vector<Event*>> (event, "event_sequence.bin");
      std::cout << "Sequence exported in bin file : \"event_sequence.bin\"." << std::endl;
      for (size_t i = 0; i < event.size(); i++){
         delete event[i];
      }
      event.clear();

      getFromFile<binary_iarchive, vector<Event*>> (event, "event_sequence.bin");
      if(event.size()>0){
        std::cout<<"Sequence check ok !"<<std::endl;
        for (size_t i = 0; i < event.size(); i++){
           delete event[i];
        }
      }else{
        std::cout<<"Error in the sequence !"<<std::endl;
      }




    }else{ //yes, there is errors  during the event additon to the vector
      for (size_t i = 0; i < event.size(); i++){
         delete event[i];
      }
      std::cout << "\nERROR: event sequence have "<<error<<" errors on a "<<event_nb<<" events sequence" << std::endl;
      std::cout << "ERROR: Impossible to serialize." << std::endl;
    }
  }


}

/**
 * \brief This function export a given event sequence to a event vector on binary format
 * \param event sequence - string
 * \return
 */
void Check_action_path(std::string gesture){
std::cout << gesture << std::endl;
}

/**
 * \brief This function export a given event sequence to a event vector on binary format
 * \param event sequence - string
 * \return
 */
void Check_drug_path(std::string drug){
std::cout << drug << std::endl;
}
