//============================================================================================================================================
// Name        : 
// Author      : 
// Version     :
// Copyright   : 
// Description : 
//============================================================================================================================================

#include "Event.h"
#include "Impact.h"


//*******************************************CONSTRUCTORS/DESTROYER***********************************

Event::Event()
{

}
//====================================================================================================
Event::Event(const Event &event)
{
  
  //copy field impacts
  impacts = vector<Impact*>();
  for(Impact *imp: event.impacts){
    //call to Impact class copy constructor
    if(imp->get_impact_type() == "ImpactDrug"){
      impacts.push_back(new ImpactDrug( (ImpactDrug&)(*imp) ) );

    }else if(imp->get_impact_type() == "ImpactGest"){
      impacts.push_back(new ImpactGest( (ImpactGest&)(*imp) ) );

    }else{
      string error_sms = "ERROR: class Event: copy constructor: invalid type of Impact!\n";
      cerr << error_sms;
      throw error_sms;
    }
  }

  //copy field break_events, class vector copy constructor
  break_events = vector<string>(event.break_events);

  //copy other fields
  name = event.get_name();
  typeE = event.get_typeE();
  id = event.get_id();
  unique = event.is_unique();

}
//=================================================================================================
Event::Event(string name_, string type, string num, vector<Impact*> imp, vector<string> b_events, bool uniqueness)
{
  
  string error_sms;
  id = num;
    
  // typeE = A, R or D
  if (type=="A" || type=="R" || type == "D"){
    typeE = type;
    
  }else{
    error_sms = "ERROR: class Event: undefined Event type " + type + " !\n";
    cerr << error_sms;
    throw error_sms;
  }

  name = name_;
  impacts = imp;
  break_events = b_events;
  unique = uniqueness;
  
}
//=====================================================================================================
Event::~Event()
{
  //cout << "Class Event: destructor:" << endl; //comm off
  for (Impact *imp: impacts){
    //cout << imp->toString() << endl;         //comm off
    delete imp;
  }
}

//**********************************************VIRTUAL********************************************
string Event::toString ()
{
  
  string fields = "Class Event fields: " + name + ", type = " + typeE + ", id = " + id + ", uniqueness = " + to_string(unique) + "\n \n";
  
  //add impacts
  for (Impact *imp: impacts){
    fields = fields + imp->toString() + "\n";
  }

  //add break_events
  fields = fields + "Break Event Ids: ";
  for (string id: break_events){
    fields = fields + id + ", ";
  }
  fields = fields + "\n";
  
  return fields;
  
  //return typeE + "-" + to_string(id); //minimal version
  
}

//**************************************************GETTERS******************************
string Event::get_name() const
{
  return name;
}

string Event::get_typeE() const
{
  return typeE;
}

string Event::get_id() const
{
  return id;
}

bool Event::is_unique() const
{
  return unique;
}

//*******************************************SERIALIZATION WITH BOOST******************************
template<class Archive>
void Event::serialize(Archive &ar, const unsigned int version)
{

  ar & name & typeE & id & unique & impacts & break_events;

}
