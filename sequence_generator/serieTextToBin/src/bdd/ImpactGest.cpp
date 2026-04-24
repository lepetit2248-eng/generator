//=============================================================================================================================
// Name        : 
// Author      :  Fatoumata Dama
// Version     :
// Copyright   : 
// Description : 
//===============================================================================================================================

#include "Impact.h"


/**************************************************************CONSTRUCTORS***********************/

ImpactGest::ImpactGest() : Impact()
{

}
//================================================================================================
ImpactGest::ImpactGest(const ImpactGest &impGest) : Impact(impGest)
{
  varLin = impGest.get_varLin();
  varLin_unity = impGest.get_varLin_unity();
}

//=================================================================================================
ImpactGest::ImpactGest(string parameter, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int tren, int trenE, float var, string varLin_un) :
                                         Impact (parameter, d_inf, d_sup, d_unity, dur_inf, dur_sup, dur_unity, tren, trenE, 0.0, 0.0, 0.0, 0.0)
{

  //Valid domain of values
  assert(var >= 0);

  if(varLin_un == "h" || varLin_un == "mn" || varLin_un == "s"){
     varLin = var;
     varLin_unity = varLin_un;

  }else{
    string error_sms = "Error: class ImpactGest: constructor, \"" + varLin_un + "\" is an invalid unity !\n";
    cerr << error_sms;
    throw error_sms;
  }
  
  if (tren != 2 && tren != 3 ){
    string error_sms = "ERROR: class ImpactGest: constructor ImpactGest(string, int, int, string, int, int, string, int, int, float, string) can be only applied when trend equals 2 or 3 !\n";
    cerr << error_sms;
    throw error_sms;
  }

  // trend 2 and 3 does not dimp
  if (trenE == 1){
    string error_sms = "ERROR: class ImpactGest: constructor: trend 2 and 3 does not dimp so trendE must equal 0 !\n";
    cerr << error_sms;
    throw error_sms;
  }
  
}
//=================================================================================================
ImpactGest::ImpactGest(string parameter, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity,
		       int tren, int trendE, float q_inf, float q_sup, float obj_inf, float obj_sup) :
                       Impact (parameter, d_inf, d_sup, d_unity, dur_inf, dur_sup, dur_unity, tren, trendE, q_inf, q_sup, obj_inf, obj_sup)
{

  //non relevant field
  varLin = 0.0;
  varLin_unity = "na";

  if (tren != 4 && tren != 5){
    string error_sms = "ERROR: class ImpactGest: constructor ImpactGest(string, int, int, string, int, int, string, int, int, float, float, float, float) can be only applied when trend equals 4 or 5 !\n";
    cerr << error_sms;
    throw error_sms;
  }
  
}

//********************************************GETTERS*********************************************
float ImpactGest::get_varLin() const
{
  return varLin;
}

string ImpactGest::get_varLin_unity() const
{
  return varLin_unity;
}

//***************************************OTHER METHODS*********************************************
string ImpactGest::toString()
{

  string fields = "Class ImpactGest fields: varLin = " + to_string(varLin) +  "  varLin_unity = " + varLin_unity + "\n";
    
  return fields + Impact::toString();
  
}

//================================================================================================
string ImpactGest::get_impact_type() const
{
  return "ImpactGest";
}

//================================================================================================
float ImpactGest::quantify_impact (float base_value, bool plateau_oscillation)
{
  //next sample 
  sample_index = sample_index + 1;

  //check whether the impact has been activated (sample_index != -1)
  if(sample_index == -1){
    string error_sms = "ERROR: class ImpactGest: in function quantify_impact(...), you try to use an impact that has not been activated yet!\n";
    cerr << error_sms;
    throw error_sms;
  }
  assert(sample_index > 0);
  
  float quant_value;
    
  if (trend == 2){
    //cout << "trend=2" << endl; //comm off
    quant_value = quantify_impact_2(base_value);
    
  }else if (trend == 3){
    //cout << "trend=3" << endl; //comm off
    quant_value = quantify_impact_3(begin_value);

  }else if (trend == 4){
    //cout << "trend=4" << endl; //comm off
    quant_value = Impact::quantify_impact_4(base_value, plateau_oscillation);
    
  }else if (trend == 5){
    //cout << "trend=5" << endl; //comm off
    quant_value = Impact::quantify_impact_5(base_value, plateau_oscillation);

  }else{
    string error_sms = "ERROR: class ImpactDrug: in funtion quantify_impact(...), invalid trend found! \n";
    cerr << error_sms ;
    throw error_sms ;
  }
  
  return quant_value;

}

//************************INTERMEDIATE FUNCTIONS USED IN quantify_impact function******************
float ImpactGest::quantify_impact_2 (float base_value)
{
  
  int varLin_unity_in_s = convert_to(1, varLin_unity, "s");
  int delta_t_in_s = convert_to(delta_t, delta_t_unity, "s");
  float var_per_delta_t = varLin * delta_t_in_s / varLin_unity_in_s;
    
  return base_value*var_per_delta_t;

}
  

float ImpactGest::quantify_impact_3 (float base_value)
{

  int varLin_unity_in_s = convert_to(1, varLin_unity, "s");
  int delta_t_in_s = convert_to(delta_t, delta_t_unity, "s");
  float var_per_delta_t = varLin * delta_t_in_s / varLin_unity_in_s;

  return -base_value*var_per_delta_t;
  
}

//********************************SERIALIZATION WITH BOOST****************************************

template<class Archive>
void ImpactGest::serialize(Archive & ar, const unsigned int version)
{
  ar & boost::serialization::base_object<Impact>(*this); // serialization of base class
  ar & varLin & varLin_unity;                           // serialization of ImpactGest own fields
}
  
