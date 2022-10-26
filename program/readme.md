### ESP8266:
資料夾內有兩個東西

#### FW:
先把8266的韌體燒錄成資料夾內的BIN檔，就可以使用arduino進行開發(軟體為nodemcu flasher)
至於燒錄的方法還有線怎麼接，就自己去網路上找。

#### feeder:
Aruino的主程式 自己研究

### lib:
Arduino用到的外部lib，記得安裝，如果上船的時候有跳錯誤，在自己去找少了哪個lib(upload fail的紅色字裡面都有寫)

程式運作原理基本上是8266作為網路跳板，arduino會藉由Serial通訊，傳輸字串給8266(例如"/feed","/wl"等等)，8266那邊會把傳輸過來的字串加到你heroku的母網域名稱上，變成特定的子網頁(ex: https://myheroku.heroku.com/feed )，並且使用POST的方式(網路POST GET 方法自己去網路查)，當heroku那邊，自己寫的flask伺服器接收到有POST的訊息時，就會利用line notify api傳輸資料，以此達到餵食通知的效果(line notify的使用方法，自己去查囉:D)

另外硬體上也有螢幕+按鈕的輸入介面，所以可以顯示上次餵食時間以及系統專用LINE帳號的ID。
