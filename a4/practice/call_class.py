
import A_class
import sys
import argparse
import datetime


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--file', type=str, help='file to be processed')
    parser.add_argument('--start', type=str, help='start of date range')
    parser.add_argument('--end', type=str, help='end of data range')

    args = parser.parse_args()

    if not args.file:
        print("Need --file=<ics filename>")

    if not args.start:
        print("Need --start=yyyy/mm/dd")

    if not args.end:
        print("Need --end=yyyy/mm/dd")

    (start_yy, start_mm, start_dd) = \
        (int(field) for field in args.start.split("/"))

    (end_yy, end_mm, end_dd) = \
        (int(field) for field in args.end.split("/"))

    start_dt = datetime.datetime(start_yy, start_mm, start_dd)
    end_dt   = datetime.datetime(end_yy, end_mm, end_dd)



    filename =  "../ics/"+args.file 
    p1 = A_class.process_cal(filename)
   
    """
     p1.get_events_for_day(start_dt)
     This is the second line of the docstring.
     """

    curr_dt = start_dt
    while (curr_dt <= end_dt):
        days_events = p1.get_events_for_day(curr_dt)
        if (days_events):
            print(days_events, "\n")
        curr_dt = curr_dt + datetime.timedelta(days=1)





if __name__ == "__main__":
    main()


