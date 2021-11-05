import random

def randomize_list(names):
    return random.shuffle(names)

def check_diff(names, ran):
    i = 0
    for n in names:
        if ran[i] == n:
            return -1
        i = i+1
    return 20

def output_files(filename, content):
    f = open(filename, 'w')
    f.write(content)


def main():
    names = "Dani Jaime Juanca Mariana"
    ran = names.split(" ")
    names = names.split(" ")

    ready = -1
    while ready < 1:
        randomize_list(ran)
        ready = check_diff(names, ran)

    i =0
    for n in names:
        output_files(n+".txt", ran[i]) 
        i= i+1

    
if __name__ == "__main__":
    main()                                                                                                                                                                                                                                            
              

