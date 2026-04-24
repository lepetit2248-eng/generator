
//============================================================================
// Name        : 
// Author      :  
// Version     :
// Copyright   : 
// Description : 
//============================================================================

#include "Event.h"
#include "Impact.h"


//******************************************CONSTRUCTORS*****************************************
Descriptor::Descriptor() : Event()
{

}
//============================================================================
Descriptor::Descriptor(const Descriptor &descriptor) : Event(descriptor)
{
  comment = descriptor.get_comment();
}
//============================================================================
Descriptor::Descriptor(string id, string name_, string comm, bool uniqueness) : Event(name_, "D", id, vector<Impact*>(), vector<string>(), uniqueness)
{
  
  comment = comm;

}  
//*******************************************VIRTUAL**********************************************
string Descriptor::toString()
{

  string fields = "Descriptor: comment = " + comment + "\n";
  
  return fields + Event::toString();

  //return typeA + Event::toString(); //minimal version
  
}

string Descriptor::get_event_type() const
{
  return "D";
}
//*******************************************GETTER*********************************************
string Descriptor::get_comment() const
{
  return comment;
}

//*******************************************SERIALIZATION WITH BOOST**********************************
template<class Archive>
void Descriptor::serialize(Archive &ar, const unsigned int version)
{

  ar & boost::serialization::base_object<Event>(*this);  // serialization of base class Event
  ar & comment;                                         // serialization of Descriptor own fields

}



