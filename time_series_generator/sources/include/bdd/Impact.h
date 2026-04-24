//====================================================================================================================================================================================
/**!
  * \file Imapct.h
  * \brief Top level class of the inheritance modelling Impact types. Impact is the base class of this inheritance, it has two derived classes ImpactDrug and ImpactGest.
  * \author Fatoumata Dama
  * \version 1.0
  */
//====================================================================================================================================================================================

#ifndef DEF_IMPACT
#define DEF_IMPACT

//general inclusion
#include <iostream>
#include <string>
#include <vector>
#include <utility>  //pair
#include <stdlib.h>
#include <time.h> //for random generator seed setting

// boost serialization header files
#include "boost/serialization/access.hpp"
#include "boost/serialization/base_object.hpp"

using namespace std;

class Event; //in order to avoid cycle inclusion


/*********************************************************************************************************************************************************************************************************/
// Inheritance on the types of impacts
/********************************************************************************************************************************************************************************************************/


//********************************************************************************************** Class Impact **********************************************************************************************
/**!
  * \class Impact Impact.h [<header-name>]
  * \brief Top level class of Impact types modelling inheritance.
  *  It is the base class of this inheritance, it is abstract.
  *
  */
class Impact
{

//=====================================================================SERIALIZATION WITH BOOST================================================

private:

  friend class boost::serialization::access;

/**!
  * \fn void serialize (Archive &ar, const unsigned int version)
  *
  * \brief
  * (1) In order to use boost serialization on class Impact, it must implement "serialize" function. This function specifies class fields to be serialized and in which order.
  * As Impact is the base class of our inheritance, its "serialize" function is declared private (in order to prevent derived classes to call it).
  * class boost::serialization::access is declared friend of Impact, this allows us to serialize fields of base class in derived class "serialize" functions.
  * (2) Only the general fields, fields for trend and field sample_index are serialized, i.e. the fields thet are set within the constructor.
  * The other parameters are not intrinsic to impact class, they are either patient depend or simulation depend.
  * (3) Default constructor must be defined, it is used in unserialization function in order to build objects then set their field.
  *
  * \param ar
  * \param version
  */
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version);

//========================================================================GENERAL FIELDS=======================================================
//#The following fields characterize Impact object
protected:

/**!
  * \brief Parameter impacted by the impact (called target parameter), must be a value within {FC, PAS, PAD, PAM, SpO2, FiO2, FeO2, DGF, FiCO2, FeCO2, FiNO2, FR, Vt, Vme, Pmoy, Pmax, PEP, temp, QC, RVS, FiHAL, FeHAL, T1/T4, TOF}
  */
  string parameter;

/**!
  * \brief The lower bound of delay interval. .... . Positive interger value.
  */
  int delay_inf; // >= 0

/**!
  * \brief The higher bound of delay interval. ... . Positive interger value.
  */
  int delay_sup; // >= 0

/**!
  * \brief The effective delay of the impact, randomly chosen within [delay_inf, delay_sup].
  */
  int delay;

/**!
  * \brief Unity in which the delay is expressed, three possible values: "s" for second, "mn" for minute and "h" for hour.
  */
  string delay_unity; // can be second "s", minute "mn" or hour "h"

/**!
  * \brief The lower bound of plateau duration interval. ... . Positive interger value.
  */
  int plateauDur_inf; // >= 0

/**!
  * \brief The higher bound of plateau duration interval. ... . Positive interger value.
  */
  int plateauDur_sup; // >= 0

/**!
  * \brief The effective plateau Duration of the impact, randomly chosen within [plateauDur_inf, plateauDur_sup].
  */
  int plateauDur;

/**!
  * \brief Unity in which the plateau duration is expressed, three possible values: "s" for second, "mn" for minute and "h" for hour.
  */
  string plateauDur_unity; // can be second "s", minute "mn" or hour "h"

//================================================================FIELDS FOR TREND MODELING=======================================================

protected:

/**!
  * \brief Codes that modelize the trend of impacts. Six possible trend have been modeled: \n
  * 0 stationary regime, the target parameter oscillate between ]-0.025v, 0.025*v[ where v is its value at the beginning of the impact. \n
  * 1 stationary regime, the target parameter oscillate between [v*cstat/2, -v*cstat/2] where v is its value at the beginning of the impact. \n
  * 2 linear increasing trend, target parmeter increases of varLin each varLin_unity. \n
  * 3 linear decreasing trend, target parmeter decreases of varLin each varLin_unity. \n
  * 4 increasing -> plateau -> decreasing. \n
  * 5 decreasing -> plateau -> increasing.
  */
  int trend;

/**!
  * \brief Codes that modelize the dimming of impacts. Two possibilies: \n
  * 0 impact does not dimmed, this is characterized by an infinite plateauDur which is stopped by the next event. \n
  * 1 the decay of impact is the reverse and symmetric of its delay phase. Impacts completly dimmed after 2*delay + plateauDur.
  */
  int trendE;

/**!
  * \brief This parameter is relevant for trend 4 or 5 only, its must be >= 0.
  *
  */
  float quant_inf;

/**!
  * \brief This parameter is relevant for trend 4 or 5 only, its must be >= 0.
  *
  */
  float quant_sup;

/**!
  * \brief This parameter is relevant for trend 4 or 5 only, its must be a value within [quant_inf, quant_sup].
  *
  */
  float quant; //in [quant_inf, quant_sup].

/**!
  * \brief This parameter is relevant for trend 4 or 5 only. Positive value.
  *
  */
  float objective_inf; // >= 0

/**!
  * \brief This parameter is relevant for trend 4 or 5 only. Positive value.
  *
  */
  float objective_sup; // >= 0

/**!
  * \brief This parameter is relevant for trend 4 or 5 only. its must be a value within [objective_inf, objective_sup].
  *
  */
  float objective; //in [objective_inf, objective_sup]

//=====================================================FIELDS USED IN TIME SERIES GENERATION=======================================================
//#The following fields are essentially used for the target parameter time series generation
//#They are initialized in function activate_impact(...) defined bellow.

protected:
  // begin context of the impact

/**!
  * \brief The time at which the Event that causes the impact occurs, expressed in second.
  *
  */
  int begin_time;

/**!
  * \brief parameter value at time begin_time-1.
  *
  */
  float begin_value;

  // sampling frequency and sample generation

/**!
  * \brief the sampling frequency, i.e. the time between sampled values of the target parameter.
  *  Precondition: delta_t <= delay + plateauDur
  *
  */
  int delta_t;

/**!
  * \brief The unity in which the sampling frequency is expressed, can be "s" second, "mn" minute and "h" hour.
  *
  */
  string delta_t_unity;

/**!
  * \brief Index of the next generated sample (start from 1).  \n
  * If equals -1, that means the Impact has not been activated, therefore parameter cannot be sampled. \n
  * Enable to know in which phase the sample is generated: delay phase, plateau phase and decay phase such that this information
  * (issue) becomes invisible to the program that generates time series. \n
  * The corresponding parameter value is associated with time: begin_time + delta_t*sample_index.
  */
  int sample_index; //global index


/**!
  * \brief Stationary regime management. Previous value added to the plateau value in stationary regime (trend=0,1 or in plateau phase).
  * This parameter avoid increasing/decreasing that leaves the oscillation window, e.g. outside -0.025*v to +0.025*v.
  */
  float prev_oscillation_val;

//================================================================CONSTRUCTOR======================================================================

public:

/**!
  * \brief Default constructor having zero argument, necessary for Impact deserialization.
  */
  Impact();

/**!
  * \brief Copy constructor.
  *
  * \param imp Object to copy.
  */
  Impact(const Impact &imp);

/**!
  * \brief Constructor of object Impact, as Impact is abstract it is only callable by by derived classes constructors,
  * set the fields delay, plateauDur, quant and objective at -1.
  *
  * \param param must be within {FC, PAS, PAD, PAM, SpO2, FiO2, FeO2, DGF, FiCO2, FeCO2, FiNO2, FR, Vt, Vme, Pmoy, Pmax, PEP, temp}.
  * \param d_inf The lower bound of the impact delay, positive value.
  * \param d_sup The higher bound of the impact delay, d_sup > d_inf.
  * \param d_unity The unity in which delay is expressed, "s" for second, "mn" for minute and "h" for hour.
  * \param dur_inf The lower bound of the plateau duration, positve value.
  * \param dur_sup The higher bound of the plateau duration, dur_sup > dur_inf.
  * \param dur_unity The unity in which plateau duration is expressed, "s" for second, "mn" for minute and "h" for hour.
  *
  * \param trend_ Trend code:
  * 0 stationary regime, the target parameter oscillate between ]-0.025v, 0.025*v[ where v is its value at the beginning of the impact. \n
  * 1 stationary regime, the target parameter oscillate between [v*cstat/2, -v*cstat/2] where v is its value at the beginning of the impact. \n
  * 2 linear increasing trend, target parmeter increases of varLin each varLin_unity. \n
  * 3 linear decreasing trend, target parmeter decreases of varLin each varLin_unity. \n
  * 4 increasing -> plateau -> decreasing. \n
  * 5 decreasing -> plateau -> increasing.
  *
  * \param trendE_ Dimming model: \n
  * 0 impact does not dimmed, this is characterized by an infinite plateauDur which is stopped by the next event. \n
  * 1 the decay of impact is the reverse and symmetric of its delay phase. Impacts completly dimmed after 2*delay + plateauDur.
  *
  * \param quant_inf_ The lower bound of quant, its must be a value within [0,1].
  * \param quant_sup_ The higher bound of quant, its must be a value within [0,1].
  * \param objective_inf_ The lower bound of objective, positive value.
  * \param objective_sup_ The higher bound of objective, positive value.
  *
  */
  Impact(string param, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int trend_, int trendE_,
	float quant_inf_, float quant_sup_, float objective_inf_, float objective_sup_);

//================================================================GETTERS: GENERAL FIELDS======================================================================
public:  

/**!
  * \fn string get_parameter().
  * \return The target parameter.
  */
  string get_parameter() const;


/**!
  * \fn int delay_inf().
  * \return The lower bound of delay interval.
  */
  int get_delay_inf() const; 
/**!
  * \fn int delay_sup().
  * \return The upper bound of delay interval.
  */
  int get_delay_sup() const;
/**!
  * \fn int get_delay().
  * \return The value of delay.
  */
  int get_delay() const;
/**!
  * \fn string get_delay_unity().
  * \return The unity of delay.
  */
  string get_delay_unity() const;


/**!
  * \fn int get_plateauDur_inf().
  * \return The lower bound of plateauDur interval.
  */
  int get_plateauDur_inf() const;
/**!
  * \fn int get_plateauDur_sup().
  * \return The upper bound of plateauDur interval.
  */
  int get_plateauDur_sup() const;
/**!
  * \fn int get_plateauDur().
  * \return The value of plateauDur.
  */
  int get_plateauDur() const;
/**!
  * \fn string get_plateauDur_unity().
  * \return The unity of plateauDur.
  */
  string get_plateauDur_unity() const;

//================================================================GETTERS: FIELDS FOR TREND MODELING======================================================================
public:

/**!
  * \fn int get_trendE().
  * \return The field trendE.
  */
  int get_trendE() const;
/**!
  * \fn int get_trend().
  * \return The field trend.
  */
  int get_trend() const;


/**!
  * \fn float get_quant_inf().
  * \return The lower bound of quant interval.
  */
  float get_quant_inf() const;
/**!
  * \fn float get_quant_sup().
  * \return The upper bound of quant interval.
  */
  float get_quant_sup() const;
/**!
  * \fn float get_quant().
  * \return The field quant.
  */
  float get_quant() const;


/**!
  * \fn float get_objective_inf().
  * \return The lower bound of objective interval.
  */
  float get_objective_inf() const;
/**!
  * \fn float get_objective_sup().
  * \return The upper bound of objective interval.
  */
  float get_objective_sup() const; 
/**!
  * \fn float get_objective().
  * \return The field objective.
  */
  float get_objective() const;

//================================================================GETTERS: FIELDS USED IN TIME SERIES GENERATION======================================================================

public:

/**!
  * \fn get_begin_time().
  * \return The time at which the impact occurs.
  */
  int get_begin_time() const;
/**!
  * \fn get_begin_value().
  * \return The begin value field.
  */
  float get_begin_value() const;


/**!
  * \fn get_delta_t().
  * \return The sampling frequency.
  */
  int get_delta_t() const;
/**!
  * \fn get_delta_t_unity().
  * \return The sampling frequence unity.
  */
  string get_delta_t_unity() const;

/**!
  * \fn int get_sample_index().
  * \return The field sample_index.
  */
  int get_sample_index() const;
/**!
  * \fn get_prev_oscillation_val().
  * \return The field prev_oscillation_val.
  */
  float get_prev_oscillation_val() const;

//===================================================================GETTER USED IN COPY CONSTRUCTOR ==================================================
/**!
  * \brief return the type of the Impact under consideration
  */
  virtual string get_impact_type() const = 0;

//================================================================SETTERS======================================================================
//#The following methods allows to safely modify pravite fields of Impact object.

public:

/**!
  * \fn void set_delay(int duration)
  * \brief Set the field delay at the given value in parameter. \n
  * WARNING: this function is called once before starting target parameter sampling, i.e. delay = -1, otherwise an exception is raised.
  *
  * \param duration Must be within [delay_inf, delay_sup].
  */
  void set_delay(int duration);

/**!
  * \fn void set_plateauDur(int duration).
  *
  * \brief Set the field plateauDur at the given value in parameter. \n
  * WARNING: this function is called once before starting target parameter sampling, i.e. plateauDur = -1, otherwise an exception is raised.
  *
  * \param duration Must be within [plateauDur_inf, plateauDur_sup].
  */
  void set_plateauDur(int duration);

/**!
  * \fn void set_quant(float val).
  * \brief Set the field quant at the given value in parameter.
  * WARNING: this function is called once before starting target parameter sampling, i.e. quant = -1, otherwise an exception is raised.
  *
  * \param val Must be within [0,1].
  */
  void set_quant(float val);

/**!
  * \fn void set_objective(float value).
  * \brief Set the field objective at value.
  * WARNING: this function is called once before starting target parameter sampling, i.e. objective = -1, otherwise an exception is raised.
  *
  * \param value Must be positive.
  */
  void set_objective(float value);

/**!
  * \fn void activate_impact(int b_time, float b_value, int delta, string d_unity).
  * \brief  \n
  * Set the fields used in time serie generation.
  * Fix sample_index at 0, fix prev_oscillation_val at 0.0
  * This function must be called one single time on an object of class Impact. At the second call, an exception is raised.
  * If delta > plateauDur, an exception is raised
  *
  * \param b_time The time at which the Event that causes the impact occurs, expressed in second.
  * \param b_value The value of the target parameter at b_time - 1.
  * \param delta The sampling frequency of the target parameter.
  * \param d_unity The unity in which delta is expressed.
  */
  void activate_impact(int b_time, float b_value, int delta, string d_unity);

//========================================================METHODS USED IN TIME SERIES GENERATOR PROGRAM========================================================

public:
 /**!
   * \fn
   * \brief
   * Activate the impact (one call to activated_impact(...) of class Impact) of the given event that influences the target parameter.
   * The given event actes on the target parameter in one single manner
    - If no impact influences the target parameter, return -1.
    - The current activated impacts (in active_impacts vector) that have trendE=0 and infinite plateau are deactivated (removed from active_impacts).
   *
   * \param parameter
   * \param event
   * \param active_impacts
   * \param inf_plateau_vanish
   * \param be_time
   * \param previous_value
   * \param delta
   * \param delta_unity
   *
   */
  void activate_impacts_of_event(string parameter, Event* event, vector<Impact*> &active_impacts, vector<pair<int, float>> &inf_plateau_vanish, int be_time, float previous_value, int delta, string delta_unity);

/**!
  * \fn
  * \brief When called on a trend 4/5 impact with an infinite plateau, this function returns the number of time-steps of increase/decrease and the performed increase/decrease at each time-step. \n
  *  The results of this function anables to cancel the effect of any trend 4/5 impact with an infinite plateau. \n
  *  This function is used in the modeling of anesthesia' stimulation (or pain) which do not not accumulate. \n
  *
  */
  pair<int, float> infinite_plateau_deactivate_impacts();

/**!
  * \fn
  * \brief The impacts that have exceeded their decay phase (for which is_deactivated() returns true) are removed
  *
  * \param active_impacts Vector of currently activated impacts.
  */
  void deactivate_impacts(vector<Impact*> &active_impacts);

/**!
  * \return Parameter value at time begin_time + delta_t*sample_index (i.e. the sample of index sample_index). \n
  * Rule: (1) if there is a single activated impact; (2) if we are in plateau phase; (3)and if it is the first sampled value in this phase;
  * then the value of plateau plus a small variation is returned. \n
  *
  * Note that prev_oscillation is initialized at 0 when impact is activated so this value tells us if we are dealing with the first sample in plateau phase.
  * Indeed, after this initialization its probability to being null again is neglectable (since it is a float value randomly taken within an interval centered on 0).
  * However, even if prev_oscillation becomes null again this does not matter (plateau value plus a small variation is returned).
  *
  * LOOK-OUT: a call to function quantify_impact() that returns the small variation is compulsory, otherwise, sample_index wont be incremented and prev_oscillation wont be updated.
  *
  * This rule avoid missing increase/decrease of parameter value when the delay time is less than the chosen sampling frequency.
  * WARNING:
  *   - The returned value is positive (not strictly) otherwise an exception is raised.
  *   - Is there min max values
  *
  * \param previous_value
  * \param base_value Parameter value before the starting of the surgery.
  * \param impacts List of impacts that modify the parameter "parameter".
  *
  */
  float update_parameter (float previous_value, float base_value, const vector<Impact*> &impacts);

/**!
  * \return
  * - Quantify the effect of Impact (e.g. if trend is increasing, the value to add to the begin_value of the parameter) at time begin_time + delta_t*sample_index
  * - Increment sample_index
  * WARNING: sample_index must be positive strictly, i.e. the object impact must be activated first (one call to function activate_impact(...)), if this condition is not fulfill, an exception is raised.
  *
  * \param base_value
  * \param plateau_oscillation Equals true if plateau oscillation has to be added to the total
  *  impact
  */
  virtual float quantify_impact (float base_value, bool plateau_oscillation=false) = 0;

//============================================INTERMEDIATE FUNCTIONS USED IN update_parameter FUNCTION =======================================================
protected:
/**!
  * Stationary: monitoring parameter oscillates between -(var/2)*value and (var/2)*value.
  * \return A random value between -(var/2)*value and (var/2)*value minus prev_oscillation_val
  *	(one field of Impact object, see FIELDS USED IN TIME SERIES GENERATION), in such a way the return value can be added to the previous
  *	value of parameter without cummulation of oscillations.
  * Set prev_oscillation_val at the random value obtained between -(var/2)*value and (var/2)*value.
  */
  float quantify_impact_stationary (float plat_value, float var);

  /*increasing->plateau->decreasing from the begin_value*/
  float quantify_impact_4 (float base_value, bool plateau_oscillation);
  float quantify_impact_deactivated_4 (float base_value);
  float quantify_impact_delay_4 (float base_value);
  float quantify_impact_plateau_4 (float base_value, bool plateau_oscillation);
  float quantify_impact_decay_4 (float base_value);

  /*decreasing->plateau->increasing from the begin_value*/
  float quantify_impact_5 (float base_value, bool plateau_oscillation);
  float quantify_impact_deactivated_5 (float base_value);
  float quantify_impact_delay_5 (float base_value);
  float quantify_impact_plateau_5 (float base_value, bool plateau_oscillation);
  float quantify_impact_decay_5 (float base_value);

//================================================================OTHER FUNCTIONS=============================================================================
public:

/**!
  * \fn string toString()
  * \brief
  *
  * \return String representation of Impact object.
  *
  * \todo Implement a minimal version for Hugo.
  */
  virtual string toString();

/**!
  * \return One possible value of plateauDur taken within the interval [plateauDur_inf, plateauDur_sup].
  */
  int one_valid_plateauDur();

/**!
  * \return One possible value of delay taken from the interval [delay_inf, delay_sup].
  */
  int one_valid_delay();

/**
  * \return One possivle value of quand taken from the interval [quant_inf, quant_sup].
  */
  float one_valid_quant();

/**
  * \return One possible value of objective taken from the interval [objective_inf, objective_sup].
  */
  float one_valid_objective();
//==================================================================UTIL METHODS==============================================================================

public:

/**!
  * \return A pseudo-random int between min and max.
  *
  * \param min.
  * \param max min < max.
  */
  int random_int(int min, int max);

/**!
  * \return A pseudo-random float between min and max.
  *
  * \param min.
  * \param max min < max.
  */
  float random_float(float min, float max);

/**!
  * convert nb_time from unity to the target_unity.
  * Precondition: unity and target_unity must be equal "mn", "s" or "h"
  *
  * \param nb_time
  * \param unity Must be equal "s" for second, "mn" for minute or "h" for hour.
  * \param target_unity
  */
  int convert_to (int nb_time, string unity, string target_unity);

/**!
  * \return true if the time associated with the index^th sample is within delay phase, i.e. sample_index*delta_t <= delay.
  *
  * \param index Default value of parameter index allows to do the test on field sample_index which is the index of the next generated value of target parameter.
  */
  bool with_in_delay(int index=-1);

/**!
  * \return true if the time associated with the index^th sample is within plateau phase, i.e. delay < sample_index*delta_t <= delay + plateauDur.
  *
  * \param index Default value of parameter index allows to do the test on field sample_index which is the index of the next generated value of target parameter.
  */
  bool with_in_plateau(int index=-1);

/**!
  * \return true if the time associated with the index^th sample is within decay phase, i.e. delay + plateauDur < sample_index*delta_t.
  *
  * \param index Default value of parameter index allows to do the test on field sample_index which is the index of the next generated value of target parameter.
  */
  bool with_in_decay(int index=-1);

/**!
  * \return true if the impact has no effect (it is said deactivated) when the index^th value of the target parameter is calculated.
  *
  * \param index Default value of parameter index allows to do the test on field sample_index which is the index of the next generated value of target parameter.
  */
  bool is_deactivated(int index=-1);

};


//****************************************************************************************** Class ImpactGest ********************************************************************************************
/**!
  * \class ImpactGest Impact.h [<header-name>]
  *
  * \brief ImpactGest type of Impact, derived from class Impact.
  * This type of impact is caused by surgical or anesthesia gestures.
  * Only trend 2, 3, 4 and 5 are allowed; trendE can be either 0 or 1.
  *
  */
class ImpactGest: public Impact
{

//=====================================SERIALIZATION WITH BOOST=====================================

public:
/**!
  * \brief
  * (1) In oder to use boost serialization on instance of class ImpactGest, it must implement "serialize" function.
  * This function specifies class fields to be serialized and in which order.
  * (2) Default constructor must be defined, it is used in unserialization function in order to build objects then set their field.
  */
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);

//========================================================FIELDS=============================================
private:

/**!
  * \brief varying coefficient by time unit, relevant for trend = 2 or 3 only
  *
  */
  float varLin; // >= 0
  string varLin_unity;

//===================================================CONSTRUCTORS=============================================
public:

/**!
  * \brief Default constructor having zero argument, necessary for ImpactGest deserialization.
  */
  ImpactGest();

/**!
  * \brief Copy constructor.
  * \param impGest Object to copy.
  */
  ImpactGest(const ImpactGest &impGest);

/**!
  * \brief Constructor for trend 2 (linear increasing) and 3 (linear decreasing). If another trend is given, an exception is raised.
  *
  * \param parameter
  * \param d_inf
  * \param d_sup
  * \param d_unity
  * \param dur_inf
  * \param dur_sup
  * \param dur_unity
  * \param tren
  * \param trenE
  * \param var
  * \param varLin_un
  *
  */
  ImpactGest(string parameter, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int tren, int trenE, float var, string varLin_un);

/**!
  * \brief Constructor for trend 4 and 5. If another trend is given, an exception is raised.
  *
  * \param parameter
  * \param d_inf
  * \param d_sup
  * \param d_unity
  * \param dur_inf
  * \param dur_sup
  * \param dur_unity
  * \param tren
  * \param trenE
  * \param q_inf
  * \param q_sup
  * \param obj_inf
  * \param obj_sup
  *
  */
  ImpactGest(string parameter, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int tren, int trendE, float q_inf, float q_sup, float obj_inf, float obj_sup);

//========================================================METHODS=============================================
public:

/**!
  * \fn string toString()
  * \brief
  *
  * \return String representation of ImpactGest object.
  *
  * \todo Implement a minimal version for Hugo.
  */
  virtual string toString();

  virtual float quantify_impact (float base_value, bool plateau_oscillation=false);

  virtual string get_impact_type() const;

private:

  // Intermediate functions used in quantify_impact function

/**!
  * \fn float quantify_impact_2 (float base_value)
  *
  * \brief Parameter values increase linearly from the current time-step until the occurrence of a stopping event, i.e. an event that targets the same parameter. 
  * At each time-step, the increase is constant equal: base_value*(varLin*delta_t_in_second/varLin_unity_in_second); 
  * where delta_t is the sampling frequency and varLin is the varying coefficient by time unit.
  *
  * \param base_value
  */
  float quantify_impact_2 (float base_value);

/**!
  * \fn float quantify_impact_3 (float base_value)
  *
  * \brief Identic to function quantify_impact_2 when increasing is remplaced by decreasing.
  *
  * \param base_value
  */
  float quantify_impact_3 (float base_value);

//========================================================GETTERS=============================================
public:

/**!
  * \fn float get_varLin()
  *
  * \return varLin field.
  *
  */
  float get_varLin() const;

/**!
  * \fn string get_varLin_unity()
  *
  * \return varLin unity.
  *
  */
  string get_varLin_unity() const;


};

/********************************************************************************************** Class ImpactDrug **********************************************************************************************
/**!
  * \class ImpactDrug Impact.h [<header-name>]
  *
  * \brief ImpactDrug type of Impact, derived from class Impact. \n
  * This type of impact is caused by drug ingestion. \n
  * Only trend 0, 1, 4 and 5 are allowed; trendE can be either 0 or 1.
  *
  */
class ImpactDrug: public Impact
{

//=====================================SERIALIZATION WITH BOOST=====================================
public:

/**!
  * \brief
  * (1) In oder to use boost serialization on instance of class ImpactDrug, it must implement "serialize" function. This function specifies class fields to be serialized and in which order.
  * (2) Default constructor must be defined, it is used in unserialization function in order to build objects then set their field.
  */
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);

//==========================================================FIELDS=================================================
private:

/**!
  * \brief This field is relevant for trend = 1 only cstat must be within [0,1].
  */
  float cstat;

//=======================================================CONSTRUCTORS=================================================
public:

/**!
  * \brief Default constructor having zero argument, necessary for ImpactDrug deserialization.
  */
  ImpactDrug();

/**!
  * \brief Copy constructor.
  * \param impDrug Object to copy.
  */
  ImpactDrug(const ImpactDrug &impDrug);

/**
  * \brief Constructor for trend = 0. If another trend is given, an exception is raised.
  *
  * \param parameter
  * \param d_inf
  * \param d_sup
  * \param d_unity
  * \param dur_inf
  * \param dur_sup
  * \param dur_unity
  * \param tren
  * \param trenE
  *
  */
  ImpactDrug(string parameter, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int tren, int trenE);

/**!
  * \brief Constructor for trend = 1. If another trend is given, an exception is raised.
  *
  * \param parameter
  * \param d_inf
  * \param d_sup
  * \param d_unity
  * \param dur_inf
  * \param dur_sup
  * \param dur_unity
  * \param tren
  * \param trenE
  * \param ct
  *
  */
  ImpactDrug(string param, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int tren, int trenE, float ct);

/**!
  * \brief constructor for trend = 4 or 5. If another trend is given, an exception is raised
  *
  * \param parameter
  * \param d_inf
  * \param d_sup
  * \param d_unity
  * \param dur_inf
  * \param dur_sup
  * \param dur_unity
  * \param tren
  * \param trenE
  * \param q_inf
  * \param q_sup
  * \param obj_inf
  * \param obj_sup
  *
  */
  ImpactDrug(string param, int d_inf, int d_sup, string d_unity, int dur_inf, int dur_sup, string dur_unity, int tren, int trenE,
		float q_inf, float q_sup, float obj_inf, float obj_sup);

//===========================================METHODS===================================================================
public:

/**!
  * \fn string toString()
  * \brief
  *
  * \return String representation of ImpactGest object.
  *
  * \todo Implement a minimal version for Hugo.
  */
  virtual string toString();

  virtual float quantify_impact (float base_value, bool plateau_oscillation=false);

  virtual string get_impact_type() const;


private:

  // Intermediate functions used in quantify_impact function

/**!
  * \brief Stationary base: monitoring parameter oscillates between -4.99 and 4.99 % of the base_value (value at the beginning of the surgery.
  *
  */
  float quantify_impact_0 (float base_value);

/**!
  * \brief Stationary: monitoring parameter oscillates between -cstat and cstat% of plateau_value.
  *
  */
  float quantify_impact_1 (float plat_value);

//=============================================GETTERS====================================================================
public:

/**!
  * \fn string toString()
  * \brief
  *
  * \return The cstat field.
  *
  */ 
  float get_cstat() const;

};

//******************************************************************************* Class ImpactReaction *********************************************************************************************
/**!
  * \class ImpactReaction Impact.h [<header-name>]
  *
  * \brief ImpactReaction type of Impact, derived from class Impact.
  *
  */
class ImpactReaction: public Impact
{

};

#endif
