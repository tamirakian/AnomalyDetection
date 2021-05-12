#include "timeseries.h"
#include <iostream>

using namespace std;

// intializing the map keys and values from the csv file
void TimeSeries::initFeaturesMap(const char* fileName)
{
    string featuresLine;
    ifstream infile;
    infile.open(fileName);
    if (!infile.is_open()) {
        cout << "Can't open this path: " << fileName << endl;
    }
    // getting the first line from the file - the features names line
    getline(infile, featuresLine);
    // creating from each feature a key in the map by its position indes in the line
    loadFeaturesFromFile(featuresLine);
    // iterating over the time steps lines in the file
    while (!infile.eof())
    {
        string valuesLine;
        getline(infile, valuesLine);
        // assigning the time step values to their feature in map
        addValues(valuesLine);
    }
    infile.close();
}

// parsring a string into substrings and returning a vector of all of the substrings
vector<string> TimeSeries::stringParser(string str) const
{
    stringstream ss(str);
    string token;
    vector<string> vect;
    // we will parse the string by the character ','
    while (getline(ss, token, ','))
    {
        vect.push_back(token);
    }
    return vect;
}

// getting the first line of the features names and creating for each of them a new key in the map by their order from right
// to left.
void TimeSeries::loadFeaturesFromFile(string featuresLine)
{
    // creating a vector of the features names
    vector<string> vect = stringParser(featuresLine);
    // for each feature we will create its featureID and we will add the feature index (its index in the features line)
    // as the feature key, and it's featureID as the value in the map
    for (int i = 0; i < vect.size(); i++)
    {
        featureID* id = new featureID;
        id->featureName = vect.at(i);
        featuresMap[i] = id;
    }
}

// getting the features names vector
vector<string> TimeSeries::getFeaturesNames() const
{
    vector<string> featuresNames;
    for (int i = 0; i < featuresMap.size(); i++)
    {
        featuresNames.push_back(featuresMap.at(i)->featureName);
    }
    return featuresNames;
}

// adding the desired time step values to their feature in the map
void TimeSeries::addValues(string timeStepValues)
{
    vector<string> valVector = stringParser(timeStepValues);
    for (int i = 0; i < valVector.size(); i++)
    {
        //converting the string to float
        float floatValue = stof(valVector[i]);
        //adding the value i to the feature in index i in the map
        featuresMap[i]->values.push_back(floatValue);
    }
}

// getting a specific value of a specific feature 
float TimeSeries::getFeatureVal(int featureIndex, int valueIndex) const
{
    return featuresMap.at(featureIndex)->values[valueIndex];
}

// getting the specific time step values
const vector<float> TimeSeries::getTimeStemp(int stepIndex) const
{
    vector<float> vec;
    for (int i = 0; i < featuresMap.size(); i++)
    {
        vec.push_back(featuresMap.at(i)->values[stepIndex]);
    }
    return vec;
}

// getting the size of the features
int TimeSeries::getNumOfFeatures() const
{
    return featuresMap.size();
}

//getting the featureID in index i from the map
const featureID* TimeSeries::getFeatureID(int featureIndex) const
{
    return featuresMap.at(featureIndex);
}

// getting the feature indes in the map by its name
int TimeSeries::getFeatureIndex(string featureName) const
{
    for (int i = 0; i < featuresMap.size(); i++)
    {
        if (featuresMap.at(i)->featureName == featureName)
        {
            return i;
        }
    }
    return 0;
}

// get the size of the time steps
int TimeSeries::getNumOfTimesteps() const
{
    return featuresMap.at(0)->values.size();
}