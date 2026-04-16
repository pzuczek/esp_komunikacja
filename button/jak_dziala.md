trzeba edytować plik mosquito.conf
i wkleić tam

listener 1883
allow_anonymous true

potem szukasz swojego ip: ipconfig i wpisujesz w main.cpp

jak ju ustawisz to właczas program
mosquitto.exe -c mosquitto.conf -v

tak sie nasłuchuje
mosquitto_sub.exe -t "moj/test"

tak mozna wysyłać
mosquitto_pub.exe -t "moj/test" -m "elo elo"
