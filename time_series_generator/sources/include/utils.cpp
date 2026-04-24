//===============================================================================================================================
// Name        : Parameter time series generator within "entretien" phase
// Author      : Fatoumata Dama
// Version     : 1.0
// Copyright   :
// Description : 
//=================================================================================================================================

#include <assert.h>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <ctype.h>

#include "Event.h"
#include "Impact.h"


//======================================UNIFORM RANDOM SAMPLING===================================

/*return a pseudo-random float between min and max*/
float my_random_float(float min, float max)
{

  if (min > max){
    string error_sms = "ERROR: class Impact: in function random_float(float min, float max) min should be strictly less than max! \n";
    cerr << error_sms ;
    throw error_sms;
  }

  //value between 0 and 1
  float between_0_1 = ((float)rand())/(float)RAND_MAX;

  float rand_f = min + between_0_1*(max-min);

  return rand_f ;
}

/*return a pseudo-random int between min and max*/
int my_random_int(int min, int max)
{

  if (min > max){
    string error_sms = "ERROR: class Impact: in function random_int(int min, int max) min should be strictly less than max! \n";
    cerr << error_sms ;
    throw error_sms;
  }

  return rand()%(max-min+1) + min;
}


//=====================================TIMESTAMP SETTING==========================================

/**
 # @brief Returns an assossiative dictionnary whose keys are event ID and values are the associated
 #  vector of occurrence indice 
 */
map<string, vector<int>> indice_of_events(vector<Event*> event_sequence)
{
  
  int nb_events = event_sequence.size();

  //output initialization
  map<string, vector<int>> indice;
  for(size_t i=0; i < nb_events; i++){
    indice[ event_sequence[i]->get_id() ] = vector<int>();
  }
  
  //build output
  for(size_t i=0; i < nb_events; i++){
    indice[ event_sequence[i]->get_id() ].push_back(i);
  }

  return indice;
}


/*
*/
bool conformity_tests(vector<int> surgery_times)
{
  bool conformity = true;
  int nb_events = surgery_times.size();
  
  conformity = conformity && (surgery_times[0] == 0);
  
  for (size_t i=1; i < nb_events; ++i){
    conformity = conformity && (surgery_times[i] > 0) && (surgery_times[i-1] <= surgery_times[i]);
  }

  return conformity;
}


/**
 # @brief Load reference timestamps from the given file.
 #  ref_timetamps_file is comma csv file with column Time,ID,Name (event occurrence times, ID and name)
 #
 */
vector<int> load_reference_timestamp(string ref_timestamp_file)
{
  //output
  vector<int> ref_timestamps;

  //open stream to file
  ifstream infile(ref_timestamp_file);
  if(!infile.is_open()){
    cerr << "Unable to open file: " << ref_timestamp_file << endl << endl;
    exit(1);
  } 

  //read file line by line
  string line, field;
  getline(infile, line);  //headers are skipped
  
  while(getline(infile, line)){
    stringstream line_2_stream(line);
    getline(line_2_stream, field, ',');   //timetamps are the first field

    try { 
      ref_timestamps.push_back(stoi(field));
    } catch (...) {
      cerr << "Reference timestamp loading fails, please check file '" << ref_timestamp_file << "' format!\n" << endl;
      exit(1);
    }
    
  }

  //assertion
  assert(conformity_tests(ref_timestamps));
  
  return ref_timestamps;
}


/**
 # @brief Set the ocurrence time of the ind^th event at the given time.
 #  If time is greater than the curr_timestamps[ind] then performs a
 #  right-shift from ind+1. Otherwise performs a left-shift from ind-1,
 #  then all occurrence times are right-shifted in oder to get only positive values
 #
 # @param curr_timestamp Vector of occurrence times
 # @param time Occurrence time of the ind^th event, expressed in second
 */
void set_ith_timestamp(vector<int> &curr_timestamps, int ind, int time)
{

  int nb_events = curr_timestamps.size();
  assert(ind >= 0 && ind < nb_events);
    
  int diff = time - curr_timestamps[ind];
  if(diff != 0){
    //---set ocurrence time
    curr_timestamps[ind] = time;
    //---right-shift from index ind+1
    if(diff > 0){
      for (size_t i=ind+1; i < nb_events; ++i){
	curr_timestamps[i] = curr_timestamps[i] + diff;
      }
    //---left-shift from index ind-1
    }else{
      for (size_t i=0; i < ind; ++i){
	curr_timestamps[i] = curr_timestamps[i] + diff;
      }
      //right-shift from index 0
      int lag = abs(curr_timestamps[0]);
      for (size_t i=0; i < nb_events; ++i){
	curr_timestamps[i] = curr_timestamps[i] + lag;
      }
    }
  }

}


/**
 # @brief This function set the timestamp of "unilateral hernie inguinal" surgery.
 # event_sequence is the vector of events occurring during surgery.
 # ref_timestamp_file is the file containing the reference timestamp, that is the occurrence 
 # time of each event within event_sequence.
 # 
 # The output timestamp is generated by performing slight disturbances of the reference timestamp.
 # 
 # SPECIAL CASES
 # - sevoflurance injections in continu: 1% 1 mn after intubation then 1-3 min before incision
 # the concentration is increased to 2% 
 #
 # CONDITIONS: the output timestamp must check the following conditions
 # - The occurrence order of events must be preserved. 
 #   That is surgery_times[0] < surgery_times[1] < ... < surgery_times[nb_events-1]
 # - surgery_times[i] >= 0
 # 
 #
 # OLD VERSION: REFERENCE HORODATAGE WAS SET WITHIN CODE
 # The sequence of events is the following: 
 #   AGE-insta01|AGE-mofca01|AGE-mopam01|AGE-mospo01|AGE-mobis01|AGE-povvp01|AGE-baihu01|ADG-cefab01|
 #   AGE-preox01|ADG-sufei01|ADG-propi01|ADG-atrai01|AGE-occoc01|AGE-intub01|ADG-sevoc01|AGE-venco01|
 #   AGE-oxyge01|AGE-auspu01|AGE-supeb01|AGE-marea01|AGE-motem01|AGE-auspu01|AGE-supoa01|ADG-sevoc01|
 #   AGE-insci01|AGE-troca01|AGE-inpne01|AGE-auspu01|AGE-marea01|AGE-disep01|AGE-discs01|AGE-diszh01|
 #   AGE-mppro01|AGE-depne01|AGE-ferme01|AGE-panse01|AGE-cdecu01|AGE-decur01|AGE-insta01
 # There are 39 events at all, they are sorted from the most recent to the least recent. 
 # The reference horodatage provided by expert is:
 #   {0,30,60,90,120,150,390,450,540,720,750,780,810,1020,1080,1110,1140,1170,1200,1230,1320,1590,
 #    1620,1680,1830,1860,1920,1950,1980,2280,2610,2850,3690,4950,4980,5280,5880,5910,6270}
 #
 */
vector<int> check_timestamp(vector<Event *> event_sequence, vector<int> ref_timestamps)
{
  //output initialization
  vector<int> surgery_times(ref_timestamps);  //copy ref_timestamp by value
  assert(event_sequence.size() == surgery_times.size());

  //number of events
  int nb_events = event_sequence.size();

  //indice of events 
  map<string, vector<int>> indice = indice_of_events(event_sequence);   

  //--------conformity tests
  bool conformity = conformity_tests(surgery_times);

  string surgery_times_to_str = to_string(surgery_times[0]);
  for (size_t i=1; i < nb_events; ++i){
    surgery_times_to_str = surgery_times_to_str + ", " + to_string(surgery_times[i]);
  }
  
  if(conformity){
    cout << "Unique surgery timestamp for current patient = " << endl
	 << surgery_times_to_str << endl << endl ;
  }else{
    string error_sms = "\nERROR: main function: generated timestamp is not conform!\n";
    error_sms = error_sms + "Timestamp equals: " + surgery_times_to_str + "\n";
    cerr << error_sms ;
    throw error_sms;
  } 

   
  return surgery_times;
}


//=========================================PATIENT PARAMETERS=========================================
/**
 * 
 */
vector<string> load_parameters_to_be_simulated(string parameter_file){

  //output initialization
  vector<string> parameters;

  //patient parameters reading
  ifstream infile(parameter_file);  
  if(!infile.is_open()) {
    cerr << "Unable to open file: " << parameter_file << endl << endl;
    exit(1);
  }

  //local variables
  string line;
  string param;
  size_t tag_pos;
  size_t underscore_pos;
  
  while(getline(infile, line)){
    //current parameter
    tag_pos = line.find_first_of("#");
    underscore_pos = line.find_first_of("_");
    if((tag_pos != string::npos) && (underscore_pos != string::npos)){
      param = line.substr(tag_pos+1, underscore_pos-tag_pos-1);

      //if param is not in vector parameters, add it at the end
      if(find(parameters.begin(), parameters.end(), param) == parameters.end()){
	parameters.push_back(param);
      }
    }
  }

  return parameters;
}


/**
 * 
 */
void read_patient_parameters(string parameter_file, vector<string> parameters,
			     vector<float> &base_values, vector<float> &begin_values){
    
  //patient parameters reading
  ifstream myfile_stream (parameter_file);  

  if(!myfile_stream.is_open()) {
    cerr << "Unable to open file: " << parameter_file << endl << endl;
    exit(1);
  }

  //local variables
  int nb_parameter = parameters.size();
  size_t i;
  string tag;
  char tmp[256]; 
  float ba_val_inf = -1;
  float ba_val_sup = -1;
  float be_val_inf = -1;
  float be_val_sup = -1;

  for (size_t i=0; i < nb_parameter; ++i){
  
    //----base values
    //--tag
    do{
      myfile_stream.getline(tmp, 256);
      tag = string(tmp); 
    }while(tag.find("#"+parameters[i]+"_base_values") == string::npos);

    
    //--inf value, sup value (try block is for function stof)
    try{
      myfile_stream.getline(tmp, 256, ' '); 
      ba_val_inf = stof(string(tmp));  //white spaces in the begging of the number are ignored
      myfile_stream.getline(tmp, 256);
      ba_val_sup = stof(string(tmp));
    }catch (...) {
      cerr << "Patient parameter reading fails, please check file '" << parameter_file << "' format!\n" << endl;
      exit(1);
    }
    //--base value is randomly chosen within [ba_val_inf, ba_val_sup]
    if(parameters[i] != "temp"){
      base_values[i] = my_random_int(int(ba_val_inf), int(ba_val_sup));
    }else{
      base_values[i] = my_random_float(ba_val_inf, ba_val_sup);
    }

    //----begin values
    //--tag
    myfile_stream.getline(tmp, 256);
    tag = string(tmp);
    if(tag.find("#"+parameters[i]+"_begin_values") == string::npos){
      cerr << endl << "Error: begin values, parameter " << parameters[i] << ", invalid tag found. "
	   << "Please check your parameter file format! \n\n";
      exit(1);
    }
    //--inf value, sup value (try block is for function stof)
    try{
      myfile_stream.getline(tmp, 256, ' ');
      be_val_inf = stof(string(tmp));   
      myfile_stream.getline(tmp, 256);
      be_val_sup = stof(string(tmp));
    }catch(...){
      cerr << "Patient parameter reading fails, please check file '" << parameter_file << "' format!\n" << endl;
      exit(1);
    }
    //--begin value is randomly chosen within [be_val_inf, be_val_sup]
    if(parameters[i] != "temp"){
      begin_values[i] = my_random_int(int(be_val_inf), int(be_val_sup));
    }else{    
      begin_values[i] = my_random_float(be_val_inf, be_val_sup);
    }
  }
  
  myfile_stream.close();
}

//====================================EVENT SETTING==============================================
/**
 # @brief Set delay, plateauDur, quant and objective values for gesture types events.
 # These values are randomly taken within ranges given in the event objects.
 # These values may change from a parameter to another and from a patient to another.
 #
 # @param events Surgery event sequences.
 #
 */
void gesture_setting(vector<Event*> &events)
{

  //iterator initialization
  auto it = events.begin();

  while(it != events.end()){
    if((*it)->get_event_type() == "A-GE"){
      for(size_t i=0; i < (*it)->impacts.size(); i++){
	(*it)->impacts[i]->set_delay((*it)->impacts[i]->one_valid_delay());
	(*it)->impacts[i]->set_plateauDur((*it)->impacts[i]->one_valid_plateauDur());
	(*it)->impacts[i]->set_quant((*it)->impacts[i]->one_valid_quant());
	(*it)->impacts[i]->set_objective((*it)->impacts[i]->one_valid_objective());
      }
    }
    
    it = it + 1;  
  }
}


/**
 # @brief (1) Set delay, plateauDur, quant and objective values for first drug events.
 # These values are randomly taken within ranges given in the event objects.
 # These values may change from a parameter to another and from a patient to another.
 #
 # @param events Surgery event sequences.
 #
 # @todo: this implementation does not support the drug events having range values of delay,
 # plateauDur, quant and objective varying from an impactDrug to another, e.g. atropine.
 #
 # @return Vector of drug names.
 # 
 */
map<string,Event*> drug_setting(vector<Event*> &events)
{

  //keys = drug event names, values = their indices within events vector
  map<string,Event*> drug_first_injection;

  //the name of the current drug
  string curr_drug_name;
    
  //iterator initialization
  auto it = events.begin();
  
  while(it != events.end()){

    if((*it)->get_event_type() == "A-DG"){

      //drug name
      curr_drug_name = (*it)->get_name();    
      
      //-----new drug
      if(drug_first_injection.find(curr_drug_name) == drug_first_injection.end()){
	drug_first_injection[curr_drug_name] = *it;
	for(size_t i=0; i < (*it)->impacts.size(); i++){
	  (*it)->impacts[i]->set_delay( (*it)->impacts[i]->one_valid_delay() );
	  (*it)->impacts[i]->set_plateauDur( (*it)->impacts[i]->one_valid_plateauDur() );
	  (*it)->impacts[i]->set_quant( (*it)->impacts[i]->one_valid_quant() );
	  (*it)->impacts[i]->set_objective( (*it)->impacts[i]->one_valid_objective() );
	}
	
      //----already seen drug
      }else{
	//first four letter of drug's name converted to lower cases
	string tmp = curr_drug_name.substr(0, 4);
	for(size_t i=0; i < tmp.size(); i++){ tmp[i] = tolower(tmp[i]); }
	
	//---only sevoflurane is injected twice: after intubation and before incision
	if(tmp != "sevo"){
	  string error_sms = "ERROR: function drug_setting, drug " + curr_drug_name
	                   + " has been injected more than once before 'entretien'.\n";
	  cerr << error_sms;
	  throw error_sms;
	  
	}else{
	  for(size_t i=0; i < (*it)->impacts.size(); i++){
	    (*it)->impacts[i]->set_delay(
			       drug_first_injection[curr_drug_name]->impacts[i]->get_delay() );
	    (*it)->impacts[i]->set_plateauDur(
			       drug_first_injection[curr_drug_name]->impacts[i]->get_plateauDur() );
	    (*it)->impacts[i]->set_quant(
			       drug_first_injection[curr_drug_name]->impacts[i]->get_quant() );
	    (*it)->impacts[i]->set_objective(
			       drug_first_injection[curr_drug_name]->impacts[i]->get_objective() );
	  }
	}
      }
    }

    it = it + 1;
  }

  return drug_first_injection;
}


//===========================================DRUG ENTRETIEN=======================================
/**
 # @brief Usually drugs impact several parameters, the corresponding impactDrug objects may
 # have different delay and plateauDur values. This make "entretien" complex.
 # Without taking into account the question of anticipation and over-dosing.
 # Here, injection frequency is computed as follows : max(plateauDur).\n 
 #
 # I chose the min of delay because the delay of respiratory parameters is 15 times greater than that of 
 # blood parameters.  This rule must probably change when simulating respiratory parameters.
 #
 # @param drug An instance of Drug
 #
 # @return Injection frequency of the given drug, it is expressed in second.
 */
int compute_injection_frequency(Drug &drug)
{

  //maximum values expressed in second
  int min_delay = 1e6;
  int max_delay = 0;
  int min_plateauDur = 1e6;
  int max_plateauDur = 0;
  
  for(Impact *imp: drug.impacts){

    //convert the i^th impact delay and plateauDur in second
    int tmp_d = imp->convert_to(imp->get_delay(), imp->get_delay_unity(), "s");
    int tmp_p = imp->convert_to(imp->get_plateauDur(), imp->get_plateauDur_unity(), "s");
    
    if(tmp_d < min_delay){ min_delay = tmp_d; }
    if(tmp_d > max_delay){ max_delay = tmp_d; }

    if(tmp_p < min_plateauDur){ min_plateauDur = tmp_p; }
    if(tmp_p > max_plateauDur){ max_plateauDur = tmp_p; }
  }

  int freq = max_plateauDur;   
  freq = (freq/30) * 30;
  
  return freq;
}

/**
 # @brief returns the occurrence time of the first event having given id.
 #  If no event have been found returns -1 
 */
int occurrence_time(vector<Event*> events, vector<int> surgery_times, string event_id)
{
  //
  auto tmp_it_evnt = events.begin();
  auto tmp_it_times = surgery_times.begin();
  int ocr_time = -1;
  
  while(tmp_it_evnt != events.end()){
    if((*tmp_it_evnt)->get_id() == event_id){
      ocr_time = *tmp_it_times;
      break;
    }
    tmp_it_evnt = tmp_it_evnt + 1;
    tmp_it_times = tmp_it_times + 1;
  }

  return ocr_time;
}

/**
 # @brief Perform "entretien" of the given drug within surgery represented by the vector events.
 # Insert corresponding events within vector events and their occurrence times within vector 
 # surgery_times.\n
 #
 # @param drug
 # @param events
 # @param surgery_times Expressed in second
 # @param drug_evnt_database_path 
 #
 # @todo Read injection frequency directly from bolus instances of drug.
 # @WARNING: This function cannot be directly used for another surgery different from hernie inguinale
 # under coescopie. However, a huge part of the function is reusable
 */
void entretien_one_drug(Event* drug, vector<Event*> &events, vector<int> &surgery_times,
			const char *drug_evnt_database_path)
{
 cout << "Flag : begin entretien one drug " << endl;
  //assertion
  assert(events.size() == surgery_times.size());

  //-----looking for the first administration of the given drug 
  auto it_evnt = events.begin();
  auto it_times = surgery_times.begin();
  while((*it_evnt) != drug && it_evnt != events.end()){
    it_evnt = it_evnt + 1;
    it_times = it_times + 1;
  }  
  //-----if it is not found
  if(it_evnt == events.end()){
    string error_sms = "ERROR: function entretien_one_drug, drug " + drug->get_name() 
      +  " is not present within event sequences.\n";
    cerr << error_sms;
    throw error_sms;
  }
  
  //-----it is found: load entretien version of the drug (administered in induction) from the given drug events database
    
  //first four letters of the given drug's name converted to lower cases
  string drug_name = (drug->get_name()).substr(0, 4);
  for(size_t i=0; i < drug_name.size(); i++){ drug_name[i] = tolower(drug_name[i]); }

  //the occurrence time of "pansement" event
  int pansement_times = occurrence_time(events, surgery_times, "panse01");
  assert(pansement_times != -1);

  //---SPECIAL CASE OF PROPOFOL
  //propofol is only used in induction
  //it is replaced by Sevoflurane administered in continu:
  // 1% 1 mn after intubation then concentration is increased at 2% 1-3 mn before incision
  //sevoflurane_continu is not "entretenu"
  if(drug_name == "prop"){
    return;  
  }

  //---SPECIAL CASE OF SEVOFLURANE
  //sevoflurane_continu is not "entretenu", its administration is stopped 30 seconds after "pansement"
  if(drug_name == "sevo"){
    int admin_time = pansement_times+30;
    while(it_evnt != events.end()){
      if(*it_times > admin_time){
	surgery_times.insert(it_times, admin_time);
	events.insert(it_evnt, new Descriptor("stop_sevo", "stop sevoflurane", \
		"this is an event of type descriptor that marks the end of sevoflurane drug administration", true));
        break;
      }
      it_evnt = it_evnt + 1;
      it_times = it_times + 1;
    }
    return;  
  }
  //-----FOR OTHER DRUGS: THE SAME DRUG IS USED IN ENTRETIEN AND IT IS ADMINISTERED IN BOLUS
  //TODO: string file_name = join(drug_evnt_database_path, (drug_name+"b01.bin"));
  string file_name = drug_evnt_database_path + (drug_name+"b01.bin");
  Drug drug_entretien = Drug();
  getFromFile<binary_iarchive, Drug> (drug_entretien, file_name.c_str());


  //---Set patient characteristics stored into drug object
  //induction and bolus instances of drugs have the same characteristics
  
  cout<<"drug->impacts.size()"<<drug->impacts.size()<<endl;
  cout<<"drug->get_name()"<<drug->get_name()<<endl;
  for(size_t i=0; i < drug->impacts.size(); i++){
    // cout<<drug->impacts[i]->toString() <<endl;
    drug_entretien.impacts[i]->set_delay( drug->impacts[i]->get_delay() );
    drug_entretien.impacts[i]->set_plateauDur( drug->impacts[i]->get_plateauDur() );
    drug_entretien.impacts[i]->set_quant( drug->impacts[i]->get_quant() );
    drug_entretien.impacts[i]->set_objective( drug->impacts[i]->get_objective() );
  }
 cout << "Flag :add impact ok !" << endl;

  // //---SPECIAL CASE OF SUFENTANIL
  // //sufentanil_bolus is injected: before 1 min "incision" and 5 min after "Dissection cordon zone hermiale"
  // if(drug_name == "sufe"){
    
  //   //first injection time: 
  //   int incision_time = occurrence_time(events, surgery_times, "insci01");
  //   assert(incision_time != -1);
  //   int injec_time1 = incision_time - 60;
  //   //second injection time:
  //   int dissec_time = occurrence_time(events, surgery_times, "diszh01");   
  //   assert(dissec_time != -1);    
  //   int injec_time2 = dissec_time + 300;
  //   //assertion
  //   assert(injec_time1 < injec_time2);
    
  //   while(it_evnt != events.end()){   
  //     if(*it_times > injec_time1){
	// it_times = surgery_times.insert(it_times, injec_time1);
	// it_evnt = events.insert(it_evnt, new Drug(drug_entretien));
	// break;  	
  //     }
  //     it_evnt = it_evnt + 1;
  //     it_times = it_times + 1;
  //   }

  //   while(it_evnt != events.end()){   
  //     if(*it_times > injec_time2){
	// it_times = surgery_times.insert(it_times, injec_time2);
	// it_evnt = events.insert(it_evnt, new Drug(drug_entretien));
	// break;
  //     }     
  //     it_evnt = it_evnt + 1;
  //     it_times = it_times + 1;
  //   }
    
  //   return;  
  // }
  
  //---OTHER DRUGS: entretien: drug is injected at frequency injection_freq
  //times expressed in second  
  int injection_freq = compute_injection_frequency(drug_entretien);
  int injection_time_ind = *it_times;
  //the first injection time in entretien
  int next_injection_time = injection_time_ind +  injection_freq;

  /*//LOG-INFO
  cout << drug->get_name()  << ": injection time in induction = " << to_string(injection_time_ind/60)
       << endl;
  cout << "Entretien is done with " << drug_entretien.get_name() << ": injected at frecency = "
       << to_string(injection_freq/60) << endl; // comm off */

  //---break conditions, expressed in second
  //morphinic: administration is stopped 30-40 mn before "pansement"
  int break_cond_morphinic = my_random_int(30, 40) * 60;
  //curare: administration is stopped 20-30 mn before "pansement"
  int break_cond_curare = my_random_int(20, 30) * 60;
  
  //---insert entretien events
  while(it_evnt != events.end()){
    
    if(*it_times > next_injection_time){
      //insertion before the element at position it_times and it_evnt
      //after insertion, iterators point to the newly inserted elements
      it_times = surgery_times.insert(it_times, next_injection_time);
      it_evnt = events.insert(it_evnt, new Drug(drug_entretien));

      //LOG-INFO
      /*cout << "entretien at " << to_string(next_injection_time/60) << endl; // comm off */
      
      //compute next injection time
      next_injection_time = next_injection_time + injection_freq;
    }

    it_evnt = it_evnt + 1;
    it_times = it_times + 1;

    //@todo: to be completed by future morphinc except SUFENTANIL which is a special case
    if( (drug_name == "alfe") && ((pansement_times - *it_times) < break_cond_morphinic) ){
      break;
    }
    //@todo: to be completed by future curare
    if( (drug_name == "atra") && ((pansement_times - *it_times) < break_cond_curare) ){
      break;
    }  
  }
  //LOG-INFO
  /*cout << endl; // comm off */
}


/**
 # @brief Move concomitant events.
 #  If two events occurr at the same time, add 30 second to the occurrence
 #  time of all the following events in the sequence.
 #
 # @param surgery_time Horodatage of events in second.
 #
 */
void no_concomitant_event(vector<int> &surgery_times)
{

  int nb_events = surgery_times.size();
   
  for(size_t i=0; i < nb_events-1; i++){
    
    //i^{th} and (i+1)^{th} events are concomitant
    if(surgery_times[i] == surgery_times[i+1]){
      for(size_t j=i+1; j < nb_events; j++){
	surgery_times[j] = surgery_times[j] + 30;
      }
    }
  }
}


/**
 # @brief Perform "entretien" of all drugs used during the given surgery.
 #
 # @param events Surgery event sequences.
 # @param surgery_time Horodatage of events in second.
 # @param drug_first_injection 
 #
 */
void entretien(vector<Event*> &events, vector<int> &surgery_times,
	       map<string,Event*> drug_first_injection, const char *drug_evnt_database_path)
{
  cout << "======================LOG-INFO: anesthesia entretien begging=======================" << endl; // comm off 

  for(auto it=drug_first_injection.begin(); it != drug_first_injection.end(); ++it){
    //it->first = key, it->second = value
    entretien_one_drug(it->second, events, surgery_times, drug_evnt_database_path);
  }

  //---move concomitant events
  no_concomitant_event(surgery_times);


  //---assertions
  assert(events.size() == surgery_times.size());
  assert(surgery_times[0] == 0);
  for(size_t i=1; i < surgery_times.size(); i++){ assert( surgery_times[i-1] <= surgery_times[i] ); }
    
  cout << "======================LOG-INFO: anesthesia entretien end============================" << endl; // comm off 
}


//==============================================TIME SERIES GENERATOR====================================
/*
 # @brief
 #
 # @param parameters
 # @param base_values
 # @param begin_values
 # @param events
 # @param surgery_times Expressed in minute
 # @param file_name
 #
*/
void time_series_generator(vector<string> parameters, vector<float> base_values,
			   vector<float> begin_values, vector<Event*> events,
			   vector<int> surgery_times, string file_name, int delta_t)
{

  //--------output file
  fstream fs (file_name, fstream::out);
  //add headers
  fs << "Time";
  for(string param: parameters){ fs << "," << param; }
  fs << endl;
  //add begin values
  fs << "-1";
  for(float b_val: begin_values){ fs << "," << b_val;}
  fs << endl;

  //--------conformity tests
  assert (events.size() == surgery_times.size());
  assert (parameters.size() == base_values.size());
  assert (base_values.size() == begin_values.size());
  int nb_events = events.size();
  int nb_parameter = parameters.size();

  //-------sampling frequency, must be equals min(delays of impacts)
  string delta_t_unity = "s";
  
  //-------garbage impact, give an acces to the methods of class Impact
  ImpactDrug* garbage = new ImpactDrug("FC", 0, 0, "mn", 0, 0, "mn", 0, 1);

  //-------begining sampling
  vector<float> previous_values = vector<float>(begin_values);
  vector<float> curr_values = vector<float>(nb_parameter); 

  //for each parameter, initialize currently active impacts
  map<string,vector<Impact*>> active_impacts;
  for(string param: parameters){ active_impacts[param] = vector<Impact*>(0);}

  //the total effect of all infinite plateaus that are removed
  map<string, vector<pair<int, float>>> inf_plateau_vanish;
  for(string param: parameters){ inf_plateau_vanish[param] = vector<pair<int, float>>(0);}
      
  //we start at the first event
  int next_evnt = 0;

  //-------loop
  //NB: sampling stops 0 samples after the occurrence of the last event
  for(size_t t=surgery_times[0]; t <= surgery_times[nb_events-1] + 0*delta_t; t = t + delta_t){

    //add time to output file
    fs << t;
      
    //---activate the events that occur within time interval ]t-delta_t, t]
    while(next_evnt < nb_events && surgery_times[next_evnt] <= t){    
      
      //--for each parameter: activate the corresponding impact of event
      for(size_t ind_p=0; ind_p < nb_parameter; ind_p = ind_p+1){
        // cout<<parameters[ind_p]<<endl;
	      garbage->activate_impacts_of_event(parameters[ind_p], events[next_evnt], \
					     active_impacts[parameters[ind_p]], \
					     inf_plateau_vanish[parameters[ind_p]], \
					     surgery_times[next_evnt],	\
					     previous_values[ind_p], delta_t, delta_t_unity);
      }

      next_evnt = next_evnt + 1;
    }

    //---for each parameter: perform sampling
    for(size_t ind_p=0; ind_p < nb_parameter; ind_p = ind_p+1){
      float tmp = garbage->update_parameter(previous_values[ind_p], base_values[ind_p], \
					    active_impacts[parameters[ind_p]]);
      //add the effect of deactivated infinite plateaus
      auto it = inf_plateau_vanish[parameters[ind_p]].begin();
      while(it != inf_plateau_vanish[parameters[ind_p]].end()){
	//assertion
	assert((*it).first >= 0);
	
	if((*it).first > 0){
	  tmp = tmp + (*it).second;
	  (*it).first = (*it).first - 1;
	}
	
	if((*it).first == 0){
	  it = inf_plateau_vanish[parameters[ind_p]].erase(it);
	}else{
	  it = it + 1;
	}
      }

      curr_values[ind_p] = tmp;
      //add current parameter value to the output file
      fs << "," << curr_values[ind_p];
    }
    fs << endl;
    
    //---update active_impacts
    for(string param: parameters){ garbage->deactivate_impacts(active_impacts[param]); }
    
    //---update previous_value
    for(size_t ind_p=0; ind_p < nb_parameter; ind_p = ind_p+1){  previous_values[ind_p] = curr_values[ind_p]; }
    
    /*cout << "LOG-INFO: t = " << to_string(t) << endl; //comm off*/
    
  }

  //deletion
  fs.close();
  delete garbage;
  
}


//==========================================ANESTHESIA PROFIL GENERATOR================================
/*
 # @brief
 #
*/
void profile_generator(const char *event_seq_file, vector<int> ref_timestamps,
		       const char *drug_evnt_database_path,
		       string output_file_evnt, string output_file_series, int sampling_freq, 
		       vector<string> parameters, vector<float> rand_base_values,
		       vector<float> rand_begin_values)
{  
  //----Load event sequence and corresponding surgery times
  vector<Event *> event_sequence;
  getFromFile<binary_iarchive, vector<Event*>> (event_sequence, event_seq_file);



  //----Set timestamp
  vector<int> surgery_times = check_timestamp(event_sequence, ref_timestamps);



  //----Display event sequence and corresponding surgery times
  cout << endl << "Sequence of " << event_sequence.size() << " events has been loaded." << endl;
  cout << "Corresponding surgery time array of length " << to_string(surgery_times.size()) << endl;
  /**for (size_t i=0; i < event_sequence.size(); ++i){
    cout << "time = " << to_string(surgery_times[i]/60.) << " mn" << endl;
    cout << event_sequence[i]->toString();
    cout << endl << endl;
    }*/

  //----Take patient characteristics into account
  gesture_setting(event_sequence);
  map<string,Event*> drug_first_injection = drug_setting(event_sequence);
  
  cout << endl << "Event sequence after taking patient characteristics into account: "
       << event_sequence.size() << " events." << endl;
  cout << "Corresponding surgery time array of length " << to_string(surgery_times.size()) << endl;
  /**for (size_t i=0; i < event_sequence.size(); ++i){
    cout << "time = " << to_string(surgery_times[i]/60.) << " mn" << endl;
    cout << event_sequence[i]->toString();
    cout << endl << endl;
    }*/

  
  //----Perform entretien
  entretien(event_sequence, surgery_times, drug_first_injection, drug_evnt_database_path);
  
  cout << endl << "After entretien: " << event_sequence.size() << " events." << endl;
  cout << "Corresponding surgery time array of length " << to_string(surgery_times.size()) << endl;
  /**for (size_t i=0; i < event_sequence.size(); ++i){
    //comment
    cout << "time = " << to_string(surgery_times[i]/60.) << " mn" << endl;
    cout << event_sequence[i]->toString();
    cout << endl << endl;
    }*/

  //----Collect events
  fstream my_fs (output_file_evnt, fstream::out);
  my_fs << "Time," << "Event" << endl;
  for (size_t i=0; i < event_sequence.size(); ++i){
    my_fs << surgery_times[i] << "," << event_sequence[i]->get_name() << endl;
  }
  my_fs.close();

  //----Time series generator
  time_series_generator(parameters, rand_base_values, rand_begin_values, event_sequence,
			surgery_times, output_file_series, sampling_freq);
 
  
  //----Deletion
  for (Event *e: event_sequence){ delete e; }

}


