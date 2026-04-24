
//============================================================================
// Name        : 
// Author      : 
// Version     :
// Copyright   : 
// Description : 
//============================================================================

#include <assert.h>

#include "Event.h"
#include "Impact.h"


//*****************************************CONSTRUCTORS******************************************
Drug::Drug() : Action()
{

}
//=================================================================================================
Drug::Drug(const Drug &drug) : Action(drug)
{

  //general fields
  dose_inf = drug.get_dose_inf();
  dose_sup = drug.get_dose_sup();
  dose = drug.get_dose();
  dose_unity = drug.get_dose_unity();
  used_weight = drug.get_used_weight();

  //fields for bolus injection only
  frequency_inf = drug.get_frequency_inf();
  frequency_sup = drug.get_frequency_sup();
  frequency_unity = drug.frequency_unity;
  
}
//================================================================================================

//#TODO: check the given values of unity_dose and used_weight
Drug::Drug(string id, string name, vector<Impact*> imp, float do_inf, float do_sup, string unity, string u_weight, vector<string> b_events, bool uniqueness, int freq_inf, int freq_sup, string freq_unity) : Action(id, name, "DG", imp, b_events, uniqueness)
{

  //Valid domain of values
  assert(do_inf >= 0 && do_sup >= 0);
  assert(freq_inf >= 0 && freq_sup >= 0);

  //--------------------------------GENERAL FIELDS
  if(do_inf > do_sup){
    string error_sms = "ERROR: class Drug: constructor, dose_inf must be less than or equal to dose_sup!\n";
    cerr << error_sms;
    throw error_sms;
  }	
    
  dose_inf = do_inf;
  dose_sup = do_sup;
  dose = -1;
  dose_unity = unity;
  used_weight = u_weight;

  //----------------------------FIELDS FOR BOLUS INJECTION ONLY
  if(freq_inf > freq_sup){
    string error_sms = "ERROR: class Drug: constructor, frequency_inf must be less than or equal to frequency_sup!\n";
    cerr << error_sms;
    throw error_sms;
  }	

  frequency_inf = freq_inf;
  frequency_sup = freq_sup;
  frequency_unity = freq_unity;

}

//*********************************************VIRTUAL*********************************************

string Drug::toString()
{
  
  string fields = "Drug: dose = [" + to_string(dose_inf) + "," + to_string(dose_sup) + "] "
    + to_string(dose) + " " + dose_unity + " " + used_weight + " || "
    + "injection frequency = [" + to_string(frequency_inf) + "," + to_string(frequency_sup) + "] "
    + frequency_unity + "\n";

  return fields + Action::toString();
  //return Action::toString() + "_" + to_string(dose); //minimal version
}

//**************************************************SETTER****************************************
void Drug::set_dose(float d)
{

  dose = d;

}
//***********************************GETTER GENERAL FIELDS****************************************

float Drug::get_dose_inf() const
{
  return dose_inf;
}

float Drug::get_dose_sup() const
{
  return dose_sup;
}

float Drug::get_dose() const
{
  return dose;
}

string Drug::get_dose_unity() const
{
  return dose_unity;
}

string Drug::get_used_weight() const
{
  return used_weight;
}
//***********************************GETTER FIELDS FOR BOLUS INJECTION ONLY***********************

int Drug::get_frequency_inf() const
{
   return frequency_inf;
}

int Drug::get_frequency_sup() const
{
  return frequency_sup;
}

string Drug::get_frequency_unity() const
{
  return frequency_unity;
}

//***********************************SERIALIZATION WITH BOOST*************************************
template<class Archive>
void Drug::serialize(Archive & ar, const unsigned int version)
{

  ar & boost::serialization::base_object<Action>(*this); // serialization of base class Action
  ar & dose_inf & dose_sup & dose & dose_unity & used_weight
     & frequency_inf & frequency_sup & frequency_unity; // serialization of Drug own fields
  
}
  
