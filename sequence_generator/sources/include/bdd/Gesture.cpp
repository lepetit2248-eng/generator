//============================================================================
// Name        : 
// Author      : 
// Version     :
// Copyright   : 
// Description : 
//============================================================================

#include "Event.h"
#include "Impact.h"


//**************************************CONSTRUCTOR***********************************************
Gesture::Gesture() : Action()
{

}
//====================================================================================================
Gesture::Gesture(const Gesture &gesture) : Action(gesture)
{

}
//====================================================================================================
Gesture::Gesture(string id, string name, vector<Impact*> imp, vector<string> b_events, bool uniqueness) : Action (id, name, "GE", imp, b_events, uniqueness)
{
  
}
//**********************************************VIRTUAL********************************************

string Gesture::toString()
{
  
  string fields = "Gesture: no fields \n";
  return fields + Action::toString();

}
//***********************************SERIALIZATION WITH BOOST**************************************
template<class Archive>
void Gesture::serialize(Archive & ar, const unsigned int version)
{
  
  ar & boost::serialization::base_object<Action>(*this); // serialization of base class Action
  // Gesture class has no own fields

}
