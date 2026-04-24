//============================================================================================================================================================================================
/**!
  * \file event.h
  * \brief Top level class of the inheritance modelling Event types. Event is the base class of this inheritance, it has three derived class Action, Gesture and Reaction.
  * \author Fatoumata Dama
  * \version 1.0
  */
//============================================================================================================================================================================================

#ifndef DEF_EVENT
#define DEF_EVENT

//general inclusion
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <ctime> //for random generator seed setting

//boost serialization header files
#include "boost/serialization/access.hpp"
#include "boost/serialization/base_object.hpp"


using namespace std;

class Impact; //in order to avoid cycle inclusion


/***********************************************************************************************************************************************************************************************/
// Inheritance on the types of events
/**********************************************************************************************************************************************************************************************/

//*******************************************************************************Generic abstract Event class ***********************************************************************************
/**!
  * \class Event Event.h [<header-name>]
  *
  * \brief Top level class of Event types modelling inheritance.
  *  It is the base class of this inheritance, it is abstract.
  *
  */
class Event
{

//=====================================================================SERIALIZATION WITH BOOST================================================
private:

  friend class boost::serialization::access;

/**!
  * \fn void serialize (Archive &ar, const unsigned int version)
  * \brief
  * (1) \n
  * (2) Default constructor must be defined, it is used in deserialization function in order to build objects then set their fields. \n
  *
  * \param ar
  * \param version
  *
  */
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version);

//=====================================FIELDS==============================================================
public:

/**!
  * \brief vector of polymorphic pointers of Impact caused by the Event object.
  * \todo Try to put more safety here, make this field private or protected.
  */
  vector<Impact*> impacts;

/**!
  * \brief vector of Event IDs that stop the current Event, relevant when Event impacts do not dimmed.
  * \todo Try to put more safety here, make this field private or protected.
  */
  vector<string> break_events;

private:
  // Event name
  string name;

 //Event type, three possible values: A for Action, R for Reaction and D for Descriptor.
  string typeE;

  //Event ID.
  string id;

  //Tells us whether the event occurs at most once during a surgery, e.g. "induction" or closing eyelid.
  bool unique;

//===========================CONSTRUCTOR AND DESTROYER================================================

protected:

/**!
  * \brief Default constructor having zero argument, necessary for event deserialization.
  * As the class Event is abstract, this constructor is only callable from one constructor of derived classes.
  */
  Event();

/**!
  * \brief Copy constructor.
  * As the class Event is abstract, this constructor is only callable from one constructor of derived classes.
  *
  * \param event Object to copy.
  *
  */
  Event(const Event &event);

/**!
  * \brief Constructor of Event object, it takes 5 parameters.
  * As the class Event is abstract, this constructor is only callable from one constructor of derived classes.
  *
  * \param name Event name.
  * \param type Event type, three possible values: A for Action, R for Reaction and D for Descriptor.
  * \param num Event ID.
  * \param imp Vector of polymorphic pointers of Impact caused by the Event, see class Impact.
  * \param b_events Vector of Event IDs that stop the current Event.
  * \param uniqueness if true, that means the event appears at most once during a surgery, e.g. "induction" or closing eyelid.
  */
  Event(string name, string type, string num, vector<Impact*> imp, vector<string> b_events, bool uniqueness);

public:
/**!
  * \brief Destroyor of event object.
  * Releases memory dynamically allocated, i.e. field impacts whose elements are pointors.
  */
  ~Event();

//================================================VIRTUAL=================================================
//#virtual methods : will be also implemented by derived classes

public:
/**!
  * \fn string toString()
  * \brief
  *
  * \return String representation of Event.
  *
  * \todo Implement a minimal version for Hugo which returns "typeE-id".
  */
  virtual string toString ();

/**!
  * \fn string get_event_type() 
  * \brief
  *
  * \return
  *
  */
  virtual string get_event_type() const = 0;

//protected:
/**
  These two members are virtual pure.
  They won't be never called, they do nothing, they are only there in order to make some classes of our inheritance abstract.
  Therefore, they must be implemented by the non-abstract derived classes.
*/
 // virtual void serialization () = 0;
  //virtual void deserialization () = 0;

//================================================GETTERS==============================================
public:

/**!
  * \fn
  *
  * \return The name of event.
  */
  string get_name() const;

/**!
  * \fn
  *
  * \return The event type.
  */
  string get_typeE() const;

/**!
  * \fn
  *
  * \return The event id.
  */
  string get_id() const;

/**!
  * \fn
  *
  * \return The unique field which equals true if the event is unique.
  */
  bool is_unique() const;

};


//******************************************************************************* Event of type Action ***********************************************************************************
/**!
  * \class Action Event.h [<header-name>]
  *
  * \brief Action type of Event, derived from Event class, it is also abstract.
  * It has two derived classes corresponding to the two types of Action event: Drug and Gesture.
  */
class Action: public Event
{


//=====================================================================SERIALIZATION WITH BOOST================================================

private:

  friend class boost::serialization::access;

/**!
  * \fn
  * \brief
  * (1)
  * (2) Default constructor must be defined, it is used in deserialization function in order to build objects then set their field.
  *
  * \param ar
  * \param version
  *
  */
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version);


//===========================================FIELDS=======================================================

private:

  //brief DG for drug and GE for gesture
  string typeA;

//=================================================CONSTRUCTOR=============================================
//

protected:

/**!
  * \brief Default constructor having zero argument, necessary for Action deserialization.
  * As the class Action is abstract, this constructor is only callable from one constructor of derived classes.
  */
  Action();

/**!
  * \brief Copy constructor.
  * As the class Action is abstract, this constructor is only callable from one constructor of derived classes.
  *
  * \param action
  *
  */
  Action(const Action &action);

/**!
  * \brief Contructor of Action object with 5 arguments.
  * As the class Action is abstract, this constructor is only callable from one constructor of derived classes.
  *
  * \param id Event ID.
  * \param name_ Event name.
  * \param type Action type, two possible values: DG for Drug and GE for Gesture.
  * \param imp Vector of polymorphic pointers of Impact caused by the Action type event, see class Impact.
  * \param b_events Vector of Event IDs that stop the current Event. Default value is empty vector.
  * \param uniqueness if "true", that means the event appears at most once during a surgery, e.g. "induction" or closing eyelid.
  * Default value is "false".
  *
  */
  Action(string id, string name_, string type, vector<Impact*> imp, vector<string> b_events={}, bool uniqueness=false);

//============================================VIRTUAL===================================================
//

public:

/**!
  * \fn string toString()
  * \brief
  *
  * \return String representation of Action.
  *
  * \todo Implement a minimal version for Hugo.
  */
  virtual string toString ();

  string get_event_type() const;

//============================================GETTERS===================================================
public:

/**!
  * \fn string get_typeA()
  *
  * \return The type of Action under consideration, DG for drug and GE for gesture.
  *
  */
  string get_typeA() const;

};


/******************************************************************************* Action of type Drug ***********************************************************************************
/**!
  * \class Drug Event.h [<header-name>]
  *
  * \brief Drug type of Action, derived from Action class. \n
  *  For each drug, there are three possible modalities of injection: "induction", "entretien continu" and "entretien bolus". \n
  *  Each modality has a specific dose that depends of the characteristic of the patient, e.g. weight or age. \n
  *  In "induction", the drug is administered once in order to put the patient to sleep. \n
  *  In "entretien continu", it is continuously administered to the patient all along the surgery. \n
  *  In "entretien bolus", it is administered at a given frequency. \n
  *  Each modality corresponds to an instance of class Drug. \n
  *
  */
class Drug: public Action
{

//=====================================SERIALIZATION WITH BOOST=====================================
public:

/**!
  * \fn
  * \brief
  * (1) \n
  * (2) Default constructor must be defined, it is used in deserialization function in order to build objects then set their field.
  *
  * \param ar
  * \param version
  *
  */
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);


//========================================GENERAL FIELDS=============================================
private:

  /*dose_inf <= dose_sup*/
  float dose_inf;
  float dose_sup;

  /*injected dose must be  (to be set by Hugo during event trace generation)*/
  float dose;

  /*the unity in which the dose is expressed, e.g. mcg/kg or mcg/kg/h*/
  string dose_unity;

  /*the weight used in dose, can be "Poids réel", "Poids idéal théorique" or "Poids justé"*/
  string used_weight;

//===================================FIELDS FOR BOLUS INJECTION ONLY=================================
private:

  /*in bolus injection, drug is administered at a frequency taken within interval [frequency_inf, frequency_sup]*/
  int frequency_inf;
  int frequency_sup;

  /*unity in which the frequency is expressed*/
  string frequency_unity;

//================================================CONSTRUCTOR==========================================
public:

/**!
  * \brief Default constructor having zero argument, necessary for Drug deserialization.
  */
  Drug();

/**!
  * \brief Copy constructor.
  *
  * \param drug Object to copy.
  *
  */
  Drug(const Drug &drug);

/**!
  * \brief Contructor of Drug object. Field dose is initialized at -1
  *
  * \param id Event ID.
  * \param name Event name.
  * \param imp Vector of polymorphic pointers of Impact caused by the Drug type Action, see class Impact.
  * \param do_inf The lower bound of dose interval.
  * \param do_sup The righer bound of dose interval.
  * \param unity The unity in which the dose is expressed, e.g. mcg/kg or mcg/kg/h.
  * \param u_weight The weight used in dose unity, three possible values: "Poids réel", "Poids idéal théorique" or "Poids justé"
  * \param b_events Vector of Event IDs that stop the current Event. Default value is empty vector.
  * \param uniqueness if "true", that means the event appears at most once during a surgery, e.g. "induction" or closing eyelid.
  *  Default value is "false".
  * \param freq_inf In "entretien bolus" context, the lower bound of "re-injection" frequency.
  * \param freq_sup In "entretien bolus" context, the righer bound of "re-injection" frequency.
  * \param freq_unity In "entretien bolus" context, the unity in which the "re-injection" frequency is expressed.
  */
  Drug(string id, string name, vector<Impact*> imp, float do_inf, float do_sup, string unity, string u_weight, vector<string> b_events={}, bool uniqueness=false, int freq_inf=0.0, int freq_sup=0.0, string freq_unity="na");

//================================================VIRTUAL==========================================
public:

/**!
  * \fn string toString()
  * \brief
  *
  * \return String representation of Action.
  *
  * \todo Implement a minimal version for Hugo.
  */
  virtual string toString ();

//============================================SETTER==========================================
public:

/**!
  * \fn void set_dose(float d)
  * \brief set the administered dose of drug at given value.
  *
  * \todo To be set by Hugo during event trace generation.
  *
  * \param d The administred dose, this value is patient-dependent.
  */
  void set_dose(float d);
//===================================GETTER GENERAL FIELDS==========================================
public:

/**!
  * \fn float get_dose_inf()
  *
  * \return The lower bound of drug dose interval.
  */
  float get_dose_inf() const;

/**!
  * \fn float get_dose_sup()
  *
  * \return The upper bound of drug dose interval.
  */
  float get_dose_sup() const;

/**!
  * \fn float get_dose()
  *
  * \return The administered dose of drug.
  */
  float get_dose() const;

/**!
  * \fn string get_dose_unity()
  *
  * \return Dose unity.
  */
  string get_dose_unity() const;

/**!
  * \fn string get_used_weight()
  *
  * \return Dose the used weight in dose unity.
  */
  string get_used_weight() const;
//===================================GETTER FIELDS FOR BOLUS INJECTION ONLY===========================
public:

/**!
  * \fn get_frequency_inf()
  *
  *\return the lower bound of "re-injection" frequency.
  */
  int get_frequency_inf() const;

/**!
  * \fn get_frequency_sup()
  *
  *\return the upper bound of "re-injection" frequency.
  */
  int get_frequency_sup() const;

/**!
  * \fn get_frequency_unity()
  *
  *\return The unity of th "re-injection" frequency.
  */
  string get_frequency_unity() const;

};


//******************************************************************************* Action of type Gesture ***********************************************************************************
/**!
  * \class Gesture Event.h [<header-name>]
  *
  * \brief Gesture type of Action, derived from Action class.
  *
  */
class Gesture: public Action
{

//=====================================SERIALIZATION WITH BOOST=====================================
public:

/**!
  * \fn
  * \brief
  * (1)
  * (2) Default constructor must be defined, it is used in deserialization function in order to build objects then set their field.
  *
  * \param ar
  * \param version
  */
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);

//================================================CONSTRUCTOR==========================================
public:

/**!
  * \brief Default constructor having zero argument, necessary for Gesture deserialization.
  */
  Gesture();

/**!
  * \brief Copy constructor.
  *
  * \param gesture Object to copy.
  *
  */
  Gesture(const Gesture &gesture);

/**!
  * \brief Constructor of Gesture object.
  *
  * \param id Event ID.
  * \param name Event name.
  * \param imp Vector of polymorphic pointers of Impact caused by the Drug type Action, see class Impact.
  * \param b_events Vector of Event IDs that stop the current Event. Default value is empty vector.
  * \param uniqueness if "true", that means the event appears at most once during a surgery, e.g. "induction" or closing eyelid.
  *  Default value is "false".
  */
  Gesture(string id, string name, vector<Impact*> imp, vector<string> b_events={}, bool uniqueness=false);

//================================================VIRTUAL==========================================
public:

/**!
  * \fn string toString()
  * \brief
  *
  * \return String representation of Action.
  *
  * \todo Implement a minimal version for Hugo.
  */
  virtual string toString ();

};

//******************************************************************************* Event of type Reaction ***********************************************************************************
/**!
  * \class Reaction Event.h [<header-name>]
  *
  * \brief Reaction type of Event, derived from Event class.
  *
  */
class Reaction: public Event
{

//================================================CONSTRUCTORS=========================================
public:

/**!
  * \brief Default constructor having zero argument, necessary for Gesture deserialization.
  */
  Reaction();

/**!
  * \brief Copy constructor.
  *
  * \param reaction Object to copy.
  *
  */
  Reaction(const Reaction &reaction);

};

//******************************************************************************* Event of type Descriptor ***********************************************************************************
/**!
  * \class Descriptor Event.h [<header-name>]
  *
  * \brief Descriptor type of Event, derived from Event class.
  *
  */
class Descriptor: public Event
{

//=====================================SERIALIZATION WITH BOOST=====================================
public:

/**!
  * \fn
  * \brief
  * (1)
  * (2) Default constructor must be defined, it is used in deserialization function in order to build objects then set their field.
  *
  * \param ar
  * \param version
  */
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);


//===============================================FIELDS================================================
private:

  //comment on the descriptor
  string comment;

//================================================CONSTRUCTORS=========================================
public:

/**!
  * \brief Default constructor having zero argument, necessary for Gesture deserialization.
  */
  Descriptor();

/**!
  * \brief Copy constructor.
  *
  * \param descriptor Object to copy.
  *
  */
  Descriptor(const Descriptor &descriptor);

/**!
  * \brief As Descriptor type of event has no impact (on the monitored parameters), field impacts of base class Event is set at empty.
  *
  * \param id Event ID.
  * \param name_ Event name.
  * \param uniqueness if "true", that means the event appears at most once during a surgery, e.g. "induction" or closing eyelid.
  *  Default value is "false".
  *
  */
  Descriptor(string id, string name_, string comm, bool uniqueness=false);


//================================================VIRTUAL==========================================
public:

/**!
  * \fn string toString()
  * \brief
  *
  * \return String representation of Descriptor.
  *
  * \todo Implement a minimal version for Hugo.
  */
  virtual string toString ();

  string get_event_type() const;
//================================================GETTER==========================================
public:

/**!
  * \fn string get_comment()
  * \brief
  *
  * \return The field comment.
  *
  */
  string get_comment() const;


};


#endif
