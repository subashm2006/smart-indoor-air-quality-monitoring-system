import requests
import time

CHANNEL_ID = "3289317"
READ_API_KEY = "Enter ThingSpeak Read API Key here"

BOT_TOKEN = "Enter Telegram Bot Token here"
CHAT_ID = "Enter Telegram Chat ID here"

THRESHOLD = 800

while True:
    try:
        url = f"https://api.thingspeak.com/channels/{CHANNEL_ID}/feeds/last.json?api_key={READ_API_KEY}"

        response = requests.get(url)
        data = response.json()

        gas = int(data["field3"])

        print("Gas Value:", gas)

        if gas > THRESHOLD:

            message = f" GAS DETECTED!\nCheck it Immediately...\nGas Value: {gas}"

            telegram_url = f"https://api.telegram.org/bot{BOT_TOKEN}/sendMessage"

            requests.post(telegram_url, data={
                "chat_id": CHAT_ID,
                "text": message
            })

            print("Telegram Alert Sent!")

        time.sleep(20)

    except Exception as e:
        print("Error:", e)
        time.sleep(10)
