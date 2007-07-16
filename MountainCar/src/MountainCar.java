//#include "bt-glue/Environment.h"
//#include "bt-glue/DynamicClassLoadingEnvironment.h"
//#include "bt-glue/RLGlueEnvironment.h"

//class DataHolder;
//
////STL Stuff
//#include <vector>

import rlglue.Action;
import rlglue.Environment;
import rlglue.Observation;
import rlglue.Random_seed_key;
import rlglue.Reward_observation;
import rlglue.State_key;


/*===========================

Dynamics

============================*/
public class MountainCar implements Environment{
	
	
	private double position;
	private double velocity;
	private int theAction=0;


	private  boolean randomStarts;

	static final int numActions = 3;
	

	private double mcar_min_position = -1.2;
	private double mcar_max_position = 0.6;
	private double mcar_max_velocity = 0.07;            // the negative of this is also the minimum velocity
	private double mcar_goal_position = 0.5;
	

	private boolean inGoalRegion(){
		return position >= mcar_goal_position;
	}

	public double getHeight(){
		return getHeightAtPosition(position);
	}
	
	public double getHeightAtPosition(double queryPosition){
		return -Math.cos(3*(queryPosition-(Math.PI/2.0f)));
	}

	public double getPosition() {
		return position;
	}

	public double getVelocity() {
		return velocity;
	}

	public double getMcar_min_position() {
		return mcar_min_position;
	}

	public double getMcar_max_position() {
		return mcar_max_position;
	}

	public double getMcar_min_velocity() {
		return -mcar_max_velocity;
	}
	public double getMcar_max_velocity() {
		return mcar_max_velocity;
	}

	public void setPosition(double position) {
		this.position = position;
	}

	public void setVelocity(double velocity) {
		this.velocity = velocity;
	}


	public String env_init() {
		position = -0.5;
	    velocity = 0.0;
		return "1:e:2_[f,f]_[,]_[,]:1_[i]_[0,3]";
	}

	public String env_message(String arg0) {
		// TODO Auto-generated method stub
		return null;
	}

	
//	//At the end of movement, decide whether to randomly reYposition the agent
//	bool MountainCarEnv::step(const DataHolder* theAction){
//		
//		int a=theAction->getIntValue(0);
//
//		mcar_velocity += (a-1)*0.001 + cos(3*mcar_position)*(-0.0025);
//	    if (mcar_velocity > mcar_max_velocity) mcar_velocity = mcar_max_velocity;
//	    if (mcar_velocity < -mcar_max_velocity) mcar_velocity = -mcar_max_velocity;
//	    mcar_position += mcar_velocity;
//	    if (mcar_position > mcar_max_position) mcar_position = mcar_max_position;
//	    if (mcar_position < mcar_min_position) mcar_position = mcar_min_position;
//	    if (mcar_position==mcar_min_position && mcar_velocity<0) mcar_velocity = 0;		
//
//		bool goalRegion=false;
//		goalRegion=inGoalRegion();
//
//		return goalRegion;
//		}
//
//	void MountainCarEnv::start(){
//	}
//
	
	public Observation env_start() {
	if(randomStarts){
		position = (Math.random()*(mcar_max_position + Math.abs((mcar_min_position))) - Math.abs(mcar_min_position));
		velocity = (Math.random()*mcar_max_velocity*2) - Math.abs(mcar_max_velocity);
	}else{
		position = -0.5;
		position = 0.0;
	}
	
		return null;
	}

	public Reward_observation env_step(Action arg0) {
		// TODO Auto-generated method stub
		return null;
	}
	
	
	public MountainCar() {
		this(false);
	}
	public MountainCar(boolean randomStarts) {
		super();
		this.randomStarts = randomStarts;
	}

	
	
	public void env_cleanup() {
		// TODO Auto-generated method stub
		
	}

	public Random_seed_key env_get_random_seed() {
		// TODO Auto-generated method stub
		return null;
	}

	public State_key env_get_state() {
		// TODO Auto-generated method stub
		return null;
	}


	public void env_set_random_seed(Random_seed_key arg0) {
		// TODO Auto-generated method stub
		
	}

	public void env_set_state(State_key arg0) {
		// TODO Auto-generated method stub
		
	}



//	void addParametersEnv(ParameterHolder *theParameters){
//				theParameters->addBoolParam("Random Start States");
//				theParameters->setBoolParam("Random Start States",false);
//				theParameters->setAlias("rss","Random Start States");
//	}
//	Environment *maker(ParameterHolder *theParams){
//			if(theParams==NULL)
//				return new MountainCarEnv();
//			else{
//				bool randomStarts=theParams->getBoolParam("Random Start States");
//				return new MountainCarEnv(randomStarts);
//			}
//		}
//


//
//	const DataHolder *MountainCarEnv::getObsForState(const DataHolder *someState){
//		//In general this might be more complicated, but here, its really just the same thing b/c the observations are the state.
//		return someState->clone();
//	}


//	const DataHolder *MountainCarEnv::getObservations(){
//		
//		DataHolder *theObs=new DataHolder();
//		theObs->addFloatValue(getPosition());
//		theObs->addFloatValue(getVelocity());
//		
//		return theObs;
//	}
//
//	double MountainCarEnv::getReward(){
//		
//		if(inGoalRegion())
//			return 0.0f;
//		else
//			return -1.0f;
//	}
//
//
//
//
//	double MountainCarEnv::getPosition(){
//		return mcar_position;
//	}
//	double MountainCarEnv::getVelocity(){
//		return mcar_velocity;
//	}
//

}

