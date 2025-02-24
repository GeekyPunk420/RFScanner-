
*********PIN DIAGRAM*************
ESP32 GND  --------> New GND Row  
NRF24L01 GND  ----> Same New GND Row  
LED Cathode (-)  ---> Same New GND Row  
LED Anode (+) --(Resistor 220Ω)--> GPIO2  

💡 Key Takeaways
✅ NRF24L01 PA LNA can send and receive with a single module.
✅ Use radio.stopListening(); to send data.
✅ Use radio.startListening(); to receive data.
✅ Addressing must be consistent for communication to work.

