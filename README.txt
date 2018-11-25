This project was designed to use the ESP 32 and LED matrix from Hackerbox 0036. 
I have not, as of this time, tested the output on a leap year or if the calculation works 
betwen christmas and new years. But I thought really hard about it and I think it
is correct. 

I used the WiFi clock example in the ESP 32 library and the code from the Jumbotron
example in the Hackerbox 0036 instructables. 

Currently set for US Central time. 
This can be changed on line 33 "const long  gmtOffset_sec = 3600 * -6;"
Change the "-6" to the appropriate offset for your timezone. 

Thanks and I hope you enjoy. 