#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector()
{
	maxC = SimpleAnomalyDetector::minCorrelation;
	minC = 0.5;
}

HybridAnomalyDetector::~HybridAnomalyDetector()
{
	// TODO Auto-generated destructor stub
}

// updating the correlation features in case they are indeed correlated
bool HybridAnomalyDetector::updateCor(const TimeSeries& ts, float maxCor, correlatedFeatures& corFeatures, const featureID* f1, const featureID* f2)
{
	if (maxCor > minC && maxCor < maxC)
	{
		SimpleAnomalyDetector* s;
		Point** pointArr = s->createPointArr(f1->values, f2->values, ts.getNumOfTimesteps());
		Circle minCirc = findMinCircle(pointArr, ts.getNumOfTimesteps());
		// the treshold will be the radius
		corFeatures.threshold = minCirc.radius * 1.1;
		// updating the center of the minCirc
		corFeatures.minCenterX = minCirc.center.x;
		corFeatures.minCenterY = minCirc.center.y;
		s->freePointArr(pointArr, f1->values.size());
		return true;
	}
	// in case the correlated features are fulfill the condition of the parent class
	else if (maxCor >= maxC)
	{
		return SimpleAnomalyDetector::updateCor(ts, maxCor, corFeatures, f1, f2);
	}
	return false;
}

// updating the anomaly vector in case there is an anomaly
void HybridAnomalyDetector::updateAnomaly(vector<AnomalyReport>& reportVector, vector<AnomalyReport>& reportVectorParallel, Point* p, correlatedFeatures& corF, int timeStepIndex, string f1, string f2)
{
	// in case the correlated features are fulfill the condition of the parent class
	if (corF.corrlation >= maxC)
	{
		return SimpleAnomalyDetector::updateAnomaly(reportVector, reportVectorParallel, p, corF, timeStepIndex, f1, f2);
	}
	Circle minCircle = Circle(Point(corF.minCenterX, corF.minCenterY), corF.threshold);
	if (!isPointInsideCircle(*p, minCircle))
	{
		// we will report as anomaly
		string descr = f1 + "  " + f2;
		long timeS = timeStepIndex + 1;
		AnomalyReport* rep = new AnomalyReport(descr, timeS);
		reportVector.push_back(*rep);
		descr = f2 + "  " + f1;
		AnomalyReport* rep1 = new AnomalyReport(descr, timeS);
		reportVectorParallel.push_back(*rep1);
	}
}