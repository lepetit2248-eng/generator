//=====================================================================================================
// Name        : 
// Author      : Fatoumata Dama
// Version     :
// Copyright   : 
// Description : 
//=====================================================================================================

#include "Impact.h"



//************************************************CONSTRUCTORS*************************************
ImpactDrug::ImpactDrug() : Impact()
{

}
//=================================================================================================
ImpactDrug::ImpactDrug(const ImpactDrug &impDrug) : Impact(impDrug)
{
  cstat = impDrug.get_cstat();
}

//=================================================================================================
ImpactDrug::ImpactDrug(string parameter, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int tren, int trenE) :
  Impact (parameter, d_inf, d_sup, d_unity, dur_inf, dur_sup, dur_unity, tren, trenE, 0.0, 0.0, 0.0, 0.0)
{

  //non relevant field
  cstat = 0.0;

  if (tren != 0){
    string error_sms = "ERROR: class ImpactDrug: the constructor ImpactDrug(string, int, int, string, int, int, string, int, int) is applicable for trend 0 only !\n";
    cerr << error_sms;
    throw error_sms;
  }
  
}

//=================================================================================================
ImpactDrug::ImpactDrug(string parameter, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int tren, int trenE, float ct) :
  Impact (parameter, d_inf, d_sup, d_unity, dur_inf, dur_sup, dur_unity, tren, trenE, 0.0, 0.0, 0.0, 0.0)
{

  //valid domain of value
  assert(ct >= 0 && ct <=1);
  
  cstat = ct;

  if (tren != 1){
    string error_sms = "ERROR: class ImpactDrug: the constructor ImpactDrug(string, int, int, string, int, int, string, int, int, float) is applicable for trend 1 only !\n";
    cerr << error_sms;
    throw error_sms;
  }
  
}
//=================================================================================================
ImpactDrug::ImpactDrug(string parameter, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int tren, int trenE, float q_inf, float q_sup, float obj_inf, float obj_sup) :
  Impact (parameter, d_inf, d_sup, d_unity, dur_inf, dur_sup, dur_unity, tren, trenE, q_inf, q_sup, obj_inf, obj_sup)
{
  
  //non relevant field 
  cstat = 0.0;

  if (tren != 4 && tren != 5){
    string error_sms = "ERROR: class ImpactDrug: the constructor ImpactDrug(string, int, int, string, int, int, string, int, int, float, float, float, float) is applicable for trend 4 and 5 only !\n";
    cerr << error_sms;
    throw error_sms;
  }
  
}

//******************************************GETTERS************************************************
float ImpactDrug::get_cstat() const
{
  return cstat;
}

//***************************************OTHERS METHODS********************************************
string ImpactDrug::toString()
{

  string fields = "Class ImpactDrug fields: cstat = " + to_string(cstat) + "\n";

  return fields + Impact::toString();
  
}

//================================================================================================
string ImpactDrug::get_impact_type() const
{
  return "ImpactDrug";
}

//=================================================================================================
float ImpactDrug::quantify_impact (float base_value, bool plateau_oscillation)
{
  //next sample
  sample_index = sample_index + 1;
  
  //check whether the impact has been activated (sample_index != -1)
  if(sample_index == -1){
    string error_sms = "ERROR: class ImpactDrug: in function quantify_impact(...), you try to use an impact that has not been activated yet!\n";
    cerr << error_sms;
    throw error_sms;
  }
  assert(sample_index > 0);
  
  float quant_value;
    
  if (trend == 0){
    //cout << "trend=0" << endl; //comm off
    quant_value = quantify_impact_0(base_value);
    
  }else if (trend == 1){
    //cout << "trend=1" << endl; //comm off
    quant_value = quantify_impact_1(begin_value);

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

  //cout << "  sample_index = " << sample_index << " sampling_time = " << to_string(Impact::delta_t*(sample_index-1) + begin_time) << endl; //comm off

  return quant_value;

}

//********************INTERMEDIATE FUNCTIONS USED IN quantify_impact function*********************

float ImpactDrug::quantify_impact_0 (float base_value)
{

  return Impact::quantify_impact_stationary (base_value, 0.0499);

}

float ImpactDrug::quantify_impact_1 (float plat_value)
{

  return Impact::quantify_impact_stationary (plat_value, cstat);    
    
}

//************************************SERIALIZATION WITH BOOST*************************************
template<class Archive>
void ImpactDrug::serialize(Archive & ar, const unsigned int version)
{
  ar & boost::serialization::base_object<Impact>(*this); // serialization of base class
  ar & cstat;                                           // serialization of ImpactGest own fields
}

  
