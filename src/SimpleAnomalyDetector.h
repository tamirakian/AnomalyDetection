#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

struct correlatedFeatures {
	std::string feature1, feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	float threshold;
	float minCenterX;
	float minCenterY;
	// you can add feilds
};

class SimpleAnomalyDetector :public TimeSeriesAnomalyDetector {
protected:
	std::vector<correlatedFeatures> cf;
	std::map<std::string, std::vector<Point*>> unitedMap;
	float minCorrelation;
public:
	SimpleAnomalyDetector() {
		minCorrelation = 0.9;
	};
	virtual ~SimpleAnomalyDetector();
	float getMinCor();
	void changeMinCor(float newMinCor);
	virtual void learnNormal(const TimeSeries& ts);
	virtual std::vector<AnomalyReport> detect(const TimeSeries& ts);
	std::map <std::string, std::vector<Point*>> getSameDescAnomalys(std::vector<AnomalyReport> anomalyVector);
	float getTreshold(const correlatedFeatures& cor, Point** points, int size);
	std::vector<correlatedFeatures> getNormalModel() {
		return cf;
	}
	virtual bool updateCor(const TimeSeries& ts, float maxCor, correlatedFeatures& corFeatures, const featureID* f1, const featureID* f2);
	virtual void updateAnomaly(std::vector<AnomalyReport>& reportVector, std::vector<AnomalyReport>& reportVectorParallel, Point* p, correlatedFeatures& corF, int timeStepIndex, std::string f1, std::string f2);
	Point** createPointArr(std::vector<float> f1, std::vector<float> f2, int size);
	void freePointArr(Point** pointArr, int size);
	// you can add helper methods
};

#endif