"""
Daniela Gallegos Dupuis
V# = V00942356
Assginment 4: python3 
"""

import datetime
import calendar
import re

"""
Program Description:
 - running program on terminal: $ ./tester4.py --start=2021/01/24 --end=2021/4/15 --file=ics/one.ics
 - all implemation code is inside this file and there are no global variables
 - this python file does not output to stout 
 - uses regular expressions methods
 - problem decomposition was highly considered in design of this solution
 - docstrings: program, classes, and for each method
 - assumes all test inputs will be well-formed
 - required methods are implemented
 - passes every test
"""


"""
class Process_cal docstring
    Purpose: 
        this relatively small class ansures that the two required methods
        are implemented just as they are described in the Assignemnt write-up.
        I chose to keep this class as such as an easy way to ensure my solution
        fits the requirments exactly and make my solution easier to debug.

        the contructor -> takes in a filename and creates an calender object that generates a dictionary_of_days
        get_event_for_day -> takes in a date -> if there are events for that day: outputs the events for that day
        re_dates: uses regular expressions to compare datetimes to key datetime
"""
class process_cal:

  """__init__(self, filename) :: docstring
        - in: filename 
        - calles calender to create: dictionary_of_days from file content
  """
  def __init__(self, filename):
    self.filename = filename
    cal = events_of_days(filename)
    self.__dictionary_of_days__ = cal.__dictionary_of_days__
  
  """re_datetime( datetime) :: docstring
        in: datetime object -> integer to compare with dictionary keys
        re.compile(r"(\d\d\d\d-\d\d-\d\d)")
        re.split(r"-",d.group(1)) 
  """
  def re_datetime(self, datetime):
      datetime = str(datetime)
      format_date = re.compile(r"(\d\d\d\d-\d\d-\d\d)")
      d = format_date.search(datetime)
      split = re.split(r"-",d.group(1))
      comp_date = split[0] + split[1] +split[2]
      dif = int(comp_date)
      return dif

  """ def get_events_for_day(datetime) :: docstring
        Parameter:a datetime object
        returns: if there are events corresponding with that date: 
        if not it returns nothing
  """
  def get_events_for_day(self, datetime):
      date = self.re_datetime(datetime)
      for k in sorted(self.__dictionary_of_days__.keys()):
            if (date - k) == 0:
                 return self.__dictionary_of_days__[k]
                 
    

"""
class events_of_all_days docstring: 
    purpose: to genterate a dictionary that holds all the events for every day indicated by the given ICS file

    Logic and function call Breakdown: 
        1. self.file(filename)
        2. self.generate_formatted_events(event_recipes)
            a. until_d(self,e):
                * week_before(self, key)
            b. get_dstart(self, e)
            c. format(self, event):
                * etime(self, one, two)
                    * convert(self,tint)
                * loc_sum(self, three, four)
            d. week_after(self,key)
        3. self.make_dict_of_day( all_formatted_events) 
            a. getdate(self, key, content)
                *convert_date(self,key)
                    * getmonth(self, month_num):
                    * find_day(self,date)
"""
class events_of_days:
  """   __init__(self, filename):
            -> in: filename
            -> out: nothing
            -> description: calls 3 driver functions:
                    1. self.file(filename) 
                    2. self.generate_formatted_events(event_recipes)
                    3. self.make_dict_of_day( all_formatted_events) 
                    """  
  def __init__(self, filename):
       event_recipes = self.file(filename)
       all_formatted_events = self.generate_formatted_events(event_recipes)
       self.make_dict_of_day( all_formatted_events)

  """ 1. self.file(filename)
            -> in: filename
            -> out: list of events = event_recipes
            -> description: opens fle and reads content: breaks down calender into a list of events """    
  def file(self, filename):
        open(filename, 'r')
        with open( filename ) as f:
            content = f.read()
        events = content.split('BEGIN:VEVENT')
        return events 

  """ 2. self.generate_formatted_events(event_recipes)
            -> in: event_recipes
            -> out: temperary dictionary of all events = all_formatted_events
            -> description: from event_recipes, add recurring, format as requirments indicate """
  def generate_formatted_events(self,events):
        dict= {} 
        rule =0
        for e in events:
            if len(e) > 30:
                if "RRULE" in e:
                    r = self.until_d(e)
                    rule = 2
                key = self.get_dstart(e)
                content = self.format(e)
                dict[key] = content
                date = key
                if rule >1:
                    while date < r:
                        date = self.week_after(date)
                        dict[date] = content
                rule = 0
        return dict

  """  3. self.make_dict_of_day( all_formatted_events) 
            -> in: all_formatted_events
            -> out: nothing
            -> description: from all_formatted_events creates a dictionary of every present day with it's corresponding events """
  def make_dict_of_day(self, dict):
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

  """ Supporting Functions for generate_formatted_events(self,events) -------------------------------------------------
  
   a. until_d(self,e)
        -> called by: self.generate_formatted_events(event_recipes)
        -> in: an event recipe, e
        -> out: integer describes the UNTIL: date of that requring event: day and time
        -> purpose: serves as the termination range of the while loop in self.generate_formatted_events """  
  def until_d(self,e):
        e= e.split('\n') 
        e[3] = e[3].split("UNTIL=")
        date = e[3][1].split(";")
        d = date[0].split("T")
        i = d[0] +d[1]
        i = int(i)
        i = self.week_before(i)
        return i

  """  b. get_dstart(self, e)
            -> called by: self.generate_formatted_events(event_recipes)
            -> in: an event recipe, e
            -> out: integer that describes date and time this events starts
            -> purpose: produces the key to the dictionary of all events outputed by self.generate_formatted_events"""
  def get_dstart(self, e):
        e= e.split('\n') 
        e[1] = e[1].strip("DTSTART:")
        e[1] = e[1].split('T')
        key = int(e[1][0]+ e[1][1])
        return key

  """  c. format(self, event) 
            -> called by: self.generate_formatted_events(event_recipes)
            -> in: an event recipe, e
            -> out: a string that contains the formatted event
            -> purpose: fromats the time, summary and location as is required"""
  def format(self, event):
      event = event.split('\n') 
      s1 = self.etime(event[1],event[2])
      if "EXDATE:" in event[4]:
         s2 = self.loc_sum(event[5],event[6])
      elif "RULE:" in event[3]:
         s2 = self.loc_sum(event[4],event[5])
      else:
         #print(event)
         s2 = self.loc_sum(event[3],event[4])
      s = s1 +s2
      #print(s)
      return s

  """  d. week_after(self,key):
            -> called by: self.generate_formatted_events(event_recipes)
            -> in: integer representing a date
            -> out: integer representing a date a week after input date
            -> purpose: used to update the while look when recurring events are generated """
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

  """Level 2 and 3 Supporting functions: support until_d(self,e) and format(self, event) --------------------------------------------

  *  week_before(self, key):
            -> called by: until_d(self,e):
            -> in: integer representing a date
            -> out: integer representing a date a week after input date
            -> purpose: until loop runs one week over if you do not set it to the week defore: terminate when it surpasses    """
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
 
  """   * etime(self, one, two)
            -> called by: format(self, event) 
            -> in: event lines with "DTSTART" = one and "DTEND" = two
            -> out: formated time in standard time
            -> purpose: produces the key to the dictionary of all events outputed by self.generate_formatted_events """
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
  
  """ * loc_sum(self, three, four):  
            -> called by: format(self, event) 
            -> in: event lines with "LOCATION" = three and "SUMMARY" = four
            -> out: a string that contains the formatted event
            -> purpose: fromats summary and location as is required"""
  def loc_sum(self, three, four): 
        loc = three.split(":")
        sum = four.split(":")
        string = " "+ sum[1]+ " {{" +loc[1] + "}}"
        #print(string)
        return string

  """  ** convert(self,tint)::
            -> called by: etime(self, one, two)
            -> in: miltary time integer
            -> out: standard time string
            -> purpose: helper function of etime"""
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
  
  """ make_dict_of_day(self, dict) Suporting Functions: --------------------------------------------------------------------------

     a. getdate(self, key, content)
            -> called by: make_dict_of_day(self, dict)
            -> in: integer representing a date
            -> out: integer representing a date a week after input date
            -> purpose: until loop runs one week over if you do not set it to the week defore: terminate when it surpasses """ 
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

  """  * convert_date(self,key)
            -> called by: getdate(self, key, content)
            -> in: integer representing a date
            -> out: integer representing a date a week after input date
            -> purpose: until loop runs one week over if you do not set it to the week defore: terminate when it surpasses """
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

  """ ** getmonth(self, month_num):
            -> called by: convert_date(self,key)
            -> in: integer representing a month 
            -> out: string of the month 
            -> purpose: get string of month"""
  def getmonth(self, month_num):
        string = calendar.month_name[month_num]
        return string

  """  ** find_day(self,date)
            -> called by: convert_date(self,key)
            -> in: integer representing a date
            -> out: string that of weekday abbriviation
            -> purpose: get weekday of date """
  def find_day(self,date):
        ds = str(date)
        day = datetime.datetime.strptime(ds, '%Y%m%d').weekday()
        return (calendar.day_abbr[day])

 

  


  


  


  
            
                 

            

