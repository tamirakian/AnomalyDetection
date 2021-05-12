#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

// representing each one of the features
struct featureID
{
	std::vector<float> values;
	std::string featureName;
};

class TimeSeries
{
private:
	std::map<int, featureID*> featuresMap;

public:
	TimeSeries(const char* CSVfileName)
	{
		initFeaturesMap(CSVfileName);
	}
	void initFeaturesMap(const char* fileName);
	void loadFeaturesFromFile(std::string featuresLine);
	std::vector<std::string> stringParser(std::string str) const;
	std::vector<std::string> getFeaturesNames() const;
	void addValues(std::string timeStepValues);
	float getFeatureVal(int featureIndex, int valueIndex) const;
	const std::vector<float> getTimeStemp(int stepIndex) const;
	int getNumOfFeatures() const;
	const featureID* getFeatureID(int featureIndex) const;
	int getFeatureIndex(std::string featureName) const;
	int getNumOfTimesteps() const;
	~TimeSeries()
	{
		std::map<int, featureID*>::iterator it;
		for (it = featuresMap.begin(); it != featuresMap.end(); it++)
		{
			delete it->second;
		}
	}
};

#endif /* TIMESERIES_H_ */