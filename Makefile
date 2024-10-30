送信/target: 送信/main.cpp
	g++ 送信/main.cpp -o 送信/target
受信/target: 受信/main.cpp
	g++ 受信/main.cpp -o 受信/target
実行: 送信/target 受信/target
	./送信/target&
	sleep 2
	./受信/target
掃除: 
	rm 送信/target 受信/target