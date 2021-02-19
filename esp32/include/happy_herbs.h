#ifndef HAPPY_HERBS_H_
#define HAPPY_HERBS_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <BH1750.h>
#include <DHT.h>
#include <PubSubClient.h>

/**
 * This class manages the entire state of the planting system, any changes to
 * an object of this class will be reflected in the connected hardware
 */
class HappyHerbsState {
 private:
  float lightThreshHold, moisThreshHold;
  int lampPinID;
  int pumpPinID;
  int moisSensorPinID;
  DHT *tempHumidSensor;
  BH1750 *lightSensorBH1750;

 public:
  HappyHerbsState(BH1750 &, DHT &, int, int, int, float, float);

  void writeLampPinID(const bool);
  bool readLampPinID();
  void writePumpPinID(const bool);
  bool readPumpPinID();

  float readLightSensorBH1750();
  float readMoisSensor();
  float readTempSensor();
  float readHumidSensor();

  void setLightThreshHold(float);
  void setMoisThreshHold(float);
  float getLightThreshHold();
  float getMoisThreshHold();
};

/**
 * This classes manages communications with AWS and synchronizes the local state
 * and the remote state
 */
class HappyHerbsService {
 private:
  String thingName;

  String topicShadowUpdate;
  String topicShadowUpdateDelta;

  int lastUpdated = 0;
  PubSubClient *pubsub;
  HappyHerbsState *hhState;

 public:
  HappyHerbsService(String &, PubSubClient &, HappyHerbsState &);

  void loop();
  bool connected();
  void reconnect();
  void handleCallback(char *, byte *, unsigned int);

  void handleShadowUpdateDelta(const JsonDocument &);
  void publishShadowUpdate();
  void publishCurrentSensorsMeasurements();
};

#endif  // HAPPY_HERBS_H_