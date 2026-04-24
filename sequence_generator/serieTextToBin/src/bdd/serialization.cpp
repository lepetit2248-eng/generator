//============================================================================
// Name        :
// Author      :
// Version     :
// Copyright   :
// Description :
//============================================================================

//stream
#include <fstream>
#include <iostream>
#include <sstream>

//vector
#include <vector>
using namespace std;

// Boost archives for binary and text format
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
// in order  pour la sérialisation de std::vector
#include <boost/serialization/vector.hpp>
using namespace boost::archive;

//my classes
#include "Impact.h"
#include "Event.h"


//============================================================================================================================================
/*
  Template for serialization of class P instances
    - P can be whatever Event, i.e. Drug, Gesture or Descriptor, or a vector of events, i.e. vector<Event*>.
    - T is the type of archive to use, i.e. output file format, it can be equal  text_oarchive or binary_oarchive
*/
template <class T, class P>
void saveIntoFile(const P &event, const char* file){

  //create output stream and archive
  ofstream ofile(file);
  T oar(ofile);

  //add serializable pointors of derived classes in Boost table
  oar.register_type(static_cast<ImpactDrug *>(NULL));
  oar.register_type(static_cast<ImpactGest *>(NULL));
  oar.register_type(static_cast<Drug *>(NULL));
  oar.register_type(static_cast<Gesture *>(NULL));
  oar.register_type(static_cast<Descriptor *>(NULL));

  //add possible upcasting in Boost table
  boost::serialization::void_cast_register(static_cast<ImpactDrug *>(NULL),static_cast<Impact *>(NULL));
  boost::serialization::void_cast_register(static_cast<ImpactGest *>(NULL),static_cast<Impact *>(NULL));

  boost::serialization::void_cast_register(static_cast<Descriptor *>(NULL),static_cast<Event *>(NULL));
  boost::serialization::void_cast_register(static_cast<Action *>(NULL),static_cast<Event *>(NULL));
  boost::serialization::void_cast_register(static_cast<Drug *>(NULL),static_cast<Action *>(NULL));
  boost::serialization::void_cast_register(static_cast<Gesture *>(NULL),static_cast<Action *>(NULL));

  //serialization
  oar << event;

  //close output stream
  ofile.close();
}


/*
  Template for deserialization of class P instances
    - P can be whatever Event, i.e. Drug, Gesture or Descriptor, or a vector of events, i.e. vector<Event*>.
    - T is the type of archive to use, i.e. output file format, it can be equal  text_iarchive or binary_iarchive
*/
template <class T, class P>
void getFromFile(P &event, const char* file){

  //create output stream and archive
  ifstream ifile(file);
  T iar(ifile);

  //add serializable pointors of derived classes in Boost table
  iar.register_type(static_cast<ImpactDrug *>(NULL));
  iar.register_type(static_cast<ImpactGest *>(NULL));
  iar.register_type(static_cast<Drug *>(NULL));
  iar.register_type(static_cast<Gesture *>(NULL));
  iar.register_type(static_cast<Descriptor *>(NULL));

  //add possible upcasting in Boost table
  boost::serialization::void_cast_register(static_cast<ImpactDrug *>(NULL),static_cast<Impact *>(NULL));
  boost::serialization::void_cast_register(static_cast<ImpactGest *>(NULL),static_cast<Impact *>(NULL));

  boost::serialization::void_cast_register(static_cast<Descriptor *>(NULL),static_cast<Event *>(NULL));
  boost::serialization::void_cast_register(static_cast<Action *>(NULL),static_cast<Event *>(NULL));
  boost::serialization::void_cast_register(static_cast<Drug *>(NULL),static_cast<Action *>(NULL));
  boost::serialization::void_cast_register(static_cast<Gesture *>(NULL),static_cast<Action *>(NULL));

  //serialization
  iar >> event;

  //close output stream
  ifile.close();

}
