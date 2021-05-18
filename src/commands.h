

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO
{
public:
	virtual string read() = 0;
	virtual void write(string text) = 0;
	virtual void write(float f) = 0;
	virtual void read(float *f) = 0;
	virtual ~DefaultIO() {}

	// you may add additional methods here
};

// you may add here helper classes

// you may edit this class
class Command
{
protected:
	DefaultIO *dio;
	string description;

public:
	Command(DefaultIO *dio) : dio(dio) {}
	virtual void execute() = 0;
	virtual ~Command() {}
	string getDescription()
	{
		return description;
	}
};

// implement here your command classes

// a class that will connect between the commands
class HelperClass
{
public:
	TimeSeries *tsTrain;
	TimeSeries *tsTest;
	HybridAnomalyDetector *ad;

	// defaul constructor
	HelperClass()
	{
		ad = new HybridAnomalyDetector();
		tsTrain = nullptr;
		tsTest = nullptr;
	}

	// initialize the time series objects
	void initHelper(TimeSeries *tsTrain, TimeSeries *tsTest)
	{
		this->tsTrain = tsTrain;
		this->tsTest = tsTest;
	}

	string deleteTrailingZeroes(string str)
	{
		string newStr = str;
		newStr.erase(newStr.find('.') + 4, string::npos);
		newStr.erase(newStr.find_last_not_of('0') + 1, string::npos);
		newStr.erase(newStr.find_last_not_of('.') + 1, string::npos);
		return newStr;
	}

	// initialize the anomalys vector
	vector<AnomalyReport> getAnomalyVector()
	{
		return ad->detect(*tsTest);
	}

	~HelperClass()
	{
		delete (ad);
		if (tsTest != nullptr)
		{
			delete (tsTest);
		}
		if (tsTrain != nullptr)
		{
			delete (tsTrain);
		}
	}
};

// option 1 command
class Option1 : public Command
{
	HelperClass *helper;

public:
	Option1(DefaultIO *dio, HelperClass *helper) : Command(dio), helper(helper)
	{
		description = "1.upload a time series csv file\n";
	}
	void creatCSVFile(string fileName)
	{
		// reading the text until we got "done" and saving it to the destination file
		string parsed = "done\n";
		string inputText = dio->read();
		inputText = inputText + "\n";
		ofstream outfile(fileName);
		while (inputText != parsed)
		{
			outfile << inputText;
			inputText = dio->read();
			inputText = inputText + "\n";
		}
		outfile.close();
	}
	void execute()
	{
		dio->write("Please upload your local train CSV file.\n");
		creatCSVFile("anomalyTrain.csv");
		TimeSeries *tsTrain = new TimeSeries("anomalyTrain.csv");
		dio->write("Upload complete.\n");
		dio->write("Please upload your local test CSV file.\n");
		creatCSVFile("anomalyTest.csv");
		TimeSeries *tsTest = new TimeSeries("anomalyTest.csv");
		dio->write("Upload complete.\n");
		helper->initHelper(tsTrain, tsTest);
		return;
	}
};

// option 2 command
class Option2 : public Command
{
	HelperClass *helper;

public:
	Option2(DefaultIO *dio, HelperClass *helper) : Command(dio), helper(helper)
	{
		description = "2.algorithm settings\n";
	}
	void execute()
	{
		// getting the current threshold from the helper class
		float currentThreshold = helper->ad->getMinCor();
		string currentStr = helper->deleteTrailingZeroes(to_string(currentThreshold));
		dio->write("The current correlation threshold is " + currentStr + "\n");
		dio->write("Type a new threshold\n");
		// getting the new threshold from the input
		string newThreshold = dio->read();
		// checking if the threshold is valid.
		if (stof(newThreshold) < 1 && stof(newThreshold) > 0)
		{
			helper->ad->changeMinCor(stof(newThreshold));
		}
		else
		{
			// if its not a valid value, it will call in recursion to execute until it will get a valid value
			dio->write("please choose a value between 0 and 1.\n");
			return execute();
		}
		return;
	}
};

// option 3 command
class Option3 : public Command
{
	HelperClass *helper;

public:
	Option3(DefaultIO *dio, HelperClass *helper) : Command(dio), helper(helper)
	{
		description = "3.detect anomalies\n";
	}
	void execute()
	{
		// learning the ts values of the train flight
		helper->ad->learnNormal(*helper->tsTrain);
		// getting the anomalys from the test flight
		helper->ad->detect(*helper->tsTest);
		dio->write("anomaly detection complete.\n");
		return;
	}
};

// option 4 command
class Option4 : public Command
{
	HelperClass *helper;

public:
	Option4(DefaultIO *dio, HelperClass *helper) : Command(dio), helper(helper)
	{
		description = "4.display results\n";
	}
	void execute()
	{
		// getting all of the anomalys
		vector<AnomalyReport> anomalyVector = helper->getAnomalyVector();

		// writing the anomalys and theiw time step location
		for (int i = 0; i < anomalyVector.size(); i++)
		{
			dio->write(to_string(anomalyVector[i].timeStep) + "\t" + anomalyVector[i].description + "\n");
		}
		dio->write("Done.\n");
		return;
	}
};

// option 5 command
class Option5 : public Command
{
	HelperClass *helper;

public:
	Option5(DefaultIO *dio, HelperClass *helper) : Command(dio), helper(helper)
	{
		description = "5.upload anomalies and analyze results\n";
	}
	// updating the True positive counter and the N for each anomaly that was given in the anomalys file
	int updateCounters(int &counter, string anomaly, vector<Point *> &unitedAnomalysVec)
	{
		stringstream ss(anomaly);
		string token;
		// we will parse the string by the character ','
		getline(ss, token, ',');
		// getting the starting time series from the anomaly
		int startTS = stoi(token);
		getline(ss, token, ',');
		// getting the ending time series from the anomaly
		int endTS = stoi(token);
		// checking on each of the united anomalys if there is an intersection with given anomaly
		for (int i = 0; i < unitedAnomalysVec.size(); i++)
		{
			// checking the cases in which at least 1 timestep of the unitedAnomaly is intersecting with the given anomaly
			if ((unitedAnomalysVec.at(i)->x >= startTS && unitedAnomalysVec.at(i)->x <= endTS) ||
				(unitedAnomalysVec.at(i)->y >= startTS && unitedAnomalysVec.at(i)->y <= endTS) ||
				(startTS >= unitedAnomalysVec.at(i)->x && startTS <= unitedAnomalysVec.at(i)->y) ||
				(endTS >= unitedAnomalysVec.at(i)->x && endTS <= unitedAnomalysVec.at(i)->y))
			{
				// in case the intersection is bigger than 0, we will add 1 to the True Positive counter
				counter++;
			}
		}
		// we will return N - the number of time steps that are not have an anomaly in the anomalys file
		return endTS - startTS + 1;
	}

	// uniting the anomalys that have the same description and are a sequence from the helper anomalys vector
	vector<Point *> getSameDescAnomalys()
	{
		vector<AnomalyReport> anomalyVector = helper->getAnomalyVector();
		vector<Point *> unitedAnomalysVec;
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
			Point *p = new Point(anomalyTS - counter, anomalyTS);
			// savung the united anomaly in a vector
			unitedAnomalysVec.push_back(p);
			//initializing the description and the time step to the current anomaly
			anomalyTS = anomalyVector.at(i).timeStep;
			anomalyDesc = anomalyVector.at(i).description;
			counter = 0;
		}
		return unitedAnomalysVec;
	}

	void delUnitedVector(vector<Point *> &unitedAnomalysVec)
	{
		for (int i = 0; i < unitedAnomalysVec.size(); i++)
		{
			delete (unitedAnomalysVec.at(i));
		}
	}

	void execute()
	{
		vector<Point *> unitedAnomalysVec = getSameDescAnomalys();
		vector<string> anomalysFileVec;
		dio->write("Please upload your local anomalies file.\n");
		// getting the first anomaly from the file
		string anomaly = dio->read();
		// initializing P - the number of anomalys from the file
		int P = 0;
		// creating N - the number of time steps that are not have an anomaly in the anomalys file
		int N = helper->tsTest->getNumOfTimesteps();
		while (anomaly != "done")
		{
			P++;
			// saving the current anomaly to the vector
			anomalysFileVec.push_back(anomaly);
			anomaly = dio->read();
		}
		// initializing the true positive counter
		int TPCounter = 0;
		for (int i = 0; i < anomalysFileVec.size(); i++)
		{
			N -= updateCounters(TPCounter, anomalysFileVec.at(i), unitedAnomalysVec);
		}
		// creating the false positive counter - the num of united anomalys thar are not exist in the file
		int FPCounter = unitedAnomalysVec.size() - TPCounter;
		// evaluating the rounded true positive rate
		float TPrate = TPCounter / (float)P;
		string newTPRate = helper->deleteTrailingZeroes(to_string(TPrate));
		// evaluating the rounded false alarms rate
		float FArate = (FPCounter / (float)N);
		string newFARate = helper->deleteTrailingZeroes(to_string(FArate));
		dio->write("Upload complete.\n");
		dio->write("True Positive Rate: " + newTPRate + "\n");
		dio->write("False Positive Rate: " + newFARate + "\n");
		delUnitedVector(unitedAnomalysVec);
		return;
	}
};

// option 6 command
class Option6 : public Command
{
	HelperClass *helper;

public:
	Option6(DefaultIO *dio, HelperClass *helper) : Command(dio), helper(helper)
	{
		description = "6.exit\n";
	}
	// exit from the interaction
	void execute()
	{
		return;
	}
};
#endif /* COMMANDS_H_ */
