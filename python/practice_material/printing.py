import csv

class event:
    def __init__(self, date, time, summary, location):
        self.date = date
        self.time = time
        self.summary =summary
        self.location = location
      
def print_event(e):
    print(e.date)
    dash = "-"
    for i in e.date:
        dash = dash +"-"
    print(dash)
    string = e.time+e.summary+" {{"+e.location+ "}}"
    return string

def print_same_day(e):
    string  = e.time+e.summary+" {{"+e.location+ "}}"
    return string

def main():
    e1 = event("October 21 2021 (Thur)", "12:00 PM to  1:00 PM: ", "Learn some python","Libray third floor")
    string =print_event(e1)
    print(string)

    e2= event("October 21 2021 (Thur)", " 2:00 PM to  3:00 PM: ", "eat something and think of happy things","biblio")
    string1 =print_same_day(e2)
    print(string1)

    e2= event("May 21 2021 (Thur)", " 2:00 PM to 12:00 PM: ", "eat something and think of happy things","biblio")
    string1 =print_same_day(e2)+"\n"
    print(string1)

    e2= event("June 23 2021 (Thur)", " 2:00 PM to 11:00 PM: ", "eat something and think of happy things","biblio")
    string1 =print_event(e2)+"\n"
    print(string1)

    e2= event("Septermber 25 2021 (Thur)", " 2:00 PM to  3:00 PM: ", "eat something and think of happy things","biblio")
    string1 =print_event(e2)+"\n"
    print(string1)

if __name__ == "__main__":
    main()
