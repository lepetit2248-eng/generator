#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <cstring>
#include <fstream>
#include "bdd/serialization.cpp"
#include "bdd/Event.cpp"
#include "bdd/Action.cpp"
#include "bdd/Drug.cpp"
#include "bdd/Gesture.cpp"
#include "bdd/Descriptor.cpp"
#include "bdd/Impact.cpp"
#include "bdd/ImpactGest.cpp"
#include "bdd/ImpactDrug.cpp"

using namespace std;


void textToBin(string textPath,string drugPath, string actionPath){
    ifstream  file;
    ifstream  binfile;
    string type;
    string evtName;
    string trash;
    string event_file_name;
    vector<Event*> event;
    int error_count = 0;


    // Open file to convert
    file.open(textPath);



    if(file){
        cout<<"BEGIN TEXT FILE READING"<<type<<endl;
        getline(file, trash);
        while(!file.eof()){

            

            getline(file,trash,',');
            getline(file,type,'-');
            getline(file,evtName,',');
            getline(file,trash);

            cout<<"Event type : "<<type<<endl;
            cout<<"Event Name : "<<evtName<<endl;
            
            if(type.size() == 3){
                if(type[0] == 'A'){
                    if(type[1] == 'D' && type[2] == 'G'){
                        cout<<"Event subtype : gesture(GE)"<<endl;

                        //create filename
                        event_file_name=drugPath+"/"+evtName+".bin";

                        //check
                        binfile.open(event_file_name); //Open file stream

                        if(binfile){ //opening is ok!
                            //create the drug object
                            Drug drug_load_bin =  Drug();
                            //formating the file name
                            char file_name[event_file_name.size() + 1];
                            strcpy(file_name, event_file_name.c_str());	// or pass &s[0]
                            //get the data from the file
                            getFromFile<binary_iarchive, Drug>(drug_load_bin, file_name);//deserialisation
                            //create event pointer
                            Drug *drug_copy = new Drug (drug_load_bin);
                            //add to the vector
                            event.push_back(drug_copy);
                        
                        }
                        else{
                            cout << "Error : Impossible to get file : "<< event_file_name << endl;
                            error_count++;
                        }
                        binfile.close();
                    }    
                    else if(type[1] == 'G' && type[2] == 'E'){
                        cout<<"Event subtype : gesture(GE), "<<endl;

                        //create filename
                        event_file_name=actionPath+"/"+evtName+".bin";
                        //check
                        binfile.open(event_file_name); //Open file stream
                        if(binfile){ //opening is ok!
                        //create the gesture object
                        Gesture gesture_load_bin = Gesture();
                        //formating the file name
                        char file_name[event_file_name.size() + 1];
                        strcpy(file_name, event_file_name.c_str());	// or pass &s[0]
                        //get the data from the file
                        getFromFile<binary_iarchive, Gesture>(gesture_load_bin, file_name);//deserialisation
                        //create event pointer
                        Gesture *gesture_copy = new Gesture(gesture_load_bin);
                        //add to the vector
                        event.push_back(gesture_copy);
                        
                        }
                        else{
                            cout << "Error : Impossible to get file "<< event_file_name << endl;
                            error_count++;
                        }
                        binfile.close();
                    }    
                    else{
                        cout << "Error : event type not found for event "<< type <<endl;
                        error_count++;
                    }
                }
                else if(type.size()==1){
                    if(type[0]=='D'){//descriptor event
                        cout<<"Event type : descriptor(d) ";
                        //create filename
                        event_file_name+="descriptors/"+event_file_name+".bin";
                        //check
                        binfile.open(event_file_name); //Open file stream
                        if(binfile){ //opening is ok!
                            //create the descriptor object
                            Descriptor descriptor_load_bin = Descriptor();
                            //formating the file name
                            char file_name[event_file_name.size() + 1];
                            strcpy(file_name, event_file_name.c_str());	// or pass &s[0]
                            //get the data from the file
                            getFromFile<binary_iarchive, Descriptor>(descriptor_load_bin, file_name);//deserialisation
                            //create event pointer
                            Descriptor *descriptor_copy = new Descriptor(descriptor_load_bin);
                            //add to the vector
                            event.push_back(descriptor_copy);
                        }else{
                            cout << "Error : Impossible to get file "<< event_file_name << endl;
                            error_count++;
                        }
                        binfile.close();
                    }
                }    

            }
            cout<<"----------------------------------------------------------------------"<<endl;  
        }
    }
    else{
        cout << "Error : Impossible open sequence file " << endl;
        error_count++;
        exit(0);
    }
    file.close();

    cout<<"END TEXT FILE READING "<<type<<endl;

    cout<<"File read with "<< error_count<<" errors"<< endl<< endl;

    if(error_count==0){


        for(Event* evtE : event){
            //cout << evtE->toString()<<endl;
        }
        cout << "\nSequence can now be serialized." << endl;
        saveIntoFile<binary_oarchive, vector<Event*>> (event, "../output/event_sequence.bin");
        cout << "Sequence exported in bin file : \"event_sequence.bin\"." << endl;
        for (size_t i = 0; i < event.size(); i++){
            delete event[i];
        }
        event.clear();

        getFromFile<binary_iarchive, vector<Event*>> (event, "event_sequence.bin");
        if(event.size()>0){
            cout<<"Sequence check ok !"<<endl;
            for (size_t i = 0; i < event.size(); i++){
                cout << event[i]->get_name()<<endl;
                delete event[i];
            }
            cout << "end checking" <<std ::endl;
        }else{
            cout<<"Error in the sequence !"<<endl;
        }
    }else{
        cout << "As there is error the program can't go further " << endl;
        exit(0);
        
    }
}

int main(int argc, char **argv){
    if(argc==4){
        // Ficher texte à transformer
        string text_file = argv[1];

        // db des medicament
        string drug_path = argv[2];

        // db des gestes médicaux
        string gesture_path = argv[3];

        textToBin(text_file,drug_path,gesture_path);
        return 1;
    }else{
        cout<<"Need 3 arguments : (1) path to the sequence (text format), (2) the path to the drug event database and (3) the path to the gesture event database"<<endl;
        return 0;
    }
} 