# Librería Arduino MultiTask
La librería MultiTask implementa un buffer de tareas temporizadas que ejecutan en paralelo una tarea transcurrido un determinado de tiempo en milisegundos. Resulta útil para dar un cierto comportamiento asíncrono a nuestros programas.

Más información https://www.luisllamas.es/libreria-arduino-multitask/

## Instrucciones de uso
El objeto de la librería MultiTask es facilitar la ejecución de varias acciones de forma no bloqueante, dotando de un cierto comportamiento asíncrono. En un proyecto en el que necesitáramos tareas temporizadas instanciaríamos un objeto MultiTask para el número de tareas estimemos vamos a necesitar, y las crearíamos e iniciaríamos según necesidades.

MultiTask se inicia con indicando la capacidad, el número máximo de tareas que puede alojar un MultiTask. La cantidad de tareas realmente empleada se almacena en Count.

Las tareas se almacenan en un array. Cada vez que se añade una tarea, se devuelve el índice de la tarea añadida. Si el buffer está lleno, la tarea no se añade.

Existen varios tipos de tareas que podemos añadir
* Delay, no ejecuta acción.
* Single, se ejecuta una única vez.
* Repetitive, se ejecuta N veces  
* Continous, se repite de forma indefinida
* Alternat, se ejecuta N veces, alternando entre dos acciones
* AlternantContinous, se ejecuta de forma indefinida, alternando entre dos acciones.

Para actualizar el estado de las tareas, debemos llamar al método Update con frecuencia. El método Update comprueba el estado de todas las tareas y, en caso de que alguna haya expirado, ejecuta las acciones permanentes.

### Constructor
La clase MultiTask se instancia a través de su constructor.
```c++
MultiTask(const uint8_t capacity);
```

### Uso de MultiTask
```c++
// Añadir tareas. Devuelven el número del Slot ocupado
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

// Actualizar el estado de las tareas
void Update();

// Cancelar una tarea
void CancelTask(uint8_t index);

// Estado del buffer 
uint8_t Capacity() const;
uint8_t Count() const;
bool IsFull() const;
bool IsEmpty() const;
```

## Ejemplos
La librería MultiTask incluye los siguientes ejemplos para ilustrar su uso.

* MultiTask: Ejemplo que muestra el uso de algunas de las funciones de conversión.
```c++
#include "MultiTaskLib.h"

MultiTask multitask(4);

void debug(String text)
{
	Serial.print(millis());
	Serial.print('\t');
	Serial.println(text);
}

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
```
