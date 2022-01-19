import sys
import calendar
import datetime

# problem one
#  --start=2021/2/14 --end=2021/2/14 --file=ics/one.ics 


def open_and_store_file(filename):
    path_to_file = filename
    open(path_to_file, 'r')
    with open( path_to_file ) as f:
        contents = f.read()
    return contents

def file(filename):
    filename = filename[7:]
    return open_and_store_file(filename)

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

def splitEvents(content):
    events = content.split('BEGIN:VEVENT')
    return events

def convert(tint):
    am = 0
    hour = tint/10000
    if hour > 11.6:
        if hour > 12.6:
            hour = hour -12
        am =1
    
    if hour <1:
        hour = hour +12
    hour = round(hour, 2)
    string = str(hour)
    string = string.replace(".", ":")
    space = " "
    if hour <10:
        string = space+string
    
    if(len(string) < 5):
        string = string + "0"
    
    if am > 0:
        string = string + " PM"
    else:
         string = string + " AM"
    
    return string

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
    
def loc_sum(three, four):
    loc = three.split(":")
    sum = four.split(":")
    string = " "+ sum[1]+ " {{" +loc[1] + "}}"
    #print(string)
    return string
     
def keywords(event):
     event = event.split('\n') 
     s1 = etime(event[1],event[2])
     if "EXDATE:" in event[4]:
         s2 = loc_sum(event[5],event[6])
     elif "RULE:" in event[3]:
         s2 = loc_sum(event[4],event[5])
     else:
         #print(event)
         s2 = loc_sum(event[3],event[4])
     s = s1 +s2
     #print(s)
     return s

def getkey(e):
     e= e.split('\n') 
     e[1] = e[1].strip("DTSTART:")
     e[1] = e[1].split('T')
     key = int(e[1][0]+ e[1][1])
     return key

def getmonth(month_num):
    string = calendar.month_name[month_num]
    return string

def findDay(date):
    ds = str(date)
    born = datetime.datetime.strptime(ds, '%Y%m%d').weekday()
    return (calendar.day_abbr[born])

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

def until_d(e):
     e= e.split('\n') 
     e[3] = e[3].split("UNTIL=")
     date = e[3][1].split(";")
     d = date[0].split("T")
     i = d[0] +d[1]
     i = int(i)
     i = week_before(i)
     return i

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

def exdate(e):
    e = e.split('\n')
    e = e[4].split('EXDATE:')
    ex_d = e[1]
    ex_d = ex_d.split("T")
    ex_d = int(ex_d[0] + ex_d[1])
    #print(ex_d)

    return 20210416113000

def main():
    startd= start(sys.argv[1])
    endd = end(sys.argv[2])
    filename = sys.argv[3]
    filecontent = file(filename)
    events = splitEvents(filecontent)
    dict= {}
    rule =0
    ex_d = 0
    for e in events:
        if len(e) > 30:
            if "RRULE" in e:
                r = until_d(e)
                rule = 2
                #if "EXDATE" in e:
                 #   ex_d = exdate(e)
                  #  print(ex_d)
            key = getkey(e)
            content = keywords(e)
            dict[key] = content
            date = key
            if rule >1:
                while date < r:
                    date = week_after(date)
                    if date != ex_d:
                        dict[date] = content
            rule = 0
            ex_d = 0

  
         
    i= 0
    date_before = 0
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
        
if __name__ == "__main__":
    main()                                                                                                                                                                                                                                            
              
