import csv

def lists():
    fruits = ['orange', 'apple', 'pear', 'banana', 'kiwi', 'apple', 'banana']
    fruits.append('grape')
    print(fruits)
    fruits.sort()
    print(fruits)

def dictionaries():
    tel = {'jack': 4098, 'sape': 4139}
    tel['guido'] = 4127
    list(tel)
    for k, v in tel.items():
        print(k, v)
   


def main():
    print("hello world")
    lists()
    dictionaries()
    

if __name__ == "__main__":
    main()