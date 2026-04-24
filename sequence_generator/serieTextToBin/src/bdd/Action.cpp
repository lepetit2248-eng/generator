
//============================================================================
// Name        : 
// Author      :  
// Version     :
// Copyright   : 
// Description : 
//============================================================================

#include "Event.h"
#include "Impact.h"


//******************************************CONSTRUCTORS******************************************
Action::Action() : Event()
{

}
//================================================================================================
Action::Action(const Action &action) : Event(action)
{

  typeA = action.get_typeA();
  
}
//================================================================================================
Action::Action(string id, string name_, string type, vector<Impact*> imp, vector<string> b_events, bool uniqueness) : Event(name_, "A", id, imp, b_events, uniqueness)
{

  typeA = type;

}  

//**************************************************VIRTUAL****************************************
string Action::toString()
{

  //return typeA + Event::toString(); //minimal version
  return Event::toString();
  
}

string Action::get_event_type() const
{
  return "A-" + typeA;
}

//************************************************GETTERS******************************************
string Action::get_typeA() const
{
  
  return typeA;

}

//*****************************************SERIALIZATION WITH BOOST********************************
template<class Archive>
void Action::serialize(Archive &ar, const unsigned int version)
{

  ar & boost::serialization::base_object<Event>(*this); // serialization of base class Event
  ar & typeA;                                          // serialization of Action own fields

}



