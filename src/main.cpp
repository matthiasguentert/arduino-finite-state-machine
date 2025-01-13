#include <Arduino.h>
#include <Fsm.h>
#include <Bounce2.h>

#define DEBOUNCE_INTERVAL 5
#define BUTTON_PIN 9

#define FROM_IDLE_TO_ALARM_EVENT_ID 1
#define FROM_ALARM_TO_IDLE_EVENT_ID 2
#define FROM_SNOOZE_TO_ALARM_EVENT_ID 3

void on_enter_alarm()
{
  Serial.println("Entering alarm state");

  // start playing alarm sound
}

void on_state_alarm()
{
  // event triggered by rtc.alarmFired(1) == true

  // e.g. blink led of alarm button
}

void on_exit_alarm()
{
  Serial.println("Exiting alarm state");

  // stop playing alarm sound
}

void on_enter_snooze()
{
  Serial.println("Entering snooze state");

  // start timer for snooze duration
}

void on_state_snooze()
{
  // event triggered by snooze button press

  // transition back to alarm in case snooze timer expires
  // fsm.trigger(FROM_SNOOZE_TO_ALARM_EVENT_ID);
}

void on_exit_snooze()
{
  Serial.println("Exiting snooze state");

  // reset snooze timer
}

void on_enter_idle()
{
  Serial.println("Entering idle state");
}

void on_state_idle()
{
  // Update display with current time
}

void on_exit_idle()
{
  Serial.println("Exiting idle state");
}

void on_enter_time_setup()
{
  // stop updating display
}

void on_state_time_setup()
{
  // after setting the time, transition to idle state
}

void on_exit_time_setup()
{
  //
}

void on_enter_alarm_setup()
{
  // Code to execute when entering the alarm setup state
}

void on_state_alarm_setup()
{
  // Code to execute while in the alarm setup state
}

void on_exit_alarm_setup()
{
  // Code to execute when exiting the alarm setup state
}

// void (*on_enter)(), void (*on_state)(), void (*on_exit)()
State alarm(on_enter_alarm, on_state_alarm, on_exit_alarm);
State snooze(on_enter_snooze, on_state_snooze, on_exit_snooze);
State idle(on_enter_idle, on_state_idle, on_exit_idle);
State time_setup(on_enter_time_setup, on_state_time_setup, on_exit_time_setup);
State alarm_setup(on_enter_alarm_setup, on_state_alarm_setup, on_exit_alarm_setup);

// State* initial_state
Fsm fsm(&idle);

// Create a debounced button object
Bounce2::Button button = Bounce2::Button();

void setup()
{
  Serial.begin(9600);

  button.attach(BUTTON_PIN, INPUT_PULLUP);
  button.interval(DEBOUNCE_INTERVAL);
  button.setPressedState(LOW);

  // State* state_from, State* state_to, int event, void (*on_transition)()
  fsm.add_transition(&idle, &alarm, FROM_IDLE_TO_ALARM_EVENT_ID, NULL);
  fsm.add_transition(&alarm, &idle, FROM_ALARM_TO_IDLE_EVENT_ID, NULL);
  fsm.add_transition(&snooze, &alarm, FROM_SNOOZE_TO_ALARM_EVENT_ID, NULL);
  // ...
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
