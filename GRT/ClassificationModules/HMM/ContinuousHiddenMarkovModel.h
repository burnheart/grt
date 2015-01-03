/**
 @file
 @author  Nicholas Gillian <ngillian@media.mit.edu>
 @version 1.0
 
 @brief This class implements a continuous Hidden Markov Model.
 
 @example ClassificationModulesExamples/HMMExample/HMMExample.cpp
 */

/**
 GRT MIT License
 Copyright (c) <2012> <Nicholas Gillian, Media Lab, MIT>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef GRT_CONTINUOUS_HIDDEN_MARKOV_MODEL_HEADER
#define GRT_CONTINUOUS_HIDDEN_MARKOV_MODEL_HEADER

#include "HMMEnums.h"
#include "../../Util/GRTCommon.h"
#include "../../CoreModules/MLBase.h"

namespace GRT {

class ContinuousHiddenMarkovModel : public MLBase {
public:
	ContinuousHiddenMarkovModel(const UINT downsampleFactor = 5,const UINT delta = 1);
    
    ContinuousHiddenMarkovModel(const ContinuousHiddenMarkovModel &rhs);
    
    virtual ~ContinuousHiddenMarkovModel();
    
    ContinuousHiddenMarkovModel& operator=(const ContinuousHiddenMarkovModel &rhs);
    
    virtual bool predict_(VectorDouble &x);
    virtual bool predict_(MatrixDouble &obs);
    
    virtual bool train_(TimeSeriesClassificationSample &trainingData);
    
    /**
     This is the main reset interface for all the GRT machine learning algorithms.
     It should be used to reset the model (i.e. set all values back to default settings). If you want to completely clear the model
     (i.e. clear any learned weights or values) then you should use the clear function.
     
     @return returns true if the derived class was reset succesfully, false otherwise (the base class always returns true)
     */
    virtual bool reset();
    
    /**
     This is the main clear interface for all the GRT machine learning algorithms.
     It will completely clear the ML module, removing any trained model and setting all the base variables to their default values.
     
     @return returns true if the derived class was cleared succesfully, false otherwise
     */
    virtual bool clear();
    
    /**
     This saves the trained model to a file.
     
     @param fstream &file: a reference to the file the model will be saved to
     @return returns true if the model was saved successfully, false otherwise
     */
    virtual bool saveModelToFile(fstream &file) const;
    
    /**
     This loads a trained model from a file.
     
     @param fstream &file: a reference to the file the model will be loaded from
     @return returns true if the model was loaded successfully, false otherwise
     */
    virtual bool loadModelFromFile(fstream &file);
    
    virtual bool print() const;
    
    UINT getNumStates() const { return numStates; }
    
    UINT getClassLabel() const { return classLabel; }
    
    double getLoglikelihood() const { return loglikelihood; }
    
    double getPhase() const { return phase; }
    
    vector< UINT > getEstimatedStates() const { return estimatedStates; }
    
    MatrixDouble getAlpha() const { return alpha; }
    
    bool setDownsampleFactor(const UINT downsampleFactor);
    
    /**
     This function sets the modelType used for each HMM.  This should be one of the HMM modelType enums.
     
     This will clear any trained model.
     
     @param const UINT modelType: the modelType in each HMM
     @return returns true if the parameter was set correctly, false otherwise
     */
    bool setModelType(const UINT modelType);
    
    /**
     This function sets the delta parameter in each HMM.
     
     The delta value controls how many states a model can transition to if the LEFTRIGHT model type is used.
     
     The parameter must be greater than zero.
     
     This will clear any trained model.
     
     @param const UINT delta: the delta parameter used for each CHMM
     @return returns true if the parameter was set correctly, false otherwise
     */
    bool setDelta(const UINT delta);
    
    bool setSigma(const double sigma);
    
protected:
    
    double gauss( const MatrixDouble &x, const MatrixDouble &y,const unsigned int i,const unsigned int j,const unsigned int N,const double sigma );
    
    UINT downsampleFactor;
	UINT numStates;             ///<The number of states for this model
    UINT classLabel;            ///<The class label associated with this model
    UINT timeseriesLength;      ///<The length of the training timeseries
    double sigma;
    double phase;
	MatrixDouble a;             ///<The transitions probability matrix
	MatrixDouble b;             ///<The emissions probability matrix
	VectorDouble pi;            ///<The state start probability vector
    MatrixDouble alpha;
    VectorDouble c;
    CircularBuffer< VectorDouble > observationSequence; ///<A buffer to store data for realtime prediction
    MatrixDouble obsSequence;
    vector< UINT > estimatedStates; ///<The estimated states for prediction

	UINT modelType;         ///<The model type (LEFTRIGHT, or ERGODIC)
	UINT delta;				///<The number of states a model can move to in a LEFTRIGHT model
	double loglikelihood;	///<The log likelihood of an observation sequence given the modal, calculated by the forward method
	double cThreshold;		///<The classification threshold for this model
    
};

}//end of namespace GRT

#endif
