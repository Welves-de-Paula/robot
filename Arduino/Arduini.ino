void setup()
{
    Serial.begin(115200);
}

void loop()
{
    if (Serial.available() > 0)
    {
        int val = Serial.read();
        if (val == '1')
        {
            digitalWrite(13, HIGH);
        }
        else if (val == '0')
        {
            digitalWrite(13, LOW);
        }
    }
}
