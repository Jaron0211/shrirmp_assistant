from __future__ import unicode_literals
import os
from flask import Flask, request, abort
from linebot import LineBotApi, WebhookHandler
from linebot.exceptions import InvalidSignatureError
from linebot.models import MessageEvent, TextMessage, TextSendMessage, ImageSendMessage, VideoSendMessage, \
    LocationSendMessage
import requests
token = 'aRJHdYv6NdwcqK5C7cmlfa4y0D92ktSrZxBJ3iSGjNw'#蝦缸系統，加上自己的line notify token

version = "1.2.0"
changelog = "Hardware update, add some new features"
debug = 0


app = Flask(__name__)

# LINE 聊天機器人的基本資料
# LINE 的 channel_access_token, channel_secret 換成在 Line Developer 裡的資料

#line 機器人的 另外一種訊息通知的方式 也是line的
line_bot_api = LineBotApi('')
handler = WebhookHandler('')


#Notify
def lineNotifyMessage(token, msg):

    headers = {
        "Authorization": "Bearer " + token, 
        "Content-Type" : "application/x-www-form-urlencoded"
    }

    payload = {'message': msg }
    r = requests.post("https://notify-api.line.me/api/notify", headers = headers, params = payload)
    return r.status_code


# 接收 LINE 的資訊
@app.route("/callback", methods=['POST'])
def callback():
    signature = request.headers['X-Line-Signature']

    body = request.get_data(as_text=True)
    app.logger.info("Request body: " + body)

    print(body)

    try:
        handler.handle(body, signature)
    except InvalidSignatureError:
        abort(400)
    return 'OK'

@app.route("/wl", methods=['POST'])
def wl():
    #line_bot_api.broadcast(TextSendMessage(text="(自動消息) 淹水囉-----🦐🦐🦐🦐🦐"))
    if not debug:
        #line_bot_api.push_message("C604268d996ed30dad6edfd1ad8683789",TextSendMessage(text="(自動消息) 地面水位感測器警告 "))
        lineNotifyMessage(token,'地面水位感測器警告')
    else:
        #line_bot_api.push_message("Ce111a7290acbbc55a05fbcd5f108d9db",TextSendMessage(text="(自動消息) 地面水位感測器警告 "))
        lineNotifyMessage(token,'地面水位感測器警告')
        print("wl")
        
    return 'OK'

@app.route("/feed", methods=['POST'])
def feed():
    #line_bot_api.broadcast(TextSendMessage(text="(自動消息) 今天有餵過蝦囉~~🍤🍤🍤🍤🍤🍤🍤"))
    if not debug:
        lineNotifyMessage(token,'今天有餵過蝦囉~~🍤🍤🍤🍤🍤🍤🍤')
        #line_bot_api.push_message("C604268d996ed30dad6edfd1ad8683789", TextSendMessage(text="(自動消息) 今天有餵過蝦囉~~🍤🍤🍤🍤🍤🍤🍤"))
    else:
        lineNotifyMessage(token,'今天有餵過蝦囉~~🍤🍤🍤🍤🍤🍤🍤')
        print('feed')
    return 'OK'

@app.route("/test", methods=['POST'])
def test():
    line_bot_api.broadcast(TextSendMessage(text=("--系統測試--,系統版本： " + version)))
    return_mes = "OK" + version
    return return_mes

@app.route("/24h", methods=['POST'])
def a24h():
    if not debug:
        lineNotifyMessage(token,'超過24小時沒餵食囉')
        #line_bot_api.push_message("C604268d996ed30dad6edfd1ad8683789", TextSendMessage(text="(自動消息) 超過24小時沒餵食囉"))
    else:
        print("24")
        lineNotifyMessage(token,'超過24小時沒餵食囉')
    #line_bot_api.broadcast(TextSendMessage(text="(自動消息) 超過24小時沒餵食囉"))
    return 'OK'

#https://youtu.be/dQw4w9WgXcQ

# 回傳 LINE 的資料
@handler.add(MessageEvent, message=TextMessage)
def echo(event):
    if event.message.text.find("因該") != -1 :  # 因為LINE有些預設資料,我們在此排除
        try:

            # event.message.text = user傳的訊息

            # 回訊息
            line_bot_api.reply_message(
                event.reply_token,
                TextSendMessage(text="應啦幹")
            )
        except:
            print("error")


    if event.message.text == "rickroll" :  # 因為LINE有些預設資料,我們在此排除
        try:

            # event.message.text = user傳的訊息

            # 回訊息
            line_bot_api.reply_message(
                event.reply_token,
                TextSendMessage(text="https://youtu.be/dQw4w9WgXcQ")
            )

        except:

            line_bot_api.reply_message(
                event.reply_token,
                ImageSendMessage(
                    original_content_url="https://onepage.nownews.com/sites/default/files/2020-05/%E9%A6%96%E9%A0%81%E5%B0%81%E9%9D%A2-%E3%80%8C%E5%8F%AF%E6%86%90%E5%93%AA%E3%80%8D%E3%80%81%E3%80%8C%E9%BB%91%E4%BA%BA%E5%95%8F%E8%99%9F%E3%80%8D%E3%80%81%E3%80%8C%E6%88%91%E5%B0%B1%E7%88%9B%E3%80%8D%E9%80%99%E4%BA%9B%E6%A2%97%E5%9C%96%E7%9A%84%E7%94%B1%E4%BE%86%E4%BD%A0%E7%9F%A5%E9%81%93%E5%97%8E%EF%BC%9F.jpg",
                    preview_image_url="https://onepage.nownews.com/sites/default/files/2020-05/%E9%A6%96%E9%A0%81%E5%B0%81%E9%9D%A2-%E3%80%8C%E5%8F%AF%E6%86%90%E5%93%AA%E3%80%8D%E3%80%81%E3%80%8C%E9%BB%91%E4%BA%BA%E5%95%8F%E8%99%9F%E3%80%8D%E3%80%81%E3%80%8C%E6%88%91%E5%B0%B1%E7%88%9B%E3%80%8D%E9%80%99%E4%BA%9B%E6%A2%97%E5%9C%96%E7%9A%84%E7%94%B1%E4%BE%86%E4%BD%A0%E7%9F%A5%E9%81%93%E5%97%8E%EF%BC%9F.jpg"
                )
            )
    
    if event.source.user_id != "Udeadbeefdeadbeefdeadbeefdeadbeef" and event.message.text == "changelog" :  # 因為LINE有些預設資料,我們在此排除
        try:

            # event.message.text = user傳的訊息

            # 回訊息
            line_bot_api.reply_message(
                event.reply_token,
                TextSendMessage(text=changelog)
            )

        except:

            line_bot_api.reply_message(
                event.reply_token,
                ImageSendMessage(
                    original_content_url="https://onepage.nownews.com/sites/default/files/2020-05/%E9%A6%96%E9%A0%81%E5%B0%81%E9%9D%A2-%E3%80%8C%E5%8F%AF%E6%86%90%E5%93%AA%E3%80%8D%E3%80%81%E3%80%8C%E9%BB%91%E4%BA%BA%E5%95%8F%E8%99%9F%E3%80%8D%E3%80%81%E3%80%8C%E6%88%91%E5%B0%B1%E7%88%9B%E3%80%8D%E9%80%99%E4%BA%9B%E6%A2%97%E5%9C%96%E7%9A%84%E7%94%B1%E4%BE%86%E4%BD%A0%E7%9F%A5%E9%81%93%E5%97%8E%EF%BC%9F.jpg",
                    preview_image_url="https://onepage.nownews.com/sites/default/files/2020-05/%E9%A6%96%E9%A0%81%E5%B0%81%E9%9D%A2-%E3%80%8C%E5%8F%AF%E6%86%90%E5%93%AA%E3%80%8D%E3%80%81%E3%80%8C%E9%BB%91%E4%BA%BA%E5%95%8F%E8%99%9F%E3%80%8D%E3%80%81%E3%80%8C%E6%88%91%E5%B0%B1%E7%88%9B%E3%80%8D%E9%80%99%E4%BA%9B%E6%A2%97%E5%9C%96%E7%9A%84%E7%94%B1%E4%BE%86%E4%BD%A0%E7%9F%A5%E9%81%93%E5%97%8E%EF%BC%9F.jpg"
                )
            )

if __name__ == "__main__":
    app.run()