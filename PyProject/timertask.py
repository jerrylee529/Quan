# -*- coding:utf-8 -*-

__author__ = 'jerry'


from threading import Timer
import time
from datetime import datetime, timedelta
import os

SECONDS_PER_DAY = 24 * 60 * 60

def gettimetowait(hour, minute, second):
    curTime = datetime.now()
    desTime = curTime.replace(hour=hour, minute=minute, second=second, microsecond=0)
    delta = curTime - desTime
    skipSeconds = SECONDS_PER_DAY - delta.total_seconds()
    #skipSeconds = delta.total_seconds()
    print "Must sleep %d seconds" % skipSeconds
    return skipSeconds

timer_interval = 1

def delayrun():
    print "start running"
    os.system('C:/Stock/bin/restart.bat')
    print "restart completed"

print "Running......"

while True:
    #print "waitting......"
    current_time = time.localtime(time.time())
    if((current_time.tm_hour == 20) and (current_time.tm_min == 00) and (current_time.tm_sec == 00)):
        delayrun()
    time.sleep(1)

