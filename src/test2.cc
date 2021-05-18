
#include "test2.h"
#include <iostream>
#include <memory>
using namespace std;

APIdll::APIdll(const Napi::CallbackInfo& info) : ObjectWrap(info) {
    Napi::Env env = info.Env();
    string normal = info[0].As<Napi::String>().ToString();
    string anomaly = info[1].As<Napi::String>().ToString();
    this->isSimple = info[2].As<Napi::Boolean>();
    this->fileNameNormal = new char[strlen(normal.c_str()) + 1];
    this->fileNameAnomaly = new char[strlen(anomaly.c_str()) + 1];
    strcpy(this->fileNameNormal, normal.c_str());
    strcpy(this->fileNameAnomaly, anomaly.c_str());
    this->tsTrain = new TimeSeries(this->fileNameNormal);
    this->tsAnomaly = new TimeSeries(this->fileNameAnomaly);
    if (this->isSimple) {
        this->simpleAD.learnNormal(*tsTrain);
        this->anomalyReport = simpleAD.detect(*tsAnomaly);
    }
    else {
        this->hybridAD.learnNormal(*tsTrain);
        this->anomalyReport = simpleAD.detect(*tsAnomaly);
    }
}

Napi::Value APIdll::getAllAnomalyReportCpp(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    string returnStr = "{\"anomalies\":[{";
    map <string, vector<Point*>> unitedMap = this->simpleAD.getSameDescAnomalys(this->anomalyReport);
    for (const auto& myPair : unitedMap) {
        returnStr += "\"" + myPair.first + "\":[[";
        for (int i = 0; i < myPair.second.size(); i++) {
            if (i == myPair.second.size() - 1) {
                returnStr += to_string((int)myPair.second.at(i)->x) + ',' + to_string((int)myPair.second.at(i)->y) + "]],";
            }
            else {
                returnStr += to_string((int)myPair.second.at(i)->x) + ',' + to_string((int)myPair.second.at(i)->y) + "],";
            }
        }
        returnStr += "\"reason\":\"CHANGE!!!!!!!!\"},{";
    }
    returnStr.pop_back();
    returnStr.pop_back();
    returnStr += "]}";
    return Napi::String::New(env, returnStr);
}

Napi::Function APIdll::GetClass(Napi::Env env) {
    return DefineClass(env, "APIdll", {
        APIdll::InstanceMethod("GetAllAnomalyReportCpp", &APIdll::getAllAnomalyReportCpp),
        });
}

//init func
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::String nameAPIdll = Napi::String::New(env, "APIdll");
    exports.Set(nameAPIdll, APIdll::GetClass(env));
    return exports;
}

NODE_API_MODULE(addon, Init)



