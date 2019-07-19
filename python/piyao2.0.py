# -*- coding: utf-8 -*-

# 登录数字中传，并跳转到“个人信息”页。

from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.chrome.options import Options
import requests
import time
import os
import re
import platform
from lxml import etree
from datetime import datetime

chrome_driver_path = ""
driver = None

headers = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3770.100 Safari/537.36'}
cookies = None
request_session = None
chrome_driver_path = "chromedriver.exe"
chrome_options = Options()
# chrome_options.add_argument('--headless')
chrome_options.add_argument('--start-maximized')
driver = webdriver.Chrome(chrome_options=chrome_options,executable_path=chrome_driver_path)
driver.implicitly_wait(20)
driver.get("http://piyao.sina.cn/")
time.sleep(1)

title_list=[]
like_list=[]
date_list=[]

# 获取页面初始高度
js = "return action=document.body.scrollHeight"
height = driver.execute_script(js)

# 将滚动条调整至页面底部
driver.execute_script('window.scrollTo(0, document.body.scrollHeight)')
time.sleep(1)

#定义初始时间戳（秒）
t1 = int(time.time())

# 重试次数
num=0

while (num<30):
	# 获取当前时间戳（秒）
    t2 = int(time.time())
    # 判断时间初始时间戳和当前时间戳相差是否大于30秒，小于30秒则下拉滚动条
    if t2-t1 < 30:
        new_height = driver.execute_script(js)
        if new_height > height :
            time.sleep(1)
            driver.execute_script('window.scrollTo(0, document.body.scrollHeight)')
            # 重置初始页面高度
            height = new_height
            # 重置初始时间戳，重新计时
            t1 = int(time.time())
    
    num+=1 

dates=driver.find_elements_by_xpath('//div[@class="day_date"]')
for i,date in enumerate(dates,start=1):
    titles = driver.find_elements_by_xpath('//div[@class="zy_day" and position()=%d]/div[@class="day_date"]/following-sibling::ul//div[@class="left_title"]'%i)
    like_nums=driver.find_elements_by_xpath('//div[@class="zy_day" and position()=%d]/div[@class="day_date"]/following-sibling::ul//div[@class="left_btns"]/div[@class="like_text"]'%i)
    for x in titles:
        date_list.append(date.text)
        title_list.append(x.text)
    for x in like_nums:
        like_list.append(int(x.text))



name_and_nums=zip(like_list,title_list,date_list)
li =sorted(name_and_nums,key=lambda x : x[0],reverse=True)

print("近三个月内新浪十大谣言")
for x in li[:10]:
    print(x[2],' ',x[1],'\t'"点赞数:",x[0])

driver.quit()
    