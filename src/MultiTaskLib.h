/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/

#ifndef _MULTITASKLIB_h
#define _MULTITASKLIB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

typedef void(*Action)();

typedef struct 
{
	unsigned long time;
	unsigned long trigger;
	unsigned int repetitions;
	Action action;
	Action alternateAction;
	Action next;
	Action callback;
} Task, *TaskRef;

class MultiTask
{
public:
	MultiTask(const uint8_t capacity);

	uint8_t Capacity() const;
	uint8_t Count() const;

	uint8_t AddDelay(int time, Action callback);
	uint8_t AddSingle(int time, Action action);
	uint8_t AddSingle(int time, Action action, Action callback);
	
	uint8_t AddRepetitive(int time, Action action, unsigned int repetitions);
	uint8_t AddRepetitive(int time, Action action, unsigned int repetitions, Action callback);
	uint8_t AddContinuous(int time, Action action);
	uint8_t AddContinuous(int time, Action action, Action callback);
	
	uint8_t AddAlternant(int time, Action action, Action alternateAction, unsigned int repetitions);
	uint8_t AddAlternant(int time, Action action, Action alternateAction, unsigned int repetitions, Action callback);
	uint8_t AddAlternantContinuous(int time, Action action, Action alternateAction);
	uint8_t AddAlternantContinuous(int time, Action action, Action alternateAction, Action callback);

	void Update();
	void CancelTask(uint8_t index);
	

	bool IsFull() const;
	bool IsEmpty() const;

private:
	TaskRef* _tasks;
	uint8_t _capacity;
	uint8_t _count;

	uint8_t addTask(int time, Action action, Action alternateAction, unsigned int repetitions, Action callback);
	static void rearmTask(TaskRef task, Action action, Action alternateAction, unsigned int repetitions);
	bool updateTask(TaskRef task) const;
	void removeTask(uint8_t index);
	bool processTask(TaskRef task) const;
};

#endif

