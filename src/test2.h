
#pragma once
#include <napi.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"
#include "HybridAnomalyDetector.h"
#include "timeseries.h"
#include <iostream>
#include <cstring>

#ifndef TEST2_H_
#define TEST2_H_

using namespace std;

class APIdll : public Napi::ObjectWrap<APIdll> {
	char* fileNameNormal;
	char* fileNameAnomaly;
	TimeSeries* tsTrain;
	TimeSeries* tsAnomaly;
	bool isSimple;
	SimpleAnomalyDetector simpleAD;
	HybridAnomalyDetector hybridAD;
	vector<AnomalyReport> anomalyReport;

public:
	APIdll(const Napi::CallbackInfo&);
	Napi::Value getAllAnomalyReportCpp(const Napi::CallbackInfo&);
	static Napi::Function GetClass(Napi::Env);
};

#endif




