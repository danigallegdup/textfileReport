# Tests for `SENG 265`, Assignment #1

* Test 1
    * Input: `one.ics`
    * Expected output: `test01.txt`
    * Command: `./process_cal3 --start=2021/2/14 --end=2021/2/14 --file=one.ics`
    * ./process_cal3  --start=1234/01/12 --end=2134/12/11 --file=ics/one.ics  | diff test/test01.txt -

* Test 2
    * Input: `two.ics`
    * Expected output: `test02.txt`
    * Command: `./process_cal3 --start=2021/4/18 --end=2021/4/21 --file=two.ics`
     ./process_cal3 --start=2021/4/18 --end=2021/4/21 --file=ics/two.ics | diff test/test02.txt -

* Test 3
    * Input: `many.ics`
    * Expected output: `test03.txt`
    * Command: `./process_cal3 --start=2021/2/1 --end=2021/3/1 --file=many.ics`
        ./process_cal3 --start=2021/2/1 --end=2021/3/1 --file=ics/many.ics | diff test/test03.txt -

* Test 4
    * Input: `two.ics`
    * Expected output: `test04.txt`
    * Command: `./process_cal3 --start=2021/4/22 --end=2021/4/23 --file=two.ics`
    ./process_cal3 --start=2021/4/22 --end=2021/4/23 --file=ics/two.ics | diff test/test04.txt -

* Test 5
    * Input: `diana-devops.ics`
    * Expected output: `test05.txt`
    * Command: `./process_cal3 --start=2021/2/1 --end=2021/2/1 --file=diana-devops.ics`
    * ./process_cal3 --start=2021/2/1 --end=2021/2/1  --file=ics/diana-devops.ics | diff test/test05.txt -

* Test 6
    * Input: `diana-devops.ics`
    * Expected output: `test06.txt`
    * Command: `./process_cal3 --start=2021/2/2 --end=2021/2/2 --file=diana-devops.ics`
    ./process_cal3 --start=2021/2/2 --end=2021/2/2 --file=ics/diana-devops.ics | diff test/test06.txt -


* Test 7
    * Input: `diana-devops.ics`
    * Expected output: `test07.txt`
    * Command: `./process_cal3 --start=2021/2/1 --end=2021/2/8 --file=diana-devops.ics`
./process_cal3 --start=2021/2/1 --end=2021/2/8 --file=ics/diana-devops.ics | diff test/test07.txt -


* Test 8
    * Input: `diana-devops.ics`
    * Expected output: `test08.txt`
    * Command: `./process_cal3 --start=2021/2/8 --end=2021/2/15 --file=diana-devops.ics`
./process_cal3 --start=2021/2/8 --end=2021/2/15 --file=ics/diana-devops.ics | diff test/test08.txt -

* Test 9
    * Input: `diana-devops.ics`
    * Expected output: `test09.txt`
    * Command: `./process_cal3 --start=2021/2/1 --end=2021/3/1 --file=diana-devops.ics`
./process_cal3 --start=2021/2/1 --end=2021/3/1 --file=ics/diana-devops.ics | diff test/test09.txt -

* Test 10
    * Input: `diana-devops.ics`
    * Expected output: `test10.txt`
    * Command: `./process_cal3 --start=2021/1/1 --end=2021/4/30 --file=diana-devops.ics`
./process_cal3 --start=2021/1/1 --end=2021/4/30 --file=ics/diana-devops.ics | diff test/test10.txt -

* Test 11
    * Input: `mlb.ics`
    * Expected output: `test11.txt`
    * Command: `./process_cal3 --start=2021/5/1 --end=2021/6/1 --file=mlb.ics`
./process_cal3 --start=2021/5/1 --end=2021/6/1 --file=ics/mlb.ics | diff test/test11.txt -

* Test 12
    * Input: `mlb.ics`
    * Expected output: `test12.txt`
    * Command: `./process_cal3 --start=2021/5/1 --end=2021/8/1 --file=mlb.ics`
./process_cal3 --start=2021/5/1 --end=2021/8/1 --file=ics/mlb.ics | diff test/test12.txt -


* Test 13
    * Input: `mlb.ics`
    * Expected output: `test13.txt`
    * Command: `./process_cal3 --start=2020/12/1 --end=2021/4/30 --file=mlb.ics`
./process_cal3 --start=2020/12/1 --end=2021/4/30 --file=ics/mlb.ics | diff test/test13.txt -

