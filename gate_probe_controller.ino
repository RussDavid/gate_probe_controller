/*
Controller to detect in ground gate probe activations.
There is an outter and inner probe, if the outer probe
is triggered the inner probe must also be triggered 
within four seconds for relay to be activated.
*/

int innerProbePin = 2; // Input pin, inner probe connected on pin 2
int outerProbePin = 3; // Input pin, outer probe connected on pin 3
int relayPin = 4; // Output pin, relay for gate trigger connected on pin 4
int timeoutMilliseconds = 4000; // Maximum milliseconds that can pass before inner probe activation
int relayHoldMilliseconds = 3000; // Time that the relay should be held on for

void setup() {
  Serial.begin(9600);

  pinMode(innerProbePin, INPUT);
  pinMode(outerProbePin, INPUT);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  // Variable to hold the time at which the outer probe was triggered
  static unsigned long outerProbeTriggerTime = NULL;
  Serial.print("Start of loop(), set outter probe trigger time: ");
  Serial.println(outerProbeTriggerTime);

  // If the outer probe gets triggered then store the time at which it was
  // triggered and check for input on the inner probe within four seconds
  if (digitalRead(outerProbePin)) {
    outerProbeTriggerTime = millis();
    Serial.print("Outer Probe Triggered at: ");
    Serial.println(outerProbeTriggerTime);

    // Continously check for input from the inner probe or if four seconds 
    // have elapsed (in which case break out of the loop and reset from the start)
    // If input from the inner probe is detected within four seconds then trigger
    // the relay connected to the gate trigger
    while (true) {
      // Four seconds with no input from inner probe, break loop
      if (millis() - outerProbeTriggerTime >= timeoutMilliseconds) {
        Serial.println("Four seconds passed without inner probe triggering");
        break;
      }

      // Inner probe triggered, trigger relay for three seconds and break loop
      if (digitalRead(innerProbePin)) {
        digitalWrite(relayPin, HIGH);
        delay(relayHoldMilliseconds);
        digitalWrite(relayPin, LOW);
        break;
      }
    }
  }
  Serial.println();
}
