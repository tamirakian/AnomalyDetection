#include <iostream>
#include <memory>
#include "SimpleAnomalyDetector.h"
#include <algorithm>

SimpleAnomalyDetector::~SimpleAnomalyDetector()
{
}

float SimpleAnomalyDetector::getMinCor()
{
	return minCorrelation;
}

void SimpleAnomalyDetector::changeMinCor(float newMinCor)
{
	minCorrelation = newMinCor;
}

// creating array of points from the values of the correlated features
Point** SimpleAnomalyDetector::createPointArr(vector<float> f1, vector<float> f2, int size)
{
	Point** pointArr = new Point * [size];
	for (int i = 0; i < size; i++)
	{
		Point* p = new Point(f1[i], f2[i]);
		pointArr[i] = p;
	}
	return pointArr;
}

// deleting all of the dynamically allocated points
void SimpleAnomalyDetector::freePointArr(Point** pointArr, int size)
{
	for (int i = 0; i < size; i++)
	{
		delete (pointArr[i]);
	}
	delete (pointArr);
}

// getting the correlated feature's threshold
float SimpleAnomalyDetector::getTreshold(const correlatedFeatures& cor, Point** points, int size)
{
	float maxTreshold = 0;
	for (int i = 0; i < size; i++)
	{
		// saving theur max treshold
		float tempTreshold = dev(*points[i], cor.lin_reg);
		if (tempTreshold > maxTreshold)
		{
			maxTreshold = tempTreshold;
		}
	}
	return maxTreshold * 1.2;
}

// updating the correlation features in case they are indeed correlated
bool SimpleAnomalyDetector::updateCor(const TimeSeries& ts, float maxCor, correlatedFeatures& corFeatures, const featureID* f1, const featureID* f2)
{
	// if the max correlatin is indeed a correct correlation, we will save them as the correlated features
	if (maxCor >= minCorrelation)
	{
		Point** pointArr = createPointArr(f1->values, f2->values, ts.getNumOfTimesteps());
		corFeatures.lin_reg = linear_reg(pointArr, ts.getNumOfTimesteps());
		corFeatures.threshold = getTreshold(corFeatures, pointArr, ts.getNumOfTimesteps());
		freePointArr(pointArr, f1->values.size());
		return true;
	}
	return false;
}

// learning the correlated features from a proper flight
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts)
{
	for (int i = 0; i < ts.getNumOfFeatures() - 1; i++)
	{
		correlatedFeatures corFeatures;
		const featureID* f1;
		// getting the feature ID of feature1
		f1 = ts.getFeatureID(i);
		corFeatures.feature1 = f1->featureName;
		float* feature1Arr = new float[ts.getNumOfTimesteps()];
		// saving all the feature 1 values
		copy(f1->values.begin(), f1->values.end(), feature1Arr);
		float maxCor = 0;
		const featureID* f2;
		for (int j = i + 1; j < ts.getNumOfFeatures(); j++)
		{
			const featureID* f2Temp;
			// getting the second feature ID
			f2Temp = ts.getFeatureID(j);
			int size = ts.getNumOfTimesteps();
			float* feature2Arr = new float[size];
			copy(f2Temp->values.begin(), f2Temp->values.end(), feature2Arr);
			// evaluating their correlation
			float cor = pearson(feature1Arr, feature2Arr, ts.getNumOfTimesteps());
			// if their correlations is the maximum from all of the other features, we will save the second feature as the temp correlatd feature
			if (abs(cor) > maxCor)
			{
				maxCor = abs(cor);
				corFeatures.feature2 = f2Temp->featureName;
				corFeatures.corrlation = cor;
				f2 = f2Temp;
			}
		}
		if (updateCor(ts, maxCor, corFeatures, f1, f2))
		{
			cf.push_back(corFeatures);
		}
	}
}

// updating the anomaly vector in case there is an anomaly
void SimpleAnomalyDetector::updateAnomaly(vector<AnomalyReport>& reportVector, vector<AnomalyReport>& reportVectorParallel, Point* p, correlatedFeatures& corF, int timeStepIndex, string f1, string f2)
{
	// if the dev between 2 values of correlated features is bigger than their thrashold
	if (dev(*p, corF.lin_reg) > corF.threshold)
	{
		// we will report as anomaly
		string descr = f1 + " " + f2;
		long timeS = timeStepIndex + 1;
		AnomalyReport* rep = new AnomalyReport(descr, timeS);
		reportVector.push_back(*rep);
		descr = f2 + " " + f1;
		AnomalyReport* rep1 = new AnomalyReport(descr, timeS);
		reportVectorParallel.push_back(*rep1);
	}
}

// uniting the anomalys that have the same description and are a sequence from the helper anomalys vector
map <string ,vector<Point*>> SimpleAnomalyDetector::getSameDescAnomalys(vector<AnomalyReport> anomalyVector)
{
	vector<Point*> unitedAnomalysVec;
	if (anomalyVector.size() == 0) {
		return unitedMap;
	}
	string anomalyDesc = anomalyVector.at(0).description;
	// getting the first anomaly description
	int anomalyTS = anomalyVector.at(0).timeStep;
	int counter = 0;
	for (int i = 1; i < anomalyVector.size(); i++)
	{
		if (anomalyVector.at(i).description == anomalyDesc)
		{
			if (anomalyVector.at(i).timeStep == anomalyTS + 1)
			{
				// we will add 1 to the counter that will count the size of the current sequence
				counter++;
				anomalyTS++;
				if (i + 1 != anomalyVector.size())
				{
					continue;
				}
			}
		}
		// creating a point that its x value is the current united anomaly starting timestep, an its y value is the ending timestep
		Point* p = new Point(anomalyTS - counter, anomalyTS);
		// savung the united anomaly in a vector
		unitedAnomalysVec.push_back(p);
		unitedMap[anomalyDesc].push_back(p);
		//initializing the description and the time step to the current anomaly
		anomalyTS = anomalyVector.at(i).timeStep;
		anomalyDesc = anomalyVector.at(i).description;
		counter = 0;
	}
	return unitedMap;
}

// detecting anomalys in a flight
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts)
{
	vector<AnomalyReport> reportVector;
	vector<AnomalyReport> reportVectorParallel;
	// checking for all of the correlated features if there is an anomaly
	for (int i = 0; i < cf.size(); i++)
	{
		string f1 = cf[i].feature1;
		string f2 = cf[i].feature2;
		// checking for all of the time steps of the flight
		for (int j = 0; j < ts.getNumOfTimesteps(); j++)
		{
			// getting the values of the current time step
			vector<float> vec = ts.getTimeStemp(j);
			Point* p = new Point(vec[ts.getFeatureIndex(f1)], vec[ts.getFeatureIndex(f2)]);
			updateAnomaly(reportVector, reportVectorParallel, p, cf[i], j, f1, f2);
			delete (p);
		}
	}
	for (int i = 0; i < reportVectorParallel.size(); i++) {
		reportVector.push_back(reportVectorParallel.at(i));
	}
	return reportVector;
}