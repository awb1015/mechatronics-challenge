/*See https://github.com/voyage/mechatronics-challenge
Create a program in C++ that sends GPS coordinates and 
accelerometer data via serial USB (heavy use of existing libraries is recommended)*/

#include "application.h"
#include "carloop/carloop.h"
#include "Particle.h"
#include "LIS3DH.h"
//Accelerometer driver courtesy of https://github.com/rickkas7/LIS3DH

void sampleGPS();
void sampleAccel();

Carloop<CarloopRevision2> carloop;
//Carloop does not connect interupt to the U1 anywhere, we will leave it as WKP for now
LIS3DHSPI accel(SPI, A2, WKP);

//After several days of running this unsigned long will overflow. Just a limitation to note with these global variables
unsigned long sampleTime = 0;
unsigned long currentTime = 0;
int gpsTimer = 0;

void setup(){
	Serial.begin(115200);
    
	carloop.begin();
	
	LIS3DHConfig config;
	config.setAccelMode(LIS3DH::RATE_100_HZ);
	bool setupSuccess = accel.setup(config);
	int accelAttempt = 0;
	//Ensure accelerometer was set-up successfully
	while(!setupSuccess){
		Serial.print("Error: Accelerometer could not be set-up");
		config.setAccelMode(LIS3DH::RATE_100_HZ);
		setupSuccess = accel.setup(config);
		if(accelAttempt>=4){
			Serial.print("Error: Accelerometer set-up failed");
			while(true){
			}
			//Stop here
		}
		accelAttempt+=1;
	}
}

void loop(){
    	carloop.update();
	
	currentTime = millis();
	//Hardcoded 10ms for every sample for now
	if(currentTime - sampleTime >= 10){
		sampleTime = millis();
		sampleAccel();
		gpsTimer+=1;
	}
	//GPS samples around 1hZ
	//As this is also hardcoded there's a bit of coupling with the code above
	if(gpsTimer>=100){
		sampleGPS();
		gpsTimer = 0; 
	}
}

void sampleGPS(){
	//Lock GPS to prevent a mismatched long/lat tuple
	WITH_LOCK(carloop.gps()){
		//Use date/time for future analysis
		float lat = carloop.gps().location.lat();
		float lng = carloop.gps().location.lng();
		bool gpsValid = carloop.gps().location.isValid();
		//Using USA style date. Could move to Year-Month-Day to avoid ambiguity
		Serial.printlnf("%02d/%02d/%02d ", carloop.gps().date.month(), carloop.gps().date.day(), carloop.gps().date.year());
		Serial.printlnf("%02d:%02d:%02d ", carloop.gps().time.hour(), carloop.gps().time.minute(), carloop.gps().time.second());
		if(gpsValid){
			Serial.printlnf("Longitude: %d , Latitude: %d", lng, lat);	
		}
		else{
			Serial.println("Error: GPS is invalid");
		}
	}
}

void sampleAccel(){
	//Grab our accelerometer data with a sample called 'accelSample'
	LIS3DHSample accelSample;
	//Send and print accelerometer data
	if (accel.getSample(accelSample)){
		Serial.printlnf("Accel X: %d, Accel Y: %d, Accel Z: %d", accelSample.x, accelSample.y, accelSample.z);
		}
	else{
		Serial.println("Error: No accelerometer sample");
	}
}
