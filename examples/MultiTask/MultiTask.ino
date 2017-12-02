/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
 
#include "MultiTaskLib.h"

MultiTask multitask(4);

void debug(String text)
{
	Serial.print(millis());
	Serial.print('\t');
	Serial.println(text);
}

//void TurnOn()
//{
//	multitask.AddTask(500, []() {digitalWrite(LED_BUILTIN, HIGH);}, TurnOff);
//}
//void TurnOff()
//{
//	multitask.AddTask(500, []() {digitalWrite(LED_BUILTIN, LOW);}, TurnOn);
//}

void setup()
{
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);

	multitask.AddRepetitive(500,
		[]() {debug("Repetitive"); }, 5,
		[]() {multitask.AddSingle(500, []()	{debug("Callback");});
	});

	multitask.AddAlternantContinuous(200,
		[]() {digitalWrite(LED_BUILTIN, HIGH); }, 
		[]() {digitalWrite(LED_BUILTIN, LOW); }	
	);
}

void loop()
{
	Serial.println("Main Loop");
	while (1)
	{
		multitask.Update();
	}
}
