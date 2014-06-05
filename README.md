mbed-servo
==========

mbedでfutaba製サーボを動作させるプログラムです

参考にしたのは[sample_programs](http://www.futaba.co.jp/robot/download/sample_programs)

##注意

* サーボを動かす時は、先にTorqueをONにしてから角度を送ってください。
　　　(ONにしないと角度を送っても、反応しません。)
　　　
* SetPositionではサーボの動作時間指定は出来ません。（最速で動くと思います）
* 時間を指定した場合は「SetTimeAndPosition」を使ってください。（引数に時間を指定すればOKです）
