import csv
import sys
import calendar
import datetime

# purpose: get month
# input: 2
# output: February
# description: calendar.month_name[month_num]
def getmonth(month_num):
    string = calendar.month_name[month_num]
    return string

# purpose:
# input: 20210201
# output: calendar.day_abbr[day] : Mon
# description: datetime.datetime.strptime(ds, '%Y%m%d').weekday()
def findDay(date):
    ds = str(date)
    day = datetime.datetime.strptime(ds, '%Y%m%d').weekday()
    return (calendar.day_abbr[day])

def main():
    print("hello world ")
    # i am going to use date and time to get 
    
if __name__ == "__main__":
    main()                                                                                                                                                                                                                                            
              

