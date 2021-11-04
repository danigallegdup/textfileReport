
    -------------------------------------------------------------------------------------------------------------------------------
    20210214-20210214 (UD)
	‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐
	T180000-T210000 Romantic dinner with Chris {{Burger King}}

    -------------------------------------------------------------------------------------------------------------------------------


BEGIN:VCALENDAR
BEGIN:VEVENT
DTSTART:20210214T180000
DTEND:20210214T210000
LOCATION:Burger King
SUMMARY:Romantic dinner with Chris
END:VEVENT
END:VCALENDAR

STEP 1: Find all events: 
    - FindKeyWord_Lines(char *line)
        - BEGIN:VEVENT:--> CreateEvent(array, index)
        - DTSTART:
        - DTEND:  --->separate_dt(char *DTSTART, *DTEND )
        - LOCATION:The Bumptious Barista --> location()
        - SUMMARY:Coffee with Pat --> summary()


STEP 2: 
    - location()
    - summary()
    - separate_dt()
        -> start_enddate()
            ->int find_weekday()
                    ->weekday(int 0);
        -> start_endtime()
            - date("2021/02/14")  











    