#include <Arduino.h>
#include <Fsm.h>
#include <Bounce2.h>

void on_enter_alarm()
{
  Serial.println("Entering alarm state");
}

void on_state_alarm()
{
  // Serial.println("In alarm state");
}

void on_exit_alarm()
{
  Serial.println("Exiting alarm state");
}

void on_enter_snooze()
{
  Serial.println("Entering snooze state");
}

void on_state_snooze()
{
  // Serial.println("In snooze state");
}

void on_exit_snooze()
{
  Serial.println("Exiting snooze state");
}

void on_enter_idle()
{
  Serial.println("Entering idle state");
}

void on_state_idle()
{
  // Serial.println("In idle state");
}

void on_exit_idle()
{
  Serial.println("Exiting idle state");
}

#define DEBOUNCE_INTERVAL 5
#define BUTTON_PIN 9

#define FROM_IDLE_TO_ALARM_EVENT_ID 1
#define FROM_ALARM_TO_IDLE_EVENT_ID 2

// void (*on_enter)(), void (*on_state)(), void (*on_exit)()
State alarming(on_enter_alarm, on_state_alarm, on_exit_alarm);
State snoozing(on_enter_snooze, on_state_snooze, on_exit_snooze);
State idleing(on_enter_idle, on_state_idle, on_exit_idle);

// State* initial_state
Fsm fsm(&idleing);

// Create a debounced button object
Bounce2::Button button = Bounce2::Button();

void setup()
{
  Serial.begin(9600);

  button.attach(BUTTON_PIN, INPUT_PULLUP);
  button.interval(DEBOUNCE_INTERVAL);
  button.setPressedState(LOW);

  // State* state_from, State* state_to, int event, void (*on_transition)()
  fsm.add_transition(&idleing, &alarming, FROM_IDLE_TO_ALARM_EVENT_ID, NULL);
  fsm.add_transition(&alarming, &idleing, FROM_ALARM_TO_IDLE_EVENT_ID, NULL);
}

void loop()
{
  button.update();
  fsm.run_machine();

  if (button.pressed())
  {
    Serial.println("Button pressed");
    fsm.trigger(FROM_IDLE_TO_ALARM_EVENT_ID);
  }
}
