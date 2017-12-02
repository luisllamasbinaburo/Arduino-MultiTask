/***************************************************
Copyright (c) 2017 Luis Llamas
(www.luisllamas.es)

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License
 ****************************************************/
 
#include "MultiTaskLib.h"

MultiTask::MultiTask(uint8_t capacity)
{
	_tasks = new TaskRef[capacity];
	_capacity = capacity;
	_count = 0;
}

uint8_t MultiTask::Capacity() const
{
	return _capacity;
}

uint8_t MultiTask::Count() const
{
	return _count;
}

uint8_t MultiTask::AddDelay(int time, Action callback)
{
	return addTask(time, nullptr, nullptr, 1, callback);
}

uint8_t MultiTask::AddSingle(int time, Action action)
{
	return addTask(time, action, nullptr, 1, nullptr);
}

uint8_t MultiTask::AddSingle(int time, Action action, Action callback)
{
	return addTask(time, action, nullptr, 1, callback);
}

uint8_t MultiTask::AddRepetitive(int time, Action action, unsigned int repetitions)
{
	return addTask(time, action, nullptr, repetitions, nullptr);
}

uint8_t MultiTask::AddRepetitive(int time, Action action, unsigned int repetitions, Action callback)
{
	return addTask(time, action, nullptr, repetitions, callback);
}

uint8_t MultiTask::AddContinuous(int time, Action action)
{
	return addTask(time, action, nullptr, 0, nullptr);
}

uint8_t MultiTask::AddContinuous(int time, Action action, Action callback)
{
	return addTask(time, action, nullptr, 0, callback);
}

uint8_t MultiTask::AddAlternant(int time, Action action, Action alternateAction, unsigned int repetitions)
{
	return addTask(time, action, alternateAction, repetitions, nullptr);
}

uint8_t MultiTask::AddAlternant(int time, Action action, Action alternateAction, unsigned int repetitions, Action callback)
{
	return addTask(time, action, alternateAction, repetitions, callback);
}

uint8_t MultiTask::AddAlternantContinuous(int time, Action action, Action alternateAction)
{
	return addTask(time, action, alternateAction, 0, nullptr);
}

uint8_t MultiTask::AddAlternantContinuous(int time, Action action, Action alternateAction, Action callback)
{
	return addTask(time, action, alternateAction, 0, callback);
}

void MultiTask::Update()
{
	for(uint8_t index = 0; index < _count; index++)
	{
		bool finished = updateTask(_tasks[index]);

		if (finished)
		{
			Action backCallback = _tasks[index]->callback;
			removeTask(index);
			if (backCallback != nullptr) backCallback();
		}
	}
}

void MultiTask::CancelTask(uint8_t index)
{
	removeTask(index);
}

bool MultiTask::IsFull() const
{
	return _count >= _capacity;
}

bool MultiTask::IsEmpty() const
{
	return _count == 0;
}

uint8_t MultiTask::addTask(int time, Action action, Action alternateAction, unsigned int repetitions, Action callback)
{
	if (IsFull()) return;
	_tasks[_count] = new Task();
	_tasks[_count]->time = time;
	_tasks[_count]->trigger = time + millis();
	_tasks[_count]->action = action;
	_tasks[_count]->alternateAction = alternateAction;
	_tasks[_count]->repetitions = repetitions;
	_tasks[_count]->callback = callback;
	_count++;
	return _count;
}

void MultiTask::rearmTask(TaskRef task, Action action, Action alternateAction, unsigned int repetitions)
{
	task->trigger = task->time + millis();
	task->action = action;
	task->alternateAction = alternateAction;
	task->repetitions = repetitions;
}

void MultiTask::removeTask(uint8_t index)
{
	if (index > _count - 1) return;

	delete _tasks[index];
	memmove(_tasks + index, _tasks + index + 1, (_count - index ) * sizeof(TaskRef));
	--_count;
}

bool MultiTask::updateTask(TaskRef task) const
{
	unsigned long _currentMillis = millis();
	if (static_cast<unsigned long>(_currentMillis) - task->trigger <= 0)
		return processTask(task);

	return false;
}

bool MultiTask::processTask(TaskRef task) const
{
	if (task->action != nullptr) task->action();

	if (task->repetitions == 1) return true;

	if (task->repetitions == 0)
		if (task->alternateAction == nullptr)
			rearmTask(task, task->action, nullptr, 0);
		else
			rearmTask(task, task->alternateAction, task->action, 0);

	if (task->repetitions > 1)
		if (task->alternateAction == nullptr)
			rearmTask(task, task->action, nullptr, task->repetitions - 1);
		else
			rearmTask(task, task->alternateAction, task->action, task->repetitions - 1);

	return false;
}
