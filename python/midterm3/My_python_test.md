My_python_test.md

Python is an interpreter language,
how does it function differently than C and other languages: pros and cons
Give 5 good points on both sides

Pro's of Python
- it is fast to implement: 
    demands less specifity, has many libray availalbe: if you are 
    creating something that uses standard theory implemenations of things: like a heap, dictionary, list.... python can help you skip the those implemenations and build ontop of many libraries: 
- diverse: 
    Forgiveing for experimantal programing and fast to learn if you already 
    have programing experiemake websites, make bots, great for machine learning, many branches: spotify, numpy - speciallizs 
- globally Known: 
    Many programming jobs and comp-sci interns involve python programing: makes you imployable

Con's of Python
- Really slow and takes up a lot of space
     -> linear: inpretor --> takes up so much memory because it later gets interpreted into c
     -> it is the slowest
- you have less control:
    -> if you need to something that is not a common thing to do: hell to do that!
    -> you have to abide by the rule lf python - it cannot go by your rules
- less clarity: 
    -> commenting is hard to do blocks
    -> you for get the types - int, string, list, you forget
- you know less about the computer:
    -> not a great foundation



From assignment 2: you learned about the following things: give example code as to how you used them and describe how they work: file, stuctures, datetime
- file manipulation
- dictionaries, lists, built in string functions
- datetime library 
- casting-  syntax

things I learned form A2 and my own programing:
# general running a program:
- python hello.py
- if __name__ == "__main__":
    main()   

# casting a int to a str
    hour = round(hour, 2)
    string = str(hour)
    to_int = int(string)

# file open and extract:
    with open( path_to_file ) as f:
            contents = f.read()
    
    f = open("Piglatin_translation.txt", 'w')
    f.write(final)

# string maniputation:
- date = date[8:] - access all after char 8 in string
- newd = date.split('/') -> splits a string into 2 list at '/' char
- len(str) -> the length of the string
-  e[1] = e[1].strip("DTSTART:") -> strips "substring" form string
-  string =  date + line + content (concatentaion)
-  w = w.replace(",", "\n") 
-  n_w = n_w.capitalize()
- if "RRULE" in e:

# import calender
- string = calendar.month_name[month_num]

# import datetime:
    def findDay(date):
        ds = str(date)
        day = datetime.datetime.strptime(ds, '%Y%m%d').weekday()
        return (calendar.day_abbr[day])
    
# dictionary:
- putting a list into a dictionary and if there is a rule : adds it into it
    def make_dict(events):
        dict= {} 
                key = getkey(e)
                content = keywords(e)
                dict[key] = content
                date = key
                if rule >1:
                    while date < r:
                        date = week_after(date)
                        dict[date] = content
                rule = 0

# Thing talked about in the slides
- Numpy
- Lambda


Provided linkes in slides that work
- 

previous test answers:
# shortest_length(list l, string x)
- if there is a string shorter than x  in list call it l[i]: it gets outputs l[i]
- if there if not output x
    - itterate through the list and print out the lengths
    - counter varible hold the shortest lenght and coressponding index in l
    - print out : the index, the shortest found and len(x) - 2 words (nothing else)
    - compare the 2 lengths: output the one with the lowest length

    index_shortest = 0
    current_index = 0
    value_shortest = len(list[0])
    for l in list:
        if len(l) <  value_shortest:
            value_shortest = len(l)
            index_shortest = current_index 
        current_index = current_index + 1

    if value_shortest < len(x):
        return  list[]
    
