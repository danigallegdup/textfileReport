#process_cal4.py
import datetime
import calendar
import re


"""
How this program Works:
process_cal:
- mangager that bridges my code to the tester function: 
- has one object Calender - > access to a dictionary:
  - key = int(20210214) 
  - item:
    February 14, 2021 (Sun) 
    -----------------------
    6:00 PM to  9:00 PM: Romantic dinner with Chris {{Burger King}}
- not a Dict yet lol but working well
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
     if (date - self.cal.e.date) == 0:
         return(self.cal.e.eventt())
    
"""
calender
- open file
- split into events 
- make the events
- sort store the date in a dictionary and make the final output
"""

class calender:
    def __init__(self, filename):
        self.open_and_store_file(filename)
        self._into_dict_of_events()

    def open_and_store_file(self, filename ):
     open(filename, 'r')
     with open(filename) as f:
        self.contents = f.read()


    def _into_dict_of_events(self):
      self.events = self.contents.split('END:VEVENT')
      self.dict = {}
      for i in self.events:
          if len(i) > 20:
            self.e = event(i)
            key = self.e.dstart
            item = self.e.printing_event
            self.dict[key] = item
            if self.e.rule > 1:
                self.rule()
      #self.event_attrubutes()
      self.dict_by_day()

    def dict_by_day(self):
        for k in sorted(self.dict.keys()):
            print(self.e.printing_date)
            print(self.dict[k], "\n")

    def event_attrubutes(self):
        print(self.e.dstart)
        print(self.e.date)
        print(self.e.printing_date)
        print(self.e.printing_event)
        print(self.e.rule )
    

    def rule(self):
        print(self.e.dstart, self.e.date) 
        print(self.e.rule)

        newe = self.e
        newe.dstart = self.week_after(self.e.dstart)
        newe.date = int(newe.dstart/1000000)
        print( newe.dstart,newe.date )
        newe.change_date(newe.date)
    
        key = newe.dstart
        item = newe.printing_event
        self.dict[key] = item
        
        


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
   

    
        
"""
attriutes of event
    - self.contents -> this is just something that is used
    - self.events -> rid of this one too
"""

class event:
   def __init__(self, contents):
       self.split_events(contents)
       
   def split_events(self, contents):
      events = contents.split('\n')
      self.keywords(events) 

   def keywords(self, events):
     counter = 1
     cp1 = 1
     cp2 = 1
     self.rule = 0
     for e in events:
        if "RRULE:" in e:
            self.rule = self.until_d(e)
        
        if "DTSTART" in e:
            self.getkey(e)
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
             #print(s)
             self.printing_event = s
             self.eventt()
             return s
     

# RRULE
# -------------------------------------------------------------------------------------------------------
   def until_d(self,e):
     e = e.split("UNTIL=")
     date = e[1].split(";")
     d = date[0].split("T")
     i = d[0] +d[1]
     i = int(i)
     i = self.week_before(i)
     return i
   
   def week_before(self,key):
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

# DSTART:  key1 =self.dstart item2 = self.day_string  key2 = self.date  
# -------------------------------------------------------------------------------------------------------

   def getkey(self,e ): 
     e =  e.strip("DTSTART:")
     e = e.split('T')
     key =int(e[0]+ e[1])
     self.dstart = key 
     self.date = int(e[0])
     self.convert_date()
     return key      

   def convert_date(self):
     #print(self.date)  
     year = round(self.date/10000)
     month = self.date - year*10000 
     month = round(month/100)
     #print(month)
     day = self.date - month*100 - year*10000 
     #print(year)
     #print(day)
     wd = self.find_day(self.date)
     m = self.getmonth(month)
     day = str(day)
     if len(day) < 2:
        day = "0"+ day
     string = m + " "+ day + ", "+ str(year) + " (" + wd + ")"
     self.printing_date = string
     #print(string)
     return string  

   def getmonth(self, month_num):
    string = calendar.month_name[month_num]
    return string

   def find_day(self,date):
    ds = str(date)
    day = datetime.datetime.strptime(ds, '%Y%m%d').weekday()
    return (calendar.day_abbr[day])  

   def change_date(self, ndate):
       self.date = ndate
       self.convert_date()
       print(self.printing_date)
#Standard Time convertion: 
# -------------------------------------------------------------------------------------------------------

   def etime(self,one, two):
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

#location and time
# -------------------------------------------------------------------------------------------------------
   def loc_sum(self, three, four):
      loc = three.split(":")
      sum = four.split(":")
      string = " "+ sum[1]+ " {{" +loc[1] + "}}"
      #print(string)
      return string 

# self.print_content, self.print_day 
# -------------------------------------------------------------------------------------------------------
   def eventt(self):
        line = "\n"
        i = 0
        while i < len(self.printing_date):
            line = line + "-"
            i = i + 1    
        
        self.printing_date = self.printing_date + line
 

