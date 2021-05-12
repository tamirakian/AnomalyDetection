#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector :public SimpleAnomalyDetector {
	float maxC;
	float minC;
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
	bool updateCor(const TimeSeries& ts, float maxCor, correlatedFeatures& corFeatures, const featureID* f1, const featureID* f2);
	void updateAnomaly(vector<AnomalyReport>& reportVector, Point* p, correlatedFeatures& corF, int timeStepIndex, string f1, string f2);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */