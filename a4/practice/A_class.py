#process_cal4.py
import datetime
import calendar
import re


"""
lets go through it :)
"""

class process_cal:
  def __init__(self, filename):
    self.filename = filename
    cal = calender(filename)
    self.cal = cal

  def re_datetime(self, datetime):
      datetime = str(datetime)
      format_date = re.compile(r"(\d\d\d\d-\d\d-\d\d)")
      d = format_date.search(datetime)
      split = re.split(r"-",d.group(1))
      comp_date = split[0] + split[1] +split[2]
      dif = int(comp_date)
      return dif

  def get_events_for_day(self, datetime):
      date = self.re_datetime(datetime)
      for k in sorted(self.cal.__dictionary_of_days__.keys()):
            if (date - k) == 0:
                 return self.cal.__dictionary_of_days__[k]
                 
    

class calender:
  def __init__(self, filename):
       filecontent = self.file(filename)
       events = self.split_events(filecontent)
       # FILL OUT DICTIONARY AND FREQUENCY
       dict = self.make_dict(events)
       self.pc( dict)


  def open_and_store_file(self, path_to_file ):
        open(path_to_file, 'r')
        with open( path_to_file ) as f:
            contents = f.read()
        return contents


  def file(self, filename):
        return self.open_and_store_file(filename)

  def split_events(self, content):
        events = content.split('BEGIN:VEVENT')
        return events

  def convert(self,tint):
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


  def etime(self, one, two):
        one = one.strip("DTSTART:")
        one = one.split('T')
        two = two.strip("DTEND:")
        two = two.split('T')  
        two[0] = one[1]  
        t1 = int(two[0]) 
        t2 = int(two[1]) 

        standard = self.convert(t1) + " to " + self.convert(t2) + ":"
        #print(standard)
        return standard


  def loc_sum(self, three, four):
        loc = three.split(":")
        sum = four.split(":")
        string = " "+ sum[1]+ " {{" +loc[1] + "}}"
        #print(string)
        return string


  def keywords(self, event):

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
                s1 = self.etime(dts,end)
                cp1 = 5 +cp1

            if "LOCATION" in e:
                loc = e 
                counter = counter+100

            if "SUMMARY" in e:
                sum = e 
                counter = counter+1000

            if  counter >= 1100 and cp2< 2:
                s2 = self.loc_sum(loc,sum)
                cp2 =5
            if (cp1 + cp2) > 9:
                s = s1 +s2
                return s
                

  def getkey(self, e):
        e= e.split('\n') 
        e[1] = e[1].strip("DTSTART:")
        e[1] = e[1].split('T')
        key = int(e[1][0]+ e[1][1])
        return key


  def getmonth(self, month_num):
        string = calendar.month_name[month_num]
        return string


  def find_day(self,date):
        ds = str(date)
        day = datetime.datetime.strptime(ds, '%Y%m%d').weekday()
        return (calendar.day_abbr[day])


  def convert_date(self,key):
        date = int(key/1000000)
        year = int(date/10000)
        month = date - year*10000 
        month = int(month/100)
        day = date - month*100 - year*10000 
        wd = self.find_day(date)
        m = self.getmonth(month)
        day = str(day)
        if len(day) < 2:
            day = "0"+ day

        string = m + " "+ day + ", "+ str(year) + " (" + wd + ")"
        return string


  def getdate(self, key, content):
        date = self.convert_date(key)
        line = "\n"
        i = 0
        while i < len(date):
            line = line + "-"
            i = i + 1
        line = line + "\n"  
        string =  date + line + content 
        return string


  def until_d(self,e):
        e= e.split('\n') 
        e[3] = e[3].split("UNTIL=")
        date = e[3][1].split(";")
        d = date[0].split("T")
        i = d[0] +d[1]
        i = int(i)
        i = self.week_before(i)
        return i


  def week_before(self, key):
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


  def week_after(self,key):
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


  def make_dict(self,events):
        dict= {} 
        rule =0
        for e in events:
            if len(e) > 30:
                if "RRULE" in e:
                    r = self.until_d(e)
                    rule = 2
                key = self.getkey(e)
                content = self.keywords(e)
                dict[key] = content
                date = key
                if rule >1:
                    while date < r:
                        date = self.week_after(date)
                        dict[date] = content
                rule = 0
        return dict

  def pc(self, dict):
        self.__dictionary_of_days__ = {}
        date_before = 0  #last event
        for d in sorted(dict.keys()):
            day = int(d/1000000)
            if day != date_before:
                item = self.getdate(d,dict[d])
            else:
                item = item + "\n"+dict[d]

            self.__dictionary_of_days__[day] = item
            date_before = day
            
                 

            

