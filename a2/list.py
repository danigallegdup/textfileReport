import csv
import sys


def main():
    print("hello world ")
    thisdict = {
    41: "11:15 AM to 12:30 PM: Coffee with Pat {{The Bumptious Barista}}",
    32: "12:15 PM to 12:30 PM: Coffee with Pat {{The Bumptious Barista}}",
    23: "1:15 PM to 12:30 PM: Coffee with Pat {{The Bumptious Barista}}",
    14: "3:15 PM to 12:30 PM: Coffee with Pat {{The Bumptious Barista}}",
    }   
    print(thisdict[41])
    x = thisdict.keys()
    print(x)
    thisdict[5]= "7:15 PM to 12:30 PM: Coffee with Pat {{The Bumptious Barista}}"
    print(x)

    x = thisdict.values()
    print(x)

    x = thisdict.items()
    print(x)
if __name__ == "__main__":
    main()                                                                                                                                                                                                                                            
              

