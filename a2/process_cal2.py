# Written By: Daniela Gallegos Dupuis
#python3 process_cal2.py
#python3 ./process_cal --start=1234/01/12 --end=2134/12/11 --file=ics/diana-devops.ics


#PART ONE: GET ALL INFROMATION FROM ARUGUMENT LINE: SPLIT EACH SEPERATE EVENT - STARTS ARE END DATES
#PART TWO: TIME, SUMMARY AND LOCATION -- DICTIONARY CONTENT
#PART THREE: DICTIONARY KEY AND STANDARD DATE AND WEEKDAY - DICTIONARY KEY - PRINTING
#PART FOUR: FREQUENCY - UNTIL - WEEK AFTER - WEEK BEFORE
#PART FIVE: FUCTION AND PRINTING FINAL OUTPUT

import sys #ARGUMENTS - COMMAND LINE
import calendar # DATE
import datetime #FREQUENCY

#PART ONE: GET ALL INFROMATION FROM ARUGUMENT LINE: SPLIT EACH SEPERATE EVENT - STARTS ARE DSTART
#----------------------------------------------------------------------------------------------------
# purpose: stores file content into a string called contents
# input: filename - path found file(filename)
# output: string contents : eveything within file
# description: open(path_to_file ) - f.read()
# called by: file(filename)
def open_and_store_file(path_to_file ):
    open(path_to_file, 'r')
    with open( path_to_file ) as f:
        contents = f.read()
    return contents

# purpose:
# input: agrv[3] --file=ics/diana-devops.ics
# output: file content
# description: modifies filename
# calls: open_and_store_file(ics/diana-devops.ics)
def file(filename):
    filename = filename[7:]
    return open_and_store_file(filename)

# purpose: start date param - int represention
# input: agrv[1]  --start=2021/2/1
# output: int 20210201
# description: split('/'), add "0" if needed, cast to int
def start(date):
    date = date[8:]
    newd = date.split('/')
    if len(newd[1]) < 2:
        newd[1] = "0"+ newd[1]
    
    if len(newd[2]) < 2:
        newd[2] = "0"+ newd[2]

    new = int(newd[0] + newd[1]+ newd[2])
    #print(new)
    return new

# purpose: end date param - int represention
# input: agrv[2] --end=2021/2/1
# output: int 20210201
# description: split('/'), add "0" if needed, cast to int
def end(date):
    date = date[6:]
    newd = date.split('/')
    if len(newd[1]) < 2:
        newd[1] = "0"+ newd[1]
    
    if len(newd[2]) < 2:
        newd[2] = "0"+ newd[2]
    new = int(newd[0] + newd[1]+ newd[2] )
    #print(new)
    return new

# purpose: split each event
# input: string content of file
# output:["", "DSTART:##########" ,...] ["", "DSTART:##########" ,...] 
# description: content.split('BEGIN:VEVENT')
def splitEvents(content):
    events = content.split('BEGIN:VEVENT')
    return events

#PART TWO: TIME, SUMMARY AND LOCATION -- DICTIONARY CONTENT
# -------------------------------------------------------------------------------------------------------
# purpose: convert military to standard
# input: military time - int 163000
# output: string standard -" 2:30 PM"
# description: int am_pm to decide which to add: replacing "." ":": using len(string) spacing and 0s 
# called by: etime(one, two)
def convert(tint):
    am = 0
    #convert hour
    hour = tint/10000
    if hour > 11.6:
        if hour > 12.6:
            hour = hour -12
        am =1
    if hour <1:
        hour = hour +12
    
    # change decimal into string hour
    hour = round(hour, 2)
    string = str(hour)
    string = string.replace(".", ":")
    space = " "

    # AM,PM, spaces,0's
    if hour <10:
        string = space+string
    
    if(len(string) < 5):
        string = string + "0"
    
    if am > 0:
        string = string + " PM"
    else:
         string = string + " AM"
    
    return string

# purpose: aquire time part of event
# input: one - DTSTART:########T######## : two  DTEND:...T...
# output: " 8:30 AM to 10:00 AM:"
# description: strip, cast to int, call and join 
# calls: convert(t1)
def etime(one, two):
    one = one.strip("DTSTART:")
    one = one.split('T')
    two = two.strip("DTEND:")
    two = two.split('T')  
    two[0] = one[1]  
    t1 = int(two[0]) 
    t2 = int(two[1]) 

    standard = convert(t1) + " to " + convert(t2) + ":"
    #print(standard)
    return standard

# purpose: event location and summary
# input: three - "LOCATION:HSD A240\n", four - "SUMMARY:MATH 102\n"
# output: MATH 102 {{HSD A240}}
# description: split and concat strings 
def loc_sum(three, four):
    loc = three.split(":")
    sum = four.split(":")
    string = " "+ sum[1]+ " {{" +loc[1] + "}}"
    #print(string)
    return string

# purpose: get key
# input: filecontent.split("BEGIN:EVENT") 
# output: " 6:00 PM to  9:00 PM: Romantic dinner with Chris {{Burger King}} "
# description: split into lines - itterate to find lines with key words
# calls: loc_sum(loc,sum), etime(dstart,dtend) - called by MAIN
def keywords(event):
     event = event.split('\n') 
     counter = 1
     cp1 = 1
     cp2 = 1
     for e in event:
        if "DTSTART" in e:
            dts = e 
            counter = counter+1

        if "DTEND" in e:
            end = e 
            counter = counter+10

        if counter > 10 and counter< 100 and cp1 < 2:
            s1 = etime(dts,end)
            cp1 = 5 +cp1

        if "LOCATION" in e:
            loc = e 
            counter = counter+100

        if "SUMMARY" in e:
            sum = e 
            counter = counter+1000

        if  counter >= 1100 and cp2< 2:
            s2 = loc_sum(loc,sum)
            cp2 =5
        if (cp1 + cp2) > 9:
             s = s1 +s2
             return s
             
#PART THREE: DICTIONARY KEY AND STANDARD DATE AND WEEKDAY - DICTIONARY KEY - PRINTING
# -------------------------------------------------------------------------------------------------------   
# purpose:
# input: e - an event after split 
# output: 20210416113000
# description: strip - DSTART - 20210416T113000 and concat
def getkey(e):
     e= e.split('\n') 
     e[1] = e[1].strip("DTSTART:")
     e[1] = e[1].split('T')
     key = int(e[1][0]+ e[1][1])
     return key

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

# purpose: get the string needed
# input: 20210201083000
# output: "February 01, 2021 (Mon)"
# description: divide ints, to string, concat
# calls:  wd = findDay(date), m = getmonth(month)
def convert_date(key):
    date = int(key/1000000)
    year = int(date/10000)
    month = date - year*10000 
    month = int(month/100)
    day = date - month*100 - year*10000 
    wd = findDay(date)
    m = getmonth(month)
    day = str(day)
    if len(day) < 2:
        day = "0"+ day

    string = m + " "+ day + ", "+ str(year) + " (" + wd + ")"
    return string

# purpose: get string to print date when needed
# input: 20210201083000 - 8:30 AM to 10:00 AM: MATH 102 {{HSD A240}}
# description: call to get date, while loop to make line, concat with content
# calls: convert_date(key)
# output:
#February 01, 2021 (Mon)
#-----------------------
# 8:30 AM to 10:00 AM: MATH 102 {{HSD A240}}
def getdate(key, content):
    date = convert_date(key)
    line = "\n"
    i = 0
    while i < len(date):
        line = line + "-"
        i = i + 1
    line = line + "\n"  
    string =  date + line + content 
    return string

#PART FOUR: FREQUENCY - UNTIL - WEEK AFTER - WEEK BEFORE
# -------------------------------------------------------------------------------------------------------
# input: e - event - split to find RULE
# output: 20210422235959
# description: RRULE:FREQ=WEEKLY;UNTIL=20210422T235959;BYDAY=FR - 20210422T235959 - 20210422235959
# calls: week_before(i)
# **assumes RULE will be listed 3rd
def until_d(e):
     e= e.split('\n') 
     e[3] = e[3].split("UNTIL=")
     date = e[3][1].split(";")
     d = date[0].split("T")
     i = d[0] +d[1]
     i = int(i)
     i = week_before(i)
     return i

# purpose: int date - week before key -
# input:  202102080830000
# output: 202102010830000
# description: next_week = datetime.datetime.strptime(ds, '%Y%m%d')+ datetime.timedelta(days=-7)
def week_before(key):
     date = int(key/1000000)
     time = key - date*1000000
     ds = str(date)

     next_week = datetime.datetime.strptime(ds, '%Y%m%d')+ datetime.timedelta(days=-7)
     string = str(next_week)
     string  = string.split(" ")
     string  = string[0].split("-")
     string  = string[0] + string[1] + string[2] 
     date = int(string)
     date = date*1000000 + time

     return(date)

# purpose:int date - week after key -
# input:  202102010830000
# output: 202102080830000
# description: next_week = datetime.datetime.strptime(ds, '%Y%m%d')+ datetime.timedelta(days=7)
def week_after(key):
     date = int(key/1000000)
     time = key - date*1000000
     ds = str(date)

     next_week = datetime.datetime.strptime(ds, '%Y%m%d')+ datetime.timedelta(days=7)
     string = str(next_week)
     string  = string.split(" ")
     string  = string[0].split("-")
     string  = string[0] + string[1] + string[2]
     date = int(string)
     date = date*1000000 + time

     return(date)

#PART FIVE: FUCTION AND PRINTING FINAL OUTPUT
# -------------------------------------------------------------------------------------------------------
# purpose: entire calender is represented by this dictionary
# input: events
# output: ical - dictionary 
# description: itterate, key    dict[key]      rule[]  
# calls: until_d(e), getkey(e),keywords(e), week_after(date)
def make_dict(events):
    dict= {} 
    rule =0
    for e in events:
        if len(e) > 30:
            if "RRULE" in e:
                r = until_d(e)
                rule = 2
            key = getkey(e)
            content = keywords(e)
            dict[key] = content
            date = key
            if rule >1:
                while date < r:
                    date = week_after(date)
                    dict[date] = content
            rule = 0
    return dict

# purpose: prints out calender: final output of program
# input: param passed- startd, endd, dict - collected data from file
# output: hopefully the same as the test_files
# description: interate,
#       if first: p= full date, 
#       if same day as before- p = content- 
#       else p= newline+date
#       if key within date param: print p

def process_cal(startd, endd, dict):
    i= 0 #first event
    date_before = 0  #last event
    for d in sorted(dict.keys()):
        x =  int(d/1000000)
        if i <1:
                p = getdate(d,dict[d])
        elif x <= date_before and x >= date_before:
                p = dict[d]

        else:
                p = "\n"+ getdate(d,dict[d])
        
        if x >= startd and x<=endd:
            print(p)
            i= i+1
            date_before = x  

def main():
     # FILE CONTENT: SPLIT INDIVIDUAL EVENTS
    startd= start(sys.argv[1])
    endd = end(sys.argv[2])
    filename = sys.argv[3]
    filecontent = file(filename)
    events = splitEvents(filecontent)
    
    # FILL OUT DICTIONARY AND FREQUENCY
    dict = make_dict(events)
  
    # SORT AND PRINT
    process_cal(startd, endd, dict)
        
if __name__ == "__main__":
    main()                                                                                                                                                                                                                                            
              
