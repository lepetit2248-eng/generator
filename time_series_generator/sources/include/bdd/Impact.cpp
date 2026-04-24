//=============================================================================================================================================
// Name        :
// Author      :
// Version     :
// Copyright   :
// Description :
//=============================================================================================================================================

#include <set>
#include <assert.h>

#include "Impact.h"
#include "Event.h"


//***********************************************CONSTRUCTORS**************************************
Impact::Impact()
{

}
//=================================================================================================
Impact::Impact(const Impact &imp)
{
  //GENERAL FIELDS
  parameter = imp.get_parameter();
  
  delay_inf = imp.get_delay_inf(); 
  delay_sup = imp.get_delay_sup();
  delay = imp.get_delay();
  delay_unity = imp.get_delay_unity();
  
  plateauDur_inf = imp.get_plateauDur_inf();
  plateauDur_sup = imp.get_plateauDur_sup();
  plateauDur = imp.get_plateauDur();
  plateauDur_unity = imp.get_plateauDur_unity();

  //FIELDS FOR TREND MODELING
  trendE = imp.get_trendE();
  trend = imp.get_trend();

  quant_inf = imp.get_quant_inf();
  quant_sup = imp.get_quant_sup();
  quant = imp.get_quant();

  objective_inf = imp.get_objective_inf();
  objective_sup = imp.get_objective_sup(); 
  objective = imp.get_objective();

  //FIELDS USED IN TIME SERIES GENERATION
  begin_time = imp.get_begin_time();
  begin_value = imp.get_begin_value();

  delta_t = imp.get_delta_t();
  delta_t_unity = imp.get_delta_t_unity();
  
  sample_index = imp.get_sample_index();
  prev_oscillation_val = imp.get_prev_oscillation_val();

}

//=================================================================================================
Impact::Impact(string param, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int trend_, int trendE_,
	float quant_inf_, float quant_sup_, float objective_inf_, float objective_sup_)
{

  //Valid domain of values
  assert(d_inf >= 0 && d_sup >= 0);
  assert(dur_inf >= 0 && dur_sup >= 0);
  assert(quant_inf_ >= 0 && quant_sup_ >= 0);
  assert(objective_inf_ >= 0 && objective_sup_ >= 0);

  //-------------------------------------------------------GENERAL FIELDS--------------------------
  //check whether given unities are valid; unity can be "mn", "s" or "h"
  set<string> possible_unity = {"s", "mn", "h"};

  if(possible_unity.find(d_unity) == possible_unity.end()){
    string error_sms = "Error: class Impact: constructor, \"" + d_unity + "\" is an invalid unity!\n";
    cerr << error_sms;
    throw error_sms;
  }
  if(possible_unity.find(dur_unity) == possible_unity.end()){
    string error_sms = "Error: class Impact: constructor, \"" + dur_unity + "\" is an invalid unity!\n";
    cerr << error_sms;
    throw error_sms;
  }

  //check if the given parameter is a valid monitoring parameter
  set<string> possible_param = {"FC","PAS","PAD","PAM","SpO2","FiO2","FeO2","DGF","FiCO2", "FeCO2","FR","Vt","Vme","Pmoy","Pmax","PEP","temp","QC", "RVS", "FiHAL", "FeHAL", "T1/T4", "TOF"};

  if (possible_param.find(param) != possible_param.end()){
    parameter = param;
  }else{
    string error_sms = "ERROR: class Impact: " + param + " is not a valid monitoring parameter!\n";
    cerr << error_sms;
    throw error_sms;
  }

  //delay
  delay_unity = d_unity;
  if(d_inf > d_sup){
    string error_sms = "ERROR: class Impact: constructor, delay_inf must be less than or equal to delay_sup!\n";
    cerr << error_sms;
    throw error_sms;

  }else{
    delay_inf = d_inf;
    delay_sup = d_sup;
  }
  delay = -1; //initial value

  //plateau
  plateauDur_unity = dur_unity;
  if(dur_inf > dur_sup){
    string error_sms = "ERROR: class Impact: constructor, plateauDur_inf must be less than or equal to plateauDur_sup!\n";
    cerr << error_sms;
    throw error_sms;

  }else{
    plateauDur_inf = dur_inf;
    plateauDur_sup = dur_sup;
  }
  plateauDur = -1; //initial value

  //------------------------------------------------FIELDS FOR TREND MODELING----------------------
  //trend
  if(trend_ != 0 && trend_ != 1 && trend_ != 2 && trend_ != 3 && trend_ != 4 && trend_ != 5){
    string error_sms = "ERROR: class Impact: constructor, given trend is invalid!\n";
    cerr << error_sms;
    throw error_sms;

  }else{
    trend = trend_;
  }

  //trendE
  if(trendE_ != 0 && trendE_ != 1){
    string error_sms = "ERROR: class Impact: constructor, given trendE is invalid!\n";
    cerr << error_sms;
    throw error_sms;

  }else{
    trendE = trendE_;
  }
  //infinite plateau
  if(trendE_ == 0){ assert (plateauDur_unity == "mn" && plateauDur_inf == plateauDur_sup && plateauDur_sup == 1000);}

  //quant
  if(quant_inf_ > quant_sup_){
    string error_sms = "ERROR: class Impact: constructor, quant_inf must be less than or equal to quant_sup!\n";
    cerr << error_sms;
    throw error_sms;

  }else{
    quant_inf = quant_inf_;
    quant_sup = quant_sup_;
  }
  quant = -1; //initial value

  //objective
  if(objective_inf_ > objective_sup_){
    string error_sms = "ERROR: class Impact: constructor, objective_inf must be less than or equal to objective_sup!\n";
    cerr << error_sms;
    throw error_sms;

  }else{
    objective_inf = objective_inf_;
    objective_sup = objective_sup_;
  }
  objective = -1; //initial value

  //allow to check whether the impact has been already activated or not
  sample_index = -1;

}

//*******************************************OTHER FUNCTIONS**************************************
string Impact::toString()
{

  string fields = "Class Impact fields: " + parameter
    + " delay: [" + to_string(delay_inf) + "," + to_string(delay_sup) + "], " + to_string(delay) + " " + delay_unity
    + " || plateauDur: [" + to_string(plateauDur_inf) + "," + to_string(plateauDur_sup) + "], " + to_string(plateauDur) + " " + plateauDur_unity
    + " || trend = " +  to_string(trend) + " trendE = " + to_string(trendE)
    + " || quant: [" + to_string(quant_inf) + "," + to_string(quant_sup) + "], " + to_string(quant)
    + " || objective: [" + to_string(objective_inf) + "," + to_string(objective_sup) + "], " + to_string(objective)
    + " || sample_index = " + to_string(sample_index) + "\n";

  return fields;

}
//=================================================================================================

int Impact::one_valid_plateauDur()
{
  return random_int (plateauDur_inf, plateauDur_sup);
}

int Impact::one_valid_delay()
{
  return random_int (delay_inf, delay_sup);
}

float Impact::one_valid_quant()
{
  return random_float (quant_inf, quant_sup);
}


float Impact::one_valid_objective()
{
  return random_float (objective_inf, objective_sup);
}

//*********************************************************GETTERS: GENERAL FIELDS*********************************
string Impact::get_parameter() const
{
  return parameter;
}

int Impact::get_delay_inf() const
{
  return delay_inf;
}
int Impact::get_delay_sup() const
{
  return delay_sup;
}
int Impact::get_delay() const
{
  return delay;
}
string Impact::get_delay_unity() const
{
  return delay_unity;
}

int Impact::get_plateauDur_inf() const
{
  return plateauDur_inf;
}
int Impact::get_plateauDur_sup() const
{
  return plateauDur_sup;
}
int Impact::get_plateauDur() const
{
  return plateauDur;
}
string Impact::get_plateauDur_unity() const
{
  return plateauDur_unity;
}

//*********************************************************GETTERS: TREND MODELING*********************************
int Impact::get_trendE() const
{
  return trendE;
}
int Impact::get_trend() const
{
  return trend;
}

float Impact::get_quant_inf() const
{
  return quant_inf;
}
float Impact::get_quant_sup() const
{
  return quant_sup;
}
float Impact::get_quant() const
{
  return quant;
}

float Impact::get_objective_inf() const
{
  return objective_inf;
}
float Impact::get_objective_sup() const
{
  return objective_sup;
}
float Impact::get_objective() const
{
  return objective;
}

//******************************GETTERS: FIELDS USED IN TIME SERIES GENERATION*********************
int Impact::get_begin_time() const
{
  return begin_time;
}
float Impact::get_begin_value() const
{
  return begin_value;
}

int Impact::get_delta_t() const
{
  return delta_t;
}
string Impact::get_delta_t_unity() const
{
  return delta_t_unity;
}

int Impact::get_sample_index() const
{
  return sample_index;
}
float Impact::get_prev_oscillation_val() const
{
  return prev_oscillation_val;
}

//*********************************************SETTERS*********************************************
void Impact::set_plateauDur(int duration)
{

  if(plateauDur == -1){

    if(plateauDur_inf <= duration && duration <= plateauDur_sup){
      plateauDur = duration;
    }else{
      string error_sms = "ERROR: class Impact: in function set_plateauDur(int), duration is out of plateauDur interval!\n";
      cerr << error_sms;
      throw error_sms;
    }

  }else{
    string error_sms = "ERROR: class Impact: in function set_plateauDur(int), plateauDur has been already set!\n";
    cerr << error_sms;
    throw error_sms;
  }

}
//=================================================================================================
void Impact::set_delay(int duration)
{

  if(delay == -1){

    if(delay_inf <= duration && duration <= delay_sup){
      delay = duration;
    }else{
      string error_sms = "ERROR: class Impact: in function set_delay(int), duration is out of delay interval!\n";
      cerr << error_sms;
      throw error_sms;
    }

  }else{
    string error_sms = "ERROR: class Impact: in function set_delay(int), delay has been already set!\n";
    cerr << error_sms;
    throw error_sms;
  }

}
//=================================================================================================
void Impact::set_quant(float val)
{

  if(quant == -1){

    if(quant_inf <= val && val <= quant_sup){
      quant = val;
    }else{
      string error_sms = "ERROR: class Impact: in function set_quant(float), val is out of quant interval!\n";
      cerr << error_sms;
      throw error_sms;
    }

  }else{
    string error_sms = "ERROR: class Impact: in function set_quant(float), quant has been already set!\n";
      cerr << error_sms;
      throw error_sms;
  }

}
//=================================================================================================
void Impact::set_objective(float value)
{

  if(objective == -1){

    if(objective_inf <= value && value <= objective_sup){
      objective = value;
    }else{
      string error_sms = "ERROR: class Impact: in function set_objective(float), value is out of objective interval!\n";
      cerr << error_sms;
      throw error_sms;
    }

  }else{
    string error_sms = "ERROR: class Impact: in function set_objective(float), objective has been already set!\n";
    cerr << error_sms;
    throw error_sms;
  }

}
//=================================================================================================
//TODO: check whether delta < plateauDur
void Impact::activate_impact(int b_time, float b_value, int delta, string d_unity)
{

  if(sample_index != -1){
    string error_sms = "ERROR: class Impact: in function activate_impact(...), you try to activate an Impact already activated!\n";
    cerr << error_sms;
    throw error_sms;
  }

  //begin context
  begin_time = b_time;
  begin_value = b_value;

  //sampling frequency
  delta_t = delta;
  delta_t_unity = d_unity;
  sample_index = 0;

  //stationary regime management
  prev_oscillation_val = 0.0;

}


/***************************************************UTIL METHODS***********************************

/*return a pseudo-random int between min and max*/
int Impact::random_int(int min, int max)
{

  if (min > max){
    string error_sms = "ERROR: class Impact: in function random_int(int min, int max) min should be strictly less than max! \n";
    cerr << error_sms ;
    throw error_sms;
  }

  return rand()%(max-min+1) + min;
}
//=================================================================================================

/*return a pseudo-random float between min and max*/
float Impact::random_float(float min, float max)
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
//=================================================================================================

/*Precondition: unity and target_unity must be equal "mn", "s" or "h"*/
int Impact::convert_to (int nb_time, string unity, string target_unity)
{

  //source and target unity are equal
  if (unity == target_unity){
    return nb_time;
  }

  // hour
  if (target_unity == "h"){
    if (unity == "s"){
      return nb_time/3600;
    }
    if (unity == "mn"){
      return nb_time/60;
    }
  }

  //minute
  if (target_unity == "mn"){
    if (unity == "s"){
      return nb_time/60;
    }
    if (unity == "h"){
      return nb_time*60;
    }
  }

  //second
  if (target_unity == "s"){
    if (unity == "mn"){
      return nb_time*60;
    }
    if (unity == "h"){
      return nb_time*3600;
    }
  }
  return 0;
}

//=================================================================================================

bool Impact::with_in_delay(int index)
{

  int delay_c = convert_to(delay, delay_unity, delta_t_unity);

  if(index == -1){//test is done on sample_index field
    return delta_t*sample_index <= delay_c;

  }else if(index > 0){//test is done on given index
    return delta_t*index <= delay_c;

  }else{
    string error_sms = "ERROR: class Impact: in function with_in_delay(index) index must be positive or equals -1!\n";
    cerr << error_sms;
    throw error_sms;
  }

}
//=================================================================================================

bool Impact::with_in_plateau(int index)
{

  int delay_c = convert_to(delay, delay_unity, delta_t_unity);
  int plateauDur_c = convert_to(plateauDur, plateauDur_unity, delta_t_unity);

  if(index == -1){
    return (delay_c < delta_t*sample_index) && (delta_t*sample_index <= delay_c + plateauDur_c);

  }else if(index > 0){
    return (delay_c < delta_t*index) && (delta_t*index <= delay_c + plateauDur_c);

  }else{
    string error_sms = "ERROR: class Impact: in function with_in_plateau(index) index must be strictly positive or equals -1!\n";
    cerr << error_sms;
    throw error_sms;
  }

}
//=================================================================================================

bool Impact::with_in_decay(int index)
{

  int delay_c = convert_to(delay, delay_unity, delta_t_unity);
  int plateauDur_c = convert_to(plateauDur, plateauDur_unity, delta_t_unity);

  if(index == -1){
    return (delay_c + plateauDur_c < delta_t*sample_index) && (delta_t*sample_index <= 2*delay_c + plateauDur_c);

  }else if(index > 0){
      return (delay_c + plateauDur_c < delta_t*index) && (delta_t*index <= 2*delay_c + plateauDur_c);

  }else{
    string error_sms = "ERROR: class Impact: in function with_in_decay(index) index must be positive or equals -1!\n";
    cerr << error_sms;
    throw error_sms;
  }

}
//=================================================================================================

bool Impact::is_deactivated(int index)
{

  int delay_c = convert_to(delay, delay_unity, delta_t_unity);
  int plateauDur_c = convert_to(plateauDur, plateauDur_unity, delta_t_unity);

  if(index == -1){
    return 2*delay_c + plateauDur_c < delta_t*sample_index;

  }else if(index > 0){
    return 2*delay_c + plateauDur_c < delta_t*index;

  }else{
    string error_sms = "ERROR: class Impact: in function is_deactivated(index) index must be positive or equals -1!\n";
    cerr << error_sms;
    throw error_sms;
  }
}

//*****************************METHODS USED IN TIME SERIES GENERATOR PROGRAM**********************

void Impact::activate_impacts_of_event(string parameter, Event* event, vector<Impact*> &active_impacts, vector<pair<int, float>> &inf_plateau_vanish, int be_time, float previous_value, int delta, string delta_unity)
{

  //the total effect of all infinite plateaus
  float inf_plateau_eff = 0.0;
  
  vector<Impact*> new_activated;
  int index = 0;
  // cout<<event->get_name()<<endl;
  for(Impact *imp: event->impacts){
      if(imp->get_parameter() == parameter){
        
	      new_activated.push_back(imp);
      }
      index = index + 1;
  }

  //an event can affect a parameter in an unique manner
  // cout<<"new_activated.size()"<<new_activated.size()<<endl;
  assert(new_activated.size() <= 1);

  /***if the given event impacts the target parameter***/
  if(new_activated.size() > 0){

    //the given event actes on the target parameter in one single manner
    assert(new_activated.size() == 1);
  
    /***If the given event has TrendE=0 therefore it takes the place of the current stimulation
     #whose impact will be stopped.
     #Event "déflation pneumoperitoine is not concerned.***/
    if( (event->get_event_type() == "A-GE") && (new_activated[0]->get_trendE() == 0) && \
	(event->get_id() != "depne01") ){

      /***For temp parameter, only event "Bair Hugger" is concerned.***/
      if(parameter != "temp" || event->get_id() == "baihu01"){   
	//iterator on vector active_impacts
	auto it = active_impacts.begin();
	while(it != active_impacts.end()){
	  if( ((*it)->get_trendE() == 0)){		 
	    inf_plateau_vanish.push_back( (*it)->infinite_plateau_deactivate_impacts() );
	    //remove the element that is pointed by the iterator it, returns an iterator pointing
	    //to the new location of the element that follows the element erased
	    it = active_impacts.erase(it);	
	  }else{
	    //Notice that iterator is incremented only when no element has been removed
	    it = it + 1;
	  }
	}
      }
    }

    /***activate and add the new impact***/
    new_activated[0]->activate_impact(be_time, previous_value, delta, delta_unity);
    active_impacts.push_back(new_activated[0]);

  /***descriptor type events that depict stopping conditions of drugs administered in 
   #"continu". These drugs are characterized by infinite "plateaus" that can damp: 
   #trendE=1 && plateauDur = 1000 mn
   #TODO: At this point, only sevoflurane drug is administered in "continu". 
   # This feature has to be extended to other drugs administered in the same maner.
   # When new drugs will be added, we must found the one that is associated to the
   # current stopping condition then its impacts have to be stopped***/
  }else if (event->get_event_type() == "D" && event->get_id() == "stop_sevo"){
        
    auto it = active_impacts.begin();
    while(it != active_impacts.end()){
      if( ((*it)->get_trendE() == 1) && ((*it)->get_plateauDur() == 1000) && \
	  ((*it)->get_plateauDur_unity() == "mn")){					  

	inf_plateau_vanish.push_back( (*it)->infinite_plateau_deactivate_impacts() );
	
	it = active_impacts.erase(it);
      }
      it = it + 1;
    }
  }

}
//=================================================================================================

pair<int, float> Impact::infinite_plateau_deactivate_impacts()
{

  //assertion
  assert(plateauDur == 1000);
  assert(plateauDur_unity == "mn");
  assert(trend == 2 || trend == 3 || trend == 4 || trend == 5);

  //output
  pair<int, float> eff;

  //----trend 2/3 and trend 4/5 when zero samples have been generated in delay and plateau phases
  if(trend == 2 || trend == 3 || sample_index == 0){
    eff = pair<int, float>(0, 0.0);
   
  }else{ //----trend 4 or 5

    //because of the infinite plateau, nb_sample_delay == nb_sample_decay
    int nb_sample = convert_to(delay, delay_unity, delta_t_unity) / delta_t;
    
    //effective number of samples generated in delay phase 
    int nb_times_incr;
    if(nb_sample <= sample_index){ //all samples with the delay phase have been generated
      assert(with_in_plateau(sample_index+1));
      nb_times_incr = nb_sample;
    }else{  //only sample_index samples have been generated
      assert(with_in_delay(sample_index));
      nb_times_incr = sample_index;
    }

    //assertion
    assert ( quant != 0 || objective != 0 );
    assert ( quant == 0 || objective == 0 );
    
    //--trend 4
    if(trend == 4){

      //total increasing/decreasing effect of the impact
      float total_incr;
      if (quant != 0){
	total_incr = quant * begin_value;
      }else{
	// assert(objective >= begin_value); // Désactivé pour flexibilité bidirectionnelle
	total_incr = objective - begin_value;
      }
      //output
      if(nb_sample == 0){ //abrupt decreasing
	assert(nb_times_incr == 0);
	eff = pair<int, float>(1, -total_incr);
      }else{
	eff = pair<int, float>(nb_times_incr, -(total_incr/nb_sample));
      }
      
    }else if(trend == 5) { //---trend 5

      //total increasing/decreasing effect of the impact
      float total_incr;
      if (quant != 0){
	total_incr = -(quant * begin_value);
      }else{
	// assert(objective <= begin_value); // Désactivé pour flexibilité bidirectionnelle
	total_incr = objective - begin_value;
      }
      //output
      if(nb_sample == 0){ //abrupt increasing
	assert(nb_times_incr == 0);
	eff = pair<int, float>(1, -total_incr);}
      else{
	eff = pair<int, float>(nb_times_incr, -(total_incr/nb_sample));
      }
    }
  }

  //assertion
  assert(eff.first >= 0);
  
  return eff;
}

void Impact::deactivate_impacts(vector<Impact*> &active_impacts)
{

  //initialization of an iterator on vector active_impacts
  auto it = active_impacts.begin();

  while(it != active_impacts.end()){

    if((*it)->is_deactivated()){
      
      /*cout << "LOG-INFO: deactivate_impacts: Event that began at "
	   << (*it)->get_begin_time() <<  "s: "
	   << (*it)->get_parameter() << endl << endl; //comm off */
      
      it = active_impacts.erase(it);
      
    }else{
      it = it + 1;
    }
  }
}

//=====================================================================================================
//TODO: The returned value is positive (not strictly)
//TODO: Are there min max values?
float Impact::update_parameter (float previous_value, float base_value, const vector<Impact*> &impacts)
{

  float new_sample = 0.0;
  float total_impact = 0.0;

  //-----no impacts are activated, base_value takes the hand
  if(impacts.size() == 0){
    float quant_var = (base_value*0.0499)/2;
    float oscillation = random_float(-quant_var, quant_var);
    new_sample = base_value + oscillation;

  //-----at least one impact is activated
  }else{
    
    //equals true if at least one impact within plateau phase has been already treated
    bool one_plateau_phase = false;
    
    for(Impact *imp: impacts){

      if(!one_plateau_phase && imp->with_in_plateau()){
	one_plateau_phase = true;
	total_impact = total_impact + imp->quantify_impact(base_value, true);
      }else{	
	total_impact = total_impact + imp->quantify_impact(base_value, false);
      }
    }
  }

  new_sample = previous_value + total_impact;

  //-----ckeck whether new_sample is consitent
  if(new_sample < 0){
    new_sample = 0.0;
  }

  return new_sample;

}

//***************************INTERMEDIATE FUNCTIONS USED IN update_parameter FUNCTION**************

float Impact::quantify_impact_stationary (float plat_value, float var)
{

  float quant_var = (plat_value*var)/2;
  float new_oscillation_val = random_float(-quant_var, quant_var);
  float effectif_oscillation_val = new_oscillation_val - prev_oscillation_val;
  prev_oscillation_val = new_oscillation_val;

  return effectif_oscillation_val;

}
//=====================================================trend 4=====================================
float Impact::quantify_impact_4 (float base_value, bool plateau_oscillation)
{
  if (with_in_delay()){
    /*cout << "delay" << endl; //comm off*/
    return quantify_impact_delay_4 (base_value);

  }else if (with_in_plateau()){
    /*cout << "plateau" << endl; //comm off*/
    return quantify_impact_plateau_4(base_value, plateau_oscillation);

  }else if (with_in_decay()) {
    /*cout << "decay" << endl; //comm off*/
    assert (trendE == 1);
    return quantify_impact_decay_4 (base_value);
    
  }else if(is_deactivated()){
    /*cout << "deactivated" << endl; //comm off*/
    return quantify_impact_deactivated_4(base_value);

  }else{
    string error_sms = "ERROR: class Impact: in function quantify_impact_4, invalid phase found!\n";
    cerr << error_sms;
    throw error_sms;
  }

}
//=========================================================
float Impact::quantify_impact_deactivated_4(float base_value)
{
  //-----instantaneous/missed Impact: 2*delay + plateauDur < sampling frequency delta_t
  if(sample_index == 1){
    /*cout << "WARNING: class Impact: one instantaneous/missed Impact, "
	 << "i.e. 2*delay + plateauDur < sampling frequency, occurs at time "
	 << to_string(begin_time) << endl; //comm off */
    return 0.0;
  }

  //-----if zero samples have been generated in decay phase
  //here we add the missed decreasing of the target parameter
  if(!with_in_decay(sample_index-1)){

    /*cout << "WARNING: class Impact: in function quantify_impact_deactivated_4: "
	 << "Event that began at " << get_begin_time() <<  "s: " << get_parameter()
	 << ": zero samples have been generated in decay phase!\n"
	 << endl; //comm off */
    
    assert ( quant != 0 || objective != 0 );
    assert ( quant == 0 || objective == 0 );
    
    if(quant != 0){ //trend 4 modeled by quant
      return - (quant*begin_value);
	
    }else{ //trend 4 modeled by objective
      // assert(begin_value < objective); // Désactivé pour flexibilité bidirectionnelle
      return  - (objective - begin_value);
    }

  //-----in the other cases
  }else{
    return 0.0;
  }
}
//=========================================================
float Impact::quantify_impact_plateau_4(float base_value, bool plateau_oscillation)
{

  //assertion
  assert ( quant != 0 || objective != 0 );
  assert ( quant == 0 || objective == 0 );

  /*if(sample_index == 1){
    cout << "WARNING: class Impact: in function quantify_impact_plateau_4: "
	 << "Event that began at " << get_begin_time() <<  "s: " << get_parameter()
	 << ": zero samples have been generated in delay phase!\n"
	 << endl; //comm off
  }*/
  
  //-----trend 4 modeled by quant
  if(quant != 0){
    
    float plateau_value = begin_value + quant*begin_value;
    float oscillation = 0.0;
    if(plateau_oscillation){
      oscillation = quantify_impact_stationary(plateau_value, 0.0499);
    }

    //if zero samples have been generated in delay phase
    //here we add the missed increasing of the target parameter
    if(sample_index == 1){
      return quant * begin_value;
      
    }else{
      return oscillation;
    }

  //-----trend 4 modeled by objective
  }else{
   
    float oscillation = 0.0;
    if(plateau_oscillation){
      oscillation = quantify_impact_stationary(objective, 0.0499);
    }

    //if zero samples have been generated in delay phase
    //here we add the missed increasing of the target parameter
    if(sample_index == 1){
      // assert(begin_value < objective); // Désactivé pour flexibilité bidirectionnelle
      return objective - begin_value;
      
    }else{
      return oscillation;
    }
  }
}
//=========================================================
float Impact::quantify_impact_delay_4(float base_value)
{

  //assertion
  assert ( quant != 0 || objective != 0 );
  assert ( quant == 0 || objective == 0 );

  int nb_sample = convert_to(delay, delay_unity, delta_t_unity)/delta_t;
  assert(nb_sample > 0);
  float incr;

  if (quant != 0){
    incr = quant*begin_value/nb_sample;
  }

  if (objective != 0){

    //assertion
    // if(parameter != "temp"){ // assert ( (objective - begin_value) > 0 ); // Désactivé pour flexibilité bidirectionnelle } // Désactivé pour flexibilité bidirectionnelle

    incr = (objective - begin_value)/nb_sample;
  }

  return  incr;

}
//=========================================================
float Impact::quantify_impact_decay_4(float base_value)
{

  //assertion
  assert ( quant != 0 || objective != 0 );
  assert ( quant == 0 || objective == 0 );
  
  //if zero samples have been generated in delay and plateau phases
  if(sample_index == 1){
    /*cout << "WARNING: class Impact: Event that began at " 
	 <<  get_begin_time() << "s: " << get_parameter() 
	 << ": zero samples have been generated in delay and plateau phases!\n"
	 << endl; //comm off */
    return 0.0;
  }

  int tmp = convert_to(delay, delay_unity, delta_t_unity) + \
      convert_to(plateauDur, plateauDur_unity, delta_t_unity);
  tmp = tmp%delta_t;
  int nb_sample = (tmp + convert_to(delay, delay_unity, delta_t_unity)) / delta_t;
  float incr;
  
  //compensation of the last oscillation in plateau phase
  float var = 0.0;
  if(with_in_plateau(sample_index - 1)){
    var = -prev_oscillation_val;
  }

  if (quant != 0){
    //abrupt decreasing (that means there was an abrupt increasing in delay phase)
    if (nb_sample == 0){
      incr = quant*begin_value;
    }else{
      incr = quant*begin_value/nb_sample;
    }
  }

  if (objective != 0){
    //assert
    // assert ( (objective - begin_value) > 0 ); // Désactivé pour flexibilité bidirectionnelle

    //abrupt decreasing (that means there was an abrupt increasing in delay phase)
    if (nb_sample == 0){
      incr = (objective - begin_value);
    }else{
      incr = (objective - begin_value)/nb_sample;
    }
  }

  return -incr + var;

}
//=====================================================trend 5=====================================
float Impact::quantify_impact_5 (float base_value, bool plateau_oscillation)
{
  if (with_in_delay()){
    /*cout << "delay" << endl; //comm off*/
    return quantify_impact_delay_5 (base_value);

  }else if (with_in_plateau()){
    /*cout << "plateau" << endl; //comm off*/
    return quantify_impact_plateau_5(base_value, plateau_oscillation);

  }else if (with_in_decay()) {
    /*cout << "decay" << endl; //comm off*/
    assert (trendE == 1);
    return quantify_impact_decay_5 (base_value);
    
  }else if(is_deactivated()){
    /*cout << "deactivated" << endl; //comm off*/
    return quantify_impact_deactivated_5(base_value);

  }else{
    string error_sms = "ERROR: class Impact: in function quantify_impact_5, invalid phase found!\n";
    cerr << error_sms;
    throw error_sms;
  }

}
//=========================================================
float Impact::quantify_impact_deactivated_5(float base_value)
{
  //-----instantaneous/missed Impact: 2*delay + plateauDur < sampling frequency delta_t
  if(sample_index == 1){
    /*cout << "WARNING: class Impact: one instantaneous/missed Impact, "
	 << "i.e. 2*delay + plateauDur < sampling frequency, occurs at time "
	 << to_string(begin_time) << endl; //comm off*/
    return 0.0;
  }

  //-----if zero samples have been generated in decay phase
  //here we add the missed increasing of the target parameter
  if(!with_in_decay(sample_index-1)){

    /*cout << "WARNING: class Impact: in function quantify_impact_deactivated_5: "
	 << "Event that began at " << get_begin_time() <<  "s: " <<  get_parameter() 
	 << ": zero samples have been generated in decay phase!\n"
	 << endl; //comm off */
    
    assert ( quant != 0 || objective != 0 );
    assert ( quant == 0 || objective == 0 );
    
    if(quant != 0){ //trend 4 modeled by quant
      return quant * begin_value;
	
    }else{ //trend 4 modeled by objective
      // assert(begin_value > objective); // Désactivé pour flexibilité bidirectionnelle
      return  begin_value - objective;
    }

  //-----in the other cases
  }else{
    return 0.0;
  }
}
//=========================================================
float Impact::quantify_impact_plateau_5 (float base_value, bool plateau_oscillation)
{

  //assertion
  assert ( quant != 0 || objective != 0 );
  assert ( quant == 0 || objective == 0 );

  /*if(sample_index == 1){
    cout << "WARNING: class Impact: in function quantify_impact_plateau_5: "
	 << "Event that began at " << get_begin_time() <<  "s: " << get_parameter()
	 << ": zero samples have been generated in delay phase!\n"
	 << endl; //comm off
  }*/
  
  //-----trend 5 modeled by quant
  if(quant != 0){
    
    float plateau_value = begin_value - quant*begin_value;
    float oscillation = 0.0;
    if(plateau_oscillation){
      oscillation = quantify_impact_stationary(plateau_value, 0.0499);
    }

    //if zero samples have been generated in delay phase
    //here we add the missed decreasing of the target parameter
    if(sample_index == 1){
      return  - (quant*begin_value);
    }else{
      return oscillation;
    }

  //-----trend 5 modeled by objective
  }else{
    
    float oscillation = 0.0;
    if(plateau_oscillation){
      oscillation = quantify_impact_stationary(objective, 0.0499);
    }

    //if zero samples have been generated in delay phase
    //here we add the missed decreasing of the target parameter
    if(sample_index == 1){
      // assert(begin_value > objective); // Désactivé pour flexibilité bidirectionnelle
      return  - (begin_value - objective);
    }else{
      return oscillation;
    }
  }

}
//=========================================================
float Impact::quantify_impact_delay_5(float base_value)
{

  //assertion
  assert ( quant != 0 || objective != 0 );
  assert ( quant == 0 || objective == 0 );

  int nb_sample = convert_to(delay, delay_unity, delta_t_unity)/delta_t;
  assert(nb_sample > 0);
  float incr;

  if (quant != 0){
    incr = quant*begin_value/nb_sample;
  }

  if(objective != 0){

    //assertion
    // assert ( (begin_value - objective) > 0 ); // Désactivé pour flexibilité bidirectionnelle

    incr = (begin_value - objective)/nb_sample;
  }

  return  - incr;

}
//=========================================================
float Impact::quantify_impact_decay_5(float base_value)
{

  //assertion
  assert ( quant != 0 || objective != 0 );
  assert ( quant == 0 || objective == 0 );

  //zero samples have been generated in delay and plateau phases
  if(sample_index == 1){
    /*cout << "WARNING: class Impact: Event that began at " 
	 << get_begin_time() << "s: " << get_parameter() 
	 << ": zero samples have been generated in delay and plateau phases!\n"
	 << endl; //comm off */
    return 0.0;
  }

  int tmp = convert_to(delay, delay_unity, delta_t_unity) + \
      convert_to(plateauDur, plateauDur_unity, delta_t_unity);
  tmp = tmp%delta_t;
  int nb_sample = (tmp + convert_to(delay, delay_unity, delta_t_unity)) / delta_t;
  float incr;

  //compensation of the last oscillation in plateau phase
  float var = 0.0;
  if(with_in_plateau(sample_index - 1)){
    var = -prev_oscillation_val;
  }

  if (quant != 0){
    //abrupt decreasing (that means there was an abrupt increasing in delay phase)
    if (nb_sample == 0){
      incr = quant*begin_value;
    }else{
      incr = quant*begin_value/nb_sample;
    }
  }

  if (objective != 0){

    //assert
    // assert ( (begin_value - objective) > 0 ); // Désactivé pour flexibilité bidirectionnelle

    //abrupt decreasing (that means there was an abrupt increasing in delay phase)
    if (nb_sample == 0){
      incr = (begin_value - objective);
    }else{
      incr = (begin_value - objective)/nb_sample;
    }
  }

  return incr + var;

}

//===============================================SERIALIZATION WITH BOOST=========================
template<class Archive>
void Impact::serialize(Archive &ar, const unsigned int version)
{

  //-------------------General fields serialization
  ar & parameter;                                                        // target parameter
  ar & delay_inf & delay_sup & delay & delay_unity;                      //delay
  ar & plateauDur_inf & plateauDur_sup & plateauDur & plateauDur_unity; //plateau
  //------------------Fields for trend modeling
  ar & trend & trendE & quant_inf & quant_sup & quant & objective_inf & objective_sup & objective;
  //------------------Sample_index
  ar & sample_index;
}
