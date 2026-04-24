//===============================================================================================================================
// Name        : 
// Author      : Fatoumata Dama
// Version     : 2.0 New physiologic parameters added. The generator an deal with variation among the sequence of events taken as input.
// Copyright   :
// Description : 
//=================================================================================================================================

#include <fstream>

#include "serialization.cpp"
#include "utils.cpp"

#include "Impact.cpp"
#include "ImpactDrug.cpp"
#include "ImpactGest.cpp"

#include "Event.cpp"
#include "Action.cpp"
#include "Drug.cpp"
#include "Gesture.cpp"
#include "Descriptor.cpp"


/**
 # @brief Usage: ./complete_anesthesia event_seq_file parameter_file sampling_freq output_dir nb_profiles
 # This program generates nb_profiles anesthesia profile corresponding to "unilateral hernie" surgery
 */
int main(int argc, char *argv[]){

  //Fix random generator seed: 
  srand(time(NULL));

  
  //----------------COMMAND LINE PARAMETERS READING
  const char *event_seq_file;
  string ref_timestamp_file;
  string parameter_file;
  const char *drug_evnt_database_path;
  int sampling_freq; //expressed in second
  string output_dir;
  int nb_profiles;
      
  if(argc == 8){
    event_seq_file = argv[1];
    ref_timestamp_file = string(argv[2]);
    parameter_file = string(argv[3]);
    drug_evnt_database_path = argv[4];

    sampling_freq = stoi( string(argv[5]) );
    if(sampling_freq != 30 && sampling_freq != 60 && sampling_freq != 90 && sampling_freq != 120
       && sampling_freq != 150 && sampling_freq != 180 && sampling_freq != 360){
      cout << "======================COMMAND LINE PARAMETERS======================" << endl;
      cout << "ERROR: The given sampling_freq value is incorrect."
	   << "Can take values 30, 60, 90 120, 150, 180 and 360, expressed in second. \n";
      exit(1);
    }

    output_dir = string(argv[6]);
    nb_profiles = stoi(argv[7]);

  }else{
    cout << "\n Usage: ./complete_anesthesia  event_seq_file  ref_timestamp_file  parameter_file  drug_evnt_database_path  sampling_freq  output_dir  nb_profiles \n\n"
	 << "  *event_seq_file: a binary file containing the sequence of events occuring during the surgery."
	 << "\n\n"
	 << "  *ref_timestamp_file: reference timestamps to be used to generate a unique timestamp for each simulated patient. "
	 << "A csv file with three columns: Time,ID,Name = (event occurrence times, event IDs and event names)."
	 << "\n\n"
	 << "  *parameter_file: a text file which contains the intervals of base values and begin values of patient parameters."
	 << "The format of this file is the following: \n"
	 << "\t #X_base_values \n\t min_val max_val \n" << "\t #X_begin_values \n\t min_val max_val \n"
	 << "     with X in [FC, PAS, PAM, PAD, temp, FiHAL, FeHAL, T1/T4, TOF]. "
	 << "     No empty space in the beginning of lines and no empty lines between information. \n"
	 << "     Only the parameters for which the previous information are provided will be similated by the generator."
	 << "\n\n"
	 << "  *drug_evnt_database_path: the path of drug events database to be used to perform \"entretien\". "
	 << "\n\n"
	 << "  *sampling_freq: the sampling frequency of the simulated time series depicted the dynamics of patient parameters. "
	 << "   Can take values 30, 60, 90 120, 150, 180 and 360, expressed in second."
	 << "\n\n"
	 << "  *output_dir: the directory in which the simulator's output files are saved."
	 << "\n\n"
	 << "  *nb_profiles: the number of profile to generate." << endl
	 << endl << "NB: For all directory paths the end character '/' must be added."
	 << "\n" << endl;

    exit(1);
  }

  //-------------Patients parameters and reference timestamp
  //----Physiologic parameters to be simulated are the one given in parameter_file 
  //PEGASE patients
  //vector<string> parameters = load_parameters_to_be_simulated(parameter_file + "1.txt");

  //standard patients
  vector<string> parameters = load_parameters_to_be_simulated(parameter_file);
  int nb_parameter = parameters.size();

  //----Load surgery reference timestamps
  vector<int> ref_timestamps = load_reference_timestamp(ref_timestamp_file);

  //LOG-info
  cout << endl << "Surgery reference timestamps = {" ;
  for(size_t i=0; i < ref_timestamps.size(); i++){ cout << to_string(ref_timestamps[i]) << ",  " ;}
  cout << "}" << "    " << "nb_entry = " << ref_timestamps.size() << endl << endl;
  
  //------------Simulation of nb_profiles profiles
  //Output files's basename. Todo: use join(output_dir, "parameters") function if there is any
  string output_file_basename = output_dir + "parameters";
  for(string param: parameters){ output_file_basename = output_file_basename + "-" + param; }

  //Simulation
  for(size_t i=1; i < nb_profiles+1; i++){

    //---draw random base values and begin values for physiologic parameters.
    //interval of base values and begin values are load from file parameter_file.
    vector<float> base_values(nb_parameter);
    vector<float> begin_values(nb_parameter);

    //---standard patients
    read_patient_parameters(parameter_file, parameters, base_values, begin_values);
   
    //---PEGASE patients
    //string ith_parameter_file = parameter_file + to_string(i) + ".txt";
    //read_patient_parameters(ith_parameter_file, parameters, base_values, begin_values);   
    
    //---output files for the i^th profile    
    string output_file_evnt = output_file_basename + "_events_" + to_string(i) + ".txt";
    string output_file_series = output_file_basename + "_series_" + to_string(i) + ".txt";

    //---log-info
    cout << "----------------------LOG-INFO: Patient " << to_string(i) << endl;
    cout << "Patient parameters " << endl;
    for(size_t j=0; j < nb_parameter; j++){
      cout << parameters[j] << ": base_value = " << to_string(base_values[j])
	   << ", begin_value = " << to_string(begin_values[j]) << endl;
    }
    cout << endl;
   
    //---i^th profile generation
    profile_generator(event_seq_file, ref_timestamps, drug_evnt_database_path, output_file_evnt,
		      output_file_series, sampling_freq, parameters, base_values, begin_values);
    
  }
  
}

